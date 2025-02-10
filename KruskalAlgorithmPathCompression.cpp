// Kruskal's Algorithm Using path compression
#include<bits/stdc++.h>
using namespace std;

#define edge pair<int,int>

class Graph
{
    int V;
    vector<pair<int,edge>> G;
    vector<pair<int,edge>> T;
    vector<int> parent;
    vector<int> rank;
public:
    Graph(int V);
    void addEdge(int u,int v,int w);
    int findSet(int i);
    void unionSet(int u,int v);
    vector<pair<int, edge>> kruskal();
};

Graph::Graph(int V)
{
    this->V=V;
    parent.resize(V);
    for(int i=0;i<V;i++)
    {
        parent[i]=i;
    }
    rank.resize(V,0);
    G.clear();
    T.clear();
}

void Graph::addEdge(int u,int v,int w)
{
    G.push_back(make_pair(w,edge(u,v)));
}

int Graph::findSet(int i)
{
    if(parent[i]!=i)                 // Path compression
    {
        parent[i]=findSet(parent[i]);
    }
    return parent[i];
}

void Graph::unionSet(int u,int v)
{
    int rootU=findSet(u);           // Updating parent
    int rootV=findSet(v);
    parent[rootU]=rootV;
}

// void Graph::unionSet(int u,int v)
// {
//     // parent[u]=parent[v];
//     int rootU=findSet(u);
//     int rootV=findSet(v);
//     if(rank[rootU]>rank[rootV])
//     {
//         parent[rootV]=rootU;
//     }
//     else if(rank[rootU]<rank[rootV])
//     {
//         parent[rootU]=rootV;
//     }
//     else
//     {
//         parent[rootV]=rootU;
//         rank[rootU]++;
//     }
// }

vector<pair<int, edge>> Graph::kruskal()
{
    int i,uRep,vRep;
    sort(G.begin(),G.end());
    for(int i=0;i<G.size();i++)
    {
        uRep=findSet(G[i].second.first);
        vRep=findSet(G[i].second.second);
        if(uRep!=vRep)
        {
            T.push_back(G[i]);
            unionSet(uRep,vRep);
        }
    }
    return T;
}

int main()
{
    int V,E;
    cin>>V>>E;
    Graph g(V);
    for(int i=0;i<E;i++)
    {
        int u,v,w;
        cin>>u>>v>>w;
        g.addEdge(u,v,w);
    }
    vector<pair<int, edge>> T=g.kruskal();
    for(int i=0;i<T.size();i++)
    {
        cout<<T[i].second.first<<"-"<<T[i].second.second<<" "<<T[i].first<<"\n";
    }
    return 0;
}


// Input :
// 6 15
// 0 1 4
// 0 2 4
// 1 2 2
// 1 0 4
// 2 0 4
// 2 1 2
// 2 3 3
// 2 5 2
// 2 4 4
// 3 2 3
// 3 4 3
// 4 2 4
// 4 3 3
// 5 2 2
// 5 4 3

// Minimum Spanning Tree
// 1-2 2
// 2-5 2
// 2-3 3
// 3-4 3
// 0-1 4