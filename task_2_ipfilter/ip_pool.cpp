#include "ip_pool.h"

using namespace std;

void ip_pool::push_back(const ip_addr& ip)
{
    this->ip_addr_pool.push_back(ip);
}

void ip_pool::push_back(const vector<ip_addr>& ip_pool)
{
    for (const auto& ip : ip_pool)
        this->push_back(ip);
}

void ip_pool::reverse_sort()
{
    sort(this->ip_addr_pool.begin(), this->ip_addr_pool.end(), [](ip_addr a, ip_addr b) {return a > b; });
}

vector<ip_addr> ip_pool::filter(const string& mask)
{
    vector<ip_addr> filtered;

    for (auto& ip : this->ip_addr_pool)
    {
        if(ip.match(mask))
            filtered.push_back(ip);
    }

    return filtered;
}

vector<ip_addr> ip_pool::filter_any(const string& octet)
{
    vector<ip_addr> filtered;

    for (const auto& ip : this->ip_addr_pool)
    {
        //for(const auto& ip_octet : ip.str_octets)
        for(int i = 0; i < 4; i++)
        {
            if (ip.octets[i] == stoul(octet))
            {
                //i'ts enough for our filter condition
                filtered.push_back(ip);
                //go to check next ip_addr
                break;
            }
        }
    }

    return filtered;
}

string ip_pool::to_string()
{
    string out = "";

    for (const auto& ip : this->ip_addr_pool)
    {
        out += ip.str_addr;
        out += "\n";
    }

    return out;
}