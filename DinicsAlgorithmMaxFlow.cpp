// Dinic's Algorithm for Maximum Flow
// Author : Samin Yasar

#include<bits/stdc++.h>
using namespace std;

class NetworkFlowSolverDinicsAlgorithm        // Contains the implementation of Dinic's Algorithm
{
protected:
    struct Edge                 // Structure to represent an edge in the graph
    {
        int from,to,rev;        // from and to are the vertices of the edge, rev is the index of the reverse edge in the adjacency list
        long long flow,capacity;// flow is the current flow through the edge, capacity is the maximum capacity of the edge
        bool isResidual;        // isResidual is true if the edge is a residual edge (backward edge in the flow network)
        string toString(int s, int t) const    // Way to output an edge
        {
            string u = (from == s) ? "s" : ((from == t) ? "t" : to_string(from));
            string v = (to == s) ? "s" : ((to == t) ? "t" : to_string(to));
            return "Edge " + u + " -> " + v + " | flow = " + to_string(flow) +
                   " | capacity = " + to_string(capacity) + " | is residual : " +
                   (isResidual ? "true" : "false");
        }
    };
private:
    int n,source,sink;      // n is the number of vertices, source is the source vertex and sink is the sink vertex
    long long maxFlow;      // maxFlow is the maximum flow int he network
    bool isSolved;          // isSolved is true if the maximum flow has been solved
    vector<vector<Edge>> adjList;  // Adjacency list to representation the graph
    vector<int> level,ptr;   // level is the level of each vertex in the BFS tree, ptr is the pointer to the next edge to be explored in the DFS
    bool bfs()         // BFS to find the level of each vertex in the BFS tree
    {
        fill(level.begin(),level.end(),-1);  // Mark all node at no level
        queue<int> q;                        // Queue to do level-wise bfs
        q.push(source);                      // Start from the source(source is root of bfs tree)
        level[source]=0;                     // Level of source is 0
        while(!q.empty())                    // Do bfs until we have no node left in queue
        {
            int u=q.front();                 // Element in current level
            q.pop();
            for(Edge& e:adjList[u])
            {
                if(level[e.to]==-1 && e.flow<e.capacity)
                {
                    level[e.to]=1+level[u];       // Form each connected edges with the node select those with remaining
                    q.push(e.to);                 // capacity and push them onto the queue
                }
            }
        }
        return level[sink]!=-1;            // If there is path to sink in bfs sub graph then return true otherwise false
    }
    long long dfs(int node,long long flow)  // DFS to find the augmenting path in the residual graph
    {
        if(node==sink)                // If we reach the sink we have found a path in the BFS sub-graph
        {
            return flow;
        }
        for(;ptr[node]!=adjList[node].size();ptr[node]++)   // For each edge in the adjacency list of the node
        {
            Edge& e=adjList[node][ptr[node]];
            if(level[e.to]==1+level[node] && e.flow<e.capacity)  // if the edge.to is the next node in the BFS sub-graph and the edge has remaining capacity
            {
                long long bottleneck=dfs(e.to,min(flow,e.capacity-e.flow));  // Call dfs on the next node in the BFS sub-graph with the minimum of the flow and the remaining capacity of the edge
                if(bottleneck>0)           // Augment the path with the bottleneck value
                {
                    e.flow+=bottleneck;
                    adjList[e.to][e.rev].flow-=bottleneck;
                    return bottleneck;
                }
            }
        }
        level[node]=-1;       // Prune the node if no path is found in the dfs sub graph as Level[node] becomes useless if no path was found
        return 0;
    }
public:
    NetworkFlowSolverDinicsAlgorithm(int n,int s,int t):n(n),source(s),sink(t)   // Constructor to initialize the graph
    {
        adjList.resize(n);
        level.resize(n);
        ptr.resize(n);
        maxFlow=0;
        isSolved=false;
    }
    void addEdge(int u,int v,long long capacity)       // Add edge and its residual edge to the adjacency list
    {
        Edge forward={u,v,(int)adjList[v].size(),0,capacity,false};
        Edge backward={v,u,(int)adjList.size(),0,0,true};
        adjList[u].push_back(forward);
        adjList[v].push_back(backward);
    }
    void solve()       // Solves the max flow problem
    {
        if(isSolved)    // If already solved then return
        {
            return;
        }
        while(bfs())         // While there is a path to the sink in the BFS sub-graph
        {
            fill(ptr.begin(),ptr.end(),0);  // Reset the pointer to the next edge to be explored in the DFS
            while(long long bottleneck=dfs(source,LLONG_MAX))   // Perform dfs on the source node with the max possible flow till we find a path that reaches sink 
            {
                maxFlow+=bottleneck;
            }
        }
    }
    long long getMaxFlow()      // Returns the maximum flow in the network
    {
        solve();
        return maxFlow;
    }
    vector<vector<Edge>>& getGraph()    // Returns the Graph after solving max flow problem
    {
        solve();
        return adjList;
    }
};

