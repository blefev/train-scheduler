#include "graph.h"
//#include "pq.hpp"



GRAPH::GRAPH(int nodes)
{
	this->node_count = nodes;

	this->data = new sched**[nodes];
	for (int i = 0; i <= nodes; i++) {
		this->data[i] = new sched*[nodes];
		for (int j = 0; j <= nodes; j++) {
			this->data[i][j] = NULL;
		}
	}
}

void GRAPH::set_edge(int source, int destination, sched* schedule)
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
    std::set<int> discovered;

    std::stack<int> st;
    st.push(start);

    while (!st.empty()) {
        int cur = st.top();
        st.pop();
        if (discovered.count(cur) == 0) {
            traversal->push_back(cur);
            discovered.insert(cur);
            for (int i=1; i <= this->node_count; i++) {
                if (this->data[cur][i] != NULL) {
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

        for (int i=1; i <= this->node_count; i++) {
            if (this->data[cur][i] != NULL) {

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
    //  TODO TODO TODO TODO TODO TODO TODO
    //   YOU CANNOT GO BACKWARDS IN TIME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //   YOU CANNOT GO BACKWARDS IN TIME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //   YOU CANNOT GO BACKWARDS IN TIME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //   Remember to implement layover times!!!!!!
    //
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
                if (this->data[currentV][adjV] != NULL) {
                    // weight
                    sched* s = this->data[currentV][adjV];

                    //cout << "at(0):at(1) " << s->at(0) << " : "<< s->at(1) << "\n";
                    // time in minutes trip takes, aka weight
                    int weight = t_diff(s->at(0), s->at(1));
                    //cout << "weight for " << currentV <<" to "<<adjV << ": "<<weight<<"\n";
                    int alt_dist;

                    if (layovers) {
                        // add time diff between 
                        // predecessor arrival time 
                        // from this->data[predecessor[currentV]][currentV]
                        // and next node leave time??
                    } else {
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
    std::vector<bool> vertex_set;
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

        std::cout << "minNode: " << minNode << endl;

        vertex_set[minNode] = true;


        for (int vert=1; vert <= this->node_count; vert++) {
            if (vertex_set[vert] == false && this->data[minNode][vert]->at(0) > 0) {
                if (distances[minNode] != INT_MAX && distances[minNode] 
                        + (this->data[minNode][vert]->at(0) - this->data[minNode][vert]->at(1)) < distances[vert]) {
                    distances[vert] = distances[minNode] + (this->data[minNode][vert]->at(0) - this->data[minNode][vert]->at(1));
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
        if (this->data[station_id][i] != NULL) {


            sched* schedule = this->data[station_id][i];
            vector<int> train;

            train.push_back(i);
            train.push_back(this->data[station_id][i]->at(0));
            train.push_back(this->data[station_id][i]->at(1));

            trains.push_back(train);
        }
    }
    return trains;
}
