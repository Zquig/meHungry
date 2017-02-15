#pragma once

#include <iostream>
#include "cpprest\details\basic_types.h"

using namespace utility;

class User
{
public:
	User();
	~User();
	void setLocation();
	string_t getLocation() const;
	void setSearchTerm();
	string_t getSearchTerm() const;
private:
	string_t m_userLocation;
	string_t m_searchTerm;

	void normalizeUserInput(string_t& input);
};