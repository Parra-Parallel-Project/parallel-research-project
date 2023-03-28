


void DFS::unthreadedDFS(int start,int stop, WeightedGraph& graph) {
    vector<bool> visited(graph.getnNodes(), false);
    stack<int> s;
    s.push(start);

    while (!s.empty()) {
        int curr = s.top();
        s.pop();

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


void DFS::threadedDFSWorker(int id, vector<bool>& visited, stack<int>& s, mutex& mtx, condition_variable& cv, atomic<bool>& done) {
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
            cout << "Thread " << id << ": " << curr << endl;
            visited[curr] = true;

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


void DFS::threadedDFS(int start, WeightedGraph& graph, int threadCount) {
    vector<bool> visited(graph.getnNodes(), false);
    stack<int> s;
    s.push(start);

    mutex mtx;
    condition_variable cv;
    atomic<bool> done(false);

    vector<thread> workers;
    for (int i = 0; i < threadCount; i++) {
        workers.emplace_back(threadedDFSWorker, i, ref(visited), ref(s), ref(mtx), ref(cv), ref(done));
    }

    cv.notify_all();

    for (auto& worker : workers) {
        worker.join();
    }
}