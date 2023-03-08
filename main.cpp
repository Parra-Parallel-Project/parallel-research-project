#include<iostream>
#include "WeightedGraph.h"

using namespace std;

int main() {
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

    
}