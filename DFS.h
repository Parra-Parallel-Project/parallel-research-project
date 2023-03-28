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


class DFS {

    static void threadedDFSWorker(int id, int stop, WeightedGraph& graph, vector<bool>& visited, stack<int>& s, mutex& mtx, condition_variable& cv, atomic<bool>& done);

    public:
        static void unthreadedDFS(int start,int stop, WeightedGraph& graph);
        static void threadedDFS(int start, int stop, WeightedGraph& graph, int threadCount);
};