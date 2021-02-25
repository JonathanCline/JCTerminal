#pragma once

#include <SAELib_PunWrapper.h>

#include <array>
#include <cstdint>

namespace jct
{

	namespace impl
	{
		struct ColorRGBA
		{
			union
			{
				struct
				{
					uint8_t r;
					uint8_t g;
					uint8_t b;
					uint8_t a;
				};
				std::array<uint8_t, 4> arr{};
			};

			constexpr ColorRGBA(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a) noexcept :
				arr{ _r, _g, _b, _a }
			{};
			constexpr ColorRGBA() noexcept :
				ColorRGBA{ 255, 255, 255, 255 }
			{};
		};
	};
	
	using ColorRGBA = sae::PunWrapper<impl::ColorRGBA, &impl::ColorRGBA::arr>;

}