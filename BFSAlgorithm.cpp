// BFS Algorithm


#include<bits/stdc++.h>
using namespace std;

class Graph
{
    int numVertices;                // Number of vertices
    vector<vector<int>> adj;        // Adjacency List representation of Graph
    vector<bool> visited;           // To track if a node is visited or not
public:
    Graph(int n);
    void addEdge(int src,int dest);
    void bfs(int s);
    void bfsDisconnected();
};

Graph::Graph(int n)                // Initialization
{
    numVertices=n;
    adj.resize(n);
    visited.resize(n,false);       // At first no vertex is visited yet
}

void Graph::addEdge(int src,int dest)    // Adds edge of an undirected graph
{
    adj[src].push_back(dest);
    adj[dest].push_back(src);
}

void Graph::bfs(int s)                   // BFS starting with Node s
{
    queue<int> q;                        // Queue to store the node first visited
    visited[s]=true;
    q.push(s);
    while(!q.empty())                    // Go through until no element left in queue
    {
        int curr=q.front();              // First element in Queue
        q.pop();
        cout<<curr<<" ";
        for(int x:adj[curr])            // If any vertex in the adjacency list ot the
        {                               // current node is not visited add it to Queue
            if(!visited[x])             // and mark it as visited
            {
                visited[x]=true;
                q.push(x);
            }
        }
    }
}

void Graph::bfsDisconnected()
{
    for(int i=0;i<numVertices;i++)       // Starting from node 0 perform BFS
    {                       
        if(!visited[i])                  // If any node is not visited do BFS
        {                                // again and get a different BFS Tree
            bfs(i);
            cout<<"\n";  
        }
    }
}

int main()
{
    Graph g(6);
    g.addEdge(0,1);
    g.addEdge(0,2);
    g.addEdge(3,4);
    g.addEdge(4,5);
    g.bfsDisconnected();
    return 0;
}

//               Output : 0 1 2
//                        3 4 5