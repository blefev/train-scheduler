 #ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <utility>
#include <cstddef>
#include <climits>
#include <iostream>
#include <iomanip>
#include <stack>
#include <queue>
#include <set>
#include <vector>

typedef std::vector<int> sched;

using namespace std;


class GRAPH{
public:
	// Constructor to use when we have an initial number of nodes & edges
	GRAPH(int nodes);
	// Set the edge going from src to dst
	void set_edge(int src, int dst, sched schedule);
	// Print the graph's data
	bool service_available(int source, int destination);

	// find station schedule by id
	vector<vector<int> > station_schedule(int stationId);

	// find schedule for train
	sched train_schedule(int src, int dst);

	bool empty(vector<bool>);
	// Perform a dfs from the provided start node. Returns a vector containing
	// the search results
	vector<int> *dfs(int start);
	// perform a bfs from the provided start node. Returns a vector containing
	// the search results
	bool bfs(int start, int dst, bool nonstop );
	// Use dijkstra's to find the shortest distance from a source node to 
	// a destination node. Returns the minimum distance between the two or
	// -1 if a path does not exist
	int dijkstra(int, int);
	vector<int> path(int, int, bool);

    int t_diff (int a, int b);

    void print_itenerary(GRAPH* graph, map<int, string> &stations, vector<int> path); 
private:
	// keep track of the number of nodes in the graph
	int node_count;
	// this represents the adjacency matrix
    sched data[119][119];
};
#endif
