// Adjacency matrix


#include<bits/stdc++.h>
using namespace std;

class Graph
{
    bool** adjMatrix;  // Adjacency matrix for an unweighted graph
    int numVertices;   // Number of vertices in the graph
public:
    Graph(int numVertices)           // Initialization
    {
        this->numVertices=numVertices;
        adjMatrix=new bool*[numVertices];
        for(int i=0;i<numVertices;i++)
        {
            adjMatrix[i]=new bool[numVertices];
            for(int j=0;j<numVertices;j++)
            {
                adjMatrix[i][j]=false;
            }
        }
    }
    void addEdge(int s,int d)     // Add an edge for an undirected graph
    {
        adjMatrix[s][d]=true;
        adjMatrix[d][s]=true;
    }
    void removeEdge(int s,int d)     // Removes an edge from adjacency matrix
    {
        adjMatrix[s][d]=false;
        adjMatrix[d][s]=false;
    }
    void toString()
    {
        for(int i=0;i<numVertices;i++)
        {
            cout<<i<<" : ";
            for(int j=0;j<numVertices;j++)
            {
                cout<<adjMatrix[i][j]<<" ";
            }
            cout<<"\n";
        }
    }
    ~Graph()
    {
        for(int i=0;i<numVertices;i++)
        {
            delete[] adjMatrix[i];
        }
        delete[] adjMatrix;
    }
};

int main()                   // main function for a graph with 4 vertices
{
    Graph g(4);
    g.addEdge(0,1);
    g.addEdge(0,2);
    g.addEdge(1,2);
    g.addEdge(2,0);
    g.addEdge(2,3);
    g.toString();
    return 0;
}