// Max flow using Edmonds-Karp Algorithm
// Author : Samin Yasar

#include <bits/stdc++.h>
using namespace std;

class NetworkFlowSolver           // A general solver class for Network Flow Algorithms
{
protected:
    static const long long INF = LLONG_MAX / 2;    // Max flow coming to the source node

public:
    struct Edge                     // Edge of the residual graph
    {
        int from, to;               // there ia an edge from->to
        long long flow;             // Flow through this edge
        long long capacity;         // Capacity of the edge
        Edge *residual;             // Residual edge to->from with capacity 0
        Edge(int from, int to, long long capacity)        // Initialization of an edge with initial flow 0 and no residual edge
            : from(from), to(to), capacity(capacity), flow(0), residual(NULL) {}
        bool isResidual() const         // Residual edge's capacity equals to zero
        {
            return capacity == 0;
        }
        long long remainingCapacity() const    // Returns the remaining capacity by subtracting the current flow
        {
            return capacity - flow;
        }
        void augment(long long bottleneck)    // Augments an edge by adding bottleneck value to the flow 
        {                                     // and reducing the residual edge's flow by bottleneck value
            flow += bottleneck;
            residual->flow -= bottleneck;
        }
        string toString(int s, int t) const    // Way to output an edge
        {
            string u = (from == s) ? "s" : ((from == t) ? "t" : to_string(from));
            string v = (to == s) ? "s" : ((to == t) ? "t" : to_string(to));
            return "Edge " + u + " -> " + v + " | flow = " + to_string(flow) +
                   " | capacity = " + to_string(capacity) + " | is residual : " +
                   (isResidual() ? "true" : "false");
        }
    };

protected:
    const int n, s, t;               // Number of nodes in the graph n, source s, sink t
    long long maxFlow;               // Maximum flow that can go through the flow graph
    vector<vector<Edge *>> adjList;  // Adjacency List representation of the graph
    bool isSolved;                   // Whether the max flow algorithm has worked once
    vector<int> ptr;                 // Pointer to the current edge in the adjacency list for each node
    vector<int> level;               // Level of each node in the BFS

public:
    NetworkFlowSolver(int n, int s, int t) : n(n), s(s), t(t), maxFlow(0)    // Initialization of the graph and other properties
    {
        if (s < 0 || s >= n || t < 0 || t >= n || s == t)
        {
            throw invalid_argument("Invalid source or sink");
        }
        adjList.resize(n);
        ptr.resize(n);
        level.resize(n);
        isSolved = false;
    }
    void addEdge(int from, int to, long long capacity)       // Adding an edge and its residual edge to the graph
    {
        if (capacity < 0)
        {
            throw invalid_argument("Invalid capaccity (capacity<0)");
        }
        Edge *e1 = new Edge(from, to, capacity);      // Edge
        Edge *e2 = new Edge(to, from, 0);             // Residual edge
        e1->residual = e2;
        e2->residual = e1;
        adjList[from].push_back(e1);
        adjList[to].push_back(e2);
    }
    ~NetworkFlowSolver()                 // Destructor to free all the spaces
    {
        for (auto &edges : adjList)
        {
            for (Edge *edge : edges)
            {
                delete edge;
            }
        }
    }
    const vector<vector<Edge *>> &getGraph()    // After Max Flow Algorithm returns the graph
    {
        execute();
        return adjList;
    }
    long long getMaxFlow()         // Returns Max Flow after Max Flow Algorithm
    {
        execute();
        return maxFlow;
    }

private:
    void execute()               // Checks whether Max Flow Algorithm is runned once if not runs it
    {
        if (isSolved)
        {
            return;
        }
        isSolved = true;
        solve();
    }

protected:
    virtual void solve() = 0;     // Abstract method solve() to solve max flow algorithm defined in child class
};

