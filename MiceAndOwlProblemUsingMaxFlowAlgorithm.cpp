// Owls and mice problem with holes
// Given M mice with their positions, radius of a circle, H holes wwith their positions and capacity. A mouse is safe
// from owl if there is a hole inside the area of the circle with the given radius and mouse position as origin. We 
// need to determine the highest number of mice which are safe. 

// Author : Samin Yasar

#include<bits/stdc++.h>
using namespace std;

struct Point2D          // Simple 2D point on a 2D plane
{
    double x,y;                                    // Position of the point
    Point2D(double x=0,double y=0):x(x),y(y){}     // Initialization of the point    
    double distanceTo(const Point2D& other) const  // Returns distance to another point
    {
        double dx=x-other.x;
        double dy=y-other.y;
        return sqrt(dx*dx+dy*dy);
    }
};

struct Mouse                      // Simple Mouse properties
{
    Point2D position;             // Position of the mouse
    Mouse(double x,double y):position(x,y){}    // Initialization of a Mouse
};

struct Hole                       // Simple Hole properties
{
    Point2D position;             // Position of a Hoole
    int capacity;                 // Capacity of that Hole
    Hole(double x,double y,int capacity):position(x,y),capacity(capacity){}   // Initialization of a Hole
};

class NetworkFlowSolver                 // Solver class having all methods to solve the Owl and Mice problem
{
    struct Edge                         // Edge for the flow graph
    {
        int to,rev,flow,capacity;
    };
    vector<vector<Edge>> adjList;       // Adjacency list representation of the flow graph
    vector<int> level,ptr;              // Tracks level in bfs and the element of adjacency list that is next edge to be visited during dfs
    int n,source,sink;                  // Number of vertices, source node and sink node

    bool bfs()                          // bfs to find a bfs tree with subset of edges that has remaining capacity>0
    {
        fill(level.begin(),level.end(),-1);     // Mark all node at no level
        queue<int> q;                           // Queue to do level-wise bfs
        q.push(source);                         // Start from the source(source is root of bfs tree)
        level[source]=0;                        // Mark source at level 0               
        while(!q.empty())                       // Do bfs until we have no node left in queue
        {
            int u=q.front();                    // Element in current level
            q.pop();
            for(Edge& e:adjList[u])             // Form each connected edges with the node select those with remaining
            {                                   // capacity and push them onto the queue
                if(level[e.to]==-1 && e.flow<e.capacity)
                {
                    level[e.to]=level[u]+1;
                    q.push(e.to);
                }
            }
        }
        return level[sink]!=-1;           // If there is path to sink in bfs sub graph then return true otherwise false
    }
    int dfs(int u,int flow)               // dfs to find the bottleneck value in the augmenting path
    {
        if(u==sink)                       // If sink is found return the flow
        {
            return flow;
        }
        for(;ptr[u]<adjList[u].size();ptr[u]++)     // For each edges in the augmenting path of bfs sub graph with remainingg capacity
        {                                           // recursively do dfs and find the bottleneck value
            Edge& e=adjList[u][ptr[u]];
            if(level[e.to]==level[u]+1 && e.flow<e.capacity)
            {
                int pushed=dfs(e.to,e.capacity-e.flow);
                if(pushed>0)
                {
                    e.flow+=pushed;                      // Increase the flow of the edge by the bottleneck value
                    adjList[e.to][e.rev].flow-=pushed;   // Also decrease the residual edge's flow by bottlemeck value 
                    return pushed;                       // Return the bottleneck value
                }
            }
        }
        return 0;
    }
public:
    NetworkFlowSolver(int n,int s,int t):n(n),source(s),sink(t)     // Initialization of Network Flow Solver
    {
        adjList.resize(n);
        level.resize(n);
        ptr.resize(n);
    }
    void addEdge(int u,int v,int capacity)              // Add an edge and its residual edge
    {
        Edge forward={v,(int)adjList[v].size(),0,capacity};
        Edge backward={u,(int)adjList[u].size(),0,0};
        adjList[u].push_back(forward);
        adjList[v].push_back(backward);
    }
    int maxFlow()                // Returns the max flow of the flow graph
    {
        int flow=0;
        while(bfs())             // By doing bfs try to find all bfs tree possible with remaining capacity
        {
            fill(ptr.begin(),ptr.end(),0);        // Mark all adjList's starting position to 0
            while(int pushed=dfs(source,INT_MAX))    // Find the bottleneck values for each augmenting path and add them to the maxFlow
            {
                flow+=pushed;
            }
        }
        return flow;
    }
};

int solveMiceAndOwlWithHole(const vector<Mouse>& mice,const vector<Hole>& holes,double radius)    // Solves the Owl and Mice problem with Holes 
{
    const int M=mice.size();                 // Number of mouse
    const int H=holes.size();                // Number of hole
    const int N=M+H+2;                       // Number of nodes in the flow graph
    const int S=N-2;                         // Source node
    const int T=N-1;                         // Sink node
    NetworkFlowSolver solver(N,S,T);         // Object of the solver class
    for(int m=0;m<M;m++)                     // Add edge from source to each mouse with capacity 1 that means one mouse is running to their specified position
    {
        solver.addEdge(S,m,1);
    }
    for(int m=0;m<M;m++)             // If a mouse has a hole in its range then add an edge with capacity 1 between them
    {
        for(int h=0;h<H;h++)
        {
            if(mice[m].position.distanceTo(holes[h].position)<=radius)
            {
                solver.addEdge(m,M+h,1);
            }
        }
    }
    for(int h=0;h<H;h++)             // Add edge from each hole to the sink with the capacity of the hole
    {
        solver.addEdge(M+h,T,holes[h].capacity);
    }
    return solver.maxFlow();         // Run the solver class's maxFlow() method to get number of safe mice 
}

int main()
{
    vector<Mouse> mice={
        Mouse(0,0),
        Mouse(2,1),
        Mouse(3,3),
        Mouse(5,2),
        Mouse(10,10)
    };
    vector<Hole> holes={
        Hole(1,1,2),
        Hole(4,2,1),
        Hole(6,3,1)
    };
    double escapeRadius=1.5;
    int safe_mice = solveMiceAndOwlWithHole(mice, holes, escapeRadius);
    
    cout << "Maximum number of mice that can escape: " << safe_mice << endl;
    
    return 0;
}

// Output : Maximum number of mice that can escape: 4