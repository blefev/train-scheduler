#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <unistd.h>
#include "graph.h"
#include <map>
using namespace std;

void showMenu();
void menuRepl();


int main(int argc, char **argv)
{
	ifstream stationsFile;
	ifstream trainsFile;

    stationsFile.open(argv[1]);
	if (!stationsFile.is_open())
	{
		cout << "Error: Failed to open input file\n";
		return 0;
	}

	/*
	 * TODO 
	 *
	 * ok, so trains are not going to be represented by objects
	 * they are abstractions of the vertices, and will dictate how
	 * we connect stations
	 *
	 * on the other hand it would make sense to abstract nodes
	 * OR we can simply have a lookup table with station name to id
	 * ... yeah. vector. only main knows station names. graph object
	 * only knows station ids
	 *
	 * TODO
	 */


	map<int,string> stations;
	int stationId;
	string stationName;
	while (stationsFile >> stationId) {
		stationsFile >> stationName;
		stations.insert(pair<int,string>(stationId, stationName));
		// create graph node for each station id/name
		// TODO ... should I create objects for trains and stations?
		// trains would be the vertices, and stations would be the nodes

	}

	/* TODO CREATE A GRAPH RIGHT HNAH!!! */

	GRAPH* graph = new GRAPH(stations.size());

	/* TODO CREATE A GRAPH RIGHT HNAH!!! */

    trainsFile.open(argv[1]);
	if (!trainsFile.is_open())
	{
		cout << "Error: Failed to open input file\n";
		return 0;
	}

	// TODO need ADT for weight
	// maybe change weight to pair. need to
	// account for this in the graph	
	int from, to, depart, arrive;
	while (trainsFile >> from) {
		trainsFile >> to;
		trainsFile >> depart;
		trainsFile >> arrive;

		graph->set_edge(from, to,  pair<int,int>(depart, arrive));
	}
	/*
	GRAPH *g = NULL;
	vector<int> *traversal;
	vector<int> *min_tree;
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
	*/
	return 0;
}

void showMenu() {
    cout << "  _ __ ___   ___ _ __  _   _ \n";
    cout << " | \"_ ` _ \\ / _ \\ \"_ \\| | | |\n";
    cout << " | | | | | |  __/ | | | |_| |\n";
    cout << " |_| |_| |_|\\___|_| |_|\\__,_|\n";
    cout << "========================================================\n";
    cout << "( 1 )  View Train Schedule\n";
    cout << "( 2 )  View Station Schedule\n";
    cout << "( 3 )  Look Up Station ID\n";
    cout << "( 4 )  Look Up Station Name\n";
    cout << "( 5 )  Check Service Availability\n";
    cout << "( 6 )  Check Non-Stop Service Availability\n";
    cout << "( 7 )  Find Route with Shortest Riding Time\n";
    cout << "( 8 )  Find Route with Shortest Total Travel Time\n";
    cout << "( 9 )  Exit\n";
    cout << "========================================================\n";
    cout << "Enter option: \n";
}



/* TODO how to reset terminal to show menu after height exceeded? */
void menuRepl() {
    int option;

    while (option != EOF) {
        showMenu();
        cin >> option;

        switch (option) {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;
            case 8:
                break;
            case 9:
                exit(0);
                break;
            default:
                cout << "Invalid option, please try again";
        }
    }
}
