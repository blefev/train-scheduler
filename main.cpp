/*
 * Author: Brendan Lefevre <brendan.lefevre@trojans.dsu.edu>
 *
 * Important notes: A trip that spans multiple days IS allowed, but it is factored
 * into the weight for calculating the total trip time. It is ignored for shortest
 * riding time.
 *
 * A trip is considered to be non-stop when the difference between arrival and departure
 * of connecting trains is at most 1 minute.
 *
 */
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
void lookup_station_id(GRAPH* graph, map<int, string> &stations);
void lookup_station_name(GRAPH* graph, map<int, string> &stations);
void check_service_availability(GRAPH* graph, map<int, string> &stations, bool nonstop);
void find_route(GRAPH* graph, map<int, string> &stations, bool count_layovers);
void view_station_schedule(GRAPH* graph, map<int, string> &stations);
GRAPH* perform_setup(map<int, string> &stations);
string time_to_s(int time);

int main(int argc, char **argv)
{
	map<int,string> stations;

	GRAPH* graph = perform_setup(stations);

	// now that we have the pieces set up, we can start the menu repl
	menu_repl(graph, stations);

    delete graph;

	return 0;
}

GRAPH* perform_setup(map<int, string> &stations) {
	ifstream stationsFile;

    stationsFile.open("stations.dat");
	if (!stationsFile.is_open())
	{
		cout << "Error: Failed to open stations.dat\n";
		return 0;
	}

	int station_id;
	string stationName;
	while (stationsFile >> station_id) {
		stationsFile >> stationName;
		stations.insert(pair<int,string>(station_id, stationName));
	}
    stationsFile.close();

	GRAPH* graph = new GRAPH(stations.size());

	ifstream trainsFile;
    trainsFile.open("trains.dat");
	if (!trainsFile.is_open())
	{
		cout << "Error: Failed to open trains.dat\n";
		return 0;
	}

	int from, to, depart, arrive;
	while (trainsFile >> from) {
		trainsFile >> to;
		trainsFile >> depart;
		trainsFile >> arrive;

		sched schedule;
        schedule.push_back(depart);
        schedule.push_back(arrive);
		graph->set_edge(from, to,  schedule);
	}
    trainsFile.close();

	return graph;
}

void show_menu() {
    cout << "  _ __ ___   ___ _ __  _   _ \n";
    cout << " | \"_ ` _ \\ / _ \\ \"_ \\| | | |\n";
    cout << " | | | | | |  __/ | | | |_| |\n";
    cout << " |_| |_| |_|\\___|_| |_|\\__,_|\n";
    cout << "========================================================\n";
    cout << "( m, h )  View this menu\n";
    cout << "( 1 )  View Train Schedule\n";
    cout << "( 2 )  View Station Schedule\n";
    cout << "( 3 )  Look Up Station ID\n";
    cout << "( 4 )  Look Up Station Name\n";
    cout << "( 5 )  Check Service Availability\n";
    cout << "( 6 )  Check Non-Stop Service Availability (arrival and departure within 1 minute)\n";
    cout << "( 7 )  Find Route with Shortest Riding Time\n";
    cout << "( 8 )  Find Route with Shortest Total Travel Time\n";
    cout << "( 9 )  Exit\n";
    cout << "========================================================\n";
}



/* TODO how to reset terminal to show menu after height exceeded? */
void menu_repl(GRAPH* graph, map<int, string> &stations) {
    char option;

	show_menu();
    while (option != '9') {
		cout << "Enter option (m for menu):  ";
		cin.clear();
        cin >> option;

        switch (option) {
			case 'm':
			case 'h':
				show_menu();
				break;
            case '1': //view train schedule.
				print_schedules(graph, stations);
                break;
            case '2': // view station schedule
				view_station_schedule(graph, stations);
                break;
            case '3': // look up station id
				lookup_station_id(graph, stations);

				break;
			case '4': // look up station name
				lookup_station_name(graph, stations);
				break;

			case '5': // check service availability
				check_service_availability(graph, stations, false);
				break;
			case '6': // check non stop service availability
				check_service_availability(graph, stations, true);
				break;
			case '7': // route w shortest riding time
				find_route(graph, stations, false);
				break;
			case '8': // route w shortest total time
				find_route(graph, stations, true);
				break;
			case '9':
				exit(0);
				break;
			default:
				cout << "Invalid option, please try again\n";
		}
		cout << endl;
	}
}

