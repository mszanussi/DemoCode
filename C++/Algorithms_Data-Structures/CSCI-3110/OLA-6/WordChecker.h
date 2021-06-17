// #pragma once

#include <string>
#include <list>
#include <unordered_map>

// WORD CHECKER CLASS:
//		A functor that stores a dictionary from a file and
//		uses it as reference to check all words that are
//		close to the word that was passed through the functor
class WordChecker
{
	
	public:

		// VALUE CONSTRUCTOR
		//		Takes an string input and reads a file that
		//		corresponds with that filename
		WordChecker(std::string dictFilename);

		// NO REVISION SEARCH:
		//		Searches for the entry in the dictionary
		//		returns true if found
		bool SearchDictionary(std::string searchKey);

		// SINGLE OMISSION SEARCH:
		//		Searches the dictionary for all
		//		item whose key ressembles any key that can
		//		made from removing any one letter from the
		//		search key and returns a list of matches
		std::list<std::string> SingleOmissionSearch(std::string searchKey);

		// SINGLE ADDITION SEARCH:
		//		Searches the dictionary for all
		//		items whose key ressembles the search string 
		//		with any extra letter inserted into the string
		//		and returns a list of matches
		std::list<std::string> SingleAdditionSearch(std::string searchKey);

		// ADJACENT SWAP SEARCH:
		//		Searches the dictionary for all
		//		matches that can be found for every string
		//		formed by swapping any two adjacent characters
		//		in the string and returns a list of matches
		std::list<std::string> AdjacentSwapSearch(std::string searchKey);

		// SINGLE REPLACEMENT SEARCH:
		//		Returns list of matches found in dictionary
		//		by replacing any character in the search word
		//		with any other character
		std::list<std::string> SingleReplaceSearch(std::string searchKey);

		// SPLIT SEARCH:
		//		Returns list of matches found in dictionary
		//		by searching for all words made by searching for
		//		each pair of words formed by splitting the word
		//		in two.
		std::list<std::string> SplitSearch(std::string searchKey);

		// OVERLOADED PARAMETER PASSING OPERATOR:
		//		Accesses the dictionary using other functions
		//		defined in the class to aggregate all near possible
		//		near misses to be found from searching a key in
		//		the WordChecker's dictionary.
		std::list<std::string> operator()(std::string searchKey);

	private:

		// LOWERCASE STRING:
		//		Returns a lowercase version of a string
		std::string LowerString(std::string inString);

		//**********|| MEMBER VARIABLES ||**********\\

		// Dictionary of words from an external file
		std::unordered_map<std::string, std::string> dictionary;
		
};