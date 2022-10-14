#include <iostream>
#include <string>
#include <algorithm>
#include <execution>

// define a callable "copy_if" object

struct copy_if_t
{

	void handle_args(std::vector<std::string> args) {}

	template <typename... Args>
	auto operator()(Args &&...args)
	{
		return std::copy_if(std::execution::par, args...);
	}
} copy_if{};

#include "copy_if.hpp"