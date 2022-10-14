#include <iostream>
#include <string>
#include <algorithm>
#include "hpx/hpx.hpp"
#include "hpx/hpx_main.hpp"

// define a callable "copy_if" object

struct copy_if_t
{

	void handle_args(std::vector<std::string> args) {}

	template <typename... Args>
	auto operator()(Args &&...args)
	{
		return hpx::copy_if(hpx::execution::par, args...);
	}
} copy_if{};

#include "copy_if.hpp"