class EdmondsKarpMaxFlowSolver : public NetworkFlowSolver
{
public:
    // Constructor for Edmonds-Karp algorithm
    // n: number of nodes, s: source node, t: sink node
    EdmondsKarpMaxFlowSolver(int n, int s, int t) : NetworkFlowSolver(n, s, t) {}
    void solve() override           // Overriding the solve method to implement Edmonds-Karp algorithm
    {
        while (bfs())              // BFS will find the smallest augmenting path
        {
            fill(ptr.begin(), ptr.end(), 0);     // Reset the pointer for each node to the first edge
            while (int bottleneck = dfs(s, INF)) // DFS will find the bottleneck value of the augmenting path
            {                                    // until the bottleneck value is 0
                maxFlow += bottleneck;           // Add the bottleneck value to the max flow
            }
        }
    }

private:
    bool bfs()         // bfs to find a bfs tree with subset of edges that has remaining capacity>0
    {
        fill(level.begin(), level.end(), -1);  // Mark all node at no level
        queue<int> q;                          // Queue to do level-wise bfs
        level[s] = 0;                          // Start from the source(source is root of bfs tree)
        q.push(s);                             // Push the source node to the queue
        while (!q.empty())                     // Do bfs until we have no node left in queue
        {
            int u = q.front();                 // Element in current level
            q.pop();
            for (Edge *e : adjList[u])         
            {
                if (level[e->to] == -1 && e->remainingCapacity() > 0)
                {
                    level[e->to] = 1 + level[u];  // Form each connected edges with the node select those with remaining
                    q.push(e->to);                // capacity and push them onto the queue
                }
            }
        }
        return level[t] != -1;        // If there is path to sink in bfs sub graph then return true otherwise false
    }
    int dfs(int u, long long flow)    // dfs to find the bottleneck value in the augmenting path
    {
        if (u == t)                // If sink is found return the flow
        {
            return flow;
        }
        for (; ptr[u] != adjList[u].size(); ptr[u]++)   // For each edges in the augmenting path of bfs sub graph with remainingg capacity
        {                                               // recursively do dfs and find the bottleneck value
            Edge *e = adjList[u][ptr[u]];
            if (level[e->to] == 1 + level[u] && e->remainingCapacity() > 0)
            {
                long long bottleneck = dfs(e->to, min(flow, e->remainingCapacity()));
                if (bottleneck > 0)
                {
                    e->augment(bottleneck);    // Augment the edge by adding bottleneck value to the flow
                    return bottleneck;         // Return the bottleneck value
                }
            }
        }
        return 0;
    }

public:
    static void exampleGraph1() // Example 1 :
    {
        int n = 6;
        int s = n - 1;
        int t = n - 2;
        EdmondsKarpMaxFlowSolver solver1(n, s, t);
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

        EdmondsKarpMaxFlowSolver solver2(n, s, t);

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

        EdmondsKarpMaxFlowSolver solver3(n, s, t);

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

        EdmondsKarpMaxFlowSolver solver4(n, s, t);

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
    EdmondsKarpMaxFlowSolver::exampleGraph1();
    EdmondsKarpMaxFlowSolver::exampleGraph2();
    EdmondsKarpMaxFlowSolver::exampleGraph3();
    EdmondsKarpMaxFlowSolver::exampleGraph4();
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
//          Edge 0 -> s | flow = -5 | capacity = 0 | is residual : true
//          Edge 0 -> 1 | flow = 0 | capacity = 5 | is residual : false
//          Edge 0 -> t | flow = 5 | capacity = 5 | is residual : false
//          Edge 1 -> s | flow = -10 | capacity = 0 | is residual : true
//          Edge 1 -> 0 | flow = 0 | capacity = 0 | is residual : true
//          Edge 1 -> t | flow = 10 | capacity = 10 | is residual : false
//          Edge s -> 0 | flow = 5 | capacity = 10 | is residual : false
//          Edge s -> 1 | flow = 10 | capacity = 10 | is residual : false
//          Edge t -> 0 | flow = -5 | capacity = 0 | is residual : true
//          Edge t -> 1 | flow = -10 | capacity = 0 | is residual : true