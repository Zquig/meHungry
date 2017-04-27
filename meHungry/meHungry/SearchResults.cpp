#pragma once

#include "SearchResults.h"
//#include <random>
//#include <list>
#include <Windows.h>
#include <assert.h>
#include "ImageScraper.h"
#include "Utilities.h"

using namespace std;

SearchResults::SearchResults(const json::value& results)
	:m_rawResults(results)
{}

SearchResults::~SearchResults()
{}

void SearchResults::displayRawResults(bool separated) const
{
	if (!separated)
	{
		wcout << m_rawResults << endl;
		return;
	}

	json::value businessesObject = m_rawResults.at(U("businesses")); // get json object under businesses
	json::array businessesArray = businessesObject.as_array(); // businessesArray is an array of json objects

	unsigned int count = 1;
	for (auto arrIter = businessesArray.begin(); arrIter != businessesArray.end(); arrIter++)
	{
		wcout << count++ << U(".  ") << arrIter->serialize() << endl << endl;
	}

	return;
}

void SearchResults::presentResults() const
{
	const json::value& businessesObject = m_rawResults.at(U("businesses")); // get json object under businesses
	const json::array businessesArray = businessesObject.as_array(); // businessesArray is an array of json objects

	vector<size_t> presentOrder = randomizeArray(businessesArray.size()); // Change vector size depending on max number of results to present

	wcout << U("Would you like to eat at: ") << endl;
	unsigned int count = 1;
	for (size_t ele = 0; ele < presentOrder.size(); ele++)
	{
		auto arrIter = businessesArray.begin();
		arrIter = next(arrIter, presentOrder[ele]); // increment iterator to desired index

		const json::value& businessCategory = arrIter->at(U("categories"));
		json::array categoryArray = businessCategory.as_array(); // Q: Unable to make this const, if I do, I cannot access categoryArray[0]. Why?

		wcout << count++ << U(".  ")
			<< arrIter->at(U("name"))
			<< U(" (") << arrIter->at(U("price")) << U(")")
			<< U(" in ") << categoryArray[0].at(U("title"))
			<< U(" with a rating of ") << arrIter->at(U("rating"))
			<< U("?")
			<< endl;
		const string_t bizURL = arrIter->at(U("url")).as_string();
		wcout << bizURL << endl;

		ImageScraper scrapeImage(bizURL);
		scrapeImage.displayRandomPhoto().get();

		//ShellExecute(NULL, U("open"), arrIter->at(U("image_url")).as_string().c_str(), NULL, NULL, SW_SHOWNORMAL); // Show restaurant image
		// Loading images takes forever! <-- d/t slow connection ~CF 10/14/2016
		// TODO: cache images and results to allow for faster displays of searches
		// TODO: pull images from restaurant yelp site
		// Paul: "Redis" is one service" ~CF 10/10/2016

		utility::char_t userResponse = getUserInput();
		userAction(*arrIter, userResponse);
		wcout << endl;
	}
	wcout << U("That's all, Folks!") << endl;
}

// Implemented in Utilities.h ~CF 10/23/2016
//vector<size_t> SearchResults::randomizeResults(const size_t numResults) const
//{
//	if (0 >= numResults)
//	{
//		wcout << U("ERROR - Cannot randomize an array of size 0!") << endl;
//		assert(0 < numResults);
//	}
//
//	srand(time(NULL)); // Initialize random number generator
//
//	vector<size_t> randomizedIndexes;
//
//	if (1 == numResults)
//	{
//		randomizedIndexes.push_back(0);
//		return randomizedIndexes;
//	}
//
//	// Create list of size_t values to eventually randomize
//	list<size_t> resultsIndex;
//	for (size_t i = 0; i < numResults; i++)
//	{
//		resultsIndex.push_back(i);
//	}
//
//	while (resultsIndex.size() != 0)
//	{
//		size_t listIndex = rand() % resultsIndex.size();
//		auto iter = resultsIndex.begin();
//		iter = next(iter, listIndex); // increment iterator to desired index in list
//		randomizedIndexes.push_back(*iter); // add randomly selected index to vector
//		resultsIndex.remove(*iter); // delete index from list
//	}
//
//	return randomizedIndexes;
//}

utility::char_t SearchResults::getUserInput() const
{
	utility::char_t input;
	wcout << U("    yes ('y'), no ('n'), maybe ('m'), or quit ('q'): ");
	wcin >> input;

	if (input != U('y') && input != U('n') && input != U('m') && input != U('q'))
	{
		wcout << endl << U("Please enter a proper response! ('y', 'n', 'm', 'q')") << endl;
		input = getUserInput();
	}

	return input;
}

void SearchResults::userAction(const json::value& restaurant, const utility::char_t input) const
{
	assert(input == U('y') || input == U('n') || input == U('m') || input == U('q'));

	switch (input)
	{
	case U('y'):
		std::wcout << restaurant.at(U("url")) << std::endl;
		// TODO: implement a platform-independent method. This is Windows-only. ~CF 09/30/2016
		ShellExecute(NULL, U("open"), restaurant.at(U("url")).as_string().c_str(), NULL, NULL, SW_SHOWNORMAL);
		wcout << U("Enjoy!") << endl;
		system("pause");
		//exit(0);
		return;
	case U('n'):
		return;
	case U('m'):
		// TODO: implement maybe option
		return;
	case U('q'):
		std::wcout << U("Good-bye!") << std::endl;
		system("pause");
		exit(0);
	default:
		return;
	}
}