#include <iostream>
//#include <stack>
#include <queue>

#include "Graph.h"

using namespace std;


// Add a vertex
// precond: node never appear in the graph before
void Graph::addVertex(const std::string& node)
{

	// Disallow naming a node nothing
	if (node.empty()) {

		throw GraphException("Node name cannot be empty string.");

		return;
	}

	// Node not found in graph
	// => Insert node into graph
	if (adjMatrix.find(node) == adjMatrix.end()) {

		pair<string, map<string, int>> nodePair;
		nodePair.first = node;

		adjMatrix.insert(nodePair);

		++nodeCount;
	}

	// node is in the graph already
	// => Do nothing
	return;
}

// Add an edge.
// Throw a GraphException if vertices of this
// node are not in this graph
// pre-cond: the edge between srcName and destName does not exist
void Graph::addEdge(std::string srcName, std::string destName, int weight) throw (GraphException)
{
	// Either node does not exist in the graph
	// => edge cannot be connected & throw exception
	if (adjMatrix.find(srcName) == adjMatrix.end() 
		|| adjMatrix.find(destName) == adjMatrix.end()) {

		// Throw the first node out of the two nodes not found in the graph
		throw GraphException((adjMatrix.find(srcName) != adjMatrix.end() ? srcName : destName) + " not found in graph.");

		return;

	}

	// Both vertices in graph
	// => Try to create edge or replace weight if edge exists
	else {

		adjMatrix[srcName][destName] = weight;
		adjMatrix[destName][srcName] = weight;

	}

	return;

}

// Remove a vertex and related arcs
// Throw a GraphException if such vertex does not exist
void Graph::removeVertex(std::string nodeName) throw(GraphException)
{
	// Node not found in graph
	// => Node cannot be removed (throw exception)
	if (adjMatrix.find(nodeName) == adjMatrix.end()) {

		throw GraphException(nodeName + " not found in graph.");

		return;

	}

	// Remove all edges related to the vertex nodeName
	for (auto it : adjMatrix)
	{
		removeEdge(nodeName, it.first);
	}

	// Remove node from the adjMatrix
	adjMatrix.erase(nodeName);

	--nodeCount;

	return;

}

// Remove an edge 
// Throw a GraphException if both vertices are not in the graph
// Do nothing if the edge does not exist
void Graph::removeEdge(std::string srcName, std::string destName) throw(GraphException)
{
	// Either node does not exist in the graph
	// => edge cannot be connected & throw exception
	if (adjMatrix.find(srcName) == adjMatrix.end()
		|| adjMatrix.find(destName) == adjMatrix.end()) {

		// Throw the first node out of the two nodes not found in the graph
		throw GraphException((adjMatrix.find(srcName) != adjMatrix.end() ? srcName : destName) + " not found in graph.");

	}

	// Both vertices in graph
	// => Erase the edge assuming it exists
	else {

		adjMatrix[srcName].erase(destName);
		adjMatrix[destName].erase(srcName);

	}
	
	return;

}

// Depth-first traversal: visit ALL vertices once
// During the visit, print edges in the order they are visited
// If graph has no edge, print some information
void Graph::DFS(const std::string& source)
{
	// Set of visited nodes
	set<string> visited;

	// Call private function for DFS
	DFS(source, visited);
	
	// Remove source node from set to
	// check if any edges were crossed
	visited.erase(source);

	// No nodes visited => inform user
	if (visited.empty()) {
		cout << endl << "No edges in Depth-First search.";
	}

	// Not all nodes were visited
	// => Graph is disconnected
	else if (visited.size() < nodeCount - 1) {
		cout << endl << "Graph disconnects.";
	}

	cout << endl << endl;

	return;
}

// Breadth-first traversal: visit ALL vertices once
// During the visit, print edges in the order they are visited
// If graph has no edge, print some information
void Graph::BFS(const std::string& source)
{
	set<string> visited;
	queue<string> visitQueue;

	visitQueue.push(source);

	visited.insert(source);

	int iterations = 0;

	// While there are nodes to visit
	while (!visitQueue.empty()) {

		// For all nodes adjacent to current node
		for (auto i : adjMatrix[visitQueue.front()]) {

			// Node hasn't been visited
			// => Add it to the queue and mark it as visited
			if (visited.find(i.first) == visited.end()) {
				visitQueue.push(i.first);
				visited.insert(i.first);

				// Print traversed edge
				cout << '(' << visitQueue.front() << ", " << i.first << ") ";

				// Number of iterations = 4
				// => print next items on new line
				if (++iterations == 4) {
					cout << endl;
					iterations = 0;
				}
			}
			
		}

		// Pop current node from queue
		visitQueue.pop();
	}

	// Remove source from visited to check
	// if any edges have been visited
	visited.erase(source);

	// No nodes other than source visited
	// => No edges crossed
	if (visited.empty()) {
		cout << endl << "No edges in Breadth-First search.";
	}

	// Not all nodes were visited
	// => Graph is disconnected
	else if (visited.size() < nodeCount - 1) {
		cout << endl << "Graph disconnects.";
	}

	cout << endl << endl;


	return;
}

