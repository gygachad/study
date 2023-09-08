#include <regex>

#include "ip_addr.h"
#include "ip_pool.h"

#include "str_tool.h"

bool ip_addr::parse(string str_addr)
{
    bool parsed = true;

    //this->str_octets.clear();
    this->str_addr = "";

    //Minimal check
    //3 groups of xxx.
    //1 group of xxx
    //xxx - from 1 to 3 digits
    /*
    regex ip_re("(\\d{1,3}\\.){3}\\d{1,3}");
    if (!regex_match(str_addr, ip_re))
        return false;
    */
    vector<string> splitted_ip = str_tool::split(str_addr, '.');

    if (splitted_ip.size() != 4)
        return false;

    for (size_t i = 0; i < splitted_ip.size(); i++)
    {
        unsigned long octect = stoul(splitted_ip[i]);
        if (0 <= octect && octect <= 255)
        {
            this->octets[i] = (unsigned char)octect;
        }
        else
        {
            parsed = false;
            break;
        }

        string str_octet = to_string(this->octets[i]);

        this->str_addr += str_octet;
        //this->str_octets.push_back(str_octet);

        if (i != 3)//We don't need dot after last octet (a.b.c.d.)
            this->str_addr += ".";
    }

    if (!parsed)
    {
        //this->str_octets.clear();
        this->str_addr = "";
    }

    return parsed;
}

bool operator == (const ip_addr& a, const ip_addr& b)
{
    for (size_t i = 0; i < 4; i++)
    {
        if (a.octets[i] != b.octets[i])
            return false;
    }

    return true;
}

bool operator < (const ip_addr& a, const ip_addr& b)
{
    for (size_t i = 0; i < 4; i++)
        if (a.octets[i] != b.octets[i])
            return a.octets[i] < b.octets[i];

    //a == b
    return false;
}

bool ip_addr::match(const string& mask)
{
    bool matched = true;

    //Minimal check
    //3 groups of xxx. (or *)
    //1 group of xxx (or *)
    //xxx - from 1 to 3 digits
    /*
    regex mask_re("((\\d{1,3}|\\*)\\.){3}(\\d{1,3}|\\*)");
    if (!regex_match(mask, mask_re))
        return false;
    */ 
    vector<string> splitted_mask = str_tool::split(mask, '.');

    if (splitted_mask.size() != 4)
        return false;

    for(int i = 0; i < 4; i++)
    {
        //* - means any. skip octet on this position
        if(splitted_mask[i] != "*")
        {
            //if any of all 4 octet's not match
            //if (splitted_mask[i] != this->str_octets[i])
            if (stoul(splitted_mask[i]) != this->octets[i])
            {
                //break it
                matched = false;
                break;
            }
        }
    }

    return matched;
}