#pragma once

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "ip_addr.h"

class ip_pool 
{
public:
	ip_pool() = default;
	~ip_pool() = default;
	
	static vector<std::string> split(const string& str, char d);

	void push_back(const ip_addr& ip);
	void push_back(const vector<ip_addr>& ip_pool);
	void reverse_sort();
	vector<ip_addr> filter(const string& mask);
	vector<ip_addr> filter_any(const string& mask);

	string to_string();
private:
	vector<ip_addr> ip_addr_pool;
};