void print_schedules(GRAPH* graph, map<int, string> &stations) {
	cout << "\nView Train Schedule\n";
	cout << "---------------------\n";

	for (map<int,string>::iterator it=stations.begin(); it!=stations.end(); ++it){
		int station_id = it->first;

		print_schedule(graph, stations, station_id);
	}
}

void print_schedule(GRAPH* graph, map<int, string> &stations, int station_id) {
	string station_name = stations.at(station_id);

	//cout << "------------------------- \n";
	cout << station_name << " (" << station_id << ")\n";
	cout << "------------------ \n";

	//get schedule for station
	vector<vector<int> > schedule = graph->station_schedule(station_id);

	if (schedule.empty()) {
		cout << "  No trains scheduled\n";
	} else {
		// print schedules
		for (vector<vector<int> >::iterator it=schedule.begin(); it!=schedule.end(); ++it){
			cout << "  " << stations.at(it->front()) << ": ";
			cout << time_to_s(it->at(1)) << " - " << time_to_s(it->at(2)) << "\n";
		}
	}
	cout << "\n";
}

void print_itenerary(GRAPH* graph, map<int, string> &stations, vector<int> path) {
	cout << "\nItenerary\n";
	cout << "------------\n";

	for (int i=0; i < path.size() - 1; i++) {
		int j = i + 1;
		int A = path.at(i);
		int B = path.at(j);
		sched item = graph->train_schedule(A, B);

		string depart = time_to_s(item.at(0));
		string arrive = time_to_s(item.at(1));

		cout << "  Depart " << stations.at(A) << " at:\t" << depart << "\n";
		cout << "  Arrive at "<< stations.at(B) << " at\t" << arrive << "\n\n";
	}
}

void lookup_station_id(GRAPH* graph, map<int, string> &stations) {
	cout << "\nLook up station ID\n";
	cout << "---------------------\n";

	bool found = false;
	int station_id;
	string station_name;

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
}

void lookup_station_name(GRAPH* graph, map<int, string> &stations) {
	cout << "\nLook up station name\n";
	cout << "---------------------\n";

	int station_id;

	cout << "Please enter the station ID: ";
	cin >> station_id;
	if (stations.find(station_id) == stations.end()) {
		cout << "Invalid station ID\n";
	} else {
		cout << "Station name: " << stations.at(station_id) << "\n";
	}
}

void check_service_availability(GRAPH* graph, map<int, string> &stations, bool nonstop) {

	cout << "\nCheck" << (nonstop ? " Non-Stop " : " ") << "Service Availability\n";
	cout << "---------------------\n";

	int station_id, station_id_a;

	cout << "Enter departure station ID: ";
	cin >> station_id;

	cout << "Enter destination station ID: ";
	cin >> station_id_a;

	if (graph->bfs(station_id, station_id_a, nonstop)) {
		cout << "Service is available\n";
	} else {
		cout << "Service is not available\n";
	}
}

void find_route(GRAPH* graph, map<int, string> &stations, bool count_layovers) {
	cout << "\nFind Route with Shortest"<< (count_layovers ? " Riding " : " Total ") << "Time\n";
	cout << "---------------------\n";

	int station_id, station_id_a;
	vector<int> path;

	cout << "Enter departure station ID: ";
	cin >> station_id;
	cout << "Enter destination station ID: ";
	cin >> station_id_a;

	path = graph->path(station_id, station_id_a, count_layovers);

	print_itenerary(graph, stations, path);
}

void view_station_schedule(GRAPH* graph, map<int, string> &stations) {
	cout << "\nView Station Schedule\n";
	cout << "---------------------\n";

	int station_id;

	cout << "Please enter the station ID: ";
	cin >> station_id;
	print_schedule(graph, stations, station_id);

}

string time_to_s(int time) {
	char ch[6];
	sprintf(ch, "%02d:%02d\0", (time / 100), (time % 100));
	string s(ch);
	return s;
}
