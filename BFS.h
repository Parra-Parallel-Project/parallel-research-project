#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <limits>
#include <algorithm>
#include <thread>
#include <mutex>
#include <stack>
#include <condition_variable>
#include <atomic>


#ifndef WEIGHTED_GRAPH_H
#define WEIGHTED_GRAPH_H
#include "WeightedGraph.h"
#endif


class BFS {

    // static WeightedGraph graph;

    static void threadedBFSWorker(int id, int stop, WeightedGraph& graph, vector<bool>& visited, queue<int>& q, mutex& mtx, condition_variable& cv, atomic<bool>& done);

    public:
        static void unthreadedBFS(int start, int stop, WeightedGraph& graph);
        static void threadedBFS(int start, int stop, WeightedGraph& graph, int threadCount);
};