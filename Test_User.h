#pragma once

#include "User.h"

namespace TestUser
{
	void test_Initialization()
	{
		User testUser;
		std::wcout << testUser.getLocation() << std::endl;
		std::wcout << testUser.getSearchTerm() << std::endl;
	}

	void runTest()
	{
		test_Initialization();
	}
}