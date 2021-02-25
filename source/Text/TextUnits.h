#pragma once

#include <SAELib_Invariant.h>
#include <SAELib_Utility.h>

#include <cstdint>

namespace sf::client::text
{


	using font_units = sae::IntegralInvariant<int32_t, struct FontUnitsTag>;

	using frac26_pixels = sae::IntegralInvariant<int32_t, struct Fractional26PixelsTag>;

	struct pixels : public sae::IntegralInvariant<int32_t, struct PixelsTag>
	{
		constexpr pixels(frac26_pixels _fontUnits) noexcept;
		using sae::IntegralInvariant<int32_t, struct PixelsTag>::IntegralInvariant;
	};

	using font_points = sae::IntegralInvariant<int32_t, struct FontPointsTag>;

	using dots_per_inch = sae::IntegralInvariant<int32_t, struct DotsPerInchTag>;

	namespace literals
	{
		constexpr static font_units operator""_fu(long long unsigned _n) noexcept
		{
			return font_units{ (font_units::value_type)_n };
		};
		constexpr static frac26_pixels operator""_fpx26(long long unsigned _n) noexcept
		{
			return frac26_pixels{ (frac26_pixels::value_type)_n };
		};
		constexpr static pixels operator""_px(long long unsigned _n) noexcept
		{
			return pixels{ (pixels::value_type)_n };
		};
		constexpr static dots_per_inch operator""_dpi(long long unsigned _n) noexcept
		{
			return dots_per_inch{ (dots_per_inch::value_type)_n };
		};
		constexpr static font_points operator""_pt(long long unsigned _n) noexcept
		{
			return font_points{ (font_points::value_type)_n };
		};

		using sae::literals::operator""_c8;
		using sae::literals::operator""_c16;
		using sae::literals::operator""_c32;

	};

	using namespace literals;

}