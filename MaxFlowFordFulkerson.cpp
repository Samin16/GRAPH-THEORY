#include <bits/stdc++.h>
using namespace std;

class NetWorkFlowSolver         // A general solver class for Network Flow Algorithms
{
protected:
    static const long long INF = LLONG_MAX / 2;       // Max flow coming to the source node

public:
    struct Edge                        // Edge of the residual graph
    {
        int from, to;                  // there ia an edge from->to
        long long flow;                // Flow through this edge
        Edge *residual;                // Residual edge to->from with capacity 0
        const long long capacity;      // Capacity of the edge
        Edge(int from, int to, long long capacity)        // Initialization of an edge with initial flow 0 and no residual edge
            : from(from), to(to), capacity(capacity), flow(0), residual(NULL) {}
        bool isResidual() const       // Residual edge's capacity equals to zero
        {
            return capacity == 0;
        }
        long long remainingCapacity() const    // Returns the remaining capacity by subtracting the current flow
        {
            return capacity - flow;
        }
        void augment(long long bottleneck)     // Augments an edge by adding bottleneck value to the flow 
        {                                      // and reducing the residual edge's flow by bottleneck value
            flow += bottleneck;
            residual->flow -= bottleneck;
        }
        string toString(int s, int t) const    // Way to ouutput an edge
        {
            string u = (from == s) ? "s" : ((from == t) ? "t" : to_string(from));
            string v = (to == s) ? "s" : ((to == t) ? "t" : to_string(to));
            return "Edge " + u + " -> " + v + " | flow = " + to_string(flow) +
                   " | capacity = " + to_string(capacity) + " | is residual : " +
                   (isResidual() ? "true" : "false");
        }
    };

protected:
    const int n, s, t;                  // Number of nodes in the graph n, source s, sink t
    long long maxFlow;                  // Maximum flow that can go through the flow graph
    vector<bool> minCut;                // Represents the set of vertices that are reachable from source e even after the max flow algorithm 
    vector<vector<Edge *>> graph;       // Adjacency List representation of the graph
    int visitedToken;                   // A node having this visit value is visited already            
    vector<int> visited;                // If a node is visited or not
    bool solved;                        // Whether the max flow algorithm has worked once
    NetWorkFlowSolver(int n, int s, int t) : n(n), s(s), t(t), maxFlow(0)     // Initializing the graph and other properties
    {
        if (s < 0 || s >= n || t < 0 || t >= n || s == t)
        {
            throw invalid_argument("Invalid source or sink");
        }
        initializeGraph();
        visitedToken=1;
        solved=false;
        minCut.resize(n, false);
        visited.resize(n, 0);
    }
    virtual ~NetWorkFlowSolver()      // Destructor to fill up all the spaces
    {
        for (auto &edges : graph)
        {
            for (Edge *edge : edges)
            {
                delete edge;
            }
        }
    }

private:
    void initializeGraph()       // Initializing an n vertex graph with no Edges
    {
        graph.resize(n);
    }

public:
    void addEdge(int from, int to, long long capacity)    // Adding an edge and its residual edge to the graph
    {
        if (capacity < 0)
        {
            throw invalid_argument("Invalid capacity (capacity<0)");
        }
        Edge *e1 = new Edge(from, to, capacity);    // Edge
        Edge *e2 = new Edge(to, from, 0);           // Residual edge
        e1->residual = e2;
        e2->residual = e1;
        graph[from].push_back(e1);
        graph[to].push_back(e2);
    }
    void visit(int i)                     // Set vertex i as visited
    {
        visited[i] = visitedToken;
    }
    bool isVisited(int i) const            // Returns whether vertex i is visited or not
    {
        return visited[i] == visitedToken;
    }
    void markAllNodesUnvisited()          // Marks all nodes unvisited by changing the visitedToken value
    {
        visitedToken++;
    }
    const vector<vector<Edge *>> &getGraph()    // After Max Flow Algorithm returns the graph
    {
        execute();
        return graph;
    }
    long long getMaxFlow()        // Returns Max Flow after Max Flow Algorithm
    {
        execute();
        return maxFlow;
    }
    const vector<bool> &getMinCut()    // Returns the minCut array after Max Flow Algorithm
    {
        execute();
        return minCut;
    }

private:
    void execute()        // Checks whether Max Flow Algorithm is runned once if not runs it
    {                     // otherwise, simply returns
        if (solved)
        {
            return;
        }
        solved = true;
        solve();
    }

protected:
    virtual void solve() = 0;       // Abstract method solve() to solve max flow algorithm defined in child class
};

