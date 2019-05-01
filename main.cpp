#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <unistd.h>
#include "graph.h"
#include <map>
using namespace std;

void show_menu();
void menu_repl(GRAPH* graph, map<int, string> &stations);
void print_schedules(GRAPH* graph, map<int, string> &stations);
void print_schedule(GRAPH* graph, map<int, string> &stations, int station_id); 


int main(int argc, char **argv)
{
	ifstream stationsFile;
	ifstream trainsFile;

    stationsFile.open("stations.dat");
	if (!stationsFile.is_open())
	{
		cout << "Error: Failed to open stations.dat\n";
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
	int station_id;
	string stationName;
	while (stationsFile >> station_id) {
		stationsFile >> stationName;
		stations.insert(pair<int,string>(station_id, stationName));
		// create graph node for each station id/name
		// TODO ... should I create objects for trains and stations?
		// trains would be the vertices, and stations would be the nodes

	}

	/* TODO CREATE A GRAPH RIGHT HNAH!!! */

	GRAPH* graph = new GRAPH(stations.size());

	/* TODO CREATE A GRAPH RIGHT HNAH!!! */

    trainsFile.open("trains.dat");
	if (!trainsFile.is_open())
	{
		cout << "Error: Failed to open trains.dat\n";
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

		sched* schedule = new sched(depart, arrive);
		graph->set_edge(from, to,  schedule);
		graph->print_data(from,to);
		cout << "Set edge for " << stations.at(from) << " to " << stations.at(to) << endl;
	}

	// now that we have the pieces set up, we can start the menu repl
	menu_repl(graph, stations);

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

void show_menu() {
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
    cout << "Enter option:  ";
}



/* TODO how to reset terminal to show menu after height exceeded? */
void menu_repl(GRAPH* graph, map<int, string> &stations) {
    int option;

	show_menu();
    while (option != EOF) {
        cin >> option;

        switch (option) {
            case 1: //view train schedule.
					// iterate through all stations and show schedule
				vector<sched> *traversal;
				print_schedules(graph, stations);

                break;
            case 2:
				cout << "Please enter the station ID: ";
				int station_id;
				cin >> station_id;
				print_schedule(graph, stations, station_id);
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

void print_schedules(GRAPH* graph, map<int, string> &stations) {
	for (map<int,string>::iterator it=stations.begin(); it!=stations.end(); ++it){
		int station_id = it->first;

		print_schedule(graph, stations, station_id);
	}
}

void print_schedule(GRAPH* graph, map<int, string> &stations, int station_id) {
		string station_name = stations.at(station_id);

		cout << "------------------------- \n";
		cout << " " << station_name << "\n";
		cout << "------------------------- \n";

		//get schedule for station
		vector<vector<int> > schedule = graph->get_schedule(station_id);

		// print schedules
		for (vector<vector<int> >::iterator it=schedule.begin(); it!=schedule.end(); ++it){
			cout << "  " << stations.at(it->front()) << ": " << it->at(1) << " - " << it->at(2) << "\n";
		}
		cout << "\n";
}
