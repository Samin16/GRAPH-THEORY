// Tarjan's Algorithm to find SCC's of a directed Graph
// Author : Samin Yasar

#include<bits/stdc++.h>
using namespace std;

class Graph
{
    int numVertices;                 // Number of vertices
    int id;                          // Tracks id of current node during dfs 
    vector<vector<int>> adjList;     // Adjacency list representation
    vector<int> ids;                 // Tracks an id for each node
    vector<int> lowLinkValue;        // Stores Low link value (lowest node id reachable from a node during dfs)
    stack<int> seenStack;            // Stack to store ids during dfs
    vector<bool> onStack;            // If a node is in stack or not
    int sccCount;                    // Nuumber of SCC
public:
    Graph(int numVertices)           // Initializing all data
    {
        this->numVertices=numVertices;
        id=0;                           
        adjList.resize(numVertices);
        ids.resize(numVertices,-1);         // Befode dfs all ids are -1
        lowLinkValue.resize(numVertices,0);
        onStack.resize(numVertices,false);  // No node recently in stack
        sccCount=0;
    }
    void addEdge(int u,int v)               // Add edges
    {
        adjList[u].push_back(v);
    }
    void findSCCs()                        // For each unvisited (ids[i]==-1) node do dfs  
    {
        for(int i=0;i<numVertices;i++)
        {
            if(ids[i]==-1)
            {
                dfs(i);
            }
        }
    }
    void dfs(int at)                    // dfs function
    {
        seenStack.push(at);             // Push the given node on a stack
        onStack[at]=true;               // Mark it as onStack
        ids[at]=id;                     // Set an id for the node
        lowLinkValue[at]=id;            // Set id as low link value
        id++;
        for(int v:adjList[at])          // For each unvisited adjacent node do dfs and upon traceback
        {                               // if the node is on the stack set the lowLinkValue as minimum
            if(ids[v]==-1)              // of the node and the adjacent node
            {
                dfs(v);
            }
            if(onStack[v])
            {
                lowLinkValue[at]=min(lowLinkValue[at],lowLinkValue[v]);
            }
        }
        if(ids[at]==lowLinkValue[at])        // If id and lowLinkValue of a node is equal then
        {                                    // it is a starting node for a SCC and pop all nodes 
            while(!seenStack.empty() && seenStack.top()!=at)  // in stack above it and the node
            {                                                 // itself 
                onStack[seenStack.top()]=false;
                seenStack.pop();
            }
            onStack[seenStack.top()]=false;
            seenStack.pop();
            sccCount++;
        }
    }
    int getSccCount()                     // Return the SccCount
    {
        return sccCount;
    }
    void printSCCs()                     // Nodes with same lowLinkValue are part of same SCC
    {
        int modified=0;
        for(int i=0;i<numVertices;i++)
        {
            modified=0;
            for(int j=0;j<numVertices;j++)
            {
                if(lowLinkValue[j]==i)
                {
                    modified=1;
                    cout<<j<<" ";
                }
            }
            if(modified==1)
            {
                cout<<"\n";
            }
        }
    }
};

int main()
{
    Graph g(8);
    g.addEdge(3,7);
    g.addEdge(7,3);
    g.addEdge(3,4);
    g.addEdge(7,5);
    g.addEdge(4,5);
    g.addEdge(5,0);
    g.addEdge(5,6);
    g.addEdge(6,4);
    g.addEdge(6,0);
    g.addEdge(6,2);
    g.addEdge(0,1);
    g.addEdge(1,2);
    g.addEdge(2,0);
    g.findSCCs();
    cout<<g.getSccCount()<<"\n";
    g.printSCCs();
    return 0;
}

// Output : 3
//          0 1 2 
//          3 7 
//          4 5 6 