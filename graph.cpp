#include "graph.h"
//#include "pq.hpp"


GRAPH::GRAPH(int nodes)
{
	this->directed = false;

	this->node_count = nodes;

	this->data = new int*[nodes];
	for (int i = 0; i <= nodes; i++) {
		this->data[i] = new int[nodes];
		for (int j = 0; j <= nodes; j++) {
			this->data[i][j] = 0;
		}
	}
}

GRAPH::GRAPH(bool is_directed, int nodes)
{
	this->directed = is_directed;
	this->node_count = nodes;

	this->data = new int*[nodes];
	for (int i = 0; i <= nodes; i++) {
		this->data[i] = new int[nodes];
		for (int j = 0; j <= nodes; j++) {
			this->data[i][j] = 0;
		}
	}
}
void GRAPH::set_edge(int source, int destination, int w)
{
	if (!this->directed) {
		this->data[source][destination] = w;
	}
	this->data[source][destination] = w;
}

void GRAPH::print_data()
{
	std::cout<<"    ";

	for (int i=0; i <= this->node_count; i++) {
		std::cout << setw(3)<<i;
	}
	std::cout << std::endl;

	for (int i=0; i <= this->node_count; i++) {
		std::cout<<i<<" |";
		for (int j=1; j <= this->node_count; j++) {
			std::cout<<setw(3)<<this->data[i][j];
		}
		cout << std::endl;
	}
}

vector<int> *GRAPH::dfs(int start)
{
	vector<int> *traversal = new vector<int>();
	std::set<int> discovered;

	std::stack<int> st;
	st.push(start);

	while (!st.empty()) {
	int cur = st.top();
	st.pop();
	if (discovered.count(cur) == 0) {
		traversal->push_back(cur);
		discovered.insert(cur);
		for (int i=0; i <= this->node_count; i++) {
			if (this->data[cur][i] > 0) {
				st.push(i);
			}
		}
	}
	}

	return traversal;
}

vector<int> *GRAPH::bfs(int start)
{
	vector<int> *traversal = new vector<int>();

	std::queue<int> frontier;
	std::set<int> discovered;

	frontier.push(start);
	discovered.insert(start);

	while(!frontier.empty()) {
		int cur = frontier.front();
		frontier.pop();
		traversal->push_back(cur);

		for (int i=0; i <= this->node_count; i++) {
			if (this->data[cur][i] > 0) {

				std::cout<<"Found adjv " << i << std::endl;
				if (discovered.count(i) == 0) {
					frontier.push(i);
					discovered.insert(i);
				}
			}
		}
	}

	return traversal;
}

bool GRAPH::empty(vector<bool> set) {
	for (int i =0; i < set.size(); i++) {
		if (set[i] == false) {
			return false;
		}
	}
	return true;
}

int GRAPH::dijkstra(int src, int dst)
{
	std::vector<bool> vertexSet;
	int distances[this->node_count];
	int minNode;
	int minDistance;

	for (int i=0; i < this->node_count; i++) {
		distances[i] = INT_MAX;
		vertexSet.push_back(false);
	}

	distances[src] = 0;

	while (!empty(vertexSet)) {
		minNode = INT_MAX;
		minDistance = INT_MAX;

		for (int vert=0; vert < this->node_count; vert++) {
			if (vertexSet[vert] == false && distances[vert] <= minDistance) {
				minDistance = distances[vert];
				minNode = vert;
			}
		}

		std::cout << "minNode: " << minNode << endl;

		vertexSet[minNode] = true;


		for (int vert=0; vert < this->node_count; vert++) {
			if (vertexSet[vert] == false && this->data[minNode][vert]) {
				if (distances[minNode] != INT_MAX && distances[minNode] + this->data[minNode][vert] < distances[vert]) {
					distances[vert] = distances[minNode] + this->data[minNode][vert];
				}
			}
		}
	}
	return distances[dst];
}

vector<int> *GRAPH::primm(int src)
{

}
