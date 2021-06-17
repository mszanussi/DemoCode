/*
* Author: Milan Zanussi
* Date: October 8th, 2016
* File: Movie.cpp
* This file contains the implementation for
* the functions defined in the Book class.
*/

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include "Book.h"

using namespace std;

//default constructor setting every thing to empty or 0
Book::Book(string title, string author, string published,
			string barcode, double price, int copy, int demand)
	: StoreItem(barcode, price, copy, demand), m_title(title), m_author(author), m_published(published) {}

std::string Book::getTitle() const
{
	return m_title;
}

std::string Book::getAuthor() const
{
	return m_author;
}

//This function returns a dynamically created StoreItem object
//which is created from a string.
//The string info must be of the line format specificed in
//inventory.txt
Book* Book::createFromString(const std::string info) 
{
	stringstream lineInfo(info);
	string inputString;

	// Gets barcode of the book
	getline(lineInfo, inputString, DELIMITER);
	m_barcode = inputString;

	// Gets title of the book
	getline(lineInfo, inputString, DELIMITER);
	m_title = inputString;

	// Gets author of the book
	getline(lineInfo, inputString, DELIMITER);
	m_author = inputString;

	// Gets publishing date of the book
	getline(lineInfo, inputString, DELIMITER);
	m_published = inputString;

	// Gets price of the book
	lineInfo >> m_price;
	getline(lineInfo, inputString, DELIMITER);

	// Gets copy count of the book
	lineInfo >> m_copy;
	getline(lineInfo, inputString, DELIMITER);

	// Gets supplier's demand for the book
	lineInfo >> m_demand;
	getline(lineInfo, inputString, DELIMITER);

	return this;
}

//Print current item to the screen
void Book::printItem()
{
	cout << endl << "******************** BOOK BARCODE: " << m_barcode << " ********************" << endl;

	// Print all characteristic content for the movie
	cout << left << setw(20) << "Title:" << m_title << endl
				<< setw(20) << "Author(s):" << m_author << endl
				<< setw(20) << "Publishing Date:" << m_published << endl
				<< setw(20) << "Price:" << m_price << endl
				<< setw(20) << "Number of Copies:" << m_copy << endl
				<< setw(20) << "Number of Demanded:" << m_demand << endl
				<< endl; // Add one extra newline for spacing
}