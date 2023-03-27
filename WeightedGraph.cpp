#include<iostream>
#include<Vector>
#include<List>
#include "WeightedGraph.h"

using namespace std;


WeightedGraph::WeightedGraph(int n){

    adj = AdjacencyList(n); // graph with n nodes
    n_nodes = n;
    n_edges = 0;
}


void WeightedGraph::addEdge(int src, int dest, int weight) {
    
    
    for (std::list<edge>::iterator it = adj[src].begin(); it != adj[src].end(); ++it) {

        if((*it).first == dest) {
            return;
        }
    }

    // adding both directions for undirected graph
    adj[src].push_back(edge(dest, weight));
    adj[dest].push_back(edge(src, weight));

    n_edges++;
}

//TODO create a randomGraph method that takes in {number of nodes to create, upper edge weight limit} and returns a randomized graph
	//needs some way to validate a path exists from node 0 to node n-1 OR write algorithms to report if no path is possible
	//maybe BFS search from end node to start node and start randomly adding edges until BFS search finds a path to start?


list<edge>* WeightedGraph::getEdges(int node) {

    if(node >= n_nodes) return NULL;
    
    // return a ptr to the list of edges
    return &adj[node];
}


void WeightedGraph::prettyPrint() {

    for(int i = 0; i < adj.size(); i++){

        if(adj[i].empty()) {
            cout << "Node " << i << " has no connections.\n";
            continue;
        }

        cout << "Node " << i << " connects to:\n";
        for (std::list<edge>::iterator it = adj[i].begin(); it != adj[i].end(); ++it) {

            cout << "\t" << (*it).first << " (w = " << (*it).second << ")\n";
        }
        cout << "\n";
    }
}