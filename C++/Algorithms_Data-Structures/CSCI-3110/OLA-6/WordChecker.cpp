#include "WordChecker.h"
#include <cctype>
#include <fstream>
#include <cassert>

// VALUE CONSTRUCTOR
//		Takes an string input and reads a file that
//		corresponds with that filename
WordChecker::WordChecker(std::string dictFilename)
{
	// Variable for input file stream
	std::ifstream dictFile;

	//Key and entry pair in the dictionary
	std::pair<std::string, std::string> dictEntry;

	// Open the dictionary file and check if the file exists
	dictFile.open(dictFilename.c_str());
	assert(dictFile);

	// Read all items from the file
	while (dictFile >> dictEntry.second) {

		// Make the key a lowercase version of the word
		dictEntry.first = LowerString(dictEntry.second);

		// Insert entry into dictionary
		dictionary.insert(dictEntry);
	}

	// Close file after reading all words
	dictFile.close();
}

// NO REVISION SEARCH:
//		Searches for the entry in the dictionary
//		returns true if found
bool WordChecker::SearchDictionary(std::string searchKey) 
{
	return (dictionary.find(searchKey) != dictionary.end());
}

// SINGLE OMISSION SEARCH:
//		Searches the dictionary for all
//		item whose key ressembles any key that can
//		made from removing any one letter from the
//		search key and returns a list of matches
std::list<std::string> WordChecker::SingleOmissionSearch(std::string searchKey)
{
	// Container for list of matches found
	std::list<std::string> matchesFound;

	// Convert input to all lowercase string
	searchKey = LowerString(searchKey);

	// Current substring with one character omitted
	std::string currSearch;

	for (std::string::size_type i = 0; i < searchKey.size(); ++i) {

		currSearch = searchKey.substr(0, i) + searchKey.substr(i + 1);

		// If the item is found in the dictionary
		if (dictionary.find(currSearch) != dictionary.end())
		{
			// Insert the found item into the list
			matchesFound.push_back(dictionary[currSearch]);
	
		}
	}

	// Sort the entries
	matchesFound.sort();

	// Return list of items found
	return matchesFound;
}

// SINGLE ADDITION SEARCH:
//		Searches the dictionary for all
//		items whose key ressembles the search string 
//		with any extra letter inserted into the string
//		and returns a list of matches
std::list<std::string> WordChecker::SingleAdditionSearch(std::string searchKey)
{
	// Container for list of matches found
	std::list<std::string> matchesFound;

	// Convert input to all lowercase string
	searchKey = LowerString(searchKey);

	// Current substring with one character omitted
	std::string currSearch;

	// Iterate through each character and one past the end of the string
	for (std::string::size_type i = 0; i <= searchKey.size(); ++i) {

		for (char j = 'a'; j <= 'z'; ++j) {

			// String with letter right before the current iterator positon
			// i == size ==> append to the end of the string
			currSearch = searchKey.substr(0, i) + j + searchKey.substr(i);

			// If the item is found in the dictionary
			if (dictionary.find(currSearch) != dictionary.end())
			{
				// Insert the found item into the list
				matchesFound.push_back(dictionary[currSearch]);
			}
		}
	}

	// Sort the entries
	matchesFound.sort();

	// Return list of items found
	return matchesFound;
}

// ADJACENT SWAP SEARCH:
//		Searches the dictionary for all
//		matches that can be found for every string
//		formed by swapping any two adjacent characters
//		in the string and returns a list of matches
std::list<std::string> WordChecker::AdjacentSwapSearch(std::string searchKey)
{
	// Container for list of matches found
	std::list<std::string> matchesFound;

	// Convert input to all lowercase string
	searchKey = LowerString(searchKey);

	// Current substring with one character omitted
	std::string currSearch;

	// Iterate through each character and one past the end of the string
	for (std::string::size_type i = 0; i < searchKey.size()-1; ++i) {

		currSearch = searchKey.substr(0, i) + searchKey.substr(i + 1, 1) + searchKey.substr(i, 1) + searchKey.substr(i + 2);

		// If the item is found in the dictionary
		if (dictionary.find(currSearch) != dictionary.end())
		{
			// Insert the found item into the list
			matchesFound.push_back(dictionary[currSearch]);
		}

	}

	// Sort the entries
	matchesFound.sort();

	// Return list of items found
	return matchesFound;
}

