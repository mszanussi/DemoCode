/*
 * File:	Menu.cpp
 * Author:	Zhijiang Dong
 * Course:	CSCI 2170
 * Instructor:	Zhijiang Dong
 * Date:	Oct. 06 2006
 *
 * Purpose: This file is a part of the 4th open lab assignment, which 
 * includes StockItem.h, StockItem.cpp, Inventory.h, Inventory.cpp, 
 * Menu.h, Menu.cpp and main.cpp. This file gives the full definition 
 * of the class StockItem.
 */

#include <iostream>
#include <cstdio>
#include <string>
#include <stdexcept>
#include <sstream>

#include "Menu.h"

using namespace std;

//default constructor
//create a graph at the page 748 for debug purpose
Menu::Menu()
{
    m_graph.addVertex( "a" );
    m_graph.addVertex( "b" );
    m_graph.addVertex( "c" );
    m_graph.addVertex( "d" );
    m_graph.addVertex( "e" );
    m_graph.addVertex( "f" );
    m_graph.addVertex( "g" );
    m_graph.addVertex( "h" );
    m_graph.addVertex( "i" );

    m_graph.addEdge("a", "b", 6); 
    m_graph.addEdge("a", "f", 4); 
    m_graph.addEdge("a", "i", 2); 

    m_graph.addEdge("b", "c", 7); 
    m_graph.addEdge("b", "e", 9); 

    m_graph.addEdge("c", "d", 4); 
    m_graph.addEdge("c", "e", 3); 

    m_graph.addEdge("d", "g", 5); 
    m_graph.addEdge("d", "h", 1); 

    m_graph.addEdge("e", "g", 8); 

    m_graph.addEdge("f", "g", 2); 
}
void Menu::execution( void ) 
{
	while ( true ) // infinite loop	
	{
		// Display the main menu
		system("cls");
		displayMainMenu();

		// read the command
		char		command;
		cin >> command;
		cin.ignore(250, '\n');
		cout << endl;
        try {
		    switch ( command )
		    {
			    case '1': 	addVertex();
						    break;
                case '2':
                            removeVertex();
                            break;
                case '3':
                            addEdge();
                            break;
                case '4':
                            removeEdge();
                            break;
                case '5':
                            display();
                            break;
                case '6':
                            DFS();
                            break;
                case '7':
                            BFS();
                            break;
                case '8':
                            shortestPath();
                            break;
			    case '9': 	
							return;  // terminate the loop
			    default:	// wrong command
						    cout << "You input a wrong command!" << endl;
		    }
        } catch (GraphException e)
        {
            cout << e.what() << endl;
        }
		cout << "Press Enter to continue...";
		cin.ignore(200, '\n');
	}
}

void Menu::displayMainMenu( void ) const
{
	cout << "******************************************************" << endl;
	cout << "* Command   Description                              *" << endl;
	cout << "* 1         Add vertices                             *" << endl;
	cout << "* 2         Remove an vertex                         *" << endl;
    cout << "*                                                    *" << endl;
	cout << "* 3         Add edges                                *" << endl; 
	cout << "* 4         Remove an edge                           *" << endl; 
    cout << "*                                                    *" << endl;
    cout << "* 5         Display the graph                        *" << endl;
    cout << "*                                                    *" << endl;
    cout << "* 6         Depth-first search spanning tree         *" << endl;
    cout << "* 7         Breadth-first search spanning tree       *" << endl;
    cout << "*                                                    *" << endl;
    cout << "* 8         Find shortest path                       *" << endl;
    cout << "*                                                    *" << endl;
    cout << "* 9         Quit                                     *" << endl;
	cout << "******************************************************" << endl;

	cout << "Please enter your command [1-9]: ";
}


void Menu::addVertex( void ) 
{
	string		name;

    cout << "Please enter node name (type q to quit): ";
    cin >> name;
    cin.ignore(200, '\n');

    while ( name != "q" )
    {
        m_graph.addVertex( name );
        cout << "Please enter node name (type q to quit): ";
        cin >> name;
        cin.ignore(200, '\n');
    }
	return;
}


void Menu::removeVertex( void )
{
    string  name;

    cout << "Please enter node name: ";
    cin >> name;
    cin.ignore(200, '\n');

    m_graph.removeVertex( name );
	return;
}

void Menu::addEdge( void )
{
	string		src, dest;
    int         weight;

    cout << "Please enter edge info (src dest weight): ";
    cin >> src;
    cin >> dest;
    cin >> weight;
    cin.ignore(200, '\n');

    while ( weight != -1 )
    {
        m_graph.addEdge(src, dest, weight);
        cout << "Please enter edge info (src dest weight): ";
        cin >> src;
        cin >> dest;
        cin >> weight;
        cin.ignore(200, '\n');
    }
	return;
}


void Menu::removeEdge( void )
{
	string		src, dest;

    cout << "Please enter edge (src dest): ";
    cin >> src;
    cin >> dest;
    cin.ignore(200, '\n');

    m_graph.removeEdge(src, dest);
	return;
}

void Menu::display( void ) const
{
    cout << m_graph << endl;
}

void Menu::DFS( void ) 
{
    string  name;

    cout << "Please enter starting vertex: ";
    cin >> name;
    cin.ignore(200, '\n');

    m_graph.DFS(name);
}

void Menu::BFS( void ) 
{
    string  name;

    cout << "Please enter starting vertex: ";
    cin >> name;
    cin.ignore(200, '\n');

    m_graph.BFS(name);
}

void Menu::shortestPath( void ) 
{
    string  name;

    cout << "Please enter starting vertex: ";
    cin >> name;
    cin.ignore(200, '\n');

    m_graph.shortestPath(name);
}
