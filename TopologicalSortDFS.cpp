// Topological Sorting using DFS 


#include<bits/stdc++.h>
using namespace std;

// Do DFS to calculate finishing time 
void topologicalSortingUtil(int v,vector<vector<int>>& adj,vector<bool>& visited,stack<int>& Stack)
{
    visited[v]=true;
    for(int i:adj[v])
    {
        if(!visited[i])
        {
            topologicalSortingUtil(i,adj,visited,Stack);
            visited[i]=true;
        }
    }
    Stack.push(v);         // Push in stack on basis of finishing time
}

void topologicalSort(vector<vector<int>>& adj,int V)   // First calculates the finishing time 
{                                                      // and stores them in a stack
    stack<int> Stack;
    vector<bool> visited(V,false);
    for(int i=0;i<V;i++)
    {
        if(!visited[i])
        {
            topologicalSortingUtil(i,adj,visited,Stack);
        }
    }
    while(!Stack.empty())           // pops from top
    {
        cout<<Stack.top()<<" ";
        Stack.pop();
    }
    cout<<"\n";
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
    topologicalSort(adj,V);
    return 0;
}

// Input :   6 6
//           1 2
//           1 0
//           2 4
//           4 5
//           3 5
//           3 0
// Output :  3 1 2 4 5 0 