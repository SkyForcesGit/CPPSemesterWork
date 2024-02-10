#include <iostream>
#include "orderhashtable.hpp"

using DataStructures::List;
using DataStructures::OrderedHashTable;

int main() {
    // Примеры использования.
    OrderedHashTable<OrderedHashTable<int>> ht;
    OrderedHashTable<int> temp;
    temp.insert("Count", 3);

    temp.insert("WinRate", 50);
    temp.insert("Test", -507);
    temp.insert("Sun", 643);
    temp.insert("Moon", 12);
    temp.insert("House", 96);
    temp.insert("Town", -1234);
    temp.insert("Window", 0);

    temp.erase("House");
    temp["Town"] = 45;
    std::cout << temp.pop() << std::endl;

    ht.insert("Test", temp);

    for (auto it{ ht["Test"].keys()->begin() };
             it != ht["Test"].keys()->end(); it++)
        std::cout << *it << " : " << ht["Test"][*it] << std::endl;

    return 0;
}