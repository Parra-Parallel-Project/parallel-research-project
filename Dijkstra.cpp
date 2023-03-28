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
#include "Dijkstra.h"

#ifndef WEIGHTED_GRAPH_H
#define WEIGHTED_GRAPH_H
#include "WeightedGraph.h"
#endif


void Dijkstra::unthreadedDijkstra(int start, int stop, WeightedGraph& graph) {
    const int INF = std::numeric_limits<int>::max();
    vector<int> dist((graph.getnNodes())+1, INF);
    std::priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push(make_pair(0, start));
    dist[start] = 0;

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (u == stop){
            break;
        }

        for (std::list<edge>::iterator it = graph.getEdges(u)->begin(); it != graph.getEdges(u)->end(); ++it) {
            int v = it->first;
            int weight = it->second;

            if (dist[v] > dist[u] + weight) {
                dist[v] = dist[u] + weight;
                pq.push(make_pair(dist[v], v));
            }
        }
    }
        if (dist[stop] == INF) {
        //cout << "doesnt exist" << endl;
    } else {
        //cout << "from " << start << " to " << stop << "distance is: " << dist[stop] << endl;
    }
}


void Dijkstra::subThreadDijkstra(WeightedGraph& graph, vector<int>& dist, vector<bool>& visited, int start, int stop) {
    const int INF = std::numeric_limits<int>::max();

    // Initialize all distance values to infinity
    for (int i = 1; i <= graph.getnNodes(); i++) {
        dist[i] = INF;
    }
    dist[start] = 0;

    while (true) {
        // Find smallest unvisited node
        int u = -1;
        int minDist = INF;
        for (int i = 1; i <= graph.getnNodes(); i++) {
            if (!visited[i] && dist[i] < minDist) {
                u = i;
                minDist = dist[i];
            }
        }

        if (u == -1 || u == stop) {
            break;
        }

        // Mark as visited
        visited[u] = true;

        // Update the distances
        for (std::list<edge>::iterator it = graph.getEdges(u)->begin(); it != graph.getEdges(u)->end(); ++it) {
            int v = it->first;
            int weight = it->second;

            if (!visited[v] && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
            }
        }
    }
}


void Dijkstra::threadedDijkstra(int start, int stop, WeightedGraph& graph, int numberThreads) {
    const int INF = std::numeric_limits<int>::max();
    vector<thread> threads;
    vector<vector<int>> dist(numberThreads, vector<int>(graph.getnNodes()+1, INF));
    vector<vector<bool>> visited(numberThreads, vector<bool>(graph.getnNodes()+1, false));

    for (int t = 0; t < numberThreads; t++) {
        int subgraphSize = graph.getnNodes() / numberThreads;
        int subgraphStart = t * subgraphSize + 1;
        int subgraphStop = (t == numberThreads-1) ? graph.getnNodes() : (t+1) * subgraphSize;

        threads.emplace_back(subThreadDijkstra, std::ref(graph), std::ref(dist[t]), std::ref(visited[t]), subgraphStart, stop);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    int finalDist = INF;
    for (int t = 0; t < numberThreads; t++) {
        finalDist = min(finalDist, dist[t][stop]);
    }

    if (finalDist == INF) {
       // cout << "doesnt exist" << endl;
    } else {
       // cout << "from " << start << " to " << stop << " distance is: " << finalDist << endl;
    }
}