#include<iostream>
#include<Vector>
#include<List>

using namespace std;


typedef pair<int, int> edge;
typedef vector<list<edge>> AdjacencyList;


class WeightedGraph {

    private:

        AdjacencyList adj;
        int n_nodes, n_edges;

    public:

        WeightedGraph(int n);

        void addEdge(int src, int dest, int weight);
        list<edge>* getEdges(int node);
        int getnEdges();
        int getnNodes();
        void prettyPrint();
};