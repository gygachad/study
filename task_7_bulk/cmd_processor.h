#pragma once
#include <vector>
#include <deque>
#include <chrono>
#include <iostream>
#include <fstream>

using namespace std;
using namespace std::chrono;

class cmd
{
    system_clock::time_point m_create_time;
    string body;

public:
    cmd(string cmd)
    {
        m_create_time = system_clock::now();
        body = cmd;
    }

    string execute()
    {
        return body;
    }

    uint64_t get_create_time()
    {
        auto time_point_ms = time_point_cast<milliseconds>(m_create_time);
        uint64_t value_ms = duration_cast<milliseconds>(time_point_ms.time_since_epoch()).count();
        return value_ms;
    }
};

template<template <typename> class ContT>
class cmd_block
{
    ContT<cmd> m_queue;

public:
    bool empty() { return m_queue.size() == 0; }
    size_t size() {return m_queue.size();}

    void reserve(size_t N) { m_queue.reserve(N); }

    void add(cmd command)
    {
        m_queue.push_back(command);
    }

    void bulk()
    {
        if (m_queue.empty())
            return;

        //Create bulk line for output
        string bulk_str = "bulk: ";

        for (auto it = m_queue.begin(); it != m_queue.end(); it++)
        {
            if (it != m_queue.begin())
                bulk_str += ", ";

            bulk_str += it->execute();// +", ";
        }

        bulk_str += "\r\n";
        cout << bulk_str;

        //Create log file
        uint64_t create_time = m_queue[0].get_create_time();
        string file_name = "bulk" + std::to_string(create_time) + ".log";

        ofstream fout(file_name);
        fout << bulk_str;
        fout.close();

        m_queue.clear();
    }
};

class cmd_processor
{
    //Static block
    cmd_block<vector> m_sblock;

    //Dynamic block
    cmd_block<deque> m_dblock;

    size_t dynamic_mode = 0;
    size_t m_N = 0;

public:
    cmd_processor(size_t N) 
    { 
        m_N = N;
        m_sblock.reserve(N); 
    }

    bool process_spec_ops(string cmd_body)
    {
        bool bspec_ops = false;

        if (cmd_body == "{")
        {
            m_sblock.bulk();
            dynamic_mode++;
            bspec_ops = true;
        }

        if (cmd_body == "}")
        {
            if (dynamic_mode)
            {
                if (dynamic_mode == 1)
                    m_dblock.bulk();

                dynamic_mode--;
            }

            bspec_ops = true;
        }

        return bspec_ops;
    }

    void add_cmd(string cmd_body)
    {
        if (!process_spec_ops(cmd_body))
        {
            //This is not spec ops cmd
            if (dynamic_mode)
            {
                cmd new_cmd = cmd(cmd_body);
                m_dblock.add(new_cmd);
            }
            else
            {
                cmd new_cmd = cmd(cmd_body);
                m_sblock.add(new_cmd);

                if (m_sblock.size() == m_N)
                    m_sblock.bulk();
            }
        }
    }
};