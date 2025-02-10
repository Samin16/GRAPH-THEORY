// Kruskal Algorithm Without path compression


#include<bits/stdc++.h>
using namespace std;

#define edge pair<int,int>

class Graph
{
    vector<pair<int,edge>> G;             // Graph
    vector<pair<int,edge>> T;             // MST
    int* parent;                          // To use in union find
    int V;                                // Number of vertices
public:
    Graph(int V);
    void addWeightedEdge(int u,int v,int w);
    int findSet(int i);
    void unionSet(int u,int v);
    void kruskal();
    void print();
};

Graph::Graph(int V)              // Initialization
{
    this->V=V;
    parent=new int[V];
    for(int i=0;i<V;i++)
    {
        parent[i]=i;
    }
    G.clear();
    T.clear();
}

void Graph::addWeightedEdge(int u,int v,int w) 
{
    G.push_back(make_pair(w,edge(u,v)));      // Add an edge with its weight
}

int Graph::findSet(int i)             // Union find data structure to
{                                     // to know the parent or which 
    if(i==parent[i])                  // set node i belongs to
    {
        return i;
    }
    else
    {
        return findSet(parent[i]);
    }
}

void Graph::unionSet(int u,int v)      // After adding to MST update parent 
{                                      // for v
    int uRoot=findSet(u);
    int vRoot=findSet(v);
    parent[uRoot]=parent[vRoot];
}

void Graph::kruskal()
{
    int i,uRep,vRep;
    sort(G.begin(),G.end());           // Sort the edges according to increasing Weight
    for(int i=0;i<G.size();i++)        // Iterate through all edges
    {
        uRep=findSet(G[i].second.first);
        vRep=findSet(G[i].second.second);
        if(uRep!=vRep)                // If two vertices are not in the same set
        {                             // add it to MST and update parent
            T.push_back(G[i]);
            unionSet(uRep,vRep);
        }
    }
}

void Graph::print()
{
    cout<<"Edge : Weight"<<endl;
    for(int i=0;i<T.size();i++)
    {
        cout<<T[i].second.first<<"-"<<T[i].second.second<<":"<<T[i].first<<endl;
    }
}

int main()
{
    Graph g(6);
    g.addWeightedEdge(0,1,4);
    g.addWeightedEdge(0,2,4);
    g.addWeightedEdge(1,2,2);
    g.addWeightedEdge(1,0,4);
    g.addWeightedEdge(2,0,4);
    g.addWeightedEdge(2,1,2);
    g.addWeightedEdge(2,3,3);
    g.addWeightedEdge(2,5,2);
    g.addWeightedEdge(2,4,4);
    g.addWeightedEdge(3,2,3);
    g.addWeightedEdge(3,4,3);
    g.addWeightedEdge(4,2,4);
    g.addWeightedEdge(4,3,3);
    g.addWeightedEdge(5,2,2);
    g.addWeightedEdge(5,4,3);
    g.kruskal();
    g.print();
    return 0;
}

// Output :    Edge : Weight
//             1-2 : 2
//             2-5 : 2
//             2-3 : 3
//             3-4 : 3
//             0-1 : 4