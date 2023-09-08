#include <algorithm>
#include <iostream>
#include <list>
#include <vector>
#include <climits>
#include <tuple>

#include "debug.h"

/// <summary>
/// Print vector and list in ip mode
/// </summary>
/// <typeparam name="Container">The Type of the container.</typeparam>
/// <param name="container">Container for print.</param>
/// <returns>None.</returns>
template <
	template <typename, typename> typename Container,
	typename Type,
	typename Allocator = std::allocator<Type>,
	typename = std::enable_if_t<
		std::is_same_v<Container<Type, Allocator>, std::vector<Type, Allocator>> 
		||
		std::is_same_v<Container<Type, Allocator>, std::list<Type, Allocator>>>>
void print_ip(const Container<Type, Allocator> &container) 
{
	for (auto iter = std::begin(container); iter != std::end(container); ++iter) 
	{
		if (iter != std::begin(container))
			std::cout << ".";

		std::cout << *iter;
	}

	std::cout << std::endl;
}

/// <summary>
/// Print int*_t in ip mode
/// </summary>
/// <typeparam name="T">The Type of the int*. Can be int8_t, int16_t, int32_t and int64_t</typeparam>
/// <param name="int_obj">Integer for print.</param>
/// <returns>None.</returns>
template< 
	typename T,
	typename = std::enable_if_t <
		std::is_same_v<T, int8_t>	||
		std::is_same_v<T, int16_t>	||
		std::is_same_v<T, int32_t>	||
		std::is_same_v<T, int64_t>>>
void print_ip(const T& int_obj)
{
	for (size_t i = 0; i < sizeof(T); i += sizeof(int8_t))
	{
		if(i != 0)
			std::cout << ".";

		std::cout << ((int_obj >> ((sizeof(T) - i - 1) * CHAR_BIT)) & 0xff);
	}

	std::cout << std::endl;

	return;
}

/// <summary>
/// Print string as string
/// </summary>
/// <typeparam name="T">T is always string</typeparam>
/// <param name="str_obj">String for print.</param>
/// <returns>None.</returns>
template<
	typename T,
	typename = std::enable_if_t<
	std::is_same_v<T, std::string>>>
const std::string& print_ip(const T& str_obj)
{
	std::cout << str_obj << std::endl;

	return str_obj;
}

/// <summary>
/// Print tuple in ip mode. Print last element.
/// </summary>
/// <typeparam name="T">T is always tuple of Args</typeparam>
/// <typeparam name="I">Tuple elment position for print. Last position, I == sizeof...(Args) - 1</typeparam>
/// <param name="tuple_obj">Tuple for print.</param>
/// <returns>None.</returns>
template<
	size_t I = 0,
	template <typename...> typename T,
	typename... Args,
	typename = std::enable_if_t<
	std::is_same_v<T<Args...>, std::tuple<Args...>>
	&&
	I == sizeof...(Args) - 1// - 1 for last element
	>>
int print_ip(const T<Args...>& tuple_obj)
{
	std::cout << std::get<I>(tuple_obj) << std::endl;
	return 0;
}

/// <summary>
/// Print tuple in ip mode.
/// </summary>
/// <typeparam name="T">T is always tuple of Args</typeparam>
/// <typeparam name="I">Tuple elment position for print. I != sizeof...(Args) - 1</typeparam>
/// <param name="tuple_obj">Tuple for print.</param>
/// <returns>None.</returns>
template<
	size_t I = 0,
	template <typename...> typename T,
	typename... Args,
	typename = std::enable_if_t<
	std::is_same_v<T<Args...>, std::tuple<Args...>>
	&&
	I != sizeof...(Args) - 1
	>>
void print_ip(const T<Args...>& tuple_obj)
{
	std::cout << std::get<I>(tuple_obj) << ".";
	print_ip<I + 1>(tuple_obj);
	return;
}

int main() 
{
	
	print_ip(int8_t{ -1 }); // 255
	print_ip(int16_t{ 0 }); // 0.0
	print_ip(int32_t{ 2130706433 }); // 127.0.0.1
	print_ip(int64_t{ 8875824491850138409 });// 123.45.67.89.101.112.131.41

	print_ip(std::string{ "Hello, World!" }); // Hello, World!

	print_ip(std::vector<int>{100, 200, 300, 400}); // 100.200.300.400
	print_ip(std::list<short>{400, 300, 200, 100}); // 400.300.200.100

	print_ip(std::make_tuple(123, 456, 789, 0)); // 123.456.789.0

	return 0;
}