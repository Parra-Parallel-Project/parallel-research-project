
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
#include "DFS.h"


#ifndef WEIGHTED_GRAPH_H
#define WEIGHTED_GRAPH_H
#include "WeightedGraph.h"
#endif


void DFS::unthreadedDFS(int start,int stop, WeightedGraph& g) {
    vector<bool> visited(g.getnNodes(), false);
    stack<int> s;
    s.push(start);

    while (!s.empty()) {
        int curr = s.top();
        s.pop();

        if (!visited[curr]) {
            visited[curr] = true;
        }

        for (const auto& e : *g.getEdges(curr)) {
            if (!visited[e.first]) {
                s.push(e.first);
            }
        }
    }
}


void DFS::threadedDFSWorker(int id, int stop, WeightedGraph& graph, vector<bool>& visited, stack<int>& s, mutex& mtx, condition_variable& cv, atomic<bool>& done) {
    unique_lock<mutex> lock(mtx, defer_lock);

    while (true) {
        lock.lock();
        while (!done && s.empty()) {
            cv.wait(lock);
        }

        if (done) {
            break;
        }

        int curr = s.top();
        s.pop();
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
                if (!visited[e.first]) {
                    s.push(e.first);
                }
            }
            lock.unlock();
            cv.notify_all();
        }
    }
}


void DFS::threadedDFS(int start, int stop, WeightedGraph& graph, int threadCount) {
    vector<bool> visited(graph.getnNodes(), false);
    stack<int> s;
    s.push(start);

    mutex mtx;
    condition_variable cv;
    atomic<bool> done(false);

    vector<thread> threads;
    threads.reserve(threadCount);

    for (int i = 0; i < threadCount; ++i) {
        threads.emplace_back([i, stop, &graph, &visited, &s, &mtx, &cv, &done] {
            threadedDFSWorker(i, stop, graph, visited, s, mtx, cv, done);
        });
    }

    for (auto& t : threads) {
        t.join();
    }
}