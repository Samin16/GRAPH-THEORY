// Lazy Dijekstra Implementation
// A Single Sourse Shortest Path Algorithm
// Author : Samin Yasar

// #include <bits/stdc++.h>
// using namespace std;

// class Edge
// {
//     int v;
//     int w;

// public:
//     Edge() {}
//     Edge(int v, int w)
//     {
//         this->v = v;
//         this->w = w;
//     }
//     int getV()
//     {
//         return v;
//     }
//     int getW()
//     {
//         return w;
//     }
// };

// class IndexedpriorityQueue
// {
//     vector<int> value;
//     vector<int> pm;
//     vector<int> im;
//     int capacity;
//     int size;

// public:
//     IndexedpriorityQueue() {}
//     IndexedpriorityQueue(int capacity)
//     {
//         this->capacity = capacity;
//         size = 0;
//         pm.resize(capacity,-1);
//         im.resize(capacity,-1);
//         value.resize(capacity);
//     }
//     bool less(int i, int j)
//     {
//         return value[im[i]] < value[im[j]];
//     }
//     void Swap(int i, int j)
//     {
//         pm[im[i]] = j;
//         pm[im[j]] = i;
//         swap(im[i], im[j]);
//     }
//     void swim(int i)
//     {
//         int p = (i - 1) / 2;
//         while (i > 0 && less(i, p))
//         {
//             Swap(i, p);
//             i = p;
//             p = (i - 1) / 2;
//         }
//     }
//     void sink(int i)
//     {
//         int smallest = i;
//         int left = 2 * i + 1, right = 2 * i + 2;
//         if (left < size && less(left, smallest))
//         {
//             smallest = left;
//         }
//         if (right < size && less(right, smallest))
//         {
//             smallest = right;
//         }
//         if (smallest != i)
//         {
//             Swap(i, smallest);
//             sink(smallest);
//         }
//     }
//     void insert(int key, int val)
//     {
//         if (pm[key] != -1) {
//             updateValue(key, val);
//             return;
//         }
//         value[key] = val;
//         pm[key] = size;
//         im[size] = key;
//         swim(size);
//         size += 1;
//     }
//     int peekKey()
//     {
//         if (size == 0)
//         {
//             cout << "No data in the Indexed P.Queue\n";
//             return INT_MAX;
//         }
//         return im[0];
//     }
//     int peekVal()
//     {
//         if (size == 0)
//         {
//             cout << "No data in the Indexed P.Queue\n";
//             return INT_MAX;
//         }
//         return value[im[0]];
//     }
//     vector<int> poll()
//     {
//         if (size == 0)
//         {
//             cout << "No value to be polled in IPQ\n";
//             return {};
//         }
//         vector<int> result(2);
//         result[0] = im[0];
//         result[1] = value[im[0]];
//         Swap(0, size - 1);
//         pm[result[0]] = -1;
//         size -= 1;
//         sink(0);
//         return result;
//     }
//     void remove(int key)
//     {
//         int k = pm[key];
//         if (k == -1)
//         {
//             cout << "No data to remove from IPQ\n";
//             return;
//         }
//         Swap(k, size - 1);
//         size -= 1;
//         swim(k);
//         sink(k);
//         pm[key] = -1;
//         value[key] = INT_MAX;
//         im[size] = -1;
//     }
//     void updateValue(int key, int newVal)
//     {
//         int k = value[key];
//         value[key] = newVal;
//         if (k > newVal)
//         {
//             swim(pm[key]);
//         }
//         else
//         {
//             sink(pm[key]);
//         }
//     }
//     void increaseVal(int key, int newVal)
//     {
//         int k = value[key];
//         if (newVal > k)
//         {
//             value[key] = newVal;
//         }
//         sink(pm[key]);
//     }
//     void decreaseVal(int key, int newVal)
//     {
//         int k = value[key];
//         if (newVal < k)
//         {
//             value[key] = newVal;
//         }
//         swim(pm[key]);
//     }
//     int getSize()
//     {
//         return size;
//     }
//     vector<int> getPm()
//     {
//         return pm;
//     }
// };

// class Graph
// {
//     int numVertices;
//     vector<vector<Edge>> adj;
//     vector<int> dist;
//     vector<bool> visited;
//     IndexedpriorityQueue ipq;
//     vector<int> prev;
// public:
//     Graph(int numVertices)
//     {
//         this->numVertices = numVertices;
//         adj.resize(numVertices);
//         dist.resize(numVertices, INT_MAX);
//         ipq = IndexedpriorityQueue(numVertices);
//         visited.resize(numVertices, false);
//         prev.resize(numVertices,-1);
//     }
//     void addEdge(int u, int v, int w)
//     {
//         adj[u].push_back(Edge(v, w));
//     }
//     vector<int> DijkstraShortestPath(int s)
//     {
//         dist[s] = 0;
//         ipq.insert(s, 0);
//         while (ipq.getSize() != 0)
//         {
//             vector<int> data = ipq.poll();
//             int index = data[0], minValue = data[1];
//             visited[index] = true;
//             for (Edge edge : adj[index])
//             {
//                 if (visited[edge.getV()])
//                 {
//                     continue;
//                 }
//                 int newDist = dist[index] + edge.getW();
//                 if (newDist < dist[edge.getV()])
//                 {
//                     dist[edge.getV()] = newDist;
//                     prev[edge.getV()] = index;
//                     // ipq.updateValue(edge.getV(), newDist);
//                     vector<int> pm=ipq.getPm();
//                     if(pm[edge.getV()]==-1)
//                     {
//                         ipq.insert(edge.getV(),newDist);
//                     }
//                     else
//                     {
//                         ipq.updateValue(edge.getV(),newDist);
//                     }
//                 }
//             }
//         }
//         return dist;
//     }
//     vector<int> shortestPath(int s,int e)
//     {
//         vector<int> result;
//         vector<int> dist=DijkstraShortestPath(s);
//         if(prev[e]==-1)
//         {
//             cout<<"No path exists\n";
//             return {};
//         }
//         for(int i=e;i>=s;i=prev[i])
//         {
//             result.push_back(i);
//         }
//         reverse(result.begin(),result.end());
//         if(result[0]!=s)
//         {
//             cout<<"No path exists\n";
//             return{};
//         }
//         return result;
//     }
// };

// int main()
// {
//     int vertices = 5;
//     Graph g(vertices);

//     g.addEdge(0, 1, 10);
//     g.addEdge(0, 4, 5);
//     g.addEdge(1, 2, 1);
//     g.addEdge(1, 4, 2);
//     g.addEdge(2, 3, 4);
//     g.addEdge(3, 0, 7);
//     g.addEdge(3, 2, 6);
//     g.addEdge(4, 1, 3);
//     g.addEdge(4, 2, 9);
//     g.addEdge(4, 3, 2);

//     int startNode = 0;
//     vector<int> shortestPaths = g.DijkstraShortestPath(startNode);

//     cout << "Shortest distances from node " << startNode << ":\n";
//     for (int i = 0; i < vertices; i++)
//     {
//         cout << "Node " << i << ": " << (shortestPaths[i] == INT_MAX ? -1 : shortestPaths[i]) << "\n";
//     }
//     vector<int> path=g.shortestPath(0,3);
//     for(int element:path)
//     {
//         cout<<element<<" ";
//     }
//     cout<<"\n";
//     return 0;
// }

