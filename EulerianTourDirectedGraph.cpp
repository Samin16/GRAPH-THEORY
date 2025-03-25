// Eulerian Tour of A Directed Graph
// Author : Samin Yasar

#include<bits/stdc++.h>
using namespace std;

class Graph
{
    int numVertices;      // Number of vertices in the graph
    int numEdge;          // To track number of edges in the graph
    vector<vector<int>> adjList;       // Adjacency list representation
    vector<int> inDegree;         // In Degree of each vertices
    vector<int> outDegree;        // Out Degree of each vertices
    stack<int> eulerianPath;      // Eulerian path from top of the stack
public:
    Graph(int numVertices)        // Initialization of the Graph
    {
        this->numVertices=numVertices;
        numEdge=0;
        adjList.resize(numVertices);
        inDegree.resize(numVertices,0);
        outDegree.resize(numVertices,0);
    }
    void addEdge(int u,int v)      // Add graph edges and increase the edgeCounter by 1 for each edge
    {
        adjList[u].push_back(v);
        numEdge++;
    }
    bool findEulerianPath()   // Function to find eulerian tour and returns true if exists otherwise false
    {
        countInAndOutDegree();     // Count in and out degree
        if(!graphHasEulerianPath())    // If there is no Eulerian path return false
        {
            return false;
        }
        dfs(findStartNode());       // Dfs from the starting node
        if(eulerianPath.size()==numEdge+1)     // Because complete path with all edges contains one
        {                                      // more vertices than path as 2 vertices contribute 
            return true;                       // to 1 edge
        }
        return false;                          // If a Eulerian path does not exist
    }
    void countInAndOutDegree()            // Counts in and out degree
    {
        for(int i=0;i<numVertices;i++)
        {
            outDegree[i]+=adjList[i].size();   // Out Degree calculation
            for(int v:adjList[i])
            {
                inDegree[v]++;           // In Degree calculation
            }
        }
    }
    bool graphHasEulerianPath()
    {
        int startNodes=0,endNodes=0;     // Count for start and end nodes
        for(int i=0;i<numVertices;i++)
        {
            if((outDegree[i]-inDegree[i]>1) || (inDegree[i]-outDegree[i]>1))
            {                   
                return false;  // If any nodes in Degree and out Degree is not equal or difference is greater than 1 then no path exists
            }
            else if(outDegree[i]-inDegree[i]==1)   // Starting node case
            {
                startNodes++;     
            }
            else if(inDegree[i]-outDegree[i]==1)   // End node case
            {
                endNodes++;
            }
        }
        // If no specific end and start node or only one set of start and end node then eulerian path exists
        return (endNodes==0 && startNodes==0) || (endNodes==1 && startNodes==1);
    }
    int findStartNode()
    {
        int start;
        for(int i=0;i<numVertices;i++)
        {
            if(outDegree[i]-inDegree[i]==1)  // If any specific starting node contains
            {
                return i;
            }
            if(outDegree[i]>0)   // If no specific starting node any node can be chosen 
            {                    // as starting node if Eulerian Path exists
                start=i;
            }
        }
        return start;
    }
    void dfs(int at)          // Do dfs from the starting node
    {
        while(outDegree[at]!=0)     // While not already have visited all the out edges
        {
            int nextNode=adjList[at][--outDegree[at]];   // Take the node from the adjacency list and decrease outDegree for the ancestor
            dfs(nextNode);                            // Recursively call dfs    
        }
        eulerianPath.push(at);                        // Backtrack by pushing the node which's all out edges have been visited on top of the stack
    }
    void printPath()      // Prints the Eulerian Path
    {
        if(!findEulerianPath())         // If not exists a path
        {
            cout<<"No Path Exists\n";
            return;
        }
        int n=eulerianPath.size();         // If get a path
        cout<<"The Eulerian Path : \n";
        for(int i=0;i<n-1;i++)
        {
            cout<<eulerianPath.top()<<" - ";
            eulerianPath.pop();
        }
        cout<<eulerianPath.top()<<"\n";
        eulerianPath.pop();
    }
};

int main()
{
    // Graph 1
    Graph g(7); 
    vector<pair<int,int>> edges={{1,2},{1,3},{2,2},{2,4},{2,4},{3,2},{3,1},{3,5},{4,6},{4,3},{5,6},{6,3}};
    // Graph 2
    // Graph g(4);
    // vector<pair<int,int>> edges={{0,1},{0,2},{2,3},{3,1}};
    for(auto edge:edges)
    {
        g.addEdge(edge.first,edge.second);
    }
    g.printPath();
    return 0;
}

// Graph 1 : 
// Output : The Eulerian Path : 
//          1 - 3 - 5 - 6 - 3 - 1 - 2 - 4 - 3 - 2 - 2 - 4 - 6

// Graph 2 :
// Output : No Path Exists