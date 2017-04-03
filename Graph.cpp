#include "Graph.h"

/*
* Read all the given files to initialize the adjency list
*/
Graph::Graph(const string pNodesPath, const string pEdgesPath, const bool pDirected)
{
	ifstream nodesFile(pNodesPath);
	string currentLine;
	int linesCounter = 0;

	if (!nodesFile.is_open()) {
		throw "Exception: Nodes file could not be opened";
	}

	while (getline(nodesFile, currentLine))
	{
		/* As the first row (number of nodes) is not needed, it'll be skipped */
		if (linesCounter++ > 0) 
		{
			/* Create a node for every remaining line of the node file 
			and insert it in the nodes list  */
			nodes.insertAt(0, new Node(currentLine));
		}
	}
	nodesFile.close();

	ifstream edgesFile(pEdgesPath);
	linesCounter = 0;

	if (!edgesFile.is_open()) {
		throw "Exception: Connections file could not be opened";
	}

	while (getline(edgesFile, currentLine))
	{
		/* As the first row (number of connections) is not needed, it'll be skipped */
		if (linesCounter++ > 0)
		{
			/* As every column in the file has 3 rows of information
			the row will be split into 3 strings with the helper function splitString*/
			string currentLineSplit[3];
			splitString(currentLine, ' ', currentLineSplit, 3);

			/* Find the nodes with corresponding names of the two first rows of data*/
			Node * currentNode = findNode(currentLineSplit[0]);
			Node * nextNode = findNode(currentLineSplit[1]);

			/* Insert an edge to the first node with a pointer to the neighbor node and the cost (3d row) */
			currentNode->edges.insertAt(0, Edge(nextNode, atoi(currentLineSplit[2].c_str())));

			/* If the list is not directed, insert an edge to the other node aswell
			with a pointer to the other node and the same cost */
			if (!pDirected) {
				nextNode->edges.insertAt(0, Edge(currentNode, atoi(currentLineSplit[2].c_str())));
			}
		}
	}
	edgesFile.close();

	/*
	// Debug all nodes with their edges
	for (int i = 0; i < nodes.size(); i++)
	{
		Node * node = nodes[i];

		cout << node->name;

		for (int j = 0; j < node->edges.size(); j++)
		{
			Edge edge = node->edges[j];

			cout << " (" << edge.node->name << ", " << edge.cost << ")";
		}

		cout << endl;
	}
	
	cout << endl;
	*/
}

/* Free all memory associated with the graph (the nodes) */
Graph::~Graph()
{
	for (int i = 0; i < nodes.size(); i++) {
		delete nodes[i];
	}
}

/* This helper function will split a string with the given delimeter and put the new strings in an given array
as long as the array don't exceed the given maxLength */
void Graph::splitString(const string pString, const char pDel, string pArr[], const int pMaxLength)
{
	string currLine = "";
	int currLength = 0;

	/* Iterate the string char by char */
	for (int i = 0; i < (signed) pString.length(); i++)
	{
		/* If the char is not equal to the delimeter, add the char to the current line*/
		if (pString[i] != pDel) {
			currLine += pString[i];
		}
		/* If the char IS equal to the delimeter 
		and the max length of the array has not been exceeded
		then put the current string in the array and reset the current line*/
		else
		{
			if (currLength < pMaxLength)
			{
				pArr[currLength++] = currLine;
				currLine = "";
			}
		}
	}

	/* Insert the remaining line into the array */
	if (currLength < pMaxLength) {
		pArr[currLength++] = currLine;
	}
}

