// Eulerian Tour for an undirected Graph
// Author : Samin Yasar

#include<bits/stdc++.h>
using namespace std;

class Graph              // Graph representation and contains necessary functions
{
    int numVertices;     // Number of vertices
    int numEdges;        // Number of edges
    vector<list<int>> adjList;       // Adjacency list representation of Graph
    vector<int> degree;              // To track degree for each vertex
    stack<int> eulerianPath;         // For storing the Eulerian Path
public:
    Graph(int numVertices)           // Initialization
    {
        this->numVertices=numVertices;
        adjList.resize(numVertices);
        degree.resize(numVertices,0);
        numEdges=0;
    }
    void addEdge(int u,int v)      // Add undirected edges and as one edge contributes as two edges (bidirectional)
    {                              // So, count a bidirectional edge as an edge and increase edge count
        adjList[u].push_back(v);
        adjList[v].push_back(u);
        numEdges++;
    }
    bool findEulerianPath()     // If finds an Eulerian path returns true otherwise false
    {
        countDegree();          // Count degree for each node
        if(!hasEulerianPath())  // If noot has a path return false 
        {
            return false;
        }
        dfs(findStartNode());               // Modified DFS to find the Eulerian Path
        if(eulerianPath.size()==numEdges+1) // Because complete path with all edges contains one
        {                                   // more vertices than path as 2 vertices contribute 
            return true;                    // to 1 edge
        }
        return false;
    }
    void countDegree()
    {
        for(int i=0;i<numVertices;i++)     // Degree : How many edges are connected to a vertex !
        {
            degree[i]+=adjList[i].size();
        }
    }
    bool hasEulerianPath()       // If the graph has an eulerian path or not
    {
        int startNodes=0;        // To count the nodes with odd degrees
        for(int i=0;i<numVertices;i++)
        {
            if(degree[i]==0)      // If the undirected graph is disconnected then it has no Eulerian Path
            {
                return false;
            }
            if(degree[i]%2!=0)    // Nodes with odd degrees
            {
                startNodes++;
            }
        }
        if(startNodes==2 || startNodes==0)   // If nodes with odd degrees is exactly 0 or 2 then
        {                                    // Euulerin path possible
            return true;
        }
        return false;                        // Otherwise no Eulerian Path
    }
    int findStartNode()                      // Finds the start node
    {
        int startNode=0;
        for(int i=0;i<numVertices;i++)
        {
            if(degree[i]%2!=0)          // If has a node with odd degree then it is starting/end node
            {
                return i;
            }
            else
            {
                startNode=i;       // If graph has no specific starting/end node
            }
        }
        return startNode;
    }
    void dfs(int startNode)             // Stack based Modified DFS
    {
        stack<int> st;                  // Recursion stack
        st.push(startNode);             // Start from the starting node
        while(!st.empty())
        {
            int at=st.top();
            if(degree[at]==0)          // If visited all edges of a node then pop it from stack and add it to eulerian path
            {
                eulerianPath.push(at);
                st.pop();
            }
            else                      // Otherwise go to the next node and delete the edge from adjacency list
            {                         // and also delete the back edge from the list of next node (as one edge
                int nextNode=adjList[at].back();   // contributes as a bidirectional edge)
                adjList[at].pop_back();
                degree[at]--;
                degree[nextNode]--;
                for(auto it=adjList[nextNode].begin();it!=adjList[nextNode].end();it++)
                {
                    if(*it==at)
                    {
                        adjList[nextNode].erase(it);
                        break;
                    }
                }
                st.push(nextNode);     // Add the next node on the recursion stack
            }
        }
    }
    void printEulerianPath()       // Prints the Eulerian path if exists otherwise prints message
    {
        if(!findEulerianPath())
        {
            cout<<"No path exists\n";;
            return;
        }
        int n=eulerianPath.size();
        cout<<"Eulerian Path : ";
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
    // Graph 1 :
    Graph g(5);
    vector<pair<int,int>> edgeList={{0,1},{1,2},{2,3},{2,4},{3,4}};

    // Graph 2 :
    // Graph g(5);
    // vector<pair<int,int>> edgeList={{0,1},{0,2},{1,2},{1,3},{1,4},{2,3},{2,4},{4,3}};

    // Graph 3 :
    // Graph g(6);
    // vector<pair<int,int>> edgeList={{0,1},{0,3},{1,3},{1,5},{1,2},{2,5},{3,4},{3,4},{4,5},{4,5}};

    // Graph 4 : 
    // Graph g(5);
    // vector<pair<int,int>> edgeList={{0,1},{0,2},{0,3},{1,4},{3,4},{2,4},{2,3}};

    for(auto edge:edgeList)
    {
        g.addEdge(edge.first,edge.second);
    }
    g.printEulerianPath();
    return 0;
}


// Graph 1
// Output : Eulerian Path : 0 - 1 - 2 - 4 - 3 - 2

// Graph 2
// Output : Eulerian Path : 3 - 4 - 2 - 3 - 1 - 2 - 0 - 1 - 4

// Graph 3 
// Output : Eulerian Path : 5 - 4 - 5 - 2 - 1 - 3 - 4 - 3 - 0 - 1 - 5

// Graph 4 
// Output : No path exists