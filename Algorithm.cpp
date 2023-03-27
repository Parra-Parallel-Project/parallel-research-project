//write algorithms into this file

//Single Thread Section

//SingleAStar


//BFS and DFS don't pay attention to edge weights, just return boolean value whether target node was found or not
//SingleBFS
	//create a shared queue and boolean visitedArray
	//loop function
		//is this the target node?
		//cycle through list received from getEdges and add it to the queue if node hasn't been visited

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


//Multi Thread Section

//MultiAStar

//MultiBFS
	//threads share a queue and flag
	//each thread is grabbing nodes and adding neighbors, shared flag only set when a thread finds the target node
	//threads act using a while(flag) loop so that once the target is found they stop executing

//MultiDFS
	//share visited array and flag
	//threads spawn if there are multiple neighbors to current node and less threads active than max allowed
	//threads die if they are in a dead end node and can't access any nodes that haven't been visited

void threadedDFSWorker(int id, vector<bool>& visited, stack<int>& s, mutex& mtx, condition_variable& cv, atomic<bool>& done) {
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

void threadedDFS(int start, WeightedGraph& graph, int threadCount) {
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
//MultiDijkstra
