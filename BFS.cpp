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
#include "BFS.h"

#ifndef WEIGHTED_GRAPH_H
#define WEIGHTED_GRAPH_H
#include "WeightedGraph.h"
#endif


// WeightedGraph BFS::graph = WeightedGraph(0);

void BFS::unthreadedBFS(int start, int stop, WeightedGraph& graph) {
    vector<bool> visited(graph.getnNodes(), false);
    queue<int> q;
    q.push(start);

    while (!q.empty()) {
        int curr = q.front();
        q.pop();

        if (curr == stop) {
            break;
        }


        if (!visited[curr]) {
            visited[curr] = true;
        }

        for (const auto& e : *graph.getEdges(curr)) {
            if (!visited[e.second]) {
                q.push(e.second);
            }
        }
    }
}


void BFS::threadedBFS(int start, int stop, WeightedGraph& graph, int threadCount) {
    vector<bool> visited(graph.getnNodes(), false);
    queue<int> q;
    q.push(start);

    mutex mtx;
    condition_variable cv;
    atomic<bool> done(false);

    vector<thread> threads;
    threads.reserve(threadCount);

    for (int i = 0; i < threadCount; ++i) {
        threads.emplace_back([i, stop, &graph, &visited, &q, &mtx, &cv, &done] {
            threadedBFSWorker(i, stop, graph, visited, q, mtx, cv, done);
        });
    }

    for (auto& t : threads) {
        t.join();
    }
}


void BFS::threadedBFSWorker(int id, int stop, WeightedGraph& graph, vector<bool>& visited, queue<int>& q, mutex& mtx, condition_variable& cv, atomic<bool>& done) {
    unique_lock<mutex> lock(mtx, defer_lock);

    while (true) {
        lock.lock();
        while (!done && q.empty()) {
            cv.wait(lock);
        }

        if (done) {
            break;
        }

        int curr = q.front();
        q.pop();
        lock.unlock();

        if (!visited[curr]) {
            visited[curr] = true;

            if (curr == stop) {
                done = true;
                cv.notify_all();
                break;
            }

            lock.lock();
            for (const auto& e : *graph.getEdges(curr)) {
                if (!visited[e.second]) {
                    q.push(e.second);
                }
            }
            lock.unlock();
            cv.notify_all();
        }
    }
}