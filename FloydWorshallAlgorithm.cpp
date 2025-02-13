// Floyd-Worshall Algorithm
// Author : Samin Yasar

#include<bits/stdc++.h>
using namespace std;

class Graph                // Graph structure and handling functions
{
    int numVertices;                // Number of vertices
    vector<vector<int>> adjMatrix;  // Adjacency Matrix representation of Graph
    vector<vector<int>> dp;         // dp table
    vector<vector<int>> next;       // Needed to construct SP
public:
    Graph(vector<vector<int>>& M)   // Initializes the Graph with a given 2D Matrix
    {
        numVertices=M.size();
        adjMatrix=vector<vector<int>>(numVertices,vector<int>(numVertices));
        adjMatrix=M;
    }
    void setUp();
    void FloydWorshallAPSP();
    void detectNegativeCycle();
    vector<int> makePath(int start,int end);
    vector<vector<int>> getResult()         // DP stores the shortest path between nodes (i,j)
    {
        return dp;
    }
};

// Actually here we finds path (i,j) through k; k=0,1,..,numVertices-1
// So, we need a 3D DP table but while finding best path we can do so 
// by track the previous distance like for k=1, we need k=0's result
// for k=3, we need k=2's result. TW, out DP table is 2D

void Graph::setUp()
{
    dp=vector<vector<int>>(numVertices,vector<int>(numVertices,INT_MAX));
    next=vector<vector<int>>(numVertices,vector<int>(numVertices,-1));
    for(int i=0;i<numVertices;i++)
    {
        for(int j=0;j<numVertices;j++)       // Sets the initial distance for k=0 layer as the Matrix distance specified
        {
            dp[i][j]=adjMatrix[i][j];
            if(adjMatrix[i][j]!=INT_MAX)
            {
                next[i][j]=j;
            }
        }
        dp[i][i]=0;         // Distance between same nodes is 0
    }
}

void Graph::FloydWorshallAPSP()
{
    setUp();
    for(int k=0;k<numVertices;k++)
    {
        for(int i=0;i<numVertices;i++)
        {
            for(int j=0;j<numVertices;j++)       // Checks if we've a better path through k=0,1,..
            {
                if(dp[i][k]!=INT_MAX && dp[k][j]!=INT_MAX && dp[i][j]>dp[i][k]+dp[k][j])
                {
                    dp[i][j]=dp[i][k]+dp[k][j];
                    next[i][j]=next[i][k];      // If path through k is better then next of i is k(not j)
                }
            }
        }
    }
    detectNegativeCycle();
}

void Graph::detectNegativeCycle()
{
    for(int k=0;k<numVertices;k++)
    {
        for(int i=0;i<numVertices;i++)
        {
            for(int j=0;j<numVertices;j++)     // If we find better path again then there is a negative cycle
            {
                if(dp[i][k]!=INT_MAX && dp[k][j]!=INT_MAX && dp[i][j]>dp[i][k]+dp[k][j])
                {
                    dp[i][j]=INT_MIN;
                    next[i][j]=-1;
                }
            }
        }
    }
}

vector<int> Graph::makePath(int start,int end)
{
    vector<int> result;
    if(dp[start][end]==INT_MAX)
    {
        return result;          // No Path exists
    }
    int at;
    for(at=start;at!=end;at=next[at][end])    // Makes Path from start to end-1
    {
        if(at==-1)
        {
            return {};      // Negative cycle
        }
        result.push_back(at);
    }
    if(next[at][end]==-1)
    {
        return {};               // Negative cycle
    }
    result.push_back(at);        // Adds end node
    return result;
}


int main()
{
    vector<vector<int>> M = {
        {0, 3, INT_MAX, 7},
        {8, 0, 2, INT_MAX},
        {5, INT_MAX, 0, 1},
        {2, INT_MAX, INT_MAX, 0}
    };

    Graph g(M);
    g.FloydWorshallAPSP();

    vector<vector<int>> result = g.getResult();
    cout << "Shortest distances between every pair of vertices:\n";
    for(int i = 0; i < result.size(); i++)
    {
        for(int j = 0; j < result[i].size(); j++)
        {
            if(result[i][j] == INT_MAX)
                cout << "INF ";
            else if(result[i][j] == INT_MIN)
                cout << "-INF ";
            else
                cout << result[i][j] << " ";
        }
        cout << endl;
    }

    int start = 0, end = 3;
    vector<int> path = g.makePath(start, end);
    if(path.empty())
    {
        cout << "No path from " << start << " to " << end << " or path affected by negative cycle.\n";
    }
    else
    {
        cout << "Path from " << start << " to " << end << ": ";
        for(int node : path)
        {
            cout << node << " ";
        }
        cout << endl;
    }

    return 0;
}


// Output  :  Shortest distances between every pair of vertices:
//            0 3 5 6
//            5 0 2 3
//            3 6 0 1
//            2 5 7 0
//            Path from 0 to 3: 0 1 2 3