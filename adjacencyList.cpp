// Adjacency list


#include<bits/stdc++.h>
using namespace std;

class Graph
{
    int numVertices;
    list<int>* adjVertices;         // List to store adjacent nodes of a node
public:
    Graph(int vertex);
    void addEdge(int s,int d);
    void printGraph();
};

Graph::Graph(int vertex)             // Initialization
{
    numVertices=vertex;
    adjVertices=new list<int>[numVertices];
}

void Graph::addEdge(int s,int d)            // Adding an undirected edge
{
    adjVertices[s].push_back(d);
    adjVertices[d].push_back(s);
}

void Graph::printGraph()
{
    for(int v=0;v<numVertices;v++)
    {
        cout<<"Vertex "<<v<<":";
        for(auto x:adjVertices[v])
        {
            cout<<"->"<<x;
        }
        cout<<"\n";
    }
}

int main()
{
    Graph g(4);
    g.addEdge(0,1);
    g.addEdge(0,2);
    g.addEdge(0,3);
    g.addEdge(1,2);
    g.printGraph();
    return 0;
}
