#pragma once

#include "User.h"
#include "cpprest\details\basic_types.h"
#include "cpprest\json.h"
#include "cpprest\http_client.h"
#include <map>
#include "SearchResults.h"

using namespace utility;
using namespace web;
using namespace web::http::client;
using namespace concurrency::streams;

class YelpSearch
{
public:
	YelpSearch();
	~YelpSearch();
	bool setSearchParameters();
	bool searchYelp();
	void displayResults() const;
private:
	const string_t SEARCH_HOST = U("https://api.yelp.com/v3");
	const string_t SEARCH_PATH = U("/businesses/search");

	// Define parameter keys
	const string_t PARAM_ACCESS_TOKEN = U("access_token");
	const string_t PARAM_LIMIT = U("limit");
	const string_t PARAM_LOCATION = U("location");
	const string_t PARAM_OPEN = U("open_now");
	const string_t PARAM_TERM = U("term");

	const string_t RESULTS_LIMIT = U("50"); // Parameter: "limit"; Sets number of results to return. Default is 20. Max is 50.
	const string_t OPEN_NOW = U("1"); // Parameter: "open_now"; passed in as a boolean (0 or 1)
	string_t m_searchLocation;
	string_t m_searchTerm;

	User* m_user;
	std::map<const string_t, string_t>* m_parametersList;
	json::value m_rawResults;
	uri m_requestURL;
	SearchResults* m_searchResults;

	//void buildRequestURL(const string_t accessToken); // Attempt at placing access_token as a URL query parameter ~CF 10/07/2016
	void buildRequestURL();
	pplx::task<json::value> performSearch();
	//void authenticateClient(http_client& client); // Attempt at putting in authorization info into client object ~CF 10/07/2016
};