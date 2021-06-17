/*
* Author: Milan Zanussi
* Date: October 8th, 2016
* File: main.cpp
* This file implements the Movie, Book, and StoreItem
* classes to create a console-based application that
* allows rental store employees to manage the inventory
* of the rental store with the following functions:
*	'M'	-- Print movie information by title
*	'B'	-- Print book information by author
*	'L'	-- Print inventory list in order of barcode
*	'R'	-- Return item by barcode
*	'C' -- Checkout item by barcode
*/

#include <iostream>
#include <fstream>
#include <list>
#include <cassert>
#include <string>
#include <typeinfo>
#include <cctype>

#include "Book.h"
#include "Movie.h"

using namespace std;


//****************// MISCELLANEOUS FUNCTIONS \\*****************\\

// Display Menu: Displays the menu of commands that the user
//		may use while accessing the inventory data
void DisplayMenu();

// Sorted Insert Function for STL list of StoreItem*:
//		Inserts an item of StoreItem* type into a list
//		sorted from least to greatest
void SortedInsert(StoreItem* insertItem, list<StoreItem*>& itemList);

//*******************// COMMAND FUNCTIONS \\*********************\\

// 'M': Search the list for all titles that match the name
//		associated with the entry information
//		This will output all movies that contain the input string
//		in their titles.
bool FindMovie(string movieTitle, list<StoreItem*> inventory);

// 'B': Search the list by author name and return whatever 
//		the entry information items match
//		This will output all books whose authors contain the
//		input string in their names.
bool FindBook(string authorName, list<StoreItem*> inventory);

// 'L': Prints the contents of the inventory
//		to the console.
void PrintInventory(list<StoreItem*> inventory);

// 'R': Returns an item of the input barcode to the inventory
//		If there is demand for the item, then the function
//		will decrement the demand.
bool ReturnItem(string barcode, list<StoreItem*> inventory);

// 'C': Checks out an item of the input barcode from
//		the inventory. If no more of that item is present,
//		then the function will increment the demand.
bool CheckoutItem(string barcode, list<StoreItem*> inventory);


