#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <shared_mutex>
#include <array>
#include <deque>

#include "asio_server.h"

using namespace std;

using write_lock = unique_lock<shared_mutex>;
using read_lock = shared_lock<shared_mutex>;

struct table
{
	unordered_map<int, string> m_values;
	shared_mutex m_values_lock;
};

using table_ptr_type = shared_ptr<table>;

struct table_map
{
	unordered_map<string, table_ptr_type> m_table_map;
	shared_mutex m_table_map_lock;
};

class join_server_dispatcher
{
public:
	virtual string execute(const vector<string>& cmd_args, table_map& map) = 0;

	int convert_id(const string& id_str);
	string format_answer(const deque<tuple<int, array<string, 2>>>& values);
};

class join_server_insert : public join_server_dispatcher
{
	string execute(const vector<string>& cmd_args, table_map& map);
};

class join_server_truncate : public join_server_dispatcher
{
	string execute(const vector<string>& cmd_args, table_map& map);
};

class join_server_intersection : public join_server_dispatcher
{
	string execute(const vector<string>& cmd_args, table_map& map);
};

class join_server_symmetric_difference : public join_server_dispatcher
{
	string execute(const vector<string>& cmd_args, table_map& map);
};

class join_server : public ICmd_dispatcher
{
	table_map m_table_map;
	using dispatch_ptr_type = shared_ptr<join_server_dispatcher>;
	
	//m_dispatchers_table["COMMAND_DISPATCH"] = tuple<arg_count, ptr_to_dispatcher>
	unordered_map<string, tuple<size_t, dispatch_ptr_type>> m_dispatchers_table =
	{
	{"INSERT", make_tuple<size_t, dispatch_ptr_type>(4, make_shared<join_server_insert>())},
	{"TRUNCATE", make_tuple<size_t, dispatch_ptr_type>(2, make_shared<join_server_truncate>())},
	{"INTERSECTION", make_tuple<size_t, dispatch_ptr_type>(1, make_shared<join_server_intersection>())},
	{"SYMMETRIC_DIFFERENCE", make_tuple<size_t, dispatch_ptr_type>(1, make_shared<join_server_symmetric_difference>())}
	};

	string m_cmd_del = " ";

public:
	string execute(string& cmd_line);
};