// Find shortest path from the node "source"
void Graph::shortestPath(const std::string& source)
{

	// Specified source node does not exist
	if (adjMatrix.find(source) == adjMatrix.end()) {
		throw GraphException(source + " not found in graph.");
		return;
	}

	set<string> shortestFound;

	// Map of nodes and their shortest paths
	map<string, pair<int, string>> pathList;

	// List the source as found
	shortestFound.insert(source);

	// For all vertices, add it to pathList
	for (auto i : adjMatrix) {

		// Node isn't source
		// => Shortest path exists
		if (source != i.first) {

			pair<string, pair<int, string>> initPath;

			initPath.first = i.first;

			// Source connects to the current node
			// => Make the initial weight the edge weight
			if (adjMatrix[source].find(i.first) != adjMatrix[source].end()) {

				(initPath.second).first = adjMatrix[source][i.first];
				(initPath.second).second = source;

			}

			// Source does not connect to node found
			else {

				// Empty string indicates no nodes connect
				(initPath.second).second = "";

			}

			// Insert information into map
			pathList.insert(initPath);

		}

	} // END FOR LOOP

	cout << "Shortest Path:" << endl;

	int iterations = 0;

	// While the shortestFound set is not full
	while (shortestFound.size() < map<string, pair<int,string>>::size_type(nodeCount)) {

		pair<string, pair<int, string>> shortestPath;

		// Find a shortest path
		shortestPath = findShortest(pathList, shortestFound);

		if ((shortestPath.second).second.empty()) {
			cout << endl << "Graph disconnects.";
			break;
		}
		// Print current shortest path
		cout << '(' << (shortestPath.second).second << ", " << (shortestPath.first) << ") ";

		// Number of iterations = 4
		// => print next items on new line
		if (++iterations == 4) {
			cout << endl;
			iterations = 0;
		}

		// For all adjacent nodes to the found minimum
		for (auto k : adjMatrix[shortestPath.first]) {

			// Shortest path for node corresponding to k hasn't been found
			if (shortestFound.find(k.first) == shortestFound.end()) {

				// Currently known shortest path to k is greater than
				// weight of shortestPath plus weight of adjacency
				// or no path has been found yet to that node
				if ( (pathList[k.first].second).empty() || pathList[k.first].first > (shortestPath.second).first + k.second) {

					// Set current shortest known path to k to sum of path weight and edge weight
					pathList[k.first].first = (shortestPath.second).first + k.second;

					// Set the current known previous path to the node of the current shortest path
					pathList[k.first].second = shortestPath.first;

				} // END IF

			} // END IF

		} // END FOR LOOP


	} // END WHILE LOOP

	cout << endl << endl;

	return;

}

// print the graph
std::ostream& operator << (std::ostream& os, const Graph& rhs)
{
	// Stack to store edges in graph

	os << "Vertices:" << endl;

	// Print all vertices of graph
	for (auto it :rhs.adjMatrix) {
		os << it.first << ' ';
	}

	// Space out vertices from edges
	os << endl << endl << "Edges:" << endl;

	int iterations = 0;

	// For each vertex in the matrix
	for (auto i : rhs.adjMatrix) {

		// For each vertex i connects to
		// Print the node pair and weight
		for (auto j : i.second) {
			os << '(' << i.first << ", " << j.first << ", " << j.second << ") ";

			// Number of iterations = 5
			// => print next items on new line
			if (++iterations == 5) {
				cout << endl;
				iterations = 0;
			}
		}

	}

	// Make space
	cout << endl << endl;

	return os;
}

// Depth-first traversal: takes input of a node to visit
// and a list of nodes that've already been visited
void Graph::DFS(const std::string& source, std::set<std::string>& visitedNodes)
{
	// Mark current node as visited
	visitedNodes.insert(source);

	// For all nodes connected to source
	for (auto i : adjMatrix[source]) {
		// Connected node hasn't been visited
		// => Cross the edge and visit the node
		if (visitedNodes.find(i.first) == visitedNodes.end()) {

			// Print edge traversed
			cout << '(' << source << ", " << i.first << ") ";

			// Recursive call on next node to visit
			DFS(i.first, visitedNodes);

		}
	}

	return;
}


// Find the shortestPath given the current options
// This private function is used in the shortestPath
// function.
pair<string, pair<int, string>> Graph::findShortest(const map<string, pair<int, string>>& pathList, set<string>& shortestFound )
{
	pair<string, pair<int, string>> shortestPath;

	// No minimum exists yet for the iteration
	(shortestPath.second).second = "";

	// For each node whose shortest path hasn't been found yet
	for (auto j : pathList) {

		// Path at j hasn't had its shortest path from source found yet
		if (shortestFound.find(j.first) == shortestFound.end()) {

			// A minimum hasn't been declared yet or the
			// item at j weighs less than the current minimum and
			// connects to something
			if (((shortestPath.second).second).empty() 
				|| (((shortestPath.second).first > (j.second).first) && !(j.second).second.empty())) {

				shortestPath.first = j.first;
				(shortestPath.second).first = (j.second).first;
				(shortestPath.second).second = (j.second).second;

			} // END IF

		} // END IF

	} // END FOR LOOP

	// Any path was found
	// => Insert found minimum into shortestFound
	if( !(shortestPath.second).second.empty())
		shortestFound.insert(shortestPath.first);

	return shortestPath;
}