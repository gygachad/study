#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "allocator.h"
#include "single_linked_list.h"

int factorial(unsigned int n)
{
    if (n == 0)
        return 1;
    return n * factorial(n - 1);
}


int main() 
{
    map<int, int, less<int>, linear_allocator<pair<const int, int>>> m1;
    map<int, int> m2;

    for (int i = 0; i < 10; i++)
    {
        m1[i] = factorial(i);
        m2[i] = factorial(i);
    }

    for (const auto& [key, value] : m1) 
        std::cout << key << " " << value << endl;

    for (const auto& [key, value] : m1)
        std::cout << key << " " << value << endl;
    
    single_linked_container<int> c1;
    for (int i = 0; i < 10; i++)
        c1.push_back(i);
    
    for (const auto& m : c1)
        cout << m << endl;

    single_linked_container<int, linear_allocator<int>> c2;
    for (int i = 0; i < 10; i++)
        c2.push_back(i);

    for (const auto& m : c2)
        cout << m << endl;
        

    single_linked_container<string> c3;
    for (int i = 0; i < 10; i++)
        c3.push_back("c3_string " + to_string(i));

    for (auto it = c3.begin(); it != c3.end(); it++)
    {
        cout << *it << endl;
    }

    single_linked_container<string, linear_allocator<string>> c4;
    for (int i = 0; i < 10; i++)
        c4.push_back("c4_string " + to_string(i));
    
    for (const auto& m : c4)
        cout << m << endl;

    return 0;
}
