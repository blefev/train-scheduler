#ifndef GRAPH_H
#define GRAPH_H

#include <utility>
#include <cstddef>
#include <climits>
#include <iostream>
#include <iomanip>
#include <stack>
#include <queue>
#include <set>
#include <vector>

typedef std::pair<int,int> sched;

using namespace std;


class GRAPH{
public:
	// Constructor to use when we have an initial number of nodes & edges
	GRAPH(int nodes);
	// Set the edge going from src to dst
	void set_edge(int src, int dst, pair<int,int> schedule);
	// Print the graph's data
	void print_data(void);
	// find station schedule by id
	vector<sched*>* get_schedule(int stationId);
	bool empty(vector<bool>);
	// Perform a dfs from the provided start node. Returns a vector containing
	// the search results
	vector<int> *dfs(int start);
	// perform a bfs from the provided start node. Returns a vector containing
	// the search results
	vector<int> *bfs(int start);
	// Use dijkstra's to find the shortest distance from a source node to 
	// a destination node. Returns the minimum distance between the two or
	// -1 if a path does not exist
	int dijkstra(int, int);

private:
	// keep track of the number of nodes in the graph
	int node_count;
	// this represents the adjacency matrix
	sched ***data;
};
#endif
