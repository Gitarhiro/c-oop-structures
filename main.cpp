#include <iostream>
#include <algorithm>
#include <string>
#include "VLHtree.h"

using namespace std;


int main() {
    std::cout << "Hello, world!\n";
    Dictionary<int,string> a;
    a.insert(1 , string("Szymon"));
    a.insert(2 , string("Igor"));
    a.insert(3, string("Krzysztof"));

    a.inOrderTraversal();
}
