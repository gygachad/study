#include "str_tool.h"

namespace str_tool
{
    // ("",  '.') -> [""]
    // ("11", '.') -> ["11"]
    // ("..", '.') -> ["", "", ""]
    // ("11.", '.') -> ["11", ""]
    // (".11", '.') -> ["", "11"]
    // ("11.22", '.') -> ["11", "22"]
    vector<std::string> split(const string& str, char d)
    {
        vector<std::string> r;

        string::size_type start = 0;
        string::size_type stop = str.find_first_of(d);
        while (stop != string::npos)
        {
            r.push_back(str.substr(start, stop - start));

            start = stop + 1;
            stop = str.find_first_of(d, start);
        }

        r.push_back(str.substr(start));

        return r;
    }
}