class FordFulkersonMaxFlowDFSAdjacencyList : public NetWorkFlowSolver      // Class to have the methods related to Ford Fulkerson Max Flow Algorithm
{
public:
    FordFulkersonMaxFlowDFSAdjacencyList(int n, int s, int t) : NetWorkFlowSolver(n, s, t) {}   // Initialization
    void solve() override                             // Overriding the solve method 
    {
        for (long long f = dfs(s, INF); f != 0; f = dfs(s, INF))   // Runs the algorithm until the bottleneck value
        {                                                          // not equals to zero that means there is lack of capacity
            markAllNodesUnvisited();                               // if we push more flow to the source
            maxFlow += f;                                 // adding the bottleneck value for each augmenting path to get max flow
        }
        for (int i = 0; i < n; i++)
        {
            if (isVisited(i))                 // 'true' means there is still some vertices that can be reached
            {                                 //  from source even after getting the max flow but cannot form an
                minCut[i] = true;             //  augmenting path.
            }
        }
    }

private:
    long long dfs(int node, long long flow)      // DFS to find the bottleneck value of each augmenting path
    {
        if (node == t)                           // If we reached the sink returns the flow (Base case)
        {
            return flow;
        }
        auto &edges = graph[node];              
        visit(node);
        for (Edge *edge : edges)                 // For each out edges from the node get remaining capacity
        {                                        // and if we have remaining capacity and the to node of the edge 
            long long remainedCapacity = edge->remainingCapacity();   // is not yet in the augmenting path then 
            if (remainedCapacity > 0 && !isVisited(edge->to))       // recursively call dfs to get the bottleneck value
            {
                long long bottleneck = dfs(edge->to, min(flow, remainedCapacity));
                if (bottleneck > 0)             // If we get a valid bottleneck augment each edge
                {
                    edge->augment(bottleneck);
                    return bottleneck;
                }
            }
        }
        return 0;
    }

public:
    static void exampleGraph1()       // Example 1 :
    {
        int n = 6;
        int s = n - 1;
        int t = n - 2;
        FordFulkersonMaxFlowDFSAdjacencyList solver1(n, s, t);
        solver1.addEdge(s, 0, 10);
        solver1.addEdge(s, 1, 10);
        solver1.addEdge(2, t, 10);
        solver1.addEdge(3, t, 10);
        solver1.addEdge(0, 1, 2);
        solver1.addEdge(0, 2, 4);
        solver1.addEdge(0, 3, 8);
        solver1.addEdge(1, 3, 9);
        solver1.addEdge(3, 2, 6);
        cout << "Max flow for Graph 1 : " << solver1.getMaxFlow() << "\n";
        auto g = solver1.getGraph();
        for (auto &edges : g)
        {
            for (Edge *e : edges)
            {
                if (e->to == s || e->from == t)
                {
                    continue;
                }
                if (e->from == s || e->to == t || e->from < e->to)
                {
                    cout << e->toString(s, t) << "\n";
                }
            }
        }
        cout << "\n";
    }
    static void exampleGraph2()      // Example 2 :
    {
        int n = 12;
        int s = n - 2;
        int t = n - 1;

        FordFulkersonMaxFlowDFSAdjacencyList solver2(n, s, t);

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

        auto g = solver2.getGraph();
        for (auto &edges : g)
        {
            for (Edge *e : edges)
            {
                if (e->to == s || e->from == t)
                    continue;
                if (e->from == s || e->to == t || e->from < e->to)
                    cout << e->toString(s, t) << endl;
            }
        }
        cout<<"\n";
    }

    static void exampleGraph3()      // Example 3 :
    {
        int n = 6;
        int s = n - 2;
        int t = n - 1;

        FordFulkersonMaxFlowDFSAdjacencyList solver3(n, s, t);

        solver3.addEdge(s, 1, 10);
        solver3.addEdge(1, 3, 15);
        solver3.addEdge(3, 0, 6);
        solver3.addEdge(0, 2, 25);
        solver3.addEdge(2, t, 10);

        solver3.addEdge(s, 0, 10);
        solver3.addEdge(3, t, 10);

        cout <<  "Max flow for Graph 3 : " << solver3.getMaxFlow() << "\n";

        auto g = solver3.getGraph();
        for (auto &edges : g)
        {
            for (Edge *e : edges)
            {
                cout << e->toString(s, t) << endl;
            }
        }
        cout<<"\n";
    }
    static void exampleGraph4()      // Example 4 :
    {
        int n = 4;
        int s = n - 2;
        int t = n - 1;

        FordFulkersonMaxFlowDFSAdjacencyList solver4(n, s, t);

        solver4.addEdge(s, 0, 10);
        solver4.addEdge(s, 1, 10);
        solver4.addEdge(0, 1, 5);
        solver4.addEdge(0, t, 5);
        solver4.addEdge(1, t, 10);

        cout <<  "Max flow for Graph 4 : " << solver4.getMaxFlow() << "\n";

        auto g = solver4.getGraph();
        for (auto &edges : g)
        {
            for (Edge *e : edges)
            {
                cout << e->toString(s, t) << endl;
            }
        }
        cout<<"\n";
    }
};

