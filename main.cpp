#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <unistd.h>
#include "graph.h"
#include "pretty_print.h"
using namespace std;

int main(int argc, char **argv)
{
	GRAPH *g = NULL;
	vector<int> *traversal;
	vector<int> *min_tree;
	ifstream file;
	int node_count;
	int nodes, edges, source, destination, cmd, p1, p2, w;
	int mode;
	int weight;
	int i;
	bool directed;

    if(argc < 2)
    {
        cout << "useage: ./graph.out edges.txt cmd.txt\n";
        return -1;
    }
    
    // open the file which contains the edge information
    file.open(argv[1]);
	if (!file.is_open())
	{
		cout << "Error: Failed to open input file\n";
		return 0;
	}

	// check if this is a directed graph
	file >> mode;

	// get the number of nodes
	file >> node_count;

	// initalize the new graph
	g = new GRAPH(directed, node_count);

	// read in the edges of our graph
	while (file >> source)
	{                
		file >> destination;
		file >> weight;
		g->set_edge(source, destination, weight);
	}
	file.close();

	// open the file that contains the cmd information
    file.open(argv[2]);
	if (!file.is_open())
	{
		cout << "Error: Failed to open cmd file\n";
		return 0;
	}

	while (file >> cmd)
	{
		switch (cmd) 
		{
			case 1:
				file >> p1;
				traversal = g->dfs(p1);
				if(NULL == traversal)
				{
					cout << "Could not complete traversal\n";
				}
				cout << "DFS from " << p1 << ":\n";
				for(i=0;i<traversal->size();i++)
				{
					cout << traversal->at(i) << " ";
				}
				cout << endl;
				break;
			case 2:
				file >> p1;
				traversal = g->bfs(p1);
				if(NULL == traversal)
				{
					cout << "Could not complete traversal\n";
				}
				cout << "BFS from " << p1 << ":\n";
				for(i=0;i<traversal->size();i++)
				{
					cout << traversal->at(i) << " ";
				}
				cout << endl;
				break;
			case 3:
				file >> p1;
				file >> p2;
				i = g->dijkstra(p1, p2);
				cout << "Distance from " << p1 << " to " << p2 << ": " << i << endl;
				break;
			case 4:
				file >> p1;
				min_tree = g->primm(p1);
				pretty_print_mst(min_tree, p1);
				break;
			default:
				cout << "Error: Unknown cmd\n";
		}
	}
	
	file.close();
	return 0;
}
