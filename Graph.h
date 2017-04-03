#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "List.h"

using namespace std;

class Graph
{
	static const int INF = -1;

private:
	class Node;
	class Edge
	{
	public:
		Node * node;
		int cost;
		
		Edge() { node = nullptr; cost = INF; }
		Edge(Node * pNode, const int pCost) { node = pNode; cost = pCost; }
	};

	class Node
	{
	public:
		string name;
		List<Edge> edges;

		int tempCost;
		Node * tempPath;
		bool tempVisited;

		Node(const string pName) { name = pName; resetTempData(); }
		void resetTempData();
	};

	List<Node *> nodes;
	void splitString(const string pString, const char pDel, string pArr[], const int pMaxLength);
	Node * findNode(const string pName) const;
	Node * findCheapestNode() const;
	
public:
	Graph(const string pNodesPath, const string pEdgesPath, const bool pDirected = true);
	~Graph();

	string findShortestPath(const string pStart, const string pEnd);
};

#endif