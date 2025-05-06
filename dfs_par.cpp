#include <iostream>
#include <vector>
#include <stack>
#include <atomic>
#include <omp.h>

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

    void sequentialDFS(int start) {
        vector<bool> visited(V, false);
        stack<int> st;
        st.push(start);

        while (!st.empty()) {
            int node = st.top();
            st.pop();

            if (visited[node]) continue;
            visited[node] = true;

            // Simulated work
            int dummy = 0;
            for (int i = 0; i < 1000; ++i){
                dummy += i * i;
            }
            for (auto it = adj[node].rbegin(); it != adj[node].rend(); ++it) {
                if (!visited[*it]) {
                    st.push(*it);
                }
            }
        }
    }

    void parallelDFS(int start) {
        vector<atomic<bool>> visited(V);
        for (int i = 0; i < V; ++i) visited[i] = false;

        stack<int> shared_stack;
        shared_stack.push(start);

        #pragma omp parallel
        {
            while (true) {
                int node = -1;

                // Safely pop from shared stack
                #pragma omp critical
                {
                    if (!shared_stack.empty()) {
                        node = shared_stack.top();
                        shared_stack.pop();
                    }
                }

                if (node == -1) break;

                bool expected = false;
                if (!visited[node].compare_exchange_strong(expected, true)) {
                    continue;
                }

                // Begin dummy code
                int dummy = 0;
                for (int i = 0; i < 1000; ++i){
                    dummy += i * i;
                }
                // End of dummy code
                // Push neighbors in reverse order (DFS style)
                for (auto it = adj[node].rbegin(); it != adj[node].rend(); ++it) {
                    if (!visited[*it]) {
                        #pragma omp critical
                        shared_stack.push(*it);
                    }
                }
            }
        }
    }
};

int main() {
    omp_set_num_threads(12); // Set number of threads

    // ---------- Choose graph type ----------
    //Binary Tree:
    int nodes = 1000000;
    Graph g(nodes);
    for (int i = 0; 2 * i + 2 < nodes; i++) {
        g.addEdge(i, 2 * i + 1);
        g.addEdge(i, 2 * i + 2);
    }

    // OR Grid (comment out tree above if using this):
    
    // int rows = 1000;
    // int cols = 1000;
    // Graph g(rows * cols);
    // for (int r = 0; r < rows; ++r) {
    //     for (int c = 0; c < cols; ++c) {
    //         int node = r * cols + c;
    //         if (c < cols - 1) g.addEdge(node, node + 1);
    //         if (r < rows - 1) g.addEdge(node, node + cols);
    //     }
    // }
    

    // ---------- Sequential DFS ----------
    cout << "Sequential DFS: ";
    double start_seq = omp_get_wtime();
    g.sequentialDFS(0);
    double end_seq = omp_get_wtime();
    cout << "\nSequential DFS Time: " << (end_seq - start_seq) << " seconds\n";

    // ---------- Parallel DFS ----------
    cout << "\nParallel DFS: ";
    double start_par = omp_get_wtime();
    g.parallelDFS(0);
    double end_par = omp_get_wtime();
    cout << "\nParallel DFS Time: " << (end_par - start_par) << " seconds\n";

    return 0;
}