int main()
{
	// ************* \\ FILE READ SEGMENT // **************\\

	list<StoreItem*> inventoryList;		// List for storing item information
	ifstream inventoryFile;				// ifstream for reading inventory information
	string readLine;					// string for storing lines read from inventoryFile

	// Open inventory file and check if file opens properly
	inventoryFile.open("../inventory.txt");
	assert(inventoryFile);

	// Skip until the first appearance of the
	// Entry-leading delimiter
	getline(inventoryFile, readLine, DELIMITER);

	// For every line in the data file in the expected format
	while (getline(inventoryFile, readLine, DELIMITER)) {

		// If the line entry is a movie
		if (readLine == "Movie") {
			// Initialize entry pointer
			Movie* movieEntry = new Movie;

			// Read until the end of the line
			getline(inventoryFile, readLine);

			// Create an inventory entry of Movie type
			movieEntry = movieEntry->createFromString(readLine);

			// Insert the item into the inventory list
			SortedInsert(movieEntry, inventoryList);
		}

		// If the line entry is a book
		else if (readLine == "Book") {
			// Initialize entry pointer
			Book* bookEntry = new Book;

			// Read until the end of the line
			getline(inventoryFile, readLine);

			// Create an inventory entry of Movie type
			bookEntry = bookEntry->createFromString(readLine);

			// Insert the item into the inventory list
			SortedInsert(bookEntry, inventoryList);
		}

		// Skip until the entry-leading delimiter of
		// the next line
		getline(inventoryFile, readLine, DELIMITER);

	}

	// ************* \\ MENU SEGMENT // **************\\

	// Display the store menu
	DisplayMenu();

	// Initialize variables for inventory access
	char commandChoice = '0';
	string searchTerm, commandLine;

	// Command loop until Q is entered
	while (tolower(commandChoice) != 'q'){

		// Prompt use to input command
		cout << "Please enter a command (M, B, L, R, C, Q): ";
		getline(cin, commandLine);

		// Take the first character from cin as the command
		commandChoice = tolower(commandLine[0]);

		// Bool for if the command is invalid
		bool validCommand = false;

		// Switch based on input command
		switch (tolower(commandChoice)){

		case('m') :

			// Prompt user for movie keywords
			cout << endl << "Enter a movie title keyword: ";
			getline(cin, searchTerm);

			// If no movies are printed, inform the user
			if (!FindMovie(searchTerm, inventoryList))
				cout << endl << "No movies match the keyword \"" << searchTerm << "\"." << endl << endl;

			validCommand = true;
			break;

		case('b'):

			// Prompt user for author keywords
			cout << endl << "Enter a book author keyword: ";
			getline(cin, searchTerm);

			// If no books are printed, inform the user
			if (!FindBook(searchTerm, inventoryList))
				cout << endl << "No books match the author \"" << searchTerm << "\"." << endl << endl;

			validCommand = true;
			break;

		case('l') :

			// Print entire inventory
			PrintInventory(inventoryList);

			validCommand = true;
			break;

		case('r'):

			// Prompt user for barcode
			cout << endl << "Enter a barcode: ";
			getline(cin, searchTerm);

			// If no books are printed, inform the user
			if (!ReturnItem(searchTerm, inventoryList))
				cout << endl << "No items match the barcode \"" << searchTerm << "\"." << endl << endl;

			// Else, inform user of success
			else
				cout << endl << "Successfully returned item \"" << searchTerm << "\"." << endl << endl;

			validCommand = true;
			break;

		case('c'):

			// Prompt user for barcode
			cout << endl << "Enter a barcode: ";
			getline(cin, searchTerm);

			// If no books are printed, inform the user
			if (!CheckoutItem(searchTerm, inventoryList))
				cout << endl << "No items match the barcode \"" << searchTerm << "\"." << endl << endl;

			// Else, inform user of success
			else
				cout << endl << "Successfully checked out item \"" << searchTerm << "\"." << endl << endl;

			validCommand = true;
			break;

		}

		// If the user does not enter a valid command
		if (!validCommand)
			cout << endl << "Command not recognized." << endl << endl;

		// Re-display menu
		DisplayMenu();

	}

	//PrintInventory(inventoryList);

	// Close the inventory file
	inventoryFile.close();

	return 0;
}

// Display Menu: Displays the menu of commands that the user
//		may use while accessing the inventory data
void DisplayMenu()
{
	cout << "                  Bankbuster Video                   " << endl	// Store name
		<< "*****************************************************" << endl	// Separator
		<< "* COMMAND    DESCRIPTION                            *" << endl	// Headers
		<< "* 'M'        Search movie by title                  *" << endl	// __
		<< "* 'B'        Search book by author                  *" << endl	//   |
		<< "* 'L'        Print entire inventory                 *" << endl	//   | 
		<< "* 'R'        Return item by barcode                 *" << endl	//   | Commands
		<< "* 'C'        Checkout item by barcode               *" << endl	//   |
		<< "* 'Q'        Exit the program                       *" << endl	// __|
		<< "*****************************************************" << endl;	// Separator

	return;
}

// Sorted Insert Function for STL list of StoreItem*:
// Inserts an item of StoreItem* type into a list
// sorted from least to greatest
void SortedInsert(StoreItem* insertItem, list<StoreItem*>& itemList)
{
	// List iterator for finding sort position
	list<StoreItem*>::iterator insertIterator;

	// For loop to iterate through the list
	for (insertIterator = itemList.begin(); insertIterator != itemList.end(); ++insertIterator)
	{
		// If the current position's item is < the insertion item
		if (*insertItem < **insertIterator) {

			// Add the insert item at the current position
			itemList.insert(insertIterator, insertItem);
			return;
		}
	}

	// If a proper place wasn't found for the item, make sure it was
	// added to then end of the list.
	itemList.push_back(insertItem);

	return;
}

