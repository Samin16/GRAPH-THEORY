// BFS and shortest path


#include<bits/stdc++.h>
using namespace std;

class Graph
{
    int numVertices;              // Number of vertices
    vector<vector<int>> adjList;  // Adjacency list representation
    vector<bool> visited;         // Track if the node visited or not
    vector<int> prev;             // Storing previous nodes to make path
public:
    Graph(int n)                  // Initialization
    {
        numVertices=n;
        adjList.resize(n);
        visited.resize(n,false);
        prev.resize(n,-1);
    }
    void addEdge(int s,int e)
    {
        adjList[s].push_back(e);
        adjList[e].push_back(s);
    }
    vector<int> shortestPath(int s,int e)
    {
        vector<int> med=bfs(s);     // Doing BFS from starting node s
        return makePath(s,e,med);   // Make a path from start s to end e
    }
    vector<int> bfs(int s)          // Simple BFS Algorithm
    {
        queue<int> q;
        q.push(s);
        visited[s]=true;
        while(!q.empty())
        {
            int m=q.front();
            q.pop();
            for(int next:adjList[m])
            {
                if(!visited[next])
                {
                    q.push(next);
                    visited[next]=true;
                    prev[next]=m;       // Prev of next is dequed element m
                }
            }
        }
        return prev;
    }
    vector<int> makePath(int s,int e,vector<int>& med)
    {
        vector<int> path;
        for(int i=e;i!=-1;i=prev[i])     // Builds path starting from end node
        {                                // and iterate until finds end or no Prev
            path.push_back(i);
        }
        reverse(path.begin(),path.end()); 
        if(path[0]==s)                   // If reversed path starts from s then 
        {                                // we get the path
            return path;
        }
        return {};
    }
};

int main()
{
    Graph g(8);
    g.addEdge(0,1);
    g.addEdge(0,2);
    g.addEdge(1,3);
    g.addEdge(2,3);
    g.addEdge(2,4);
    g.addEdge(3,4);
    g.addEdge(4,5);
    g.addEdge(4,6);
    g.addEdge(5,7);
    g.addEdge(6,7);
    vector<int> res=g.shortestPath(0,7);
    for(int i=0;i<res.size()-1;i++)
    {
        cout<<res[i]<<"->";
    }
    cout<<res[res.size()-1];
    cout<<"\n";
    return 0;
}

//                Output:  0->2->4->5->7