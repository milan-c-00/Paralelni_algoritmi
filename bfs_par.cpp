#include <iostream>
#include <vector>
#include <omp.h>
#include <queue>

using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;

public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void sequentialBFS(int start) {
        vector<bool> visited(V, false);
        vector<int> queue;

        visited[start] = true;
        queue.push_back(start);

        while (!queue.empty()) {
            vector<int> next_level;

            for (int node : queue) {
                // Dummy code to simulate work
                int dummy = 0;
                for (int i = 0; i < 1000; ++i) {
                    dummy += i * i;
                }
                // End of dummy code
                for (int neighbor : adj[node]) {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        next_level.push_back(neighbor);
                    }
                }
            }

            queue = next_level;
        }
    }

    void parallelBFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;
        q.push(start);
        visited[start] = true;

        while (!q.empty()) {
            int level_size = q.size();
            vector<int> current_level;

            // Collect nodes for current level
            for (int i = 0; i < level_size; i++) {
                int node = q.front();
                q.pop();
                current_level.push_back(node);
            }

            // Thread-local queues for next level
            vector<vector<int>> next_level_threads(omp_get_max_threads());

            // Parallel processing of neighbors
            #pragma omp parallel
            {
                int tid = omp_get_thread_num();  // Get thread ID

                for (int i = tid; i < current_level.size(); i += omp_get_num_threads()) {
                    int node = current_level[i];
                    // Dummy code to simulate work
                    int dummy = 0;
                    for (int i = 0; i < 1000; ++i) {
                        dummy += i * i;
                    }
                    // End of dummy code
                    // Check each neighbor
                    for (int neighbor : adj[node]) {
                        if (!visited[neighbor]) {
                            visited[neighbor] = true;
                            next_level_threads[tid].push_back(neighbor);  // Add to thread-local list
                        }
                    }
                }
            }

            // Merge thread-local next level lists into the main queue
            for (int tid = 0; tid < omp_get_max_threads(); ++tid) {
                for (int node : next_level_threads[tid]) {
                    q.push(node);
                }
            }
        }
    }
};

int main() {

    omp_set_num_threads(12); // Setting threads according to the system

    // Large binary tree for benchmark
    Graph g(1000000);
    for (int i = 0; 2 * i + 2 < 1000000; i++) {
        g.addEdge(i, 2 * i + 1);
        g.addEdge(i, 2 * i + 2);
    }

    // Large grid for benchmark
    // int rows = 1000;
    // int cols = 1000;
    // Graph g(rows * cols);

    // for (int r = 0; r < rows; ++r) {
    //     for (int c = 0; c < cols; ++c) {
    //         int node = r * cols + c;
    //         if (c < cols - 1) g.addEdge(node, node + 1);         // Right neighbor
    //         if (r < rows - 1) g.addEdge(node, node + cols);      // Bottom neighbor
    //     }
    // }

    // Sequential BFS Timing
    cout << "Sequential BFS: ";
    double start_seq = omp_get_wtime();
    g.sequentialBFS(0);
    double end_seq = omp_get_wtime();
    cout << "\nSequential BFS Time: " << (end_seq - start_seq) << " seconds\n";

    // Parallel BFS Timing
    cout << "\nParallel BFS: ";
    double start_par = omp_get_wtime();
    g.parallelBFS(0);
    double end_par = omp_get_wtime();
    cout << "\nParallel BFS Time: " << (end_par - start_par) << " seconds\n";

    return 0;
}
