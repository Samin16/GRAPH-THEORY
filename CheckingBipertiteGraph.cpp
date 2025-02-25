// Checking Bipertite
// Checks a graph is bipertite or not
// Author : Samin Yasar

#include <bits/stdc++.h>
using namespace std;

template<typename T>
class Graph
{
    int vertices;
    unordered_map<T,vector<T>> adj;           // To make a adjacency list
    unordered_map<T,int> color;

public:
    Graph(int vertices)                       // Constructor to specify number of vertices
    {
        this->vertices = vertices;
    }
    void makeAdjList(vector<pair<T, T>>& edgeList)    // Making adjacency list from given list of edge pairs
    {
        for (auto edge : edgeList)
        {
            T u = edge.first;
            T v = edge.second;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
    }
    bool isBipartite()         // Returns true if the graph is bipertite
    {
        queue<T> q;
        for (auto& vertex:adj)       // Traverse through all vertices and do BFS until all vertices is not BFS tree
        {
            T v1=vertex.first;
            if (color.find(v1)==color.end()) 
            {
                color[v1] = 0;    // Specify color 
                q.push(v1);
                while (!q.empty())       // BFS : It simply goes through the adjacency list and if a node is not
                {                        //       visited marks it in different set by setting alternate color to u.(like if color[u]=0 then color[v]=1)
                    T u = q.front();     //       But if a node already has a color and it matches with color of 
                    q.pop();             //       node u then it's a common node in both set and need a different color (not 0 or 1)
                    for (T v : adj[u])   //       to distinguish it, that's why not a bipertite graph
                    {
                        if (color.find(v)==color.end())
                        {
                            color[v] = 1 - color[u];
                            q.push(v);
                        }
                        else if (color[u] == color[v])
                        {
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }
};

int main()
{
    int n = 5;
    vector<pair<int, int>> rivalries = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}};
    Graph<int> g(n);
    g.makeAdjList(rivalries);
    if (g.isBipartite())
    {
        cout << "Bipertite Graph" << endl;
    }
    else
    {
        cout << "Not bipertite" << endl;
    }
    return 0;
}

// Output : Not bipertite

// int main()
// {
//     int n = 5;
//     vector<pair<string,string>> rivalries = {{"Student","School"}, {"School","Teacher"}, {"Teacher", "University"}, {"University", "Place"}, {"Place", "School"}};
//     Graph<string> g(n);
//     g.makeAdjList(rivalries);
//     if (g.isBipartite())
//     {
//         cout << "Bipertite Graph" << endl;
//     }
//     else
//     {
//         cout << "Not bipertite" << endl;
//     }
//     return 0;
// }

// Output : Bipertite Graph