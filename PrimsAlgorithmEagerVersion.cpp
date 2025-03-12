// Prim's Minimum Spanning Tree Algorithm using IPQ(Eager Version)
// Author : Samin Yasar

#include <bits/stdc++.h>
using namespace std;

template <typename S, typename T>
class Edge
{
    S v;
    T cost;

public:
    Edge(S v, T cost) : v(v), cost(cost) {}
    S getV() const
    {
        return v;
    }
    T getCost() const
    {
        return cost;
    }
    bool operator<(const Edge &other) const
    {
        return cost < other.cost;
    }
};

template <typename S, typename T>
using keyValuePair = std::pair<S, T>;

template <typename S, typename T>
class IndexedPriorityQueue
{
    vector<T> value;
    vector<int> pm;
    vector<S> im;
    int capacity;
    int size;

public:
    IndexedPriorityQueue(int capacity) : capacity(capacity), size(0)
    {
        value.resize(capacity, numeric_limits<T>::max());
        pm.resize(capacity, -1);
        im.resize(capacity, -1);
    }

    bool less(int i, int j)
    {
        return value[im[i]] < value[im[j]];
    }

    void Swap(int i, int j)
    {
        pm[im[i]] = j;
        pm[im[j]] = i;
        swap(im[i], im[j]);
    }

    void swim(int i)
    {
        while (i > 0 && less(i, (i - 1) / 2))
        {
            Swap(i, (i - 1) / 2);
            i = (i - 1) / 2;
        }
    }

    void sink(int i)
    {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        if (left < size && less(left, smallest))
            smallest = left;
        if (right < size && less(right, smallest))
            smallest = right;

        if (smallest != i)
        {
            Swap(i, smallest);
            i = smallest;
        }
    }

    void insert(S key, T val)
    {
        if (pm[key] != -1)
        {
            decreaseValue(key, val);
            return;
        }
        value[key] = val;
        pm[key] = size;
        im[size] = key;
        swim(size);
        size++;
    }

    keyValuePair<S, T> poll()
    {
        if (size == 0)
        {
            cout << "No data in queue\n";
            return {-1, {-1, -1}};
        }
        keyValuePair<S, T> result = {im[0], value[im[0]]};
        Swap(0, size - 1);
        pm[result.first] = -1;
        size--;
        sink(0);
        return result;
    }

    S peekKey()
    {
        if (size == 0)
        {
            cout << "No element to peek\n";
            return -1;
        }
        return im[0];
    }

    T peekValue()
    {
        if (size == 0)
        {
            cout << "No element to peek\n";
            return numeric_limits<T>::max();
        }
        return value[im[0]];
    }

    void remove(S key)
    {
        int k = pm[key];
        if (k == -1)
        {
            cout << "No element to delete\n";
            return;
        }
        Swap(k, size - 1);
        size--;
        swim(k);
        sink(k);
        pm[key] = -1;
        value[key] = numeric_limits<T>::max();
    }

    void decreaseValue(S key, T val)
    {
        int k = pm[key];
        if (k == -1)
        {
            cout << "No value entered here yet\n";
            return;
        }
        if (value[key] > val)
        {
            value[key] = val;
            swim(pm[key]);
        }
    }

    bool contains(S key)
    {
        return pm[key] != -1;
    }

    bool empty()
    {
        return size == 0;
    }
};

class Graph
{
    int numVertices;
    vector<vector<Edge<int, int>>> adjList;
    vector<bool> visited;
    vector<tuple<int, int, int>> MST;
    IndexedPriorityQueue<int, pair<int, int>> ipq;
    int mstCost;

public:
    Graph(int numVertices) : numVertices(numVertices), ipq(numVertices)
    {
        adjList.resize(numVertices);
        visited.resize(numVertices, false);
        mstCost = 0;
    }

    void addEdge(int u, int v, int w)
    {
        adjList[u].emplace_back(v, w);
        adjList[v].emplace_back(u, w);
    }

    void addEdgesToCurrent(int currentNode)
    {
        visited[currentNode] = true;
        for (auto &edge : adjList[currentNode])
        {
            if (visited[edge.getV()])
                continue;
            int neighbor = edge.getV();
            int cost = edge.getCost();
            if (!ipq.contains(neighbor))
            {
                ipq.insert(neighbor, {cost, currentNode});
            }
            else
            {
                ipq.decreaseValue(neighbor, {cost, currentNode});
            }
        }
    }

    void primsMST(int start = 0)
    {
        int edgeCount = 0;
        addEdgesToCurrent(start);
        while (!ipq.empty() && edgeCount < numVertices - 1)
        {
            auto [currentIndex, edge] = ipq.poll();
            MST.push_back({edge.second, currentIndex, edge.first});
            mstCost += edge.first;
            edgeCount++;
            addEdgesToCurrent(currentIndex);
        }
        cout << "Minimum Spanning Tree Cost: " << mstCost << "\n";
        cout << "Edges in MST:\n";
        for (auto &[u, v, w] : MST)
        {
            cout << u << " -- " << v << " (weight: " << w << ")\n";
        }
    }
};

int main()
{
    // Graph 1 :
    Graph g(5);
    g.addEdge(0, 1, 2);
    g.addEdge(0, 3, 6);
    g.addEdge(1, 2, 3);
    g.addEdge(1, 3, 8);
    g.addEdge(1, 4, 5);
    g.addEdge(2, 4, 7);
    g.addEdge(3, 4, 9);

    // Graph 2 :
    // Graph g(7);
    // g.addEdge(0,2,0);
    // g.addEdge(2,5,6);
    // g.addEdge(0,5,7);
    // g.addEdge(0,3,5);
    // g.addEdge(5,3,2);
    // g.addEdge(5,6,1);
    // g.addEdge(0,1,9);
    // g.addEdge(3,1,-2);
    // g.addEdge(3,6,3);
    // g.addEdge(1,4,3);
    // g.addEdge(6,4,6);

    cout << "Running Prim's Algorithm...\n";
    g.primsMST();

    return 0;
}

// For Graph 1:
// Output   :   Running Prim's Algorithm...
//              Minimum Spanning Tree Cost: 16
//              Edges in MST:
//              0 -- 1 (weight: 2)
//              1 -- 2 (weight: 3)
//              1 -- 4 (weight: 5)
//              0 -- 3 (weight: 6)     

// For Graph 2:
// Output   :   Running Prim's Algorithm...
//              Minimum Spanning Tree Cost: 9
//              Edges in MST:
//              0 -- 2 (weight: 0)
//              0 -- 3 (weight: 5)
//              3 -- 1 (weight: -2)
//              3 -- 5 (weight: 2)
//              5 -- 6 (weight: 1)
//              1 -- 4 (weight: 3)