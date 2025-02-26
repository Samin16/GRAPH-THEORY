#include<bits/stdc++.h>
using namespace std;

class Edge         // Edge class to represent each edge
{
    int v,w;
public:
    Edge(int v,int w)
    {
        this->v=v;
        this->w=w;
    }
    int getV()
    {
        return v;
    }
    int getW()
    {
        return w;
    }
};

class Graph
{
    int numVertices;                   // Number of vertices
    vector<vector<Edge>> adjList;      // Adjacency list
    vector<bool> visited;              // If a node is visited or not
    vector<tuple<int,int,int>> result; // To store result
    priority_queue<tuple<int,int,int>,vector<tuple<int,int,int>>,greater<tuple<int,int,int>>> pq;  // Min Priority Queue
    int mstCost;                        // Cost of the MST
public:
    Graph(int numVertices)
    {
        this->numVertices=numVertices;
        adjList.resize(numVertices);
        visited.resize(numVertices,false);
        mstCost=0;
    }
    void addEdge(int u,int v,int w)       // Add an undirected edge
    {
        adjList[u].push_back(Edge(v,w));
        adjList[v].push_back(Edge(u,w));
    }
    vector<tuple<int,int,int>> primsAlgorithm(int s=0)
    {
        int edges=numVertices-1;         // From the starting node(default 0) add all
        int edgeCount=0;                 // the edges pointing to unvisited nodes in a priority
        addEdges(s);                     // queue and pop the minimum weighted edge and add it 
        while(!pq.empty() && edgeCount!=edges)     // to the MST and to same until
        {                                          // PQ is empty or we have added 
            auto[w,u,v]=pq.top();                  // # of vertices-1 edges to MST
            pq.pop();
            if(visited[v])
            {
                continue;
            }
            result.push_back({u,v,w});
            mstCost+=w;
            edgeCount++;
            addEdges(v);
        }
        if(edgeCount!=edges)
        {
            return {};
        }
        return result;
    }
    void addEdges(int nodeIndex)         // Adds all the edges of a node pointing to
    {                                    // to unvisited nodes
        visited[nodeIndex]=true; 
        for(Edge e:adjList[nodeIndex])
        {
            if(!visited[e.getV()])
            {
                pq.push({e.getW(),nodeIndex,e.getV()});
            }
        }
    }
    int getMSTCost()            // Returns the MST Cost
    {
        return mstCost;
    }
};

int main() 
{
    // Create a graph with 5 vertices (numbered 0 through 4)
    int n = 9;
    Graph g(n);
    
    // Add edges: (u, v, weight)
    g.addEdge(0,1,4);
    g.addEdge(0,7,8);
    g.addEdge(1, 2, 8);
    g.addEdge(1, 7, 11);
    g.addEdge(7,8,7);
    g.addEdge(7,6,1);
    g.addEdge(8,6,5);
    g.addEdge(8,2,2);
    g.addEdge(6,5,2);
    g.addEdge(5,2,4);
    g.addEdge(2,3,7);
    g.addEdge(3,5,14);
    g.addEdge(3,4,9);
    g.addEdge(4,5,10);
    
    // Run Prim's algorithm to compute the MST
    auto mst = g.primsAlgorithm();
    if(mst.empty()) {
        cout << "The graph is not connected, so an MST does not exist." << endl;
        return 0;
    }
    
    // int totalCost = 0;
    cout << "Edges in the Minimum Spanning Tree:" << endl;
    // Each tuple is stored as (u, v, w)
    for(auto tup : mst) 
    {
        int u, v, w;
        tie(u, v, w) = tup;
        cout << u << " - " << v << " with weight " << w << endl;
    }
    cout << "Total MST cost: " << g.getMSTCost() << endl;
    
    return 0;
}

// Output : Edges in the Minimum Spanning Tree:
//          0 - 1 with weight 4
//          0 - 7 with weight 8
//          7 - 6 with weight 1
//          6 - 5 with weight 2
//          5 - 2 with weight 4
//          2 - 8 with weight 2
//          2 - 3 with weight 7
//          3 - 4 with weight 9
//          Total MST cost: 37