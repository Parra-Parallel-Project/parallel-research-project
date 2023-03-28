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


class AStar {

    static void subThreadAStar(WeightedGraph& graph, vector<int>& dist, vector<bool>& visited, int start, int stop);

    public:
        static void unthreadedAStar(int start, int stop, WeightedGraph& graph);
        static void threadedAStar(int start, int stop, WeightedGraph& graph, int numberThreads);
};