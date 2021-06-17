/*
* Author: Milan Zanussi
* Date: October 8, 2016
* File: StoreItem.cpp
* As a part of the 3110 assignment, this file
* implements the functions of the StoreItem class
*/

#include "StoreItem.h"
#include <iostream>

//default constructor setting every thing to empty or 0
StoreItem::StoreItem(std::string barcode, double price, int copy, int demand)
	: m_barcode(barcode), m_price(price), m_copy(copy), m_demand(demand) {}

//Get item barcode
std::string StoreItem::getBarcode(void) const
{
	return m_barcode;
}

//Get price
double StoreItem::getPrice(void) const
{
	return m_price;
}

//Get number of copies in the store
int StoreItem::getCopy(void) const
{
	return m_copy;
}

//Get number of copies to be ordered
int StoreItem::getDemand(void) const
{
	return m_demand;
}

//Increase # of copies by 1
//precondition: m_copy >= 0
void StoreItem::increaseCopy(void)
{
	++m_copy;
}

//Decrease # of copies by 1
//precondition: m_copy > 0
void StoreItem::decreaseCopy(void)
{
	--m_copy;
}

//Increase # of copies to be ordered by 1
//precondition: m_copy == 0
void StoreItem::increaseDemand(void)
{
	++m_demand;
}

//Decrease # of copies to be ordered by 1
//precondition: m_demand > 0
void StoreItem::decreaseDemand(void)
{
	--m_demand;
}

//Compare two store items. Return true if they have the same barcode,
//otherwise false.
bool StoreItem::operator == (const StoreItem& rhs) const
{
	return (m_barcode == rhs.m_barcode);
}

//Compare two store items by barcode
bool StoreItem::operator < (const StoreItem& rhs) const
{
	return (m_barcode < rhs.m_barcode);
}