// SINGLE REPLACEMENT SEARCH:
//		Returns list of matches found in dictionary
//		by replacing any character in the search word
//		with any other character
std::list<std::string> WordChecker::SingleReplaceSearch(std::string searchKey)
{
	// Container for list of matches found
	std::list<std::string> matchesFound;

	// Convert input to all lowercase string
	searchKey = LowerString(searchKey);

	// Current substring with one character omitted
	std::string currSearch;

	// Iterate through each character and one past the end of the string
	for (std::string::size_type i = 0; i < searchKey.size(); ++i) {

		for (char j = 'a'; j <= 'z'; ++j) {

			// String with letter right before the current iterator positon
			// i == size ==> append to the end of the string
			currSearch = searchKey.substr(0, i) + j + searchKey.substr(i + 1);

			// If the item is found in the dictionary
			if (dictionary.find(currSearch) != dictionary.end())
			{
				// Insert the found item into the list
				matchesFound.push_back(dictionary[currSearch]);
			}
		}
	}

	// Sort the entries
	matchesFound.sort();

	// Return list of items found
	return matchesFound;
}

// SPLIT SEARCH:
//		Returns list of matches found in dictionary
//		by searching for all words made by searching for
//		each pair of words formed by splitting the word
//		in two.
std::list<std::string> WordChecker::SplitSearch(std::string searchKey)
{
	// Container for list of matches found
	std::list<std::string> matchesFound;

	// Convert input to all lowercase string
	searchKey = LowerString(searchKey);

	// Current substring with one character omitted
	std::string currSearch1;
	std::string currSearch2;

	// Iterate through each character and one past the end of the string
	for (std::string::size_type i = 1; i < searchKey.size(); ++i) {

		// String with letter right before the current iterator positon
		// i == size ==> append to the end of the string
		currSearch1 = searchKey.substr(0, i);
		currSearch2 = searchKey.substr(i + 1);

		// If the first half is found in the dictionary
		if (dictionary.find(currSearch1) != dictionary.end())
		{
			// Insert the found item into the list
			matchesFound.push_back(dictionary[currSearch1]);
		}

		// If the second half is found in the dictionary
		if (dictionary.find(currSearch2) != dictionary.end())
		{
			// Insert the found item into the list
			matchesFound.push_back(dictionary[currSearch2]);
		}
		
	}

	// Sort the entries
	matchesFound.sort();

	// Return list of items found
	return matchesFound;
}

// OVERLOADED PARAMETER PASSING OPERATOR:
//		Accesses the dictionary using other functions
//		defined in the class to aggregate all near possible
//		near misses to be found from searching a key in
//		the WordChecker's dictionary.
std::list<std::string> WordChecker::operator()(std::string searchKey)
{
	// Create a list to store all matches
	std::list<std::string> AllMatchesFound;

	// Add all entries from the single addition search
	AllMatchesFound.merge(SingleAdditionSearch(searchKey));

	// Add all entries from the single omission search
	AllMatchesFound.merge(SingleOmissionSearch(searchKey));

	// Add all entries from the single replacement search
	AllMatchesFound.merge(SingleReplaceSearch(searchKey));

	// Add all entries from the adjacency swap search
	AllMatchesFound.merge(AdjacentSwapSearch(searchKey));

	// Add all entries from the string split search
	AllMatchesFound.merge(SplitSearch(searchKey));

	// Remove all duplicate values
	AllMatchesFound.unique();

	// Return list of all search matches
	return AllMatchesFound;
}

// LOWERCASE STRING:
//		Returns a lowercase version of a string
std::string WordChecker::LowerString(std::string inString)
{
	// Create new empty string
	std::string lowString;

	// For every character in the original string
	for (std::string::size_type i = 0; i < inString.size(); ++i) {

		// Take each character from inString, lower it, and
		// append it to the output string
		lowString.push_back(tolower(inString[i]));
	}

	return lowString;
}