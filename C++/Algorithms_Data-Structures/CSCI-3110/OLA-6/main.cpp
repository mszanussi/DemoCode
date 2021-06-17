/*
 *	Author: Milan Zanussi
 *	Class: CSCI 3110
 *	Professor: Dr. Zhijiang Dong
 *	Program: Open Lab 6
 *	Description: Load a dictionary of words into
 *	an STL unordered map and implement several
 *	search operations
 */
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <unordered_map>
#include <cctype>
#include <list>
#include "WordChecker.h"

using namespace std;

const string DICT_FILENAME = "dict.txt";

int main()
{
	// Open a word checker using the dictionary
	// stored in "dict.txt"
	WordChecker dictChecker(DICT_FILENAME);

	// Create a list to store all matches found in a search
	list<string> matchList;

	string keyword;

	cout << "Please enter a word to check (-1 to exit): ";
	while (cin >> keyword) {

		// Break on sentinel -1
		if (keyword == "-1")
			break;

		// Check if the word is in the dictionary
		if (dictChecker.SearchDictionary(keyword))
			cout << "OK" << endl;

		// The word is not in the dictionary
		else {

			// Run keyword through word checker
			matchList = dictChecker(keyword);

			// If there were some matches
			if (!matchList.empty()) {

				// Print all matches
				for (list<string>::iterator it = matchList.begin(); it != matchList.end(); ++it)
					cout << (*it) << endl;
			}

			// No matches found
			else {
				cout << "Not found" << endl;
			}

		}

		// Prompt user for next entry
		cout << "Please enter a word to check (-1 to exit): ";
	}

	return 0;
}