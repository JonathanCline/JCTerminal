#pragma once

#include "TextUnits.h"
#include "Texture/GlyphTexture.h"

#include <SAELib_Decorator.h>

namespace jct::text
{

	struct Glyph_BBox
	{
		int32_t xMin = 0;
		int32_t xMax = 0;
		int32_t yMin = 0;
		int32_t yMax = 0;
	};

	

	struct Glyph
	{
		using BBox = Glyph_BBox;
		using character_type = char32_t;

		frac26_pixels bearingX = 0_fpx26;
		frac26_pixels bearingY = 0_fpx26;
		frac26_pixels width = 0_fpx26;
		frac26_pixels height = 0_fpx26;
		frac26_pixels advance = 0_fpx26;

		character_type codepoint = 0;

		GlyphTexture texture{};
		pixels texture_width = 0_px;
		pixels texture_height = 0_px;

	};

}