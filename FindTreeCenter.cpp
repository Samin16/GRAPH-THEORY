// Finding center of a tree


#include<bits/stdc++.h>
using namespace std;

class Graph
{
    int numVertices;
    vector<vector<int>> adj;     // Adjacency list representation of Graph
    vector<int> degree;          // Array to count degree
    vector<int> leaves;          // To track the current leaf nodes
public:
    Graph(int n)                 // Initialization
    {
        numVertices=n;
        adj.resize(n);
        degree.resize(n,0);
    }
    void addEdge(int s,int d)    // Adds an undirected edge
    {
        adj[s].push_back(d);
        adj[d].push_back(s);
    }
    void countDegree()           // Counting degree of each node
    {
        for(int i=0;i<numVertices;i++)
        {
            degree[i]=adj[i].size();
        }
    }
    void findLeaves()           // Finds node with degree = 1
    {
        for(int i=0;i<numVertices;i++)
        {
            if(degree[i]==1)
            {
                leaves.push_back(i);
                degree[i]=0;
            }
        }
    }
    void findCenter()
    {
        countDegree();
        findLeaves();
        int count=leaves.size();       // Counts the number of leaves  
        while(count<numVertices)       // Loop until all vertices are encountered
        {
            vector<int> newLeaves;
            for(int leaf:leaves)
            {
                for(int neighbour:adj[leaf])   // For each node v adjacent to u decrease uts degree
                {                              // and push it in newLeaves
                    degree[neighbour]--;
                    if(degree[neighbour]==1)
                    {
                        newLeaves.push_back(neighbour);
                    }
                }
                degree[leaf]=0;
            }
            count+=newLeaves.size();         // After one loop increase the leaf count
            leaves=newLeaves;                // and set leaves to newLeaves
        }
    }
    vector<int> getLeaves()
    {
        return leaves;           // 1 or 2 leaves will remain and that is the answer ; returns these nodes
    }
};

int main()
{
    Graph g(10);
    g.addEdge(0,1);
    g.addEdge(1,2);
    g.addEdge(2,3);
    g.addEdge(3,4);
    g.addEdge(3,5);
    g.addEdge(2,9);
    g.addEdge(2,6);
    g.addEdge(6,7);
    g.addEdge(6,8);
    g.findCenter();
    vector<int> leaves=g.getLeaves();
    for(int element:leaves)
    {
        cout<<element<<" ";
    }
    return 0;
}

//    Output :    2