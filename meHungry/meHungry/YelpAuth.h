#pragma once

// Created 10/05/2016

#include "cpprest\details\basic_types.h" // For string_t
#include "cpprest\http_listener.h"
#include "cpprest\http_client.h"
#include "cpprest\json.h"

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::oauth2::experimental;

class YelpAuth
{
public:
	YelpAuth();
	//YelpAuth(http_client& client); // Attempt at putting in authorization info into client object ~CF 10/07/2016
	~YelpAuth();
	void obtainAuthToken();
	utility::string_t authorizationHeaderValue(); // Attempt at putting in access_token into Authorization header with value: "Bearer access_token" ~CF 10/07/2016
	//utility::string_t accessToken() const; // Attempt at placing access_token as a URL query parameter ~CF 10/07/2016
private:
	// TODO: place the following into a separate file and import values
	const utility::string_t APP_ID = U("omitted"); // passed in with parameter key: "client_id"
	const utility::string_t APP_SECRET = U("omitted"); // passed in with parameter key: "client_secret"

	const utility::string_t LOCAL_HOST = U("http://localhost:5001"); // alternative: http://127.0.0.1
	const utility::string_t TOKEN_ENDPOINT = U("https://api.yelp.com/oauth2/token");
	const utility::string_t GRANT_TYPE = U("client_credentials"); // passed in with parameter key: "grant_type"

	http_request* m_yelpAuthRequest;
	//http_client& m_yelpClient; // Attempt at putting in authorization info into client object ~CF 10/07/2016
	json::value m_authGrant;

	uri buildRequest();
	pplx::task<web::json::value> tokenRequest();
	//void authorizeClient(http_client_config& clientConfig); // Attempt at putting in authorization info into client object ~CF 10/07/2016
};