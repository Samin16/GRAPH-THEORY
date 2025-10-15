#include<bits/stdc++.h>
using namespace std;

class Graph
{
    int numVertices;
    vector<vector<pair<int,int>>> adjList;
    vector<tuple<int,int,int>> edgeList;
    vector<int> h;
    vector<vector<int>> dist;
    vector<bool> visited;
public:
    Graph(int n)
    {
        numVertices=n;
        adjList.resize(n);
        h.resize(n+1,INT_MAX);
        dist=vector<vector<int>>(numVertices,vector<int>(numVertices,INT_MAX));
    }

    // Add directed edge u → v with weight w
    void addEdge(int u,int v,int w)
    {
        adjList[u].push_back({v,w});
        edgeList.push_back({u,v,w});
    }

    // Add extra vertex connected to all others with 0-weight edges
    void addSuperSource()
    {
        for(int i=0;i<numVertices;i++)
        {
            edgeList.push_back({numVertices,i,0});
        }
    }

    bool bellmanFord();   // Finds vertex potentials h[] using Bellman-Ford
    void adjustWeight();  // Reweights edges to remove negatives
    void dijkstra(int start); // Runs Dijkstra from given start vertex

    // Johnson’s algorithm: combines Bellman-Ford + Dijkstra
    void Johnson()
    {
        addSuperSource();
        if(!bellmanFord())
        {
            cout<<"Negative weight cycle detected\n";
            return;
        }
        adjustWeight();
        for(int i=0;i<numVertices;i++)
        {
            dijkstra(i);
        }
        // Restore original distances using potentials
        for(int i=0;i<numVertices;i++)
        {
            for(int j=0;j<numVertices;j++)
            {
                if(dist[i][j]!=INT_MAX)
                {
                    dist[i][j]=dist[i][j]-h[i]+h[j];
                }
            }
        }
    }

    // Print all-pairs shortest path matrix
    void printDistMat()
    {
        cout<<"All pair shortest distances : \n";
        for(int i=0;i<numVertices;i++)
        {
            for(int j=0;j<numVertices;j++)
            {
                if(dist[i][j]==INT_MAX)
                {
                    cout<<"INF  ";
                }
                else 
                {
                    cout<<dist[i][j]<<"  ";
                }
            }
            cout<<"\n";
        }
    }
};

// Bellman-Ford from super source to compute h[] (potential)
bool Graph::bellmanFord()
{
    h[numVertices]=0;
    for(int i=0;i<=numVertices;i++)
    {
        for(auto [u,v,w]:edgeList) 
        {
            if(h[u]!=INT_MAX && h[v]>h[u]+w)
            {
                h[v]=h[u]+w;
            }
        }
    }
    for(auto [u,v,w]:edgeList)
    {
        if(h[u]!=INT_MAX && h[v]>h[u]+w)
        {
            return false;
        }
    }
    return true;
}

// Adjust edge weights using h[] to ensure non-negative edges
void Graph::adjustWeight()
{
    for(int i=0;i<numVertices;i++)
    {
        for(auto& p:adjList[i])
        {
            int v=p.first,w=p.second;
            p.second=w+h[i]-h[v];
        }
    }
}

// Standard Dijkstra’s algorithm for single-source shortest paths
void Graph::dijkstra(int start)
{
    visited.assign(numVertices,false);
    dist[start][start]=0;
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<>> pq;
    pq.push({0,start});
    
    while(!pq.empty())
    {
        auto [d,u]=pq.top();
        pq.pop();
        if(dist[start][u]<d || visited[u]) 
        {
            continue;
        }
        visited[u]=true;
        for(auto [v,w]:adjList[u])
        {
            if(dist[start][u]!=INT_MAX && dist[start][v]>dist[start][u]+w)
            {
                dist[start][v]=dist[start][u]+w;
                pq.push({dist[start][v],v});
            }
        }
    }
}

// Driver code for testing Johnson’s algorithm
int main() {
    /*
       Example graph from CLRS (with negative edges)
         0 → 1 (3)
         0 → 2 (8)
         0 → 4 (-4)
         1 → 3 (1)
         1 → 4 (7)
         2 → 1 (4)
         3 → 0 (2)
         3 → 2 (-5)
         4 → 3 (6)
    */
    Graph g(5);
    g.addEdge(0, 1, 3);
    g.addEdge(0, 2, 8);
    g.addEdge(0, 4, -4);
    g.addEdge(1, 3, 1);
    g.addEdge(1, 4, 7);
    g.addEdge(2, 1, 4);
    g.addEdge(3, 0, 2);
    g.addEdge(3, 2, -5);
    g.addEdge(4, 3, 6);

    g.Johnson();
    g.printDistMat();

    return 0;
}

// Output :  All pair shortest distances :
//           0  1  -3  2  -4
//           3  0  -4  1  -1
//           7  4  0  5  3
//           2  -1  -5  0  -2
//           8  5  1  6  0