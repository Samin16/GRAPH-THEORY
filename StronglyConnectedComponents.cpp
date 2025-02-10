// Kosaraju's Algorithm
#include <iostream>
#include <list>
#include <stack>

using namespace std;

class Graph
{
    int V;                              // Number of vertices
    list<int> *adjList;                 // Can also use vector<vector<int>> adjList
    void fillStack(int s, bool visitedV[], stack<int> &Stack);
    void DFS(int s, bool visitedV[]);

public:
    Graph(int V);
    void addEdge(int s, int d);
    void printSCC();
    Graph transpose();
};

Graph::Graph(int V)            // Initialization
{
    this->V = V;
    adjList = new list<int>[V];  // If use vector then adjList.resize(V);
}

void Graph::addEdge(int s, int d)    // Add edge to Directed Graph
{
    adjList[s].push_back(d);
}

void Graph::DFS(int s, bool visitedV[])   // DFS algorithm to find the SCCs
{
    visitedV[s] = true;
    cout << s << " ";
    list<int>::iterator i;
    for (i = adjList[s].begin(); i != adjList[s].end(); i++)
    {
        if (visitedV[*i] == false)
        {
            DFS(*i, visitedV);
        }
    }
}

Graph Graph::transpose()     // Transpose a graph : if there is an from i to j
{                            // then in new graph it will contain j to i
    Graph g(V);
    for (int s = 0; s < V; s++)
    {
        list<int>::iterator i;
        for (i = adjList[s].begin(); i != adjList[s].end(); i++)
        {
            g.adjList[*i].push_back(s);
        }
    }
    return g;
}

void Graph::fillStack(int s, bool visitedV[], stack<int> &Stack)   // DFS to ind the finishing time of each vertex
{
    visitedV[s] = true;
    list<int>::iterator i;
    for (i = adjList[s].begin(); i != adjList[s].end(); i++)
    {
        if (visitedV[*i] == false)
        {
            fillStack(*i, visitedV, Stack);
        }
    }
    Stack.push(s);    // If no node adjacent to s then put it in a stack
}

void Graph::printSCC()
{
    stack<int> Stack;
    bool *visitedV = new bool[V];   // To track if a node is visited or not
    for (int i = 0; i < V; i++)      
    {
        visitedV[i] = false;
    }
    for (int i = 0; i < V; i++)     // DFS for all component's finishing time
    {
        if (visitedV[i] == false)
        {
            fillStack(i, visitedV, Stack);
        }
    }
    Graph gr = transpose();         // Transpose to find if we can visit from 
    for (int i = 0; i < V; i++)     // Node i to j and node j to i
    {
        visitedV[i] = false;
    }
    while (Stack.empty() == false)  // According to the finishing time find SCC
    {
        int s = Stack.top();
        Stack.pop();
        if (visitedV[s] == false)
        {
            gr.DFS(s, visitedV);
            cout << endl;
        }
    }
}

int main()
{
    Graph g(8);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(2, 4);
    g.addEdge(3, 0);
    g.addEdge(4, 5);
    g.addEdge(5, 6);
    g.addEdge(6, 4);
    g.addEdge(6, 7);

    cout << "Strongly Connected Components:\n";
    g.printSCC();
}


//     Output : Strongly Connected Components:
//              0 3 2 1 
//              4 6 5 
//              7