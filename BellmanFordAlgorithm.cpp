// Bellman Ford Algorithm In Edge Graph
// Author:Samin Yasar

#include<bits/stdc++.h>
using namespace std;

class Edge         // An edge contains from(u),to(v),weight(w) which tells there is an edge u->v with w weight
{
    int from,to;
    double weight;
public:
    Edge(){}
    Edge(int from,int to,double weight)
    {
        this->from=from;
        this->to=to;
        this->weight=weight;
    }
    int getFrom()
    {
        return from;
    }
    int getTo()
    {
        return to;
    }
    double getWeight()
    {
        return weight;
    }
};


vector<double> bellManFord(vector<Edge> edges,int V,int start)
{
    vector<double> dist(V,INT_MAX);
    dist[start]=0;

    // Only the worst case needs V-1 iterations for BF algorithm
    // Another stopping condition is when we're unable to relax an
    // edge, this means we have reached the optimal solution early
    bool relaxedEdge=true;
    for(int v=0;v<V-1 && relaxedEdge;v++)
    {
        relaxedEdge=false;
        for(Edge edge:edges)
        {                          // Edge relaxation
            if(dist[edge.getFrom()]+edge.getWeight()<dist[edge.getTo()])
            {
                dist[edge.getTo()]=dist[edge.getFrom()]+edge.getWeight();
                relaxedEdge=true;
            }
        }
    }

    // Run this algorithm second time to detect nodes that are 
    // Part of a negative cycle . A negative cycle has occured if
    // we can find a better path even after optimal solution

    relaxedEdge=true;
    for(int v=0;v<V-1 && relaxedEdge;v++)
    {
        relaxedEdge=false;
        for(Edge edge:edges)
        {
            if(dist[edge.getFrom()]+edge.getWeight()<dist[edge.getTo()]) // If edge relaxation possible after 
            {                                                            // previous step then the node is in a
                dist[edge.getTo()]=INT_MIN;                              // negative cycle or affected by negative cycle
                relaxedEdge=true;
            }
        }
    }
    return dist;
}

int main()
{
    int E = 10, V = 9, start = 0;
    vector<Edge> edges(E);
    edges[0] = Edge(0, 1, 1);
    edges[1] = Edge(1, 2, 1);
    edges[2] = Edge(2, 4, 1);
    edges[3] = Edge(4, 3, -3);
    edges[4] = Edge(3, 2, 1);
    edges[5] = Edge(1, 5, 4);
    edges[6] = Edge(1, 6, 4);
    edges[7] = Edge(5, 6, 5);
    edges[8] = Edge(6, 7, 4);
    edges[9] = Edge(5, 7, 3);
    vector<double> dist=bellManFord(edges,V,start);
    for(int i=0;i<V;i++)
    {
        if(dist[i]==INT_MIN)
        {
            cout<<"The cost from node 0 to "<<i<<" is "<<"Negative Infinity"<<endl;
        }
        else if(dist[i]==INT_MAX)
        {
            cout<<"Node 0 to node "<<i<<" is Not Reachable"<<endl;
        }
        else
        {
            cout<<"The cost from node 0 to "<<i<<" is "<<dist[i]<<endl;
        }
    }
    return 0;
}

// Output : The cost from node 0 to 0 is 0
//          The cost from node 0 to 1 is 1
//          The cost from node 0 to 2 is Negative Infinity
//          The cost from node 0 to 3 is Negative Infinity
//          The cost from node 0 to 4 is Negative Infinity
//          The cost from node 0 to 5 is 5
//          The cost from node 0 to 6 is 5
//          The cost from node 0 to 7 is 8
//          Node 0 to node 8 is Not Reachable