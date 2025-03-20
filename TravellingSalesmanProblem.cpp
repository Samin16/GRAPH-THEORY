// Travelling Salesman Problem (In a complete weighted cycle find the Hamiltonian
// cycle with minimum cost)
// Author : Samin Yasar

#include<bits/stdc++.h>
using namespace std;

class Graph         
{
    int numVertices;                // Number of vertices in Graph
    vector<vector<int>> adjMatrix;  // Adjacency matrix representation
    vector<vector<int>> memo;       // Memoization table to store dist for each state
    int minCost;                    // To store Miinimum cost
    vector<int> tour;               // The sequence of tor
public:
    Graph(vector<vector<int>>& M)   // Initialization
    {
        this->numVertices=M.size();
        adjMatrix=vector<vector<int>>(numVertices,vector<int>(numVertices));
        adjMatrix=M;
        memo=vector<vector<int>>(numVertices,vector<int>((1<<numVertices)));
        minCost=0;
        tour.resize(numVertices+1);
    }
    void TSP(int start=0)       // Solves whole TSP problem
    {
        setUp(start);           // Set up the dp table
        solve(start);           // Fill the dp table for all sub-problems
        minCost=findMinCost(start);  // Find the min cost of tour
        tour=findOptimalTour(start); // Finds the cycle with min cost
    }
    void setUp(int start)        
    {
        for(int i=0;i<numVertices;i++)  // Set all distance from start node to other nodes 
        {                               
            if(i==start)                // without the starting node
            {
                continue;
            }
            memo[i][(1<<i) | (1<<start)]=adjMatrix[start][i];
        }
    }
    void solve(int s)
    {
        for(int r=3;r<=numVertices;r++)     // Set distances for sequences of length>2
        {
            vector<int> subSets=combinations(r,numVertices); 
            for(int subSet:subSets)    // For all combinations of bits with numVertices digit
            {                          // and r ones find the rth node to flip (If shortest path
                if(notIn(s,subSet))    // from last node to next node then set the next node 
                {                      // representing bit as 1
                    continue;
                }
                for(int next=0;next<numVertices;next++)  // Check for each possible next node
                {
                    if(next==s || notIn(next,subSet))
                    {
                        continue;
                    }
                    int state=subSet^(1<<next);   // Set the next nodes digit to zero
                    int minDist=INT_MAX;
                    for(int e=0;e<numVertices;e++)  // Find the closest next node from the last node 
                    {                               // of sequence with length r-1
                        if(e==s || e==next || notIn(e,subSet))
                        {
                            continue;
                        }
                        int newDist=memo[e][state]+adjMatrix[e][next];
                        if(newDist<minDist)
                        {
                            minDist=newDist;
                        }
                    }
                    memo[next][subSet]=minDist;      // Set appropiate next node's distance
                }
            }
        }
    }
    vector<int> combinations(int r,int n)    // Returns all combinations of bitmask with n length and r ones
    {
        vector<int> subSets={};
        findCombinations(0,0,r,n,subSets);
        return subSets;
    }
    void findCombinations(int set,int at,int r,int n,vector<int>& subSets)  // Helper method for combinations
    {
        if(r==0)
        {
            subSets.push_back(set);
        }
        else
        {
            for(int i=at;i<n;i++)
            {
                set=set|(1<<i);
                findCombinations(set,i+1,r-1,n,subSets);
                set=set&~(1<<i);        // Traceback by flipping the node which was made 1
            }
        }
    }
    bool notIn(int i,int subSet)      // Does the subset contain 1 at ith position
    {
        return ((1<<i)&subSet)==0;
    }
    int findMinCost(int s)           // Finds the minimum cost
    {
        int endState=(1<<numVertices)-1;    // The endstate
        int minTourCost=INT_MAX;
        for(int e=0;e<numVertices;e++)      // For each possible end node in its endState choose the best
        {                                   // one that creates lowest cost cycle with starting node
            if(e==s)
            {
                continue;
            }
            int tourCost=memo[e][endState]+adjMatrix[e][s];
            if(tourCost<minTourCost)
            {
                minTourCost=tourCost;
            }
        }
        return minTourCost;
    }
    vector<int> findOptimalTour(int s)   // Starting from the last node choose the best
    {                                    // next node to the tour
        int lastIndex=s;
        int state=(1<<numVertices)-1;
        for(int i=numVertices-1;i>=1;i--)  // Without first and last position find the best nodes 
        {                                  // to minimize the distance
            int index=-1;
            for(int j=0;j<numVertices;j++)
            {
                if(j==s || notIn(j,state))
                {
                    continue;
                }
                if(index==-1)
                {
                    index=j;
                }
                // Select the best node that creates shortest distance with the next node
                // for the last node starting node is the next node
                int prevDist=memo[index][state]+adjMatrix[index][lastIndex];
                int newDist=memo[j][state]+adjMatrix[j][lastIndex];
                if(newDist<prevDist)
                {
                    index=j;
                }
            }
            tour[i]=index;
            state=state^(1<<index);     // Update state and last index to complete the path
            lastIndex=index;            // by finding current nodes previous vertices
        }
        tour[0]=s;
        tour[numVertices]=s;
        return tour;
    }
    int getTourCost()      // Returns the minimum cost
    {
        return minCost;
    }
    vector<int> getTour()      // Returns the tour that minimizes path distance
    {
        return tour;
    }
};

int main()
{
    int n;
    cout<<"Enter number of vertices : ";
    cin>>n;
    vector<vector<int>> adjMatrix(n,vector<int>(n));
    cout<<"Enter edges as adjacency matrix (If not exist enter 0)\n";
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            cin>>adjMatrix[i][j];
        }
    }
    Graph g(adjMatrix);
    g.TSP();
    cout<<"Cost for the Hamiltonian Cycle : "<<g.getTourCost()<<"\n";
    vector<int> path=g.getTour();
    for(int vertex:path)
    {
        cout<<vertex<<" ";
    }
    cout<<"\n";
    return 0;
}

// Output : Enter number of vertices : 4
//          Enter edges as adjacency matrix (If not exist enter 0)
//          0 4 1 9
//          3 0 6 11
//          4 1 0 2
//          6 5 -4 0
//          Cost for the Hamiltonian Cycle : 9
//          0 3 2 1 0