class TestDinicsAlgorithm:public NetworkFlowSolverDinicsAlgorithm
{
public:
    TestDinicsAlgorithm(int n,int s,int t):NetworkFlowSolverDinicsAlgorithm(n,s,t){}
    static void exampleGraph1()      // Example 1 :
    {
        int n=6;
        int s=n-1;
        int t=n-2;
        TestDinicsAlgorithm solver1(n,s,t);
        solver1.addEdge(s,0,10);
        solver1.addEdge(s,1,10);
        solver1.addEdge(2,t,10);
        solver1.addEdge(3,t,10);
        solver1.addEdge(0, 1, 2);
        solver1.addEdge(0, 2, 4);
        solver1.addEdge(0, 3, 8);
        solver1.addEdge(1, 3, 9);
        solver1.addEdge(3, 2, 6);
        cout<<"Max flow for Graph 1 : "<<solver1.getMaxFlow()<<"\n";
        auto g=solver1.getGraph();
        for(auto& edges:g)
        {
            for(Edge& e:edges)
            {
                if(e.to==s || e.from==t)
                {
                    continue;
                }
                if(e.from==s || e.to==t || e.from<e.to)
                {
                    cout<<e.toString(s,t)<<"\n";
                }
            }
        }
        cout<<"\n";
    }
    static void exampleGraph2()      // Example 2 :
    {
        int n = 12;
        int s = n - 2;
        int t = n - 1;

        TestDinicsAlgorithm solver2(n, s, t);

        solver2.addEdge(s, 1, 2);
        solver2.addEdge(s, 2, 1);
        solver2.addEdge(s, 0, 7);
        solver2.addEdge(0, 3, 2);
        solver2.addEdge(0, 4, 4);
        solver2.addEdge(1, 4, 5);
        solver2.addEdge(1, 5, 6);
        solver2.addEdge(2, 3, 4);
        solver2.addEdge(2, 7, 8);
        solver2.addEdge(3, 6, 7);
        solver2.addEdge(3, 7, 1);
        solver2.addEdge(4, 5, 8);
        solver2.addEdge(4, 8, 3);
        solver2.addEdge(5, 8, 3);
        solver2.addEdge(6, t, 1);
        solver2.addEdge(7, t, 3);
        solver2.addEdge(8, t, 4);

        cout << "Max flow for Graph 2 : " << solver2.getMaxFlow() << endl;

        auto g=solver2.getGraph();
        for(auto& edges:g)
        {
            for(Edge& e:edges)
            {
                if(e.to==s || e.from==t)
                {
                    continue;
                }
                if(e.from==s || e.to==t || e.from<e.to)
                {
                    cout<<e.toString(s,t)<<"\n";
                }
            }
        }
        cout<<"\n";
    }

    static void exampleGraph3()      // Example 3 :
    {
        int n = 6;
        int s = n - 2;
        int t = n - 1;

        TestDinicsAlgorithm solver3(n, s, t);

        solver3.addEdge(s, 1, 10);
        solver3.addEdge(1, 3, 15);
        solver3.addEdge(3, 0, 6);
        solver3.addEdge(0, 2, 25);
        solver3.addEdge(2, t, 10);

        solver3.addEdge(s, 0, 10);
        solver3.addEdge(3, t, 10);

        cout <<  "Max flow for Graph 3 : " << solver3.getMaxFlow() << "\n";

        auto g=solver3.getGraph();
        for(auto& edges:g)
        {
            for(Edge& e:edges)
            {
                if(e.to==s || e.from==t)
                {
                    continue;
                }
                if(e.from==s || e.to==t || e.from<e.to)
                {
                    cout<<e.toString(s,t)<<"\n";
                }
            }
        }
        cout<<"\n";
    }
    static void exampleGraph4()      // Example 4 :
    {
        int n = 4;
        int s = n - 2;
        int t = n - 1;

        TestDinicsAlgorithm solver4(n, s, t);

        solver4.addEdge(s, 0, 10);
        solver4.addEdge(s, 1, 10);
        solver4.addEdge(0, 1, 5);
        solver4.addEdge(0, t, 5);
        solver4.addEdge(1, t, 10);

        cout <<  "Max flow for Graph 4 : " << solver4.getMaxFlow() << "\n";

        auto g=solver4.getGraph();
        for(auto& edges:g)
        {
            for(Edge& e:edges)
            {
                if(e.to==s || e.from==t)
                {
                    continue;
                }
                if(e.from==s || e.to==t || e.from<e.to)
                {
                    cout<<e.toString(s,t)<<"\n";
                }
            }
        }
        cout<<"\n";
    }
};

