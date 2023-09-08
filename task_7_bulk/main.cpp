// task_7_bulk.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>

#include "cmd_processor.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cout << "Usage: bulk.exe N" << endl;
        return 0;
    }

    string static_size(argv[1]);

    stringstream sstream(static_size);
    size_t N;
    sstream >> N;

    if (N <= 0)
    {
        cout << "N must be greater than 0" << endl;
        return 0;
    }

    cmd_processor cmd_proc(N);
    
    string cmd;

    while (getline(cin, cmd))
    {
        cmd_proc.add_cmd(cmd);
    }

    return 0;
}