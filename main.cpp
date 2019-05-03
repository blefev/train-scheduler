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
void print_itenerary(GRAPH* graph, map<int, string> &stations, vector<int> path);


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
    stationsFile.close();

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

        cout << "f:t:d:a -> "<<from<<":"<<to<<":"<<depart<<":"<<arrive<<"\n";
		sched schedule;
        schedule.push_back(depart);
        schedule.push_back(arrive);
		graph->set_edge(from, to,  schedule);
        cout << "Service available? : " << graph->service_available(from, to) << "\n";
		cout << "Set edge for " << stations.at(from) << " to " << stations.at(to) << endl;
	}
    trainsFile.close();

	// now that we have the pieces set up, we can start the menu repl
	menu_repl(graph, stations);

    delete graph;

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
}



/* TODO how to reset terminal to show menu after height exceeded? */
void menu_repl(GRAPH* graph, map<int, string> &stations) {
    char option;
	int station_id;
	int station_id_a;
	string station_name;
	bool found = false;
    vector<int> path;

	show_menu();
    while (option != '9') {
		cout << "Enter option:  ";
        cin >> option;

        switch (option) {
            case '1': //view train schedule.
					// iterate through all stations and show schedule
				vector<sched> *traversal;
				print_schedules(graph, stations);

                break;
            case '2':
				cout << "Please enter the station ID: ";
				cin >> station_id;
				print_schedule(graph, stations, station_id);
                break;
            case '3':

				cout << "Please enter the station name: ";
				cin >> station_name;

				for (map<int,string>::iterator it=stations.begin(); it!=stations.end(); ++it){
					if (it->second == station_name) {
						found = true;
						cout << "Station ID for " << station_name << ": " << it->first << "\n";
					}
				}
				if (!found) {
					cout << "No station ID found\n";
				}
				found = false;

				break;
			case '4':
				cout << "Please enter the station ID: ";
				cin >> station_id;
				if (stations.find(station_id) == stations.end()) {
					cout << "Invalid station ID\n";
				} else {
					cout << "Station name: " << stations.at(station_id) << "\n";
				}

				break;

			case '5':
                // TODO TODO TODO this needs to do a SEARCH!!!
				cout << "Enter departure station ID: ";
				cin >> station_id;

				cout << "Enter destination station ID: ";
				cin >> station_id_a;

				if (graph->bfs(station_id, station_id_a, false)) {
					cout << "Service IS available\n";
				} else {
					cout << "Service IS NOT available\n";
				}

				break;
			case '6':
                // non stop service
				cout << "Enter departure station ID: ";
				cin >> station_id;

				cout << "Enter destination station ID: ";
				cin >> station_id_a;

				if (graph->bfs(station_id, station_id_a, true)) {
					cout << "Service IS available\n";
				} else {
					cout << "Service IS NOT available\n";
				}
				break;
			case '7':
                // shortest riding time
				cout << "Enter departure station ID: ";
				cin >> station_id;
				cout << "Enter destination station ID: ";
				cin >> station_id_a;

                path = graph->path(station_id, station_id_a, false);

                cout << "Shortest path : \n";
                for (auto it = path.begin(); it != path.end(); it++) {
                    cout << *it << " " << endl;
                }
                cout << "\n";

				print_itenerary(graph, stations, path);

				break;
			case '8':
				// shortest total time
				cout << "Enter departure station ID: ";
				cin >> station_id;
				cout << "Enter destination station ID: ";
				cin >> station_id_a;

				path = graph->path(station_id, station_id_a, true);

				cout << "Shortest path : \n";
				for (auto it = path.begin(); it != path.end(); it++) {
					cout << *it << " " << endl;
				}
				cout << "\n";
				break;
			case '9':
				exit(0);
				break;
			default:
				cout << "Invalid option, please try again\n";
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
	cout << " " << station_name << " (" << station_id << ")\n";
	cout << "------------------------- \n";

	//get schedule for station
	vector<vector<int> > schedule = graph->station_schedule(station_id);

	// print schedules
	for (vector<vector<int> >::iterator it=schedule.begin(); it!=schedule.end(); ++it){
		cout << "  " << stations.at(it->front()) << ": " << it->at(1) << " - " << it->at(2) << "\n";
	}
	cout << "\n";
}

void print_itenerary(GRAPH* graph, map<int, string> &stations, vector<int> path) {
	cout << "Itenerary:\n";
	cout << "------------\n";
	for (int i=0; i < path.size() - 1; i++) {
		int j = i + 1;
		int A = path.at(i);
		int B = path.at(j);
		sched item = graph->train_schedule(A, B);

		char depart[6];
		sprintf(depart, "%02d:%02d\0", (item.at(0) / 100), (item.at(0) % 100));
		char arrive[6];
		sprintf(arrive, "%02d:%02d\0", (item.at(1) / 100), (item.at(1) % 100));

		cout << "  Depart " << stations.at(A) << " at  " << depart << "\n";
		cout << "  Arrive at "<< stations.at(B) << " at " << arrive << "\n\n";
	}
}
