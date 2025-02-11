// Finding tree Isomorphism
// Author : Samin Yasar

#include<bits/stdc++.h>
using namespace std;

class TreeNode                     // Represents one node off a tree
{
    int val;                    // Value of a node
    TreeNode* parent;           // Parent Node
    vector<TreeNode*> children; // Childrens of the node
public:
    TreeNode(int id,TreeNode* p,vector<TreeNode*> c)
    {
        val=id;
        parent=p;
        children=c;
    }
    void addChild(TreeNode* c)
    {
        children.push_back(c);
    }
    int getVal()
    {
        return val;
    }
    string encodeTree(TreeNode* root)     // Encodes a tree in Parentheses form
    {
        if(root==NULL)
        {
            return "";
        }
        vector<string> s;
        for(auto child:root->children)
        {
            s.push_back(encodeTree(child));
        }
        // for(auto c:s)                  // For debug
        // {
        //     cout<<c<<" ";
        // }
        // cout<<"\n";
        sort(s.begin(),s.end());
        string result="";
        for(string c:s)
        {
            result+=c;
        }
        return "("+result+")";
    }
};

class Graph             // Finds center of a tree from a Graph and builds the tree
{
    int numVertices;
    vector<vector<int>> adj;
    vector<int> degree;
    vector<int> leaf;
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
    void foundLeaf()
    {
        for(int i=0;i<numVertices;i++)
        {
            if(degree[i]==1)
            {
                leaf.push_back(i);
                degree[i]=0;
            }
        }
    }
    void foundCenter()
    {
        countDegree();
        foundLeaf();
        int count=leaf.size();
        while(count<numVertices)
        {
            vector<int> newLeaf;
            for(int l:leaf)
            {
                for(int n:adj[l])
                {
                    degree[n]--;
                    if(degree[n]==1)
                    {
                        newLeaf.push_back(n);
                    }
                }
                degree[l]=0;
            }
            count+=newLeaf.size();
            leaf=newLeaf;
        }
    }
    vector<int> getLeaf()
    {
        return leaf;
    }
    TreeNode* makeTree(int id);
    TreeNode* buildTree(TreeNode* root,TreeNode* parent);
};

TreeNode* Graph::makeTree(int id)
{
    TreeNode* root=new TreeNode(id,NULL,{});
    return buildTree(root,NULL);
}

TreeNode* Graph::buildTree(TreeNode* root,TreeNode* parent)
{
    for(int child:adj[root->getVal()])
    {
        if(parent!=NULL && child==parent->getVal())
        {
            continue;
        }
        TreeNode* newChild=new TreeNode(child,root,{});
        root->addChild(newChild);
        buildTree(newChild,root);
    }
    return root;
}

bool isIsomorphic(Graph g1,Graph g2)   // Checks if two trees represented by given two Graphs has the same Structure
{
    g1.foundCenter();
    g2.foundCenter();
    vector<int> root1=g1.getLeaf();
    vector<int> root2=g2.getLeaf();
    TreeNode* tree1=g1.makeTree(root1[0]);
    string s1=tree1->encodeTree(tree1);
    cout<<s1<<"\n";
    for(int r:root2)
    {
        TreeNode* tree2=g2.makeTree(r);
        string s2=tree2->encodeTree(tree2);
        cout<<s2<<"\n";
        if(s1==s2)
        {
            return true;
        }
    }
    return false;
}

int main()
{
    Graph g1(10);
    // g1.addEdge(0,4);
    // // g1.addEdge(0,1);
    // g1.addEdge(1,2);
    // g1.addEdge(2,3);
    // g1.addEdge(3,4);
    // g1.addEdge(3,5);
    // g1.addEdge(2,9);
    // g1.addEdge(2,6);
    // g1.addEdge(6,7);
    // g1.addEdge(6,8);
    g1.addEdge(0,1);
    g1.addEdge(0,2);
    g1.addEdge(0,3);
    g1.addEdge(0,4);
    g1.addEdge(1,5);
    g1.addEdge(2,6);
    g1.addEdge(2,7);
    g1.addEdge(4,8);
    g1.addEdge(4,9);
    Graph g2(10);
    g2.addEdge(1,2);
    g2.addEdge(2,0);
    g2.addEdge(1,9);
    g2.addEdge(1,7);
    g2.addEdge(7,6);
    g2.addEdge(7,8);
    g2.addEdge(1,4);
    g2.addEdge(4,3);
    g2.addEdge(4,5);
    bool k=isIsomorphic(g1,g2);
    cout<<(k?("YES"):"NO")<<"\n";
    return 0;
}

//     Output    :    ((()())(()())(())())
//                    ((()())(()())(())())
//                    YES
//     If the first Graph is the commented Graph then g1 and g2 are not isomorphic