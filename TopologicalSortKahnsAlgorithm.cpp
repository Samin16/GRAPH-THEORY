// Kahns Algorithm


#include <bits/stdc++.h>
using namespace std;

vector<int> topologicalSort(vector<vector<int>> &adjList, int V)
{
    vector<int> indegree(V);
    for (int i = 0; i < V; i++)    // Calculate the indegree
    {
        for (auto i : adjList[i])
        {
            indegree[i]++;
        }
    }
    queue<int> q;
    for (int i = 0; i < V; i++)     // Searches for nodes with indegree 0
    {
        if (indegree[i] == 0)
        {
            q.push(i);
        }
    }
    vector<int> result;
    while (!q.empty())          // Queue contains nodes with indegree 0 
    {
        int node = q.front();
        q.pop();
        result.push_back(node);
        for (auto i : adjList[node])
        {
            indegree[i]--;         
            if (indegree[i] == 0)
            {
                q.push(i);
            }
        }
    }
    if (result.size() != V)     // If the queue is empty but some nodes are left then Graph is not a DAG
    {
        cout << "Graph contains cycle\n";
        return {};
    }
    return result;
}

int main()
{
    int V,E;
    cin>>V>>E;
    vector<vector<int>> edges(E,vector<int>(2));
    for(int i=0;i<E;i++)
    {
        cin>>edges[i][0]>>edges[i][1];
    }
    vector<vector<int>> adj(V);
    for(auto i:edges)
    {
        adj[i[0]].push_back(i[1]);
    }
    vector<int> result=topologicalSort(adj,V);
    for(auto i:result)
    {
        cout<<i<<" ";
    }
    cout<<"\n";
    return 0;
}

// Input :   6 6
//           1 2
//           1 0
//           2 4
//           4 5
//           3 5
//           3 0
// Output :  1 3 2 0 4 5