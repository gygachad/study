#include "str_tool.h" 

namespace str_tool
{
	// ("",  '.') -> [""]
	// ("11", '.') -> ["11"]
	// ("..", '.') -> ["", "", ""]
	// ("11.", '.') -> ["11", ""]
	// (".11", '.') -> ["", "11"]
	// ("11.22", '.') -> ["11", "22"]
	vector<string> split(string& str, string_view delimiter)
	{
		vector<string> result;

		size_t pos = 0;
		string token;
		while ((pos = str.find(delimiter)) != string::npos)
		{
			token = str.substr(0, pos);
			result.push_back(token);
			str.erase(0, pos + delimiter.length());
		}

		result.push_back(str);

		return result;
	}

	size_t replace_all(string& inout, string_view what, string_view with)
	{
		size_t count{};
		for (string::size_type pos{};
			inout.npos != (pos = inout.find(what.data(), pos, what.length()));
			pos += with.length(), ++count)
		{
			inout.replace(pos, what.length(), with.data(), with.length());
		}
		return count;
	}
}