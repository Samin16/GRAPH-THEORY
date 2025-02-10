// Minimum Spanning Tree Prim's Algorithm


#include <bits/stdc++.h>
using namespace std;

int main()
{
    int V;
    cin >> V;
    vector<vector<int>> G(V, vector<int>(V));  // Adjacency Matrix Representation
    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            cin >> G[i][j];                    // G[i][j] represents 
        }
    }
    int num_edge;
    vector<bool> selected(V, false);           // To track which edge is selected and which not
    num_edge = 0;
    selected[0] = true;
    int x, y;
    cout << "Edge : Weight " << endl;
    while (num_edge < V - 1)             // Do for each vertex
    {
        int min = INT_MAX;
        x = 0;
        y = 0;
        for (int i = 0; i < V; i++)
        {
            if (selected[i])
            {
                for (int j = 0; j < V; j++)
                {
                    if (!selected[j] && G[i][j])
                    {
                        if (min > G[i][j])
                        {
                            min = G[i][j];
                            x = i;
                            y = j;
                        }
                    }
                }
            }
        }
        cout << x << "-" << y << ":" << G[x][y] << "\n";
        selected[y] = true;
        num_edge++;
    }
    return 0;
}