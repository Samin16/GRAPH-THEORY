// BFS shortest path(SP) on a grid


#include<bits/stdc++.h>
using namespace std;

#define ap pair<int,int>          //used to store position (x,y) to build SP

class Solve
{
    vector<vector<char>> dungeon;   // The Dungeon As a Collection of char in Matrix form
    int R;                          // Number of row
    int C;                          // Number of column
    vector<vector<ap>> prev;        // Stores prev pair(i,j) of a node
    queue<int> row;                 // Row and Column queue to do BFS on a grid
    queue<int> column;
    int moveCount;                  // Counting nodes in SP
    int leftInLayer;                // Layer based count
    int inNextLayer;
    bool reachedEnd;                // Have we reached end or not
    vector<vector<bool>> visited;   // Tracks if a node is visited or not
    vector<int> dR;                 // dR and dC to traverse the adjacent nodes (up,down,left,right)
    vector<int> dC;
public:
    Solve(vector<vector<char>>& dung)   // Initialization
    {
        dungeon=dung;
        R=dungeon.size();
        C=dungeon[0].size();
        moveCount=0;
        leftInLayer=1;
        inNextLayer=0;
        reachedEnd=false;
        visited = vector<vector<bool>>(R, vector<bool>(C, false));
        prev = vector<vector<ap>>(R, vector<ap>(C, {-1, -1}));
        dR={-1,1,0,0};
        dC={0,0,1,-1};
    }
    vector<int> positionOfS();
    void exploreNeighbours(int r,int c);
    int shortestPath();
    vector<int> positionOfE();
    vector<ap> makePath();
};

vector<int> Solve::positionOfS()    // Search for the starting node and return its Position(i,j)
{
    int m=dungeon.size();
    int n=dungeon[0].size();
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(dungeon[i][j]=='S')
            {
                return {i,j};
                break;
            }
        }
    }
    return {};
}

vector<int> Solve::positionOfE()    // Search for end node and return its Position(i,j)
{
    int m=dungeon.size();
    int n=dungeon[0].size();
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(dungeon[i][j]=='E')
            {
                return {i,j};
                break;
            }
        }
    }
    return {};
}

void Solve::exploreNeighbours(int r,int c)
{
    for(int i=0;i<4;i++)         // Iterate through each adjacent node
    {
        int rr=r+dR[i];
        int cc=c+dC[i];
        if(rr<0 || cc<0)        // If out of bound position or visited or blocked
        {                       // then continue
            continue;
        }
        if(rr>=R || cc>=C)
        {
            continue;
        }
        if(visited[rr][cc] || dungeon[rr][cc]=='#')
        {
            continue;
        }
        row.push(rr);          // Enqueues the adjacent node position (rr,cc)
        column.push(cc);       // and sets the prev of it to the node for
        visited[rr][cc]=true;  // which we find it and the enqued node is in next layer
        prev[rr][cc]={r,c};
        inNextLayer++;
    }
}

int Solve::shortestPath()     // BFS starts from the position of start node(i,j)
{
    vector<int> sDun=positionOfS();
    if(sDun.empty())
    {
        return -1;
    }
    row.push(sDun[0]);
    column.push(sDun[1]);
    visited[sDun[0]][sDun[1]]=true;
    while(!column.empty())
    {
        int r=row.front();
        row.pop();
        int c=column.front();
        column.pop();
        if(dungeon[r][c]=='E')     // If find the path , We are done
        {
            reachedEnd=true;
            break;
        }
        exploreNeighbours(r,c);   // Explore the neighbors to enqueue them
        leftInLayer--;            // Every time node in current layer decreases
        if(leftInLayer==0)        // If we've visited all node in current layer
        {                         // then we go to the next layer
            leftInLayer=inNextLayer;
            inNextLayer=0;
            moveCount++;          // Increasing layer count to reach end
        }
    }
    if(reachedEnd)
    {
        return moveCount;
    }
    return -1;
}

vector<ap> Solve::makePath()     // Constructing the path from start to end
{
    vector<int> end=positionOfE();
    if(end.empty() || prev[end[0]][end[1]]==ap{-1,-1})
    {
        return {};
    }
    vector<ap> path;
    ap at={end[0],end[1]};
    while(prev[at.first][at.second]!=ap{-1,-1})
    {
        path.push_back(at);
        at=prev[at.first][at.second];
    }
    path.push_back(at);
    reverse(path.begin(),path.end());
    return path;
}

//Dungeon problem
int main()
{
    int m,n;
    cin>>m>>n;
    vector<vector<char>> dungeon(m,vector<char>(n));
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            cin>>dungeon[i][j];
        }
    }

    //int k=shortestPath(dungeon,prev);
    Solve s(dungeon);
    int k=s.shortestPath();
    vector<ap> path=s.makePath();
    cout<<k<<"\n";
    for(auto element:path)
    {
        cout<<"-("<<element.first<<","<<element.second<<")";
    }
    return 0;
}


// Input : 7 5
//         S..#...
//         .#...#.
//         .#.....
//         ..##...
//         #.#E.#.
// Output : 7
//         -(0,0)-(1,0)-(2,0)-(2,1)-(3,1)-(4,1)-(5,1)-(6,1)