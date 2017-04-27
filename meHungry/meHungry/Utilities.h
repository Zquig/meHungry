#pragma once

#include <string>
#include "cpprest\details\basic_types.h"
#include <assert.h>
#include <list>
#include <vector>

using namespace std;
using namespace utility;

// Convert string_t to string
static string wstringToString(string_t& ws)
{
	string converted;
	converted.assign(ws.begin(), ws.end());
	return converted;
}

static string_t stringToWString(string& s)
{
	string_t converted;
	converted.assign(s.begin(), s.end());
	return converted;
}

static bool isAlphanumerical(char_t ch)
{
	if (ch >= U('0') && ch <= U('9'))
		return true;
	if (ch >= U('A') && ch <= U('Z'))
		return true;
	if (ch >= U('a') && ch <= U('z'))
		return true;
	return false;
}

// Find string in source string starting from the beginning and return iterator to first instance of interested string
static string_t::const_iterator findString(const string_t& srcStr, const string_t& strToFind)
// size_t a = str.find("strToFind");
// if (a == std::string::npos)
// not found
// otherwise, a is the offset

{
	if (strToFind.length() > srcStr.length())
		return srcStr.end();

	for (string_t::const_iterator strIter = srcStr.begin(); strIter != srcStr.end(); strIter++)
	{
		string_t::const_iterator findThis = strToFind.begin();

		if (*findThis == *strIter)
		{
			findThis++;
			string_t::const_iterator strIterCopy = strIter + 1;

			while (findThis != strToFind.end())
			{
				if (*findThis++ != *strIterCopy++)
				{
					break;
				}
			}
			
			if (strToFind.end() == findThis) // string found!
				return strIter;
		}
	}
	
	// Unable to find string
	return srcStr.end();
}

// Find string in source string starting at a specified position and return the next instance of the interested string
// Stop find if a specified string is reached
// If string is not found, set iterator to the end of the source string and return false
static bool findString(string_t::const_iterator& srcStrIter, const string_t& srcStr, const string_t& strToFind, const string_t& stopStr = U(""))
{
	if (strToFind.length() > srcStr.length())
	{
		srcStrIter = srcStr.end();
		wcout << U("ERROR - ") << strToFind << U(" cannot exist within ") << srcStr << U('!') << endl;
		return false;
	}

	for (; srcStrIter != srcStr.end(); srcStrIter++)
	{
		string_t::const_iterator findThis = strToFind.begin();

		if (*findThis == *srcStrIter)
		{
			findThis++;
			string_t::const_iterator strIterCopy = srcStrIter + 1;

			while (findThis != strToFind.end())
			{
				if (*findThis++ != *strIterCopy++)
					break;
			}

			if (strToFind.end() == findThis) // String found!
			{
				// srcStrIter is at start position of the interested string
				return true;
			}
		}
		
		if (!stopStr.empty())
		{
			string_t::const_iterator stopAtThis = stopStr.begin();

			if (*stopAtThis == *srcStrIter)
			{
				stopAtThis++;
				string_t::const_iterator strIterCopy = srcStrIter + 1;

				while (stopAtThis != stopStr.end())
				{
					if (*stopAtThis++ != *strIterCopy++)
						break;
				}

				if (stopStr.end() == stopAtThis) // End of block of text to search reached
				{
					// String not found with specified conditions
					srcStrIter = srcStr.end();
					return false;
				}
			}
		}
	}
	// String not found
	srcStrIter = srcStr.end();
	return false;
}

static vector<size_t> randomizeArray(const size_t arraySize)
{
	if (0 >= arraySize)
	{
		wcout << U("ERROR - Cannot randomize an array of size 0!") << endl;
		assert(0 < arraySize);
	}

	vector<size_t> randomizedIndexes;

	if (1 == arraySize)
	{
		randomizedIndexes.push_back(0);
		return randomizedIndexes;
	}

	// Create list of size_t values to eventually randomize
	list<size_t> resultsIndex;
	for (size_t i = 0; i < arraySize; i++)
	{
		resultsIndex.push_back(i);
	}

	while (resultsIndex.size() != 0)
	{
		size_t listIndex = rand() % resultsIndex.size();
		auto iter = resultsIndex.begin();
		iter = next(iter, listIndex); // increment iterator to desired index in list
		randomizedIndexes.push_back(*iter); // add randomly selected index to vector
		resultsIndex.remove(*iter); // delete index from list
	}

	return randomizedIndexes;
}