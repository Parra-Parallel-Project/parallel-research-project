#include<iostream>
#include<vector>
#include<list>


#ifndef WEIGHTED_GRAPH_H
#define WEIGHTED_GRAPH_H
#include "WeightedGraph.h"


using namespace std;


WeightedGraph::WeightedGraph(int n){

    adj = AdjacencyList(n); // graph with n nodes
    n_nodes = n;
    n_edges = 0;
}


void WeightedGraph::addEdge(int src, int dest, int weight) {
    
    if(src == dest) return; // no self loops.
    
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

int WeightedGraph::getnEdges() {
    return n_edges;
}

int WeightedGraph::getnNodes() {
    return n_nodes;
}

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


#endif