int main()
{
    FordFulkersonMaxFlowDFSAdjacencyList::exampleGraph1();
    FordFulkersonMaxFlowDFSAdjacencyList::exampleGraph2();
    FordFulkersonMaxFlowDFSAdjacencyList::exampleGraph3();
    FordFulkersonMaxFlowDFSAdjacencyList::exampleGraph4();
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
//          Edge 1 -> 4 | flow = 1 | capacity = 5 | is residual : false
//          Edge 1 -> 5 | flow = 1 | capacity = 6 | is residual : false
//          Edge 2 -> 3 | flow = 0 | capacity = 4 | is residual : false
//          Edge 2 -> 7 | flow = 1 | capacity = 8 | is residual : false
//          Edge 3 -> 6 | flow = 1 | capacity = 7 | is residual : false
//          Edge 3 -> 7 | flow = 1 | capacity = 1 | is residual : false
//          Edge 4 -> 5 | flow = 2 | capacity = 8 | is residual : false
//          Edge 4 -> 8 | flow = 1 | capacity = 3 | is residual : false
//          Edge 5 -> 8 | flow = 3 | capacity = 3 | is residual : false
//          Edge 6 -> t | flow = 1 | capacity = 1 | is residual : false
//          Edge 7 -> t | flow = 2 | capacity = 3 | is residual : false
//          Edge 8 -> t | flow = 4 | capacity = 4 | is residual : false
//          Edge s -> 1 | flow = 2 | capacity = 2 | is residual : false
//          Edge s -> 2 | flow = 1 | capacity = 1 | is residual : false
//          Edge s -> 0 | flow = 4 | capacity = 7 | is residual : false

//          Max flow for Graph 3 : 20
//          Edge 0 -> 3 | flow = 0 | capacity = 0 | is residual : true
//          Edge 0 -> 2 | flow = 10 | capacity = 25 | is residual : false
//          Edge 0 -> s | flow = -10 | capacity = 0 | is residual : true
//          Edge 1 -> s | flow = -10 | capacity = 0 | is residual : true
//          Edge 1 -> 3 | flow = 10 | capacity = 15 | is residual : false
//          Edge 2 -> 0 | flow = -10 | capacity = 0 | is residual : true
//          Edge 2 -> t | flow = 10 | capacity = 10 | is residual : false
//          Edge 3 -> 1 | flow = -10 | capacity = 0 | is residual : true
//          Edge 3 -> 0 | flow = 0 | capacity = 6 | is residual : false
//          Edge 3 -> t | flow = 10 | capacity = 10 | is residual : false
//          Edge s -> 1 | flow = 10 | capacity = 10 | is residual : false
//          Edge s -> 0 | flow = 10 | capacity = 10 | is residual : false
//          Edge t -> 2 | flow = -10 | capacity = 0 | is residual : true
//          Edge t -> 3 | flow = -10 | capacity = 0 | is residual : true

//          Max flow for Graph 4 : 15
//          Edge 0 -> s | flow = -10 | capacity = 0 | is residual : true
//          Edge 0 -> 1 | flow = 5 | capacity = 5 | is residual : false
//          Edge 0 -> t | flow = 5 | capacity = 5 | is residual : false
//          Edge 1 -> s | flow = -5 | capacity = 0 | is residual : true
//          Edge 1 -> 0 | flow = -5 | capacity = 0 | is residual : true
//          Edge 1 -> t | flow = 10 | capacity = 10 | is residual : false
//          Edge s -> 0 | flow = 10 | capacity = 10 | is residual : false
//          Edge s -> 1 | flow = 5 | capacity = 10 | is residual : false
//          Edge t -> 0 | flow = -5 | capacity = 0 | is residual : true
//          Edge t -> 1 | flow = -10 | capacity = 0 | is residual : true