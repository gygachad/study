#include "join_server.h"
#include "str_tool.h"

/*Join server dispatcher*/
int join_server_dispatcher::convert_id(const string& id_str)
{
	int id = -1;

	try
	{
		id = stoi(id_str);
	}
	catch (exception& e)
	{
		cout << e.what();
	}

	return id;
}

string join_server_dispatcher::format_answer(const deque<tuple<int, array<string, 2>>>& values)
{
	string answer = "";

	for (auto res_v : values)
	{
		int id;
		array<string, 2> v;
		tie(id, v) = res_v;

		answer += to_string(id) + "," + v[0] + "," + v[1] + "\r\n";
	}

	return answer;
}
/*************************************/

/*join server insert dispacher*/
string join_server_insert::execute(	const vector<string>& cmd_args, 
									table_map& map)
{
	table_ptr_type selected_table;

	{	//lock table with pointer's
		write_lock lock(map.m_table_map_lock);
		if (map.m_table_map.find(cmd_args[1]) == map.m_table_map.end())
		{
			selected_table = make_shared<table>();
			map.m_table_map[cmd_args[1]] = selected_table;
		}
		else
		{
			selected_table = map.m_table_map[cmd_args[1]];
		}
	}	//unlock table with pointer's

	{	//lock selected table
		write_lock lock(selected_table->m_values_lock);
		int id = convert_id(cmd_args[2]);

		if (id == -1)
			return "ERR invalid id";

		if (selected_table->m_values.find(id) != selected_table->m_values.end())
			return "ERR duplicate " + cmd_args[2];

		selected_table->m_values[id] = cmd_args[3];
	}	//unlock selected table

	return "OK";
}
/*************************************/

/*join server truncate dispacher*/
string join_server_truncate::execute(	const vector<string>& cmd_args, 
										table_map& map)
{
	table_ptr_type selected_table;

	{	//lock table with pointer's
		read_lock lock(map.m_table_map_lock);

		if (map.m_table_map.find(cmd_args[1]) == map.m_table_map.end())
			return "ERR table not found";

		selected_table = map.m_table_map[cmd_args[1]];
	}	//unlock table with pointer's

	{	//lock selected table
		write_lock lock(selected_table->m_values_lock);
		selected_table->m_values.clear();
	}	//unlock selected table

	return "OK";
}
/*************************************/

/*join server intersection dispatcher*/
string join_server_intersection::execute(	const vector<string>&, 
											table_map& map)
{
	table_ptr_type tableA;
	table_ptr_type tableB;

	{	//lock table with pointer's
		read_lock lock(map.m_table_map_lock);

		if (map.m_table_map.find("A") == map.m_table_map.end() ||
			map.m_table_map.find("B") == map.m_table_map.end())
			return "ERR table not found";

		tableA = map.m_table_map["A"];
		tableB = map.m_table_map["B"];
	}	//unlock table with pointer's

	deque<tuple<int, array<string, 2>>> result;

	{
		//Dead lock????
		read_lock lockA(tableA->m_values_lock);
		read_lock lockB(tableB->m_values_lock);

		for (auto v : tableA->m_values)
		{
			int id;
			string valueA;
			tie(id, valueA) = v;

			if (tableB->m_values.find(id) != tableB->m_values.end())
			{
				array<string, 2> arr = { valueA, tableB->m_values[id] };
				result.push_back(make_tuple(id, arr));
			}
		}
	}

	return format_answer(result) + "OK";
}
/*************************************/

/*join server symmetric differnce dispatcher*/
string join_server_symmetric_difference::execute(	const vector<string>&, 
													table_map& map)
{
	table_ptr_type tableA;
	table_ptr_type tableB;

	{	//lock table with pointer's
		read_lock lock(map.m_table_map_lock);

		if (map.m_table_map.find("A") == map.m_table_map.end() ||
			map.m_table_map.find("B") == map.m_table_map.end())
			return "ERR table not found";

		tableA = map.m_table_map["A"];
		tableB = map.m_table_map["B"];
	}	//unlock table with pointer's

	deque<tuple<int, array<string, 2>>> result;

	{
		//Dead lock????
		read_lock lockA(tableA->m_values_lock);
		read_lock lockB(tableB->m_values_lock);

		for (auto v : tableA->m_values)
		{
			int id;
			string valueA;
			tie(id, valueA) = v;

			if (tableB->m_values.find(id) == tableB->m_values.end())
			{
				array<string, 2> arr = { valueA, "" };
				result.push_back(make_tuple(id, arr));
			}
		}

		for (auto v : tableB->m_values)
		{
			int id;
			string valueB;
			tie(id, valueB) = v;

			if (tableA->m_values.find(id) == tableA->m_values.end())
			{
				array<string, 2> arr = { "", valueB };
				result.push_back(make_tuple(id, arr));
			}
		}
	}

	return format_answer(result) + "OK";
}

/*main dispatcher*/
string join_server::execute(string& cmd_line)
{
	string answer = "ERR unsupported command";
	vector<string> splitted_cmd;

	splitted_cmd = str_tool::split(cmd_line, m_cmd_del);

	if (splitted_cmd.size() < 1)
		return answer;

	if (m_dispatchers_table.find(splitted_cmd[0]) == m_dispatchers_table.end())
		return answer;

	//execute this cmd for table map
	size_t arg_count;
	join_server::dispatch_ptr_type dispatcher;

	//get dispatcher for command
	tie(arg_count, dispatcher) = m_dispatchers_table[splitted_cmd[0]];

	if (splitted_cmd.size() != arg_count)
		return "ERR invalid args";

	//execute command with dispatcher
	answer = dispatcher->execute(splitted_cmd, m_table_map) + "\r\n";

	return answer;
}
/*************************************/