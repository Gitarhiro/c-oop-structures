#include <iostream>
#include <algorithm>
#include <fstream>
#include <string.h>

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

    void copyDic(Node* &cpy,
                   Node* copied); //copies 
    
    void destroy(Node* &p); //destroys all nodes below node p, p included
    void deleteNode(Node* &rootNode, Node* &p, bool& shorter);

    void inOrder(Node* p) const;

    void rotateLeft(Node* &p);
    void rotateRight(Node* &p);

    void balanceLeft(Node* &p);
    void balanceRight(Node* &p);

    void insertNode(Node* &p, Node* newNode, bool& isTaller);

public:
    const Dictionary<Key,Info>& operator= 
        (const Dictionary<Key,Info>& copied); //overload operator

    bool isEmpty() const;   //true if dictionary is empty
    void inOrderTraversal() const; //outputs the dictionary in order

    bool searchKey(const Key& to_find);

    void destroyDic(); //destroys Dictionary, not a destructor
    void deleteFromDic(const Key& to_delete); // delete a particular word from the dictionary

    void insert(const Key& k, const Info& f); 

    Dictionary(const Dictionary<Key,Info>& copied); //copy constructor
    Dictionary();
    ~Dictionary();
}; 

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
    if(!root)
    {
        cout << "TREE IS EMPTY" << endl;
    }
    inOrder(root);
}

template<typename Key, typename Info>
void Dictionary<Key,Info> :: destroyDic()
{
    return destroy(root);
}

template<typename Key,typename Info>
void Dictionary<Key,Info> :: deleteFromDic(const Key& to_delete) 
{
    if (root == NULL) cout << "The tree is empty" << endl;
    else
    {
        bool find = false;
        Node* temp = root;
        while(temp != nullptr && !find)
        {
            if(temp->key == to_delete) find = true;
            else if(temp->key > to_delete) temp = temp->llink;
            else temp = temp->rlink;
        }
        if(temp == nullptr) cout<<"THERE IS NO SUCH NODE IN DICTIONARY"<<endl;
        else if (find) 
        {
            bool shorter = false;
            deleteNode(root, temp, shorter);
        }
        
    }
}

template<typename Key,typename Info>
bool Dictionary<Key,Info> :: searchKey(const Key& to_find)
{
    if (root == NULL)
    {
        cout << "The tree is empty" << endl;
        return false;
    } 
    else
    {
        bool find = false;
        Node* temp = root;
        while(temp != nullptr && !find)
        {
            if(temp->key == to_find) find = true;
            else if(temp->key > to_find) temp = temp->llink;
            else temp = temp->rlink;
        }
        return find;
    }
}


template<typename Key,typename Info>
void Dictionary<Key,Info> :: insert(const Key& k, const Info& f)
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
    else copyDic(root , copied.root);
}

template<typename Key, typename Info>
Dictionary<Key,Info> :: Dictionary() 
{
    root = NULL;
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
void Dictionary<Key,Info> :: copyDic(Node* &cpy,
                                        Node* copied) 
{
    if(copied == nullptr) cpy = NULL;
    else 
    {
        cpy = new Node;
        cpy->info = copied->info;
        cpy->key = copied->key;
        cpy->bfactor = copied->bfactor;
        copyDic(cpy->llink , copied->llink);
        copyDic(cpy->rlink , copied->rlink); 
    }
}

template<typename Key,typename Info>
void Dictionary<Key,Info> :: destroy(Node* &p) 
{
    if(p!=NULL) 
    {
        destroy(p->llink);
        destroy(p->rlink);
        p = NULL;
    }
}
//NODE FIND PARENT
template<typename Key,typename Info>
void Dictionary<Key,Info> :: deleteNode(Node* &rootNode, Node* &p, bool& shorter)
{
    if(p == NULL) cerr<<"NODE P IS NULL" << endl;
    else if(rootNode->key > p->key)
    {
        deleteNode(rootNode->llink, p, shorter);
        if(shorter)
        {
            switch(rootNode->bfactor)
            {
                case -1:
                    rootNode->bfactor = 0;
                    shorter = true;
                    break;
                case 0:
                    rootNode->bfactor = 1;
                    shorter = false;
                    break;
                case 1:
                    switch(rootNode->rlink->bfactor)
                    {
                        case -1:
                            balanceRight(rootNode);
                            shorter = true;
                            break;
                        case 0:
                            rotateLeft(rootNode);
                            shorter = false;
                            break;
                        case 1:
                            rotateLeft(rootNode);
                            shorter = true; 
                    }
            }
            
        }     
    }
    else if(rootNode->key < p->key)
    {
        deleteNode(rootNode->rlink, p, shorter);
        if(shorter)
        {
            switch(rootNode->bfactor)
            {
                case -1:
                    switch(rootNode->rlink->bfactor)
                        {
                            case -1:
                            rotateRight(rootNode);
                            shorter = true;
                            break;
                        case 0:
                            rotateRight(rootNode);
                            shorter = false;
                            break;
                        case 1:
                            balanceLeft(rootNode);
                            shorter = true;
                        }
                    break;
                case 0:
                    rootNode->bfactor = -1;
                    shorter = false;
                    break;
                case 1:
                    rootNode->bfactor = 0;
                    shorter = true;
            }   
        }
    }
    else 
    {
        Node* temp;
        if(rootNode->rlink == NULL && rootNode->llink == NULL)
        {
            shorter = true;
            temp = rootNode;
            rootNode = NULL;
            delete temp;
        }
        else if(rootNode->rlink == NULL)
        {
            temp = rootNode;
            rootNode = rootNode->llink;
            temp = NULL;
            shorter = true;
        }
        else if(rootNode->llink == NULL)
        {
            temp = rootNode;
            rootNode = rootNode->rlink;
            temp = NULL;
            shorter = true;
        }
        else 
        {
            temp = rootNode->rlink;
            while(temp->llink != nullptr)
            {
                temp = temp->llink;
            }
            Info help = temp->info;
            Key help1 = temp->key;
            deleteNode(rootNode, temp, shorter);
            rootNode->info = help;
            rootNode->key = help1;
        }
    }
}

template<typename Key,typename Info> 
void Dictionary<Key,Info> :: inOrder(Node *p) const
{
    if(p != NULL) 
    {
        inOrder(p->llink);
        cout<< p->key<<" "<<p->info<<" ";
        inOrder(p->rlink);
    }   
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
void Dictionary<Key,Info> :: insertNode(Node* &p, Node* newNode, bool& isTaller) 
{
    if(p == NULL)
    {
        p = newNode;
        isTaller = true;
    }
    else if(p->key == newNode->key) cerr<<"Keys must be unique."<<endl;
    else if(p->key > newNode->key) 
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

void read_file(string file_name, Dictionary<string,int>& a) 
{   
    string h1, iter, h2;
    int count = 0;
    ifstream myfile;
    myfile.open(file_name);
    size_t pos=0;
    if(!myfile.is_open())     
    {
        cerr << "There is no such file!"<<endl;
        return;
    }
    if(!a.isEmpty())
    {
        a.destroyDic();
    }
    while(!myfile.eof())
    {
        myfile >> h2;
        iter+=h2;
        iter+="\n";
        h2.clear();
    }
    for(int i=0;i<iter.size()+1;i++)
        {
            if((iter[i]>90 && iter[i]<97) || iter[i]<65 || iter[i]>122)
            {   
                if(iter[i-1] == '\n')
                {
                    iter.erase(i,1);
                }
                else iter[i] = '\n';
            }
        }
    while(!iter.empty())
    {
        pos = iter.find_first_of('\n');
        h1 = iter.substr(0 , pos);
        while(pos!=iter.npos)
        {
            if(h1=="\n" || h1 =="")
            {
                iter.erase(pos, 1);
                break;
            }
            pos = iter.find(h1,0);
            if(pos!=iter.npos) 
            {
                iter.erase(pos, h1.length()+1);
                count++;
                pos = 0;
            }
            else break;
        }
        if(h1 != "" && h1!="\n") a.insert(h1 , count);
        count = 0;
        pos = 3;
    }
    myfile.close();
}
