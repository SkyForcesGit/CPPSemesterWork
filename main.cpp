#include <iostream>
#include "orderhashtable.hpp"

using DataStructures::List;
using DataStructures::OrderedHashTable;
using DataStructures::Record;

int main() {
    // Примеры использования.
    OrderedHashTable<OrderedHashTable<int>> ht;
    std::cout << sizeof(ht) << std::endl;
    List<int> example_1 = { 6, 7, 8, 1, 2, 3, 5, 5 };
    example_1.sort();
    for (const auto it : example_1)
        std::cout << it << "\n";
    OrderedHashTable<int> temp = { Record<int>{"Test", 1},
                                Record<int>{"Test2", 2} };
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

    for (auto it : *(ht["Test"].keys()))
        std::cout << it << " : " << ht["Test"][it] << std::endl;

    return 0;
}