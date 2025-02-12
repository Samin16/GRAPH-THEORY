// Single source shortest path algorithm using topological sort
#include<bits/stdc++.h>
using namespace std;

class Edge       // Each edge u->v contains v and corresponding weight
{
public:
    int v;
    int w;
    Edge(int v,int w)
    {
        this->v=v;
        this->w=w;
    }
};

class Graph
{
    int numVertices;
    vector<vector<Edge>> adj;    // adj[u][v]=w means there is an edge with weight w
    vector<int> topSort;        // Topological ordering og the Graph
    vector<int> inDegree;        // Stores indedree for Kahn's Algorithm
public:
    Graph(int n)
    {
        numVertices=n;
        adj.resize(numVertices);
        inDegree.resize(numVertices);
    }
    void addEdge(int u,int v,int w)
    {
        adj[u].push_back(Edge(v,w));
    }
    void countIndegree()           // Counts Indegree for each vertex
    {
        for(int i=0;i<numVertices;i++)
        {
            for(Edge j:adj[i])
            {
                inDegree[j.v]++;
            }
        }
    }
    void topologicalSort()     // Topological sorting using Kahn's Algorithm 
    {
        countIndegree();
        queue<int> q;
        for(int i=0;i<numVertices;i++)
        {
            if(inDegree[i]==0)
            {
                q.push(i);
            }
        }
        while(!q.empty())
        {
            int node=q.front();
            q.pop();
            for(Edge e:adj[node])
            {
                inDegree[e.v]--;
                if(inDegree[e.v]==0)
                {
                    q.push(e.v);
                }
            }
            topSort.push_back(node);
        }
    }
    vector<int> shortestPath(int start)    // Distance count using the topologically sorted array
    {
        topologicalSort();       // The elements before 'start' in topologically sorted array then they are not reachable from start
        vector<int> dist(numVertices,INT_MAX);
        dist[start]=0;        // Sets distance from 'start' to 'start' to 0
        for(int u:topSort)
        {
            if(dist[u]!=INT_MAX)  // Actually starts after finding 'start' element
            {
                for(Edge e:adj[u])
                {
                    if(dist[e.v]>dist[u]+e.w)   // Edge Relaxation
                    {
                        dist[e.v]=dist[u]+e.w;
                    }
                }
            }
        }
        return dist;
    }
};

int main()
{
    Graph g(6);
    g.addEdge(0,1,5);
    g.addEdge(0,2,3);
    g.addEdge(1,3,6);
    g.addEdge(1,2,2);
    g.addEdge(2,4,4);
    g.addEdge(2,5,2);
    g.addEdge(2,3,7);
    g.addEdge(3,4,-1);
    g.addEdge(4,5,-2);
    int start=0;
    //int start=3;
    vector<int> dist=g.shortestPath(start);
    int k=0;
    for(int i:dist)
    {
        cout<<"Distance from "<<start<<" to "<<k<<" : ";
        k++;
        if(i!=INT_MAX)
           cout<<i<<"\n";
        else
           cout<<"NOT REACHABLE\n";
    }
    return 0;
}

// For start=0
// Output : Distance from 0 to 0 : 0
//          Distance from 0 to 1 : 5
//          Distance from 0 to 2 : 3
//          Distance from 0 to 3 : 10
//          Distance from 0 to 4 : 7
//          Distance from 0 to 5 : 5

// For start=3
// Output : Distance from 3 to 0 : NOT REACHABLE
//          Distance from 3 to 1 : NOT REACHABLE
//          Distance from 3 to 2 : NOT REACHABLE
//          Distance from 3 to 3 : 0
//          Distance from 3 to 4 : -1
//          Distance from 3 to 5 : -3