int main()
{
    TestDinicsAlgorithm::exampleGraph1();
    TestDinicsAlgorithm::exampleGraph2();
    TestDinicsAlgorithm::exampleGraph3();
    TestDinicsAlgorithm::exampleGraph4();
    return 0;
}

// Output : Max flow for Graph 1 : 19
//          Edge 0 -> 1 | flow = 0 | capacity = 2 | is residual : false
//          Edge 0 -> 2 | flow = 4 | capacity = 4 | is residual : false
//          Edge 0 -> 3 | flow = 6 | capacity = 8 | is residual : false
//          Edge 1 -> 3 | flow = 9 | capacity = 9 | is residual : false
//          Edge 2 -> t | flow = 9 | capacity = 10 | is residual : false
//          Edge 2 -> 3 | flow = -5 | capacity = 0 | is residual : true
//          Edge 3 -> t | flow = 10 | capacity = 10 | is residual : false
//          Edge s -> 0 | flow = 10 | capacity = 10 | is residual : false
//          Edge s -> 1 | flow = 9 | capacity = 10 | is residual : false

//          Max flow for Graph 2 : 7
//          Edge 0 -> 3 | flow = 2 | capacity = 2 | is residual : false
//          Edge 0 -> 4 | flow = 2 | capacity = 4 | is residual : false
//          Edge 1 -> 4 | flow = 2 | capacity = 5 | is residual : false
//          Edge 1 -> 5 | flow = 0 | capacity = 6 | is residual : false
//          Edge 2 -> 3 | flow = 0 | capacity = 4 | is residual : false
//          Edge 2 -> 7 | flow = 1 | capacity = 8 | is residual : false
//          Edge 3 -> 6 | flow = 1 | capacity = 7 | is residual : false
//          Edge 3 -> 7 | flow = 1 | capacity = 1 | is residual : false
//          Edge 4 -> 5 | flow = 1 | capacity = 8 | is residual : false
//          Edge 4 -> 8 | flow = 3 | capacity = 3 | is residual : false
//          Edge 5 -> 8 | flow = 1 | capacity = 3 | is residual : false
//          Edge 6 -> t | flow = 1 | capacity = 1 | is residual : false
//          Edge 7 -> t | flow = 2 | capacity = 3 | is residual : false
//          Edge 8 -> t | flow = 4 | capacity = 4 | is residual : false
//          Edge s -> 1 | flow = 2 | capacity = 2 | is residual : false
//          Edge s -> 2 | flow = 1 | capacity = 1 | is residual : false
//          Edge s -> 0 | flow = 4 | capacity = 7 | is residual : false

//          Max flow for Graph 3 : 20
//          Edge 0 -> 3 | flow = 0 | capacity = 0 | is residual : true
//          Edge 0 -> 2 | flow = 10 | capacity = 25 | is residual : false
//          Edge 1 -> 3 | flow = 10 | capacity = 15 | is residual : false
//          Edge 2 -> t | flow = 10 | capacity = 10 | is residual : false
//          Edge 3 -> t | flow = 10 | capacity = 10 | is residual : false
//          Edge s -> 1 | flow = 10 | capacity = 10 | is residual : false
//          Edge s -> 0 | flow = 10 | capacity = 10 | is residual : false

//          Max flow for Graph 4 : 15
//          Edge 0 -> 1 | flow = 0 | capacity = 5 | is residual : false
//          Edge 0 -> t | flow = 5 | capacity = 5 | is residual : false
//          Edge 1 -> t | flow = 10 | capacity = 10 | is residual : false
//          Edge s -> 0 | flow = 5 | capacity = 10 | is residual : false
//          Edge s -> 1 | flow = 10 | capacity = 10 | is residual : false