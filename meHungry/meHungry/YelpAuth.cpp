// Documentations:
// https://www.yelp.com/developers/documentation/v3/get_started
// https://casablanca.codeplex.com/wikipage?title=OAuth%20Client
// http://self-issued.info/docs/draft-ietf-oauth-v2-bearer.html


#include "YelpAuth.h"

//YelpAuth::YelpAuth(http_client& client) // Attempt at putting in authorization info into client object ~CF 10/07/2016
YelpAuth::YelpAuth()
	//:m_yelpClient(client) // Attempt at putting in authorization info into client object ~CF 10/07/2016
// Send a POST request for the token
{
	m_yelpAuthRequest = new http_request(methods::POST);

	// Attempt at putting in authorization info into client object ~CF 10/07/2016
	//http_client_config clientConfig = client.client_config();
	//authorizeClient(clientConfig);
}

YelpAuth::~YelpAuth()
{
	delete m_yelpAuthRequest;
}

void YelpAuth::obtainAuthToken()
{
	m_yelpAuthRequest->set_request_uri(buildRequest());
	m_authGrant = tokenRequest().get();

	// TEST
	std::wcout << U("Response body: ") << m_authGrant << std::endl;
	std::wcout << U("Access token: ") << m_authGrant.as_object().at(U("access_token")) << std::endl;
	// END_TEST: OK
}

// Build value for Authorizaton header
// Format: "Bearer access_token"
// Attempt at putting in access_token into Authorization header with value: "Bearer access_token" ~CF 10/07/2016
utility::string_t YelpAuth::authorizationHeaderValue()
{
	utility::string_t value;
	
	utility::string_t tokenType = m_authGrant.as_object().at(U("token_type")).as_string();
	utility::string_t accessToken = m_authGrant.as_object().at(U("access_token")).as_string();

	value = tokenType + U(" ") + accessToken;
	return value;
}

// Attempt at placing access_token as a URL query parameter ~CF 10/07/2016
//utility::string_t YelpAuth::accessToken() const
//{
//	return m_authGrant.as_object().at(U("access_token")).as_string();
//}

uri YelpAuth::buildRequest()
// Create request URL
{
	uri_builder requestURL(TOKEN_ENDPOINT);

	// Create mapping of required parameters: grant_type, client_id, and client_secret
	// Use a vector<unordered_map> to maintain order of parameters
	std::vector<std::map<const utility::string_t, utility::string_t>> requestParameters;
	std::map<const utility::string_t, utility::string_t> grantType = { {U("grant_type"), GRANT_TYPE} };
	std::map<const utility::string_t, utility::string_t> clientID = { {U("client_id"), APP_ID} };
	std::map<const utility::string_t, utility::string_t> clientSecret = { {U("client_secret"), APP_SECRET} };
	requestParameters.push_back(grantType);
	requestParameters.push_back(clientID);
	requestParameters.push_back(clientSecret);

	for (auto iter = requestParameters.begin(); iter != requestParameters.end(); iter++)
	{
		auto mapIter = iter->begin(); // iter points to map in vector
		requestURL.append_query(mapIter->first, mapIter->second);
	}

	// TEST:
	//std::wcout << requestURL.to_string() << std::endl;
	// END_TEST: OK

	return requestURL.to_uri();
}

pplx::task<json::value> YelpAuth::tokenRequest()
{
	http_client client(m_yelpAuthRequest->absolute_uri());

	return client.request(methods::POST).then([](http_response response)
	{
		if (status_codes::OK != response.status_code())
		{
			std::wcout << U("ERROR!") << std::endl;
			std::wcout << response.status_code(); // Currently receiving error code 400 - poor syntax!
			std::wcout << U(" - ") << response.reason_phrase();
			std::wcout << std::endl << response.to_string();
			//system("pause");
			return pplx::task_from_result(json::value()); // returns NULL
		}

		// status_codes::OK == response.status_code())
		std::wcout << response.status_code() << std::endl;
		return response.extract_json();
	});
}

// Attempt at putting in authorization info into client object ~CF 10/07/2016
//void YelpAuth::authorizeClient(http_client_config& clientConfig)
//{
//	oauth2_config oAuth2Config(APP_ID, APP_SECRET, U(""), TOKEN_ENDPOINT, LOCAL_HOST);
//	oAuth2Config.set_bearer_auth(true);
//	oAuth2Config.set_access_token_key(m_authGrant.as_object().at(U("access_token")).as_string());
//	clientConfig.set_oauth2(oAuth2Config);
//}