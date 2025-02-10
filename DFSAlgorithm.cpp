// DFS Algorithm



#include <bits/stdc++.h>
using namespace std;

class Graph
{
    int numVertices;                // Number of vertices
    list<int> *adjLists;            
    bool *visited;                  // To track if a node is visited or not

public:
    Graph(int V);
    void addEdge(int src, int dst);
    void DFS(int vertex);
};

Graph::Graph(int V)                 // Initialization of the Graph
{
    numVertices = V;
    adjLists = new list<int>[numVertices];
    visited = new bool[numVertices];
}

void Graph::addEdge(int src, int dest)
{
    adjLists[src].push_front(dest);  // Adding an edge
    adjLists[dest].push_back(src);
}

void Graph::DFS(int vertex)           // A recursive function to do DFS
{
    visited[vertex] = true;           // Marks the vertex as visited      
    list<int> adjList = adjLists[vertex];
    cout << vertex << " ";            // vertex can also be stored in an array 
    list<int>::iterator i;          
    for (i = adjList.begin(); i != adjList.end(); i++)    // Iterate through the
    {                                                     // Adjacent nodes
        if (!visited[*i])               // Id the node is not visited yet do the
        {                               // previous work for the node using recursion
            DFS(*i);
        }
    }
}

int main()
{
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.DFS(2);
    return 0;
}

//             Result: 2 3 0 1