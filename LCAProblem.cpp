#include<bits/stdc++.h>
using namespace std;

class TreeNode             // A treenode's structure
{
    int val;
    vector<TreeNode*> children;
    TreeNode* parent;
public:
    TreeNode(int id,TreeNode* p,vector<TreeNode*> C)   
    {
        val=id;
        parent=p;
        children=C;
    }
    void addChild(TreeNode* c)
    {
        children.push_back(c);
    }
    int getVal()
    {
        return val;
    }
    vector<TreeNode*> getChildren()
    {
        return children;
    }
    TreeNode* getParent()
    {
        return parent;
    }
};

class Graph              // Graph to find center of given graph and build a tree from it
{
    int numVertices;
    vector<vector<int>> adj;
    vector<int> degree;
    vector<int> leaves;
public:
    Graph(int n)
    {
        numVertices=n;
        adj.resize(n);
        degree.resize(n,0);
    }
    void addEdge(int s,int d)
    {
        adj[s].push_back(d);
        adj[d].push_back(s);
    }
    void countDegree()
    {
        for(int i=0;i<numVertices;i++)
        {
            degree[i]=adj[i].size();
        }
    }
    void findLeaves()
    {
        for(int i=0;i<numVertices;i++)
        {
            if(degree[i]==1)
            {
                leaves.push_back(i);
                degree[i]=0;
            }
        }
    }
    void findCenter()
    {
        countDegree();
        findLeaves();
        int count=leaves.size();
        while(count<numVertices)
        {
            vector<int> newLeaves;
            for(int leaf:leaves)
            {
                for(int i:adj[leaf])
                {
                    degree[i]--;
                    if(degree[i]==1)
                    {
                        newLeaves.push_back(i);
                    }
                }
            }
            count+=newLeaves.size();
            leaves=newLeaves;
        }
    }
    TreeNode* makeTree()
    {
        findCenter();
        int id=leaves[0];
        TreeNode* root=new TreeNode(id,NULL,{});
        return buildTree(root,NULL);
    }
    TreeNode* buildTree(TreeNode* node,TreeNode* parent)
    {
        for(int data:adj[node->getVal()])
        {
            if(parent!=NULL && parent->getVal()==data)
            {
                continue;
            }
            TreeNode* newChild=new TreeNode(data,node,{});
            node->addChild(newChild);
            buildTree(newChild,node);
        }
        return node;
    }
    int getTreeSize()
    {
        return numVertices;
    }
};

class SparseTable          // Sparse Table data Structure
{
    int N;
    int P;
    vector<int> Log2;
    vector<vector<int>> table;
    vector<vector<int>> it;
public:
    SparseTable(){}
    SparseTable(int n)
    {
        N=n;
        P=ceil(log2(N));
        Log2.resize(N+1,0);
        for(int i=2;i<=N;i++)
        {
            Log2[i]=1+Log2[i/2];
        }
        table=vector<vector<int>>(P+1,vector<int>(N));
        it=vector<vector<int>>(P+1,vector<int>(N));
    }
    void makeTable(vector<int>& arr)
    {
        for(int i=0;i<N;i++)
        {
            table[0][i]=arr[i];
            it[0][i]=i;
        }
        for(int p=1;p<=P;p++)
        {
            for(int i=0;i+(1<<p)<=N;i++)
            {
                int left=table[p-1][i];
                int right=table[p-1][i+(1<<(p-1))];
                table[p][i]=min(left,right);
                if(left<right)
                {
                    it[p][i]=it[p-1][i];
                }
                else
                {
                    it[p][i]=it[p-1][i+(1<<(p-1))];
                }
            }
        }
    }
    int minQuery(int l,int r)
    {
        int len=r-l+1;
        int p=Log2[len];
        int left=table[p][l];
        int right=table[p][r-(1<<p)+1];
        return min(left,right);
    }
    int minCascadingQuery(int l,int r)
    {
        int minVal=INT_MAX;
        for(int p=Log2[r-l+1];l<=r;p=Log2[r-l+1])
        {
            minVal=min(minVal,table[p][l]);
            l+=(1<<p);
        }
        return minVal;
    }
    int minIndexQuery(int l,int r)
    {
        int len=r-l+1;
        int p=Log2[len];
        int left=table[p][l];
        int right=table[p][r-(1<<p)+1];
        if(left<=right)
        {
            return it[p][l];
        }
        else
        {
            return it[p][r-(1<<p)+1];
        }
    }
};

class EulerianSearchAndLCA        // Eulerian tour and finding the lowest common ancestor
{
    vector<int> nodes;
    vector<int> depth;
    vector<int> last;
    SparseTable st;
    int eulerCount;
public:
    EulerianSearchAndLCA(Graph g)
    {
        TreeNode* root=g.makeTree();
        int n=g.getTreeSize();
        nodes.resize(2*n-1);
        depth.resize(2*n-1);
        last.resize(n);
        st=SparseTable(2*n-1);
        eulerCount=0;
        dfs(root,0);
        st.makeTable(depth);
    }
    void dfs(TreeNode* root,int nodeDepth) // Visit each node two times without leaves
    {
        if(root==NULL)                     // and store depth and use it to make a Sparse Table
        {
            return;
        }
        visit(root->getVal(),nodeDepth);
        for(auto child:root->getChildren())
        {
            dfs(child,nodeDepth+1);
            visit(root->getVal(),nodeDepth);
        }
    }
    void visit(int val,int nodeDepth)
    {
        nodes[eulerCount]=val;
        depth[eulerCount]=nodeDepth;
        last[val]=eulerCount;
        eulerCount++;
    }
    int LCA(int index1,int index2)
    {
        int l=last[index1];      // At time index1 and index2 was visited in eulerian tour
        int r=last[index2];
        if(l>r){
            swap(l,r);
        }
        int index=st.minIndexQuery(l,r);      // Query for the min depth index between l and r 
        return nodes[index];
    }
};

int main()
{
    Graph g(10);
    //Graph g(10);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(2, 6);
    g.addEdge(3, 7);
    g.addEdge(3, 8);
    g.addEdge(4, 9);
    EulerianSearchAndLCA et(g);
    int k=et.LCA(5,6);
    cout<<"Lowest common ancestor of 5 and 6 : "<<k<<"\n";
    return 0;
}


// Output :  Lowest common ancestor of 5 and 6 : 2