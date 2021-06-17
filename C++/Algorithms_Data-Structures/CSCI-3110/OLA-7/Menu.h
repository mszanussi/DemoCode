/*
 * File:	Menu.h
 * Author:	Zhijiang Dong
 * Course:	CSCI 2170
 * Instructor:	Zhijiang Dong
 * Date:	Oct. 06 2006
 *
 * This file gives the full definition 
 * of the class Menu.
 */

#ifndef _MENU_H_
#define _MENU_H_

#include <string>

#include "Graph.h"

class Menu
{
	public:
        //default constructor
        //create a graph at the page 748 for debug purpose
        Menu();
		// This function  displays the main menu, reads
		// the command and executes it.
		void execution( void );

	private:

		// This function displays the main menu
		void displayMainMenu( void ) const;

		void addVertex( void );

		void addEdge( void );

        void removeVertex( void );

        void removeEdge( void );

        void DFS( void );

        void BFS( void );

        void shortestPath( void );

        void display( void ) const;


	private:
		Graph       m_graph; // The inventory associated with the menu 
};
	
#endif
