#include <iostream>
#include <algorithm>

using namespace std;

template <typename Key, typename Info>
class Dictionary
{
private:
    struct Node
    {
        Key key;
        Info info;
        int bfactor;
        Node *llink;
        Node *rlink;
    };

    Node* root;

    void copyTree(Node* &cpy,
                   Node* copied); //copies tree copied to tree cpy
    void destroy(Node* &p); //destroys tree of node p
 
    void inOrder(Node* p) const;
    void preOrder(Node* p) const;
    void postOrder(Node* p) const;

    int height(Node* p) const; //returns the height of tree of node p 
    int nodeCount(Node* p) const;   //returns nodes in tree of node p
    int leavesCount(Node* p) const; //returns the number of leaves
                            //for node p

    void rotateLeft(Node* &p);
    void rotateRight(Node* &p);

    void balanceLeft(Node* &p);
    void balanceRight(Node* &p);

    void insertNode(Node* &p, Node* newNode, bool isTaller);

public:
    const Dictionary<Key,Info>& operator= 
        (const Dictionary<Key,Info>& copied); //overload operator
    bool isEmpty() const;   //true if tree is empty
    void inOrderTraversal() const;
    void preOrderTraversal() const;
    void postOrderTraversal() const;

    int treeHeight() const; //returns the height of the tree
    int treeNodeCount() const;  //returns the number of nodes in the tree
    int treeLeavesCount() const; //returns the number of leaves
    
    void destroyTree(); //destroys tree, not a destructor
    
    void insert(Key k, Info f);

    Dictionary(const Dictionary<Key,Info>& copied); //copy constructor
    Dictionary();
    ~Dictionary();
}; // ADD DELETE FROM THE DICTIONARY 

//
// PUBLIC METHODS
//

template<typename Key,typename Info>
const Dictionary<Key,Info>& Dictionary<Key,Info> :: operator= 
                                    (const Dictionary<Key,Info>& copied)
{
    if(this != &copied)
    {
        if(root != nullptr) destroy(root);
        if(copied.root == nullptr) root = NULL;
        else copyTree(root , copied.root);
    }
    return *this;
}

template<typename Key, typename Info>
bool Dictionary<Key,Info> :: isEmpty() const
{
    return (root == nullptr);
}

template<typename Key, typename Info>
void Dictionary<Key,Info> :: inOrderTraversal() const 
{
    inOrder(root);
}

template<typename Key, typename Info>
void Dictionary<Key,Info> :: preOrderTraversal() const 
{
    preOrder(root);
}

template<typename Key, typename Info>
void Dictionary<Key,Info> :: postOrderTraversal() const 
{
    postOrder(root);
}

template<typename Key, typename Info>
int Dictionary<Key,Info> :: treeHeight() const
{
    return height(root);
}

template<typename Key, typename Info>
int Dictionary<Key,Info> :: treeNodeCount() const
{
    return nodeCount(root);
}

template<typename Key, typename Info>
int Dictionary<Key,Info> :: treeLeavesCount() const
{
    return LeavesCount(root);
}

template<typename Key, typename Info>
void Dictionary<Key,Info> :: destroyTree()
{
    return destroy(root);
}

template<typename Key,typename Info>
void Dictionary<Key,Info> :: insert(Key k, Info f)
{
    bool isTaller = false;
    Node* newNode;
    newNode = new Node;
    newNode->info = f;
    newNode->key = k;
    newNode->bfactor = 0;
    newNode->llink = NULL;
    newNode->rlink = NULL;

    insertNode(root, newNode , isTaller);
}

template<typename Key,typename Info>
Dictionary<Key,Info> :: Dictionary(const Dictionary<Key,Info>& copied) 
{
    if(copied.root == nullptr) root = NULL;
    else copyTree(root , copied.root);
}

template<typename Key,typename Info>
Dictionary<Key,Info> :: ~Dictionary() 
{
    destroy(root);
}

//
// PRIVATE METHODS
//

template<typename Key,typename Info>
void Dictionary<Key,Info> :: copyTree(Node* &cpy,
                                        Node* copied) 
{
    if(copied == nullptr) cpy = NULL;
    else 
    {
        cpy = new Node;
        cpy->info = copied->info;
        cpy->key = copied->key;
        cpy->bfactor = copied->bfactor;
        copyTree(cpy->llink , copied->llink);
        copyTree(cpy->rlink , copied->rlink); 
    }
}

template<typename Key,typename Info>
void Dictionary<Key,Info> :: destroy(Node* &p) 
{
    if(p!=nullptr) 
    {
        destroy(p->llink);
        destroy(p->rlink);
        delete p;
        p = NULL;
    }
}

