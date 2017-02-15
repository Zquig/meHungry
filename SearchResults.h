#pragma once

#include "cpprest\json.h"

using namespace web;

class SearchResults
{
public:
	SearchResults(const json::value& results);
	~SearchResults();
	void displayRawResults(bool separated = true) const;
	void presentResults() const;
private:
	const json::value& m_rawResults;

	//std::vector<size_t> randomizeResults(const size_t numResults) const;
	utility::char_t getUserInput() const;
	void userAction(const json::value& restuarant, const utility::char_t input) const;
};