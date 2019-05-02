#include "graph.h"
//#include "pq.hpp"



GRAPH::GRAPH(int nodes)
{
	this->node_count = nodes;
}

void GRAPH::set_edge(int source, int destination, sched schedule)
{

	this->data[source][destination] = schedule;
}

bool GRAPH::service_available(int source, int destination)
{
    vector<int> s_path = path(source, destination, false);

    return !s_path.empty();
}

vector<int> *GRAPH::dfs(int start)
{
    vector<int> *traversal = new vector<int>();
    set<int> discovered;

    stack<int> st;
    st.push(start);

    while (!st.empty()) {
        int cur = st.top();
        st.pop();
        if (discovered.count(cur) == 0) {
            traversal->push_back(cur);
            discovered.insert(cur);
            for (int i=1; i <= this->node_count; i++) {
                if (!this->data[cur][i].empty()) {
                    st.push(i);
                }
            }
        }
    }

    return traversal;
}

bool GRAPH::bfs(int start, int dst, bool nonstop)
{
    vector<int> *traversal = new vector<int>();

    queue<int> frontier;
    set<int> discovered;

    frontier.push(start);
    discovered.insert(start);

    while(!frontier.empty()) {
        int cur = frontier.front();

        if (cur == dst) {
            return true;
        }

        frontier.pop();
        traversal->push_back(cur);

        for (int i=1; i <= this->node_count; i++) {
            if (!this->data[cur][i].empty()) {
                // check if this is nonstop
                if (discovered.count(i) == 0) {
                    if (nonstop && traversal->size() >= 2) {
                        int prevStation = traversal->at(traversal->size()-2); //second to last
                        sched prev = this->data[prevStation][cur];
                        sched next = this->data[cur][i];

                        // departure minus arrival
                        // considering nonstop as meaning within 1 minute
                        if (!(next.at(0) - prev.at(1) > 1)) {
                            frontier.push(i);
                            discovered.insert(i);
                        }
                    } else {
                        frontier.push(i);
                        discovered.insert(i);
                    }

                }
            }
        }
    }
    return false;
}

bool GRAPH::empty(vector<bool> set) {
    for (int i =1; i < set.size(); i++) {
        if (set[i] == false) {
            return false;
        }
    }
    return true;
}

// layovers false -> don't count layovers
// layovers true  -> do count layovers
vector<int> GRAPH::path(int src, int dst, bool layovers = false) {
    vector<int> shortest_path; 
    vector<bool> vertex_set;
    int predecessors[this->node_count];
    int distances[this->node_count];

    // create list of all possible vertexes
    for (int i=1; i <= this->node_count; i++) {
        distances[i] = INT_MAX;
        predecessors[i] = 0;
    }

    // set starting node distance 
    // this also ensures we "visit" src at(0)
    distances[src] = 0;

    int cur = src;

    // for every vertex in graph
    for (int i=1; i <= this->node_count; i++) {

        for (int currentV=1; currentV <= this->node_count; currentV++) {

            // visit each vertex adjacent to current
            for (int adjV=1; adjV <= this->node_count; adjV++) {
                // for each vertex adjV to currentV
                if (!this->data[currentV][adjV].empty()) {
                    // weight
                    sched s = this->data[currentV][adjV];

                    //cout << "at(0):at(1) " << s.at(0) << " : "<< s.at(1) << "\n";
                    // time in minutes trip takes, aka weight
                    int weight;
                    weight = t_diff(s.at(0), s.at(1));

                    if (layovers && predecessors[currentV] > 0) { // already have predecessor
                        // determine layover 
                        int pre = predecessors[currentV];
                        vector<int> schedA = this->data[pre][currentV];
                        vector<int> schedB = this->data[currentV][adjV];

                        // arrival of A later than departure of B
                        if (schedA.at(1) > schedB.at(0)) { 
                            // add 24 hours to layover
                            weight += t_diff(schedA.at(1), schedB.at(0) + 2400);
                        } else {
                            weight += t_diff(schedA.at(1), schedB.at(0));
                        }
                    }                    
                    //cout << "weight for " << currentV <<" to "<<adjV << ": "<<weight<<"\n";
                    int alt_dist;

                    alt_dist = distances[currentV] + weight;
                    //cout << "distance to " << currentV << " from src: " << alt_dist << "\n";

                    if (alt_dist < distances[adjV]) {
                        distances[adjV] = alt_dist;
                        predecessors[adjV] = currentV;
                    }
                }

            }


        }

    }
    // to recreate shortest_path, go backwards
    // following predecessors
    // may also need to add scheds to this
    int tmp = dst;
    if (predecessors[tmp] > 0) {
        shortest_path.push_back(dst);
        while (tmp != src) {
            int pre = predecessors[tmp];
            shortest_path.insert(shortest_path.begin(), pre);
            tmp = pre; 
        }
    }

    return shortest_path;
}

int GRAPH::t_diff(int a, int b) {
    // calculate difference in time 
    int h1 = a / 100;
    int m1 = a % 100;
    int h2 = b / 100;
    int m2 = b % 100;

    if (m1 > m2) {
        h2--;
        m2 += 60;
    }

    int diff_m = m2 - m1;
    int diff_h = h2 - h1;

    int minutes = diff_h * 60 + diff_m;
    return minutes;
}

int GRAPH::dijkstra(int src, int dst)
{
    vector<bool> vertex_set;
    int distances[this->node_count];
    int minNode;
    int minDistance;

    for (int i=1; i <= this->node_count; i++) {
        distances[i] = INT_MAX;
        vertex_set.push_back(false);
    }

    distances[src] = 0;

    while (!empty(vertex_set)) {
        minNode = INT_MAX;
        minDistance = INT_MAX;

        for (int vert=1; vert <= this->node_count; vert++) {
            if (vertex_set[vert] == false && distances[vert] <= minDistance) {
                minDistance = distances[vert];
                minNode = vert;
            }
        }

        cout << "minNode: " << minNode << endl;

        vertex_set[minNode] = true;


        for (int vert=1; vert <= this->node_count; vert++) {
            if (vertex_set[vert] == false && this->data[minNode][vert].at(0) > 0) {
                if (distances[minNode] != INT_MAX && distances[minNode] 
                        + (this->data[minNode][vert].at(0) - this->data[minNode][vert].at(1)) < distances[vert]) {
                    distances[vert] = distances[minNode] + (this->data[minNode][vert].at(0) - this->data[minNode][vert].at(1));
                }
            }
        }
    }
    return distances[dst];
}

vector<vector<int> > GRAPH::get_schedule(int station_id) {


    vector<vector<int> > trains;
    // iterate through data[station_id] 
    for (int i=1; i <= this->node_count; i++) {
        if (!this->data[station_id][i].empty()) {


            sched schedule = this->data[station_id][i];
            vector<int> train;

            train.push_back(i);
            train.push_back(this->data[station_id][i].at(0));
            train.push_back(this->data[station_id][i].at(1));

            trains.push_back(train);
        }
    }
    return trains;
}

void GRAPH::print_itenerary(GRAPH* graph, map<int, string> &stations, vector<int> path) {
    for (int i=0; i <= path.size() - 1; i++) {
        int j = i + 1;
        sched item = data[path.at(i)][path.at(j)];
    }
}
