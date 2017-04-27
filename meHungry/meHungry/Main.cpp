// Version 2.0 of meHungry
// Using Yelp Fusion API

//#define TEST_YELPAUTH
//#define TEST_USER
//#define TEST_YELPSEARCH
#define DEFAULT

#if defined (TEST_YELPAUTH)
#include "Test_YelpAuth.h"
using namespace TestYelpAuth;
#endif

#if defined (TEST_USER)
#include "Test_User.h"
using namespace TestUser;
#endif

#if defined (TEST_YELPSEARCH)
#include "Test_YelpSearch.h"
using namespace TestYelpSearch;
#endif

#if defined(DEFAULT)
#include "User.h"
#include "YelpSearch.h"
#include <iostream>
#include <random>
#include <time.h>

void runProgram()
{
	srand(time(NULL));
	YelpSearch newSearch;

	while (!newSearch.setSearchParameters()); // Infinite loop until user enters a location

	if (newSearch.searchYelp())
	{
		system("pause");
		newSearch.displayResults();
		return;
	}
}
#endif

int main()
{
	//runTest();
	runProgram();
	system("pause");
	return 0;
}