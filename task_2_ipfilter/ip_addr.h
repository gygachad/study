#pragma once

#include <string>
#include <vector>

using namespace std;

class ip_addr
{
public:
	ip_addr() = default;
	~ip_addr() = default;

	bool parse(string str_addr);
	bool match(const string& mask);

	friend bool operator== (const ip_addr& a, const ip_addr& b);
	friend bool operator!= (const ip_addr& a, const ip_addr& b) { return !(b == a); };

	friend bool operator< (const ip_addr& a, const ip_addr& b);
	friend bool operator> (const ip_addr& a, const ip_addr& b) { return b < a; }
	friend bool operator<=(const ip_addr& a, const ip_addr& b) { return !(a > b); }
	friend bool operator>=(const ip_addr& a, const ip_addr& b) { return !(a < b); }

	//ip splitted by octets
	unsigned char octets[4];
	//vector<string> str_octets;

	//full string with ip
	string str_addr;
};