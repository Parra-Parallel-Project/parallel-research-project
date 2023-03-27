#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <tuple>
#include <Vector>
#include <iomanip>
#include <string>
#include <chrono>
#include <fstream>

#include "WeightedGraph.h"

using namespace std;
using namespace std::chrono;

#define MAX_WEIGHT 10

// Used to test algorithms: (start, end, graph)
typedef tuple<int, int, WeightedGraph> problem;


// globals

// per unthreaded alg 
static const int n_graphs = 10; // number of test problems per group
static const int n_groups = 5; // number of groups to test
static const int groups[] = {10, 50, 100, 1000, 10000}; // nodes used in each group

// per multithreaded alg
static const int n_thread_groups = 4; // number of different thread counts to test
static const int thread_groups[] = {1, 2, 4, 8}; // threads to test each multithreaded solution using

// the algs
static const int nAlgs = 4;
static const string algNames[nAlgs] = {"Depth_First_Search", "Breadth_First_Search", "Dijkstra", "A_Star"};


// for returning arrays from a fnc
struct arr2d { 
    int a[n_groups][n_graphs];
};

struct arr3d {
    int a[n_thread_groups][n_groups][n_graphs];
};


// Helper functions
problem generateNewProblem(int n); // generates a new graph with guaranteed path between start and stop nodes
void printEdgesPerProblem(vector<vector<problem>> arr); // debug print
arr2d testUnthreaded(int alg, vector<vector<problem>> graphs); // performs test for one unthreaded  algorithm and measures the times
arr2d testThreaded(int alg, int threadCount, vector<vector<problem>> graphs); // performs test for one threaded algorithm and measures the times across different threadcounts
string toCSVString(int alg, int threadCount, arr2d times, int h, int w);
void prettyPrint2d(arr2d arr, int height, int width);


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
    arr2d times;

    ofstream fp("times.csv");

    // string csvString = "";
    // cout << "Max String Size: " << csvString.max_size() << "\n";

    for(int alg = 0; alg < nAlgs; alg++) { // do each algorithm type independently

        times = testUnthreaded(alg, allProblems); // unthreaded 
        // prettyPrint2d(times, n_groups, n_graphs);
        fp << toCSVString(alg, 0, times, n_groups, n_graphs);

        for(int t = 0; t < n_thread_groups; t++) { // threaded

            times = testThreaded(alg, thread_groups[t], allProblems);
            fp << toCSVString(alg, thread_groups[t], times, n_groups, n_graphs);
            // prettyPrint2d(times, n_groups, n_graphs);
        }
    }

    fp.close();

}


string toCSVString(int alg, int threadCount, arr2d times, int h, int w) {

    string csv = "";
    for(int i = 0; i < h; i++) {
        for(int j = 0; j < w; j++) {
            
            string row = "";

            /*
                format:
                alg_name,n_threads,n_nodes,time_in_ms
            */

            row += algNames[alg] + ",";
            row += to_string(threadCount) + ",";
            row += to_string(groups[h]) + ",";
            row += to_string(times.a[i][j]) + "\n";

            csv += row;
        }
    }

    return csv;
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


arr2d testUnthreaded(int alg, vector<vector<problem>> graphs) {

    cout << "\nRunning tests for unthreaded " << algNames[alg] << "\n";
    
    arr2d times; // times for unthreaded solutions

    int start, stop;

    for(int i = 0; i < n_groups; i++) {

        for(int j = 0; j < n_graphs; j++) {

            start = get<0>(graphs[i][j]);
            stop = get<1>(graphs[i][j]);
            WeightedGraph graph = get<2>(graphs[i][j]);
            

            auto start = high_resolution_clock::now(); // start time
            
            switch(alg) {

                case 0: // DFS
                    
                    // unthreadedDFS(start, stop, graph); 
                    break;

                case 1: // BFS

                    // unthreadedBFS(start, stop, graph); 
                    break;

                case 2: // Dijkstra's

                    // unthreadedDijkstra(start, stop, graph); 
                    break;

                case 3: // A*

                    // unthreadedAStar(start, stop, graph); 
                    break;
                default:
                    cout << "What are you doing here?\n";
            }

            auto stop = high_resolution_clock::now(); // end time

            times.a[i][j] = duration_cast<milliseconds>(stop - start).count();
        }
    }

    return times;
}


arr2d testThreaded(int alg, int threadCount, vector<vector<problem>> graphs) {

    cout << "\nRunning tests for threaded " << algNames[alg] << " using " << threadCount << " threads\n";
    
    arr2d times; // times for threaded solutions

    int start, stop;

    for(int i = 0; i < n_groups; i++) {

        for(int j = 0; j < n_graphs; j++) {

            start = get<0>(graphs[i][j]);
            stop = get<1>(graphs[i][j]);
            WeightedGraph graph = get<2>(graphs[i][j]);
            

            auto start = high_resolution_clock::now(); // start time
            
            switch(alg) {

                case 0: // DFS
                    
                    // threadedDFS(start, stop, graph, threadCount); 
                    break;

                case 1: // BFS

                    // threadedBFS(start, stop, graph, threadCount); 
                    break;

                case 2: // Dijkstra's

                    // threadedDijkstra(start, stop, graph, threadCount); 
                    break;

                case 3: // A*

                    // threadedAStar(start, stop, graph, threadCount); 
                    break;
                default:
                    cout << "What are you doing here?\n";
            }

            auto stop = high_resolution_clock::now(); // end time

            times.a[i][j] = duration_cast<milliseconds>(stop - start).count();
        }
    }

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


void prettyPrint2d(arr2d arr, int height, int width) {

    for(int i = 0; i < height; i++) {

        cout << "\n\tNodes: " << groups[i] << "\n\t\t"; 

        for(int j = 0; j < width; j++) {

            cout << setw(8) << arr.a[i][j] << "ms   ";
        }
        cout << "\n";
    }
}