template<typename Key,typename Info> 
void Dictionary<Key,Info> :: inOrder(Node *p) const
{
    if(p != nullptr) 
    {
        inOrder(p->llink);
        cout<< p->info<<" ";
        inOrder(p->rlink);
    }   
}

template<typename Key,typename Info> 
void Dictionary<Key,Info> :: preOrder(Node *p) const
{
    if(p != nullptr) 
    {
        cout<< p->info<<" ";
        preOrder(p->llink);
        preOrder(p->rlink);
    }   
}

template<typename Key,typename Info> 
void Dictionary<Key,Info> :: postOrder(Node *p) const
{
    if(p != nullptr) 
    {
        postOrder(p->llink);
        postOrder(p->rlink);
        cout<< p->info<<" ";
    }   
}

template <typename Key,typename Info>
int Dictionary<Key,Info> :: height(Node* p) const 
{
    if(p == nullptr) {
        return 0;
    }
    else return 1 + max(height(p->llink) , height(p->rlink));
}

template <typename Key,typename Info>
void Dictionary<Key,Info> :: rotateLeft(Node* &p)
{
    Node* help;
    if(p == nullptr) cerr<<"Node non-existant"<<endl;
    else if(p->rlink == nullptr) cerr<<"No right subtree to rotate"<<endl;
    else 
    {
        help = p->rlink;
        p->rlink = help->llink;
        help->llink = p;
        p = help;
    }
}

template <typename Key,typename Info>
void Dictionary<Key,Info> :: rotateRight(Node* &p)
{
    Node* help;
    if(p == nullptr) cerr<<"Node non-existant"<<endl;
    else if(p->llink == nullptr) cerr<<"No left subtree to rotate"<<endl;
    else 
    {
        help = p->llink;
        p->llink = help->rlink;
        help->rlink = p;
        p = help;
    }
}

template <typename Key,typename Info>
void Dictionary<Key,Info> :: balanceLeft(Node* &p)
{
    Node* h1;
    Node* h2;

    h1 = p->llink;
    switch(h1->bfactor)
    {
        case -1:
            p->bfactor = 0;
            h1->bfactor = 0;
            rotateRight(p);
            break;
        case 0:
            cerr<<"cannot balance from left."<<endl;
            break;
        case 1:
            h2 = h1->rlink;
            switch(h2->bfactor)
            {
                case -1:
                    p->bfactor = 1;
                    h1->bfactor = 0;     
                    break;
                case 0:
                    p->bfactor = 0;
                    h1->bfactor = 0;
                    break;
                case 1:
                    p->bfactor = 0;
                    h1->bfactor = -1;
            }
        h2->bfactor = 0;
        rotateLeft(h1);
        p->llink = h1;
        rotateRight(p);
    }
}

template <typename Key,typename Info>
void Dictionary<Key,Info> :: balanceRight(Node* &p)
{
    Node* h1;
    Node* h2;
    h1 = p->rlink;
    switch(h1->bfactor)
    {
        case -1:
            h2 = h1->llink;
            switch(h2->bfactor)
            {
                case -1:
                    p->bfactor = 0;
                    h1->bfactor = 1;
                    break;
                case 0:
                    p->bfactor = 0;
                    h1->bfactor = 0;
                    break;
                case 1:
                    p->bfactor = -1;
                    h1->bfactor = 0;   
            }
            h2->bfactor = 0;
            rotateRight(h1);
            p->rlink = h1;
            rotateLeft(p);
            break;
        case 0:
            cerr<<"can't balance from left"<<endl;
            break;
        case 1:
            p->bfactor = 0;
            h1->bfactor = 0;
            rotateLeft(p);
    }
}

template<typename Key,typename Info>
void Dictionary<Key,Info> :: insertNode(Node* &p, Node* newNode, bool isTaller) 
{
    if(p == NULL)
    {
        p = newNode;
        isTaller = true;
    }
    else if(p->info == newNode->info) cerr<<"It's already in the dictionary."<<endl;
    else if(p->key == newNode->key) cerr<<"Keys must be unique."<<endl;
    else if(p->info > newNode->info) 
    {
        insertNode(p->llink, newNode, isTaller);
        if(isTaller)
        {
            switch(p->bfactor)
            {
                case -1:
                    balanceLeft(p);
                    isTaller = false;
                    break;
                case 0:
                    p->bfactor = -1;
                    isTaller = true;
                    break;
                case 1:
                    p->bfactor = 0;
                    isTaller = false;
            }
        }
    }
    else 
    {
        insertNode(p->rlink, newNode, isTaller);
        if(isTaller)
        {
            switch(p->bfactor)
            {
                case -1:
                    p->bfactor = 0;
                    isTaller = false;
                    break;
                case 0:
                    p->bfactor = 1;
                    isTaller = true;
                    break;
                case 1:
                    balanceRight(p);
                    isTaller = false;
            }
        }
    }
}

