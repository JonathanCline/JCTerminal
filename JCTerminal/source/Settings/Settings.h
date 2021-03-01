#pragma once

#include "Color/Color.h"

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

		ColorRGBA active_text_color{ 255, 0, 100, 255 };
		ColorRGBA active_text_background_color{ 0, 0, 0, 0 };

		std::string title{};
	};
};