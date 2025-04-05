// Given n pairs of integers, find a valid assignment of operations (+, -, *) such that the results are unique for each pair. 
// The problem is solved using a max flow algorithm.

// Author : Samin Yasar

#include <bits/stdc++.h>
using namespace std;

class NumPair           // Class to represent a pair of integers and their operations
{
public:
    int x, y;           // Twoo integers in the pair
    NumPair(int x, int y) : x(x), y(y) {}       // Initializer constructor
    int addition = x + y, subtraction = x - y, multiplication = x * y;    // Operations on the pair
};

struct Edge            // Structure to represent an edge int the flow network
{
    int to, rev, flow, capacity;
};

class MaxFlowSolver              // Class to solve Max Flow problem
{
    vector<vector<Edge>> adjList;     // Adjacency list to represent the flow network
    vector<int> level, ptr;           // Level and pointer arrays for bfs and dfs
    int source, sink;                 // Source and sink vertices
    bool bfs()                  // Performs BFS to find an augmenting path
    {
        fill(level.begin(), level.end(), -1);   // Reset level array
        queue<int> q;                        // Queue for BFS
        level[source] = 0;                   // Mark the source level as 0
        q.push(source);
        while (!q.empty())                   // While there is vertex in the queue
        {
            int v = q.front();               // take the top vertex and pop it
            q.pop();                         // and for all edges pointing to a vertex without level
            for (Edge &e : adjList[v])       // and having extra capacity add them to the augmenting path
            {
                if (level[e.to] == -1 && e.flow < e.capacity)
                {
                    level[e.to] = 1 + level[v];
                    q.push(e.to);
                }
            }
        }
        return level[sink] != -1;        // If sink node is reachable then return true otherwise false
    }
    int dfs(int v, int flow)             // Performs DFS to find the maximum flow in the augmenting path
    {
        if (v == sink)                   // If the sink node is reached then return the flow
        {
            return flow;
        }
        for (; ptr[v] < adjList[v].size(); ptr[v]++)   // For all edges outgoing from the vertex
        {
            Edge &e = adjList[v][ptr[v]];
            if (level[e.to] == 1 + level[v] && e.flow < e.capacity)  // if it is on the augmenting path
            {                                                        // and has extra capacity then
                int bottleneck = dfs(e.to, min(flow, e.capacity - e.flow));  // recursively call dfs on the edge
                if (bottleneck > 0)             // If the bottleneck is greater than 0 then update the flow of the edge and 
                {                               // the residual edge
                    e.flow += bottleneck;
                    adjList[e.to][e.rev].flow -= bottleneck;
                    return bottleneck;          // Return the bottleneck value
                }
            }
        }
        return 0;               
    }

public:
    MaxFlowSolver(int n, int s, int t) : source(s), sink(t)       // Constructor to initialize the flow network
    {
        adjList.resize(n);
        level.resize(n);
        ptr.resize(n);
    }
    void addEdge(int from, int to, int capacity)        // Add an edge and its residual edge to the flow network
    {
        Edge forward = {to, (int)adjList[to].size(), 0, capacity};
        Edge backward = {from, (int)adjList[from].size(), 0, 0};
        adjList[from].push_back(forward);
        adjList[to].push_back(backward);
    }
    int maxFlow()              // Returns the max flow tn the flow network considering all augmenting paths
    {                          // until no more augmenting paths are found with left capacity
        int totalFlow = 0;
        while (bfs())
        {
            fill(ptr.begin(), ptr.end(), 0);
            while (int flow = dfs(source, INT_MAX))
            {
                totalFlow += flow;
            }
        }
        return totalFlow;
    }
    vector<vector<Edge>> &getAdjList()     // Returns the adjacency list to bbuild up the assignments
    {
        return adjList;
    }
};