/* Find the shortest path between two nodes with Dijkstras shortest path algorithm*/
string Graph::findShortestPath(const string pStart, const string pEnd)
{
	/*-------------------------------------------*/
	/*---------FIND THE SHORTEST PATH------------*/
	/*-------------------------------------------*/

	/* Reset temporay data with the nodes helper function resetTempData */
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->resetTempData();
	}

	/* Find the node with the corresponding name of the start nodes name*/
	Node * currentNode = findNode(pStart);

	if (currentNode == nullptr) {
		throw "Exception: Start node not found";
	}

	/* Initialize the cost to 0 as it'll cost nothing to go itself */
	currentNode->tempCost = 0;

	while (currentNode != nullptr)
	{
		/* Mark the node as visited */
		currentNode->tempVisited = true;

		
		// Debug current nodes name
		//cout << "Current: " << currentNode->name << endl;
		

		/* Iterate all the nodes edges */
		for (int i = 0; i < currentNode->edges.size(); i++)
		{
			Edge edge = currentNode->edges[i];
			Node * node = edge.node;

			/* Make sure the node has yet not been visisted*/
			if (!node->tempVisited)
			{
				/* Calculate the cost to go to the neighbor defined by the edge*/
				int newCost = currentNode->tempCost + edge.cost;

				/* Make sure that the current cost of the neighbor is less than the new cost */
				if (node->tempCost == INF || node->tempCost > newCost)
				{
					/* Update the neighbors cost */
					node->tempCost = newCost;

					/* Update the neighbors  */
					node->tempPath = currentNode;
				}

				
				// Debug current nodes edges 
				//cout << node->name << ": " << node->tempCost << (node->tempVisited ? " (visited)" : "") << endl;
				
			}
		}

		
		//Debug some spaces
		//cout << endl;
		
		/* Find the cheapest node */
		currentNode = findCheapestNode();
	}

	/*-------------------------------------------*/
	/*---TRACE THE GOAL NODE TO THE START NODE---*/
	/*-------------------------------------------*/

	/* Find the node with corresponding name of the end name*/
	currentNode = findNode(pEnd);

	if (currentNode == nullptr) {
		throw "Exception: End node not found";
	}

	/* Store the cost of this node */
	int totalCost = currentNode->tempCost;
	
	/* This list will store the shortest path from the start to the end node*/
	List<string> trace;
	
	/* Fill the trace list */
	while (currentNode != nullptr)
	{
		/* Insert the name of the current node */
		trace.insertAt(0, currentNode->name);

		/* Assign the current node to the node that has the shortest path */
		currentNode = currentNode->tempPath;
	}

	/*-------------------------------------------*/
	/*-------CREATE THE RESULT INFORMATION-------*/
	/*-------------------------------------------*/
	stringstream result;

	/* Make sure a path was actually found*/
	if (totalCost == INF) {
		result << "No path was found between " << pStart << " and " << pEnd;
	}
	else 
	{
		/* Iterate the trace list */
		for (int i = 0; i < trace.size(); i++)
		{
			/* Extend the result string with the name */
			result << trace[i];

			if (i < trace.size() - 1) {
				result << " - ";
			}
		}

		/* Extend the result string with the total cost */
		result << ": " << totalCost << " timmar";
	}

	return result.str();
}

/* This helper function will reset the temp data of a node*/
void Graph::Node::resetTempData()
{
	tempCost = INF;
	tempVisited = false;
	tempPath = nullptr;
}

/* Find the cheapest unvisited node*/
Graph::Node * Graph::findCheapestNode() const
{
	Node * n = nullptr;

	for (int i = 0; i < nodes.size(); i++)
	{
		if (!nodes[i]->tempVisited && nodes[i]->tempCost != INF && (n == nullptr || nodes[i]->tempCost < n->tempCost))
		{
			n = nodes[i];
		}
	}

	return n;
}

/* Find a node with a given corresponding name */
Graph::Node * Graph::findNode(const string pName) const
{
	Node * node = nullptr;

	/* Iterate all nodes */
	for (int i = 0; i < nodes.size() && node == nullptr; i++)
	{
		/* Make sure that the nodes name equals the given name */
		if (nodes[i]->name == pName){
			node = nodes[i];
		}
	}

	return node;
}