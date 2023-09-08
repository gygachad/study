#include <iostream>
#include <chrono>

#include "gtest_ipfilter.h"
#include "ip_pool.h"
#include "ip_addr.h"

#include "version.h"
#include "str_tool.h"

//#define WITH_CHRONO_TEST

#ifdef WITH_CHRONO_TEST
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
#endif

int main() 
{

#ifdef WITH_CHRONO_TEST
    auto t1 = high_resolution_clock::now();
#endif

	//std::cout << "ip_filter version: " << version() << std::endl;
    try
    {
        ip_pool filter = ip_pool();

        for (string line; getline(std::cin, line);)
        {
            vector<string> v = str_tool::split(line, '\t');

            ip_addr ip = ip_addr();

            if (ip.parse(v.at(0)))
                filter.push_back(ip);
            else
                throw exception();
        }

        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8
        filter.reverse_sort();

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8
        ip_pool filtered_by_first;
        filtered_by_first.push_back(filter.filter("1.*.*.*"));

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        ip_pool filtered_by_first_and_second;
        filtered_by_first_and_second.push_back(filter.filter("46.70.*.*"));

        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
        ip_pool filtered_by_any;
        filtered_by_any.push_back(filter.filter_any("46"));

        cout << filter.to_string();
        cout << filtered_by_first.to_string();
        cout << filtered_by_first_and_second.to_string();
        cout << filtered_by_any.to_string();
    }
    catch (const exception& e)
    {
        cerr << e.what() << endl;
    }

#ifdef WITH_CHRONO_TEST
    auto t2 = high_resolution_clock::now();

    /* Getting number of milliseconds as an integer. */
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    std::cout << ms_int.count() << "ms\n";
#endif

    return 0;
}
