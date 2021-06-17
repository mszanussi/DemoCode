/*
* Author: Milan Zanussi
* Date: October 8th, 2016
* File: Movie.cpp
* This file contains the implementation for 
* the functions defined in the Movie class.
*/

#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include "Movie.h"

using namespace std;

//default constructor setting every thing to empty or 0
Movie::Movie(string title, string director,
			string barcode, double price, int copy, int demand)
	: StoreItem(barcode, price, copy, demand), m_title(title), m_director(director) {}

// Returns movie title
string Movie::getTitle() const
{
	return m_title;
}

// Returns movie director(s)
string Movie::getDirector() const
{
	return m_director;
}

//This function returns a dynamically created StoreItem object
//which is created from a string.
//The string info must be of the line format specificed in
//inventory.txt
Movie* Movie::createFromString(const string info)
{
	stringstream lineInfo(info);
	string inputString;

	// Gets the barcode of the movie
	getline(lineInfo, inputString, DELIMITER);
	m_barcode = inputString;

	// Gets the title of the movie
	getline(lineInfo, inputString, DELIMITER);
	m_title = inputString;

	// Gets the title of the movie
	getline(lineInfo, inputString, DELIMITER);
	m_director = inputString;

	// Gets price of the movie
	lineInfo >> m_price;
	getline(lineInfo, inputString, DELIMITER);

	// Gets copy count of the movie
	lineInfo >> m_copy;
	getline(lineInfo, inputString, DELIMITER);

	// Gets supplier's demand for the movie
	lineInfo >> m_demand;
	getline(lineInfo, inputString, DELIMITER);

	return this;
}

//Print current item to the screen
void Movie::printItem()
{
	cout << endl << "******************** MOVIE BARCODE: " << m_barcode << " ********************" << endl;

	// Print all characteristic content for the movie
	cout << left << setw(20) << "Title:" << m_title << endl
				<< setw(20) << "Director(s):" << m_director << endl
				<< setw(20) << "Price:" << m_price << endl
				<< setw(20) << "Number of Copies:" << m_copy << endl
				<< setw(20) << "Number of Demanded:" << m_demand << endl
				<< endl; // Add one extra newline for spacing
}