// 'M': Search the list for all titles that match the name
//		associated with the entry information
//		This will output all movies that contain the input string
//		in their titles.
bool FindMovie(string movieTitle, list<StoreItem*> inventory)
{
	// Create iterator for traversal
	list<StoreItem*>::iterator mSearchIterator;

	// Bool for if any values match the keyword
	bool matchesFound = false;

	// Create default movie for typeid
	Movie defaultMovie;

	// Iterate through the list of StoreItem*
	for (mSearchIterator = inventory.begin(); mSearchIterator != inventory.end(); ++mSearchIterator) {

		if (typeid(**mSearchIterator) == typeid(defaultMovie)									// If the StoreItem at the current index is a movie
			&& (((Movie*)*mSearchIterator)->getTitle().find(movieTitle) != string::npos))		// and if the title of the Movie contains the search keyword
		{
			// Print the current item
			(*mSearchIterator)->printItem();

			// At least one movie matches the title keyword
			matchesFound = true;
		}

	}

	// Return bool for if any matches are found
	return matchesFound;
}

// 'B': Search the list by author name and return whatever 
//		the entry information items match
//		This will output all books whose authors contain the
//		input string in their names.
bool FindBook(string authorName, list<StoreItem*> inventory)
{
	// Create iterator for traversal
	list<StoreItem*>::iterator bSearchIterator;

	// Bool for if any values match the keyword
	bool matchesFound = false;

	// Default book for typeid
	Book defaultBook;

	for (bSearchIterator = inventory.begin(); bSearchIterator != inventory.end(); ++bSearchIterator) {

		if (typeid(**bSearchIterator) == typeid(defaultBook)									// If the StoreItem at the current index is a book
			&& (((Book*)*bSearchIterator)->getAuthor().find(authorName) != string::npos))		// and if the author of the Book contains the search keyword
		{
			// Print the current item
			(*bSearchIterator)->printItem();

			// At least one book matches the author keyword
			matchesFound = true;
		}
	}

	// Return bool for if any matches are found
	return matchesFound;
}

// 'L': Prints the contents of the inventory
//		to the console.
void PrintInventory(list<StoreItem*> inventory)
{
	list<StoreItem*>::iterator printAllIterator;

	// Create default StoreItems for typeid
	//Movie defaultMovie;
	//Book defaultBook;

	// Iterate through each item
	for (printAllIterator = inventory.begin(); printAllIterator != inventory.end(); ++printAllIterator) {

		// Print out each item
		(*printAllIterator)->printItem();
	}

	return;

}

// 'R': Returns an item of the input barcode to the inventory
//		If there is demand for the item, then the function
//		will decrement the demand.
bool ReturnItem(string barcode, list<StoreItem*> inventory)
{
	// Create an iterator for traversal
	list<StoreItem*>::iterator returnIterator;

	// Iterate through all items in list
	for (returnIterator = inventory.begin(); returnIterator != inventory.end(); ++returnIterator) {

		if ((*returnIterator)->getBarcode() == barcode) {
			
			// If the item already has demand
			// because we assume that if m_demand > 0
			// then m_copy must be == 0
			if ((*returnIterator)->getDemand() != 0 )
				// Decrease demand
				(*returnIterator)->decreaseDemand();

			// If there is no present demand
			else 
				// Increase supply
				(*returnIterator)->increaseCopy();

			// An item matches the barcode and has been returned
			return true;
		}
	}

	// The barcode does not match any item
	return false;
}

// 'C': Checks out an item of the input barcode from
//		the inventory. If no more of that item is present,
//		then the function will increment the demand.
bool CheckoutItem(string barcode, list<StoreItem*> inventory)
{
	// Create iterator for traversal
	list<StoreItem*>::iterator checkoutIterator;

	// Iterate through all items in the list
	for (checkoutIterator = inventory.begin(); checkoutIterator != inventory.end(); ++checkoutIterator) {

		if ((*checkoutIterator)->getBarcode() == barcode) {

			// If there are no copies left
			// increase the number of copie demanded
			if ((*checkoutIterator)->getCopy() == 0)
				// Increase demand
				(*checkoutIterator)->increaseDemand();

			// If there is any copies to checkout
			else
				// Decrease supply
				(*checkoutIterator)->decreaseCopy();

			// An item matches the barcode and has been returned
			return true;
		}
	}

	// The barcode does not match any item
	return false;
}