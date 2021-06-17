#pragma once
/*
* Author: Milan Zanussi
* Date: October 7th, 2016
* File: Book.h
* This file defines the book class
* for the console-based rental
* store software.
*/

#include <string>
#include "StoreItem.h"

class Book : public StoreItem
{
public:
	//default constructor setting every thing to empty or 0
	Book(std::string title = "", std::string author = "", std::string published = "",
		std::string barcode = "", double price = 0, int copy = 0, int demand = 0);

	// Returns book title
	std::string getTitle() const;

	// Returns book author(s)
	std::string getAuthor() const;

	//This function returns a dynamically created StoreItem object
	//which is created from a string.
	//The string info must be of the line format specificed in
	//inventory.txt
	Book* createFromString(const std::string info) override;

	//Print current item to the screen
	void printItem() override;

private:
	std::string m_title;		 // book title
	std::string m_author;		 // book author
	std::string m_published;	 // publishing date
};
