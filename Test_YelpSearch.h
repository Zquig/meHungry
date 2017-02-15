#pragma once

#include "YelpSearch.h"

namespace TestYelpSearch
{
	void test_Initialization()
	{
		YelpSearch* search = new YelpSearch();
		delete search;
	}

	void test_Search()
	{
		YelpSearch newSearch;
		if (newSearch.searchYelp())
		{
			system("pause");
			newSearch.displayResults();
			return;
		}
	}

	void runTest()
	{
		//test_Initialization();
		test_Search();
	}
}