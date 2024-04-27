#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
#include <stack>

using namespace std;

// Structure to represent a graph node
struct Node {
    int id;
    vector<int> neighbors;
    bool visited;
};

// Function to add an edge between two nodes
void addEdge(vector<Node>& graph, int u, int v) {
    graph[u].neighbors.push_back(v);
    graph[v].neighbors.push_back(u);
}

// Function to perform Breadth First Search in parallel
void parallelBFS(vector<Node>& graph, int start) {
    #pragma omp parallel
    {
        queue<int> q;
        #pragma omp single
        {
            q.push(start);
            graph[start].visited = true;
        }

        while (!q.empty()) {
            int current;
            #pragma omp critical
            {
                current = q.front();
                q.pop();
            }

            // Process current node
            cout << "Visited node: " << current << endl;

            // Visit neighbors in parallel
            #pragma omp for
            for (int neighbor : graph[current].neighbors) {
                if (!graph[neighbor].visited) {
                    #pragma omp critical
                    {
                        q.push(neighbor);
                        graph[neighbor].visited = true;
                    }
                }
            }
        }
    }
}

// Function to perform Depth First Search in parallel
void parallelDFS(vector<Node>& graph, int start) {
    #pragma omp parallel
    {
        stack<int> stk;
        #pragma omp single
        {
            stk.push(start);
            graph[start].visited = true;
        }

        while (!stk.empty()) {
            int current;
            #pragma omp critical
            {
                current = stk.top();
                stk.pop();
            }

            // Process current node
            cout << "Visited node: " << current << endl;

            // Visit neighbors in parallel
            #pragma omp for
            for (int neighbor : graph[current].neighbors) {
                if (!graph[neighbor].visited) {
                    #pragma omp critical
                    {
                        stk.push(neighbor);
                        graph[neighbor].visited = true;
                    }
                }
            }
        }
    }
}

int main() {
    // Construct the graph
    int numNodes = 7;
    vector<Node> graph(numNodes);

    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 1, 4);
    addEdge(graph, 2, 5);
    addEdge(graph, 2, 6);

    // Initialize visited flag for all nodes
    for (Node& node : graph) {
        node.visited = false;
    }

    cout << "Parallel Breadth First Search:" << endl;
    parallelBFS(graph, 0);

    // Reset visited flag for all nodes
    for (Node& node : graph) {
        node.visited = false;
    }

    cout << "\nParallel Depth First Search:" << endl;
    parallelDFS(graph, 0);

    return 0;
}
