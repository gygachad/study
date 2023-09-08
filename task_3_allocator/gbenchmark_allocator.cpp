#include <string>
#include <forward_list>
#include <benchmark/benchmark.h>

#include "allocator.h"
#include "single_linked_list.h"
#include "version.h"

#pragma comment ( lib, "Shlwapi.lib" )

using namespace std;

#define TEST_NUM_COUNT 0x1000

template<typename T>
void test_container(T vec)
{
    for (int i = 0; i < TEST_NUM_COUNT; i++)
        vec.push_back(to_string(i));

    for (int i = 0; i < TEST_NUM_COUNT; i++)
        vec[i] = vec[i] + "test";
}

template<typename T>
void test_map(T m)
{
    for (int i = 0; i < TEST_NUM_COUNT; i++)
        m[i] = to_string(i);

    for (int i = 0; i < TEST_NUM_COUNT; i++)
        m[i] = m[i] + "test";
}

void vector_with_allocator(benchmark::State& state) 
{
    for (auto _ : state)
    {
        vector<string, linear_allocator<string>> v;
        test_container<vector<string, linear_allocator<string>>>(v);       
    }
}
BENCHMARK(vector_with_allocator);

void vector_default_allocator(benchmark::State& state)
{
    for (auto _ : state)
    {
        vector<string> v;
        test_container<vector<string>>(v);
    }
}
BENCHMARK(vector_default_allocator);

void map_default_allocator(benchmark::State& state)
{
    for (auto _ : state)
    {
        using map_alloc = map<int, string>;
        map_alloc m;
        test_map<map_alloc>(m);
    }
}
BENCHMARK(map_default_allocator);

void map_with_allocator(benchmark::State& state) 
{
    for (auto _ : state)
    {
        using map_alloc = map<int, string, less<int>, linear_allocator<pair<const int, string>>>;
        map_alloc m;

        test_map<map_alloc>(m);
    }
}
BENCHMARK(map_with_allocator);

void linear_list_with_allocator(benchmark::State& state)
{
    for (auto _ : state)
    {
        single_linked_container<string, linear_allocator<string>> d;

        test_container<single_linked_container<string, linear_allocator<string>>>(d);
    }
}
BENCHMARK(linear_list_with_allocator);

void linear_list_default_allocator(benchmark::State& state)
{
    for (auto _ : state)
    {
        single_linked_container<string> d;

        test_container<single_linked_container<string>>(d);
    }
}
BENCHMARK(linear_list_default_allocator);

BENCHMARK_MAIN();