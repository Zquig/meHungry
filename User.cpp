#pragma once

#include <iostream>
#include "User.h"
#include "Utilities.h"

using namespace std;

User::User()
{
}

User::~User()
{
}

void User::setLocation()
{
	wcout << U("Where would you like to eat? ('f' to filter search results)") << endl;
	getline(wcin, m_userLocation);
	// TODO: allow the option to use current GPS location
	//wcin.ignore();
	wcout << endl;

	if (U("f") == m_userLocation)
	{
		setSearchTerm();
		setLocation();
	}

	normalizeUserInput(m_userLocation);
	normalizeUserInput(m_searchTerm);

	// BUG: What if user inputs spaces?
	// TODO: Remove unnecessary spaces in normalizeUserInput() (ex: "  new    american  " --> "new american") ~CF 10/10/2016

	if (m_searchTerm.empty())
	{
		m_searchTerm = U("restaurants"); // default
	}
}

string_t User::getLocation() const
{
	return m_userLocation;
}

void User::setSearchTerm()
{
	wcout << U("What would you like to eat? (leave blank for no filter)") << endl;
	getline(wcin, m_searchTerm);
	//wcin.ignore();
	wcout << endl;
}

string_t User::getSearchTerm() const
{
	return m_searchTerm;
}

// Input must only include alphanumerical characters and spaces
void User::normalizeUserInput(string_t& input)
{
	// Convert non-alphanumerical characters to a space
	for (size_t i = 0; i < input.length(); i++)
	{
		if (!isAlphanumerical(input[i]))
		{
			input[i] = U(' ');
		}
	}

	// TODO: Remove unnecessary spaces (ex: "  new    american  " --> "new american") ~CF 10/10/2016
}