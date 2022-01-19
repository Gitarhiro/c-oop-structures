#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include "VLHtree.h"

using namespace std;


int main() {
    Dictionary<int,string> a;
    a.insert(1 , string("Szymon"));
    a.insert(1 , string("Szymon")); 
    a.insert(1 , string("Krzychu")); 
    // PROPERLY DETECTS THE SAME NODES AND REACCURING KEYS
    a.insert(2 , string("Igor"));
    a.insert(3, string("Krzysztof"));
    a.insert(10,string("Zyzz"));
    a.insert(4, string("Kamil"));
    a.insert(5, string("Anna"));
    a.insert(6, string("Ania"));
    a.insert(7, string("Bania"));
    a.insert(8, string("Rafał"));
    cout<<endl;
    a.inOrderTraversal();
    cout<<endl;

    a.deleteFromDic(string("Damian")); //NO SUCH WORD IN DICTIONARY
    cout<<endl;
    a.deleteFromDic(string("Krzysztof")); // PROPER DELETION OF NODE WITHOUT CHILDREN
    a.deleteFromDic(string("Anna"));    //PROPER DELETION OF NODE WITH TWO CHILDREN
    a.inOrderTraversal();
    cout<<endl;

    Dictionary<int, int> b;
    b.insert(1,10);
    b.insert(2,9);
    b.insert(3,11);
    b.insert(4,12);

    cout<<endl;
    b.inOrderTraversal(); 
    cout << endl;
    b.deleteFromDic(9);
    cout<<endl;
    b.inOrderTraversal(); 
    cout<<endl; 
    //ANOTHER INSTANCE OF PROPER DELETION WITH ONE CHILD
    b.destroyTree();
    b.inOrderTraversal();
    //DESTROYING TREE SUCCESSFULL
    
    Dictionary<int,string> c(a);
    c.inOrderTraversal();
    c.destroyTree();
    cout<<endl;
    c = a;
    c.inOrderTraversal(); 
    //COPY CONSTRUCTOR, ASSIGNMENT OPERATOR ALL WORKING PROPERLY
    Dictionary<string,int> d;
    read_file(string("tekst.txt"),d);
    d.inOrderTraversal();
}
