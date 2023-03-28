#include <iostream>
#include <stack>
#include <vector>
#include <list>
#include <queue>
#include <limits>
#include <algorithm>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "WeightedGraph.h"


//write algorithms into this file

//Single Thread Section

//SingleAStar


//BFS and DFS don't pay attention to edge weights, just return boolean value whether target node was found or not
//SingleBFS
	//create a shared queue and boolean visitedArray
	//loop function
		//is this the target node?
		//cycle through list received from getEdges and add it to the queue if node hasn't been visited
void unthreadedBFS(int start, int stop, WeightedGraph& graph) {
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
            if (!visited[e.dest]) {
                q.push(e.dest);
            }
        }
    }
}

//SingleDFS
	//create a boolean visitedArray
	//on current node check if target : return
	//also return early if node already visited(?)
	//else add all neighbors to queue
	//go through queue and recurse down
	//recursion function should have an argument that tracks whether the target node was found

void unthreadedDFS(int start,int stop, WeightedGraph& graph) {
    vector<bool> visited(graph.getnNodes(), false);
    stack<int> s;
    s.push(start);

    while (!s.empty()) {
        int curr = s.top();
        s.pop();
        if (curr == stop) {
            return;
        }

        if (!visited[curr]) {
            visited[curr] = true;
        }

        for (const auto& e : *graph.getEdges(curr)) {
            if (!visited[e.dest]) {
                s.push(e.dest);
            }
        }
    }
}
//SingleDijkstra
	//?some way to track nodes accessible and total distance/weights from source node?
	//create priority queue PQ
	//loop function
		//pick from the front of the PQ
		//is this the target node?
		//add neighbors to PQ if they don't have a lower cost path there
void unthreadedDijkstra(int start, int stop, WeightedGraph& graph) {
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


//Multi Thread Section

//MultiAStar

//MultiBFS
	//threads share a queue and flag
	//each thread is grabbing nodes and adding neighbors, shared flag only set when a thread finds the target node
	//threads act using a while(flag) loop so that once the target is found they stop executing

void threadedBFSWorker(int id, int stop, vector<bool>& visited, queue<int>& q, mutex& mtx, condition_variable& cv, atomic<bool>& done) {
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
                if (!visited[e.dest]) {
                    q.push(e.dest);
                }
            }
            lock.unlock();
            cv.notify_all();
        }
    }
}

void threadedBFS(int start, int stop, WeightedGraph& graph, int threadCount) {
    vector<bool> visited(graph.getnNodes(), false);
    queue<int> q;
    q.push(start);

    mutex mtx;
    condition_variable cv;
    atomic<bool> done(false);

    vector<thread> workers;
    for (int i = 0; i < threadCount; i++) {
        workers.emplace_back(threadedBFSWorker, i, stop, ref(visited), ref(q), ref(mtx), ref(cv), ref(done));
    }

    cv.notify_all();

    for (auto& worker : workers) {
        worker.join();
    }
}

//MultiDFS
	//share visited array and flag
	//threads spawn if there are multiple neighbors to current node and less threads active than max allowed
	//threads die if they are in a dead end node and can't access any nodes that haven't been visited

void threadedDFSWorker(int id, int stop, vector<bool>& visited, stack<int>& s, mutex& mtx, condition_variable& cv, atomic<bool>& done) {
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
                if (!visited[e.dest]) {
                    s.push(e.dest);
                }
            }
            lock.unlock();
            cv.notify_all();
        }
    }
}


void threadedDFS(int start, int stop, WeightedGraph& graph, int threadCount) {
    vector<bool> visited(graph.getnNodes(), false);
    stack<int> s;
    s.push(start);

    mutex mtx;
    condition_variable cv;
    atomic<bool> done(false);

    vector<thread> workers;
    for (int i = 0; i < threadCount; i++) {
        workers.emplace_back(threadedDFSWorker, i, stop, ref(visited), ref(s), ref(mtx), ref(cv), ref(done));
    }

    cv.notify_all();

    for (auto& worker : workers) {
        worker.join();
    }
}

//MultiDijkstra

void subThreadDijkstra(WeightedGraph& graph, vector<int>& dist, vector<bool>& visited, int start, int stop) {
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
void threadedDijkstra(int start, int stop, WeightedGraph& graph, int numberThreads) {
    const int INF = std::numeric_limits<int>::max();
    vector<thread> threads;
    vector<vector<int>> dist(numberThreads, vector<int>(graph.getnNodes()+1, INF));
    vector<vector<bool>> visited(numberThreads, vector<bool>(graph.getnNodes()+1, false));

    for (int t = 0; t < numberThreads; t++) {
        int subgraphSize = graph.getnNodes() / numberThreads;
        int subgraphStart = t * subgraphSize + 1;
        int subgraphStop = (t == numberThreads-1) ? graph.getnNodes() : (t+1) * subgraphSize;

        threads.emplace_back(subdijkstra, std::ref(graph), std::ref(dist[t]), std::ref(visited[t]), subgraphStart, stop);
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