void elementaryMathProblemUsingMaxFlow(int n, vector<NumPair> &pairs)   // Function to solve the elementary math problem using max flow(argument : number of pairs and the pairs)
{
    unordered_set<int> uniqueSolutions;         // Set to store unique solutions
    unordered_map<int, int> resultToIndex;      // Map to store the index of each unique solution
    for (auto &pair : pairs)                    // For each pair of numbers insert the solution of each operation
    {                                           // into the set
        uniqueSolutions.insert(pair.addition);
        uniqueSolutions.insert(pair.subtraction);
        uniqueSolutions.insert(pair.multiplication);
    }
    int resultIndex = n;
    for (int result : uniqueSolutions)          // Set each index for solutions starting index is #of pairs+1
    {
        resultToIndex[result] = resultIndex++;
    }
    int vertex = uniqueSolutions.size() + n + 2;     // Total number of vertices in the flow network
    int source = vertex - 2, sink = vertex - 1;         // Source and sink vertices
    MaxFlowSolver maxFlowSolver(vertex, source, sink);  // Create the flow network
    for (int i = 0; i < n; i++)                         // Add edges from source to each pair with capacity 1
    {
        maxFlowSolver.addEdge(source, i, 1);
    }
    for (int i = 0; i < n; i++)                  // Add edges from each pair to the unique solutions with capacity 1
    {
        NumPair &pair = pairs[i];
        maxFlowSolver.addEdge(i, resultToIndex[pair.addition], 1);
        maxFlowSolver.addEdge(i, resultToIndex[pair.subtraction], 1);
        maxFlowSolver.addEdge(i, resultToIndex[pair.multiplication], 1);
    }
    for (int result : uniqueSolutions)
    {
        maxFlowSolver.addEdge(resultToIndex[result], sink, 1);   // Add edges from each unique solution to the sink with capacity 1
    }
    int maxFlow = maxFlowSolver.maxFlow();      // Calculate the max flow in the flow network
    if (maxFlow != n)                           // If the max flow is not equal to the number of pairs then 
    {                                           // there is no valid assignment value
        cout << "No valid assignment possible\n";
        return;
    }
    vector<string> assignments(n);              // Vector to store the assignments
    vector<vector<Edge>>& adjList=maxFlowSolver.getAdjList();   // Get the adjacency list after running the max flow algorithm
    for(int i=0;i<n;i++)                // For each pair of numbers check the edges in the adjacency list
    {
        for(Edge& e:adjList[i])   
        {
            if(e.flow>0)               // and if the flow is greater than 0 then it means the edge is a part of the augmenting path
            {
                int result;
                char operation;        
                if(e.to==resultToIndex[pairs[i].addition])   // Check which operation is used in the assignment
                {
                    result=pairs[i].addition;
                    operation='+';
                }
                else if(e.to==resultToIndex[pairs[i].subtraction])
                {
                    result=pairs[i].subtraction;
                    operation='-';
                }
                else
                {
                    result=pairs[i].multiplication;
                    operation='*';
                }                                      // Add the assignment to the vector of assignments
                assignments[i]=to_string(pairs[i].x)+" "+operation+" "+to_string(pairs[i].y)+" = "+to_string(result);
            }
        }
    }
    cout<<"Valid assignments are:\n";      // Print the assignments
    for (int i = 0; i < n; i++)
    {
        cout << assignments[i] << "\n";
    }
}

int main()
{
    int n;
    cout<<"Enter number of pairs : ";
    cin >> n;
    vector<NumPair> pairs;
    cout<<"Enter pairs : \n";
    for (int i = 0; i < n; i++)
    {
        int x, y;
        cin >> x >> y;
        pairs.push_back(NumPair(x, y));
    }
    elementaryMathProblemUsingMaxFlow(n, pairs);
    return 0;
}

// Input  : Enter number of pairs : 4
//          Enter pairs : 
//          1 5
//          3 3
//          -1 -6
//          2 2
// Output : Valid assignments are:
//          1 + 5 = 6
//          3 - 3 = 0
//          -1 + -6 = -7
//          2 + 2 = 4

// Input  : Enter number of pairs : 5
//          Enter pairs : 
//          1 2
//          2 1
//          1 2
//          2 1
//          1 2
// Output : No valid assignment possible

// Input  : Enter number of pairs : 5
//          Enter pairs : 
//          1 5
//          3 3
//          4 5
//          6 2
//          -1 -6
// Output : Valid assignments are:
//          1 + 5 = 6
//          3 - 3 = 0
//          4 + 5 = 9
//          6 + 2 = 8
//          -1 + -6 = -7