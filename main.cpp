#include<iostream>
#include "WeightedGraph.h"

using namespace std;

int main() {//TODO add arguments to decide which algorithm to use, thread count (1, 2, 4, 8) and graph size (10, 50, 100, 1000)
    cout << "---- | Runner Output: | ----\n\n";

    WeightedGraph g = WeightedGraph(5);

    /*
    fully connected graph example: 

    least cost path is 0 -> 1 -> 2 -> 3 -> 4 or the reversed traversal
    */ 
    g.addEdge(0, 1, 0);
    g.addEdge(0, 2, 1);
    g.addEdge(0, 3, 1);
    g.addEdge(0, 4, 1);
    g.addEdge(1, 2, 0);
    g.addEdge(1, 3, 1);
    g.addEdge(1, 4, 1);
    g.addEdge(2, 3, 0);
    g.addEdge(2, 4, 1);
    g.addEdge(3, 4, 0);

    g.prettyPrint();

	//loop 100 times
		//create random graph (g.randomGraph(graphSize, maxEdgeWeight))
		//start timer
		//call algorithm
		//end timer
		//input time and result to results array

	//print time and result{boolean for DFS/BFS, path distance for A*/Dijkstra}, to a text file if that is easier to move to CSV for analysis, use main args to determine file name
}