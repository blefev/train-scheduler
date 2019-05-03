 #ifndef GRAPH_H
#define GRAPH_H

#define MAX_NODES 199

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
	GRAPH();
	// Set the edge going from src to dst
	void set_edge(int src, int dst, sched schedule);

	bool edge_exists(int src, int dst);

	// find station schedule by id
	vector<vector<int> > station_schedule(int stationId);

	// find schedule for train
	sched train_schedule(int src, int dst);

	bool empty(vector<bool>);

	// Perform a service_available from the provided start node. Returns a vector containing
	// the search results
	bool service_available(int start, int dst, bool nonstop);

	vector<int> path(int, int, bool);

    int t_diff (int a, int b);
private:
	// keep track of the number of nodes in the graph
	int node_count;
	// this represents the adjacency matrix
    sched data[MAX_NODES+1][MAX_NODES+1];
};
#endif
