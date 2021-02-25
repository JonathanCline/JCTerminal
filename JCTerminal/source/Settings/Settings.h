#pragma once

#include <cstdint>
#include <string>

namespace jct
{
	struct TerminalSettings
	{
		int cell_width = 16;
		int cell_height = 16;

		int cells_x = 0;
		int cells_y = 0;

		std::string title{};
	};
};