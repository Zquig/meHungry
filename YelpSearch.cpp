#include "YelpSearch.h"
#include "cpprest\http_msg.h"
#include "YelpAuth.h"

YelpSearch::YelpSearch()
{
	m_user = new User;
	//m_searchLocation = m_user->getLocation();
	//m_searchTerm = m_user->getSearchTerm();
	// BUG: Searching for certain terms that return results with an "open_now" value causes an error and the program to crash! ~CF 10/16/2016

	m_parametersList = new std::map<const string_t, string_t>
	{
		{PARAM_LIMIT, RESULTS_LIMIT}
		,{PARAM_LOCATION, m_searchLocation} // Empty value
		//,{PARAM_OPEN, OPEN_NOW} // BUG: Searching for certain terms that return results with an "open_now" value causes an error and the program to crash! ~CF 10/16/2016
		,{PARAM_TERM, m_searchTerm} // Empty value
	};
}

YelpSearch::~YelpSearch()
{
	delete m_user;
	delete m_parametersList;
}

bool YelpSearch::setSearchParameters()
{
	m_user->setLocation();
	m_searchLocation = m_user->getLocation();
	m_searchTerm = m_user->getSearchTerm();

	if (m_user->getLocation().empty())
		return false;

	m_parametersList->at(PARAM_LOCATION) = m_searchLocation;
	m_parametersList->at(PARAM_TERM) = m_searchTerm;

	return true;
}

bool YelpSearch::searchYelp()
{
	m_rawResults = performSearch().get();

	if (m_rawResults.is_null())
	{
		std::wcout << U("ERROR - No results obtained!") << std::endl;
		return false;
	}

	// CONSIDER: What happens if this function is called twice? ~CF 10/24/2016
	// Solution:
	if (nullptr != m_searchResults)
	{
		delete m_searchResults;
	}
	m_searchResults = new SearchResults(m_rawResults);

	return true;
}

void YelpSearch::displayResults() const
{
	if (m_rawResults.is_null())
	{
		std::wcout << U("ERROR - No results obtained!") << std::endl;
		return;
	}

	//m_searchResults->displayRawResults(true);
	m_searchResults->presentResults();
}

//void YelpSearch::buildRequestURL(const string_t accessToken) // Attempt at placing access_token as a URL query parameter ~CF 10/07/2016
void YelpSearch::buildRequestURL()
{
	uri_builder request(SEARCH_PATH);

	//request.append_query(PARAM_ACCESS_TOKEN, accessToken); // Attempt at placing access_token as a URL query parameter ~CF 10/07/2016

	for (auto mapIter = m_parametersList->begin(); mapIter != m_parametersList->end(); mapIter++)
	{
		request.append_query(mapIter->first, mapIter->second);
	}

	m_requestURL = request.to_uri();

	std::wcout << U("Request URL: ") << m_requestURL.to_string() << std::endl; // TEST: OK
}

// Following: http://stackoverflow.com/questions/31371659/set-basic-http-authentication-in-casablanca
pplx::task<json::value> YelpSearch::performSearch()
{
	YelpAuth authenticate;
	authenticate.obtainAuthToken();

	//buildRequestURL(authenticate.accessToken());
	buildRequestURL();

	http_request searchRequest(http::methods::GET);
	searchRequest.set_request_uri(m_requestURL);
	searchRequest.headers().add(http::header_names::authorization, authenticate.authorizationHeaderValue());

	http_client yelpClient(SEARCH_HOST);

	return yelpClient.request(searchRequest).then([&](http::http_response response)
	{
		try
		{
			if (response.status_code() != http::status_codes::OK)
			{
				std::wcout << U("ERROR!") << std::endl;
				std::wcout << response.status_code(); // Currently receiving error code 400 - poor syntax!
				std::wcout << U(" - ") << response.reason_phrase();
				std::wcout << std::endl << response.to_string();
				return pplx::task_from_result(json::value()); // returns NULL
			}

			// else status_code() == OK (200)
			std::wcout << U("Status code: ") << response.status_code() << U(" - ") << U("SUCCESS!") << std::endl;
			return response.extract_json();
		}
		catch (...)
		{
			std::wcout << U("ERROR - Unable to extract json object") << std::endl;
			// return null json string
		}
	});
}

//void YelpSearch::authenticateClient(http_client& client)
//{
//	YelpAuth authorize(client);
//}