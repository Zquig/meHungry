#pragma once

#include "YelpAuth.h"

namespace TestYelpAuth
{
	void test_Initalization()
	{
		YelpAuth* testAuth = new YelpAuth();
		delete testAuth;
	}

	void test_AuthorizationHeaderValue()
	{
		YelpAuth testAuth;
		std::wcout << testAuth.authorizationHeaderValue() << std::endl;
	}
	
	void runTest()
	{
		//test_Initalization();
		test_AuthorizationHeaderValue();
	}
}