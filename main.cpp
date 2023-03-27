#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <tuple>
#include <Vector>
#include <iomanip>

#include "WeightedGraph.h"

using namespace std;

#define MAX_WEIGHT 10

// Used to test algorithms: (start, end, graph)
typedef tuple<int, int, WeightedGraph> problem;

struct arr2d {
    int a[n_groups][n_graphs];
};

struct arr3d {
    int a[n_thread_groups][n_groups][n_graphs];
};


// Helper functions
problem generateNewProblem(int n); // generates a new graph with guaranteed path between start and stop nodes
void printEdgesPerProblem(vector<vector<problem>> arr); // debug print
arr2d testUnthreaded(int alg); // performs test for one unthreaded  algorithm and measures the times
arr3d testThreaded(int alg); // performs test for one threaded algorithm and measures the times across different threadcounts

// globals
static const int n_graphs = 100; // number of test problems per group
static const int n_groups = 5; // number of groups to test
static const int groups[] = {10, 50, 100, 1000, 10000}; // nodes used in each group
static const int n_thread_groups = 4; // number of different thread counts to test
static const int thread_groups[] = {1, 2, 4, 8}; // threads to test each multithreaded solution using


int main() {

    cout << "---- | Runner Output: | ----\n\n";

    // srand(0);
    srand (time(NULL));

    vector<vector<problem>> allProblems;

    // generate all problems to be solved
    for(int group = 0; group < n_groups; group++) {

        vector<problem> currentGroup;

        for(int i = 0; i < n_graphs; i++) {

            problem p = generateNewProblem(groups[group]);
            currentGroup.push_back(p);
        }

        allProblems.push_back(currentGroup);        
    }

    // printEdgesPerProblem(allProblems); // DEBUG

    for(int alg = 0; alg < 4; alg++) { // do each algorithm type independently

        arr2d times = testUnthreaded(alg);
    }


}


problem generateNewProblem(int n) {
    
    WeightedGraph g = WeightedGraph(n);

    int maxEdges = n * (n - 1); // maximum number of edges in an undirected, fuully connected graph

    int nEdges = rand() % (maxEdges - 1) + 1; // number of edges to attempt to add

    int start, stop; // nodes that will be guaranteed to have a path between

    start = rand() % n;
    stop = start;

    while(start == stop) { // force stop to be a distinct node from start

        stop = rand() % n;
    }


    int e1, e2, w, edges;
    e1 = start;

    // guatantee a path from start node to stop node
    for(edges = 0; e2 != stop; edges++) { // exits if last added node is the 'stop' node

        e2 = rand() % n;
        w = rand() % (MAX_WEIGHT - 1) + 1; // prevent 0 weights

        g.addEdge(e1, e2, w);

        e1 = e2; // move first node to next node
    }

    // attempt to add more random edges until up to nEdges
    for(; edges < nEdges; edges++) { 

        e1 = rand() % n;
        e2 = rand() % n;
        w = rand() % (MAX_WEIGHT - 1) + 1;

        g.addEdge(e1, e2, w);
    }


    // cout << "Edges Attempted: " << nEdges << "\t\tActual: " << g.getnEdges() << "\n\n"; // DEBUG
    return problem(start, stop, g);
}


arr2d testUnthreaded(int alg) {

    arr2d times; // times for unthreaded solutions

    return times;
}

arr3d testThreaded(int alg) {

    arr3d times; // times for threaded solutions

    return times;
}


void printEdgesPerProblem(vector<vector<problem>> arr) {

    for(auto group : arr) {

        for(auto p: group) {
            cout << "Start: "<< setw(8) << get<0>(p) << "    Stop: "<< setw(8) << get<1>(p) << "    Edges: " << setw(8) << get<2>(p).getnEdges() << "\n";
        }

        cout << "--------------------------------------------------------------------------\n\n";
    }
}

