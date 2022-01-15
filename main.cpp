#include <iostream>
#include <algorithm>
#include <string>
#include "VLHtree.h"

using namespace std;


int main() {
    Dictionary<int,string> a;
    a.insert(1 , string("Szymon"));
    a.insert(2 , string("Igor"));
    a.insert(3, string("Krzysztof"));

    a.inOrderTraversal();
    cout<<endl;
    Dictionary<string, int> b;
    read_file(string("tekst") , b);
}
