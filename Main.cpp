#include "Graph.h"

#include <iostream>
#include <string>

using namespace std;

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	try 
	{
		/* Create a graph with a nodes list and connections list
		the graph should not be directed so set that to false */
		Graph g("cities.txt", "connections.txt", false);

		/* Find the shortest path between some nodes and print the result*/
		cout << "L->K: " << g.findShortestPath("Lstad", "Kstad") << endl;
		cout << "A->S: " << g.findShortestPath("Astad", "Sstad") << endl;
	}
	catch (char * e) {
		cout << e << endl;
	}

	cout << endl;

	return 0;
}