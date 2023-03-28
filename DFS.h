#include <Vector>
#include <stack>



class DFS {

    void DFS::threadedDFSWorker(int id, vector<bool>& visited, stack<int>& s, mutex& mtx, condition_variable& cv, atomic<bool>& done)

    public:
        void unthreadedDFS(int start,int stop, WeightedGraph& graph);
        void threadedDFS(int start, WeightedGraph& graph, int threadCount);
};