#pragma once

#include "Impl/Texture.h"

#include <SAELib_PunWrapper.h>

#include <array>
#include <cstdint>

namespace jct
{

	namespace tx
	{
		namespace impl
		{
			struct TexelDef
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
					std::array<uint8_t, 4> arr{ 255, 255, 255, 255 };
				};
				constexpr TexelDef() noexcept = default;
				constexpr TexelDef(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a) noexcept :
					arr{ _r, _g, _b, _a }
				{};
			};
		};

		template <>
		struct Texel<TEXTURE_ENCODING_E::RGBA_8> : public sae::PunWrapper<impl::TexelDef, &impl::TexelDef::arr>
		{
			using sae::PunWrapper<impl::TexelDef, &impl::TexelDef::arr>::PunWrapper;
		};

	};

	// RGBA Texel value
	using Texel = tx::Texel<tx::TEXTURE_ENCODING_E::RGBA_8>;

	// RGBA Texture
	using Texture = tx::Texture<tx::TEXTURE_ENCODING_E::RGBA_8>;



	namespace tx
	{
		namespace impl
		{
			struct MaskTexelDef
			{
				union
				{
					struct
					{
						uint8_t a;
					};
					std::array<uint8_t, 1> arr{ 255 };
				};
				constexpr MaskTexelDef() noexcept = default;
				constexpr MaskTexelDef(uint8_t _a) noexcept :
					arr{ _a }
				{};
			};
		};

		template <>
		struct Texel<TEXTURE_ENCODING_E::R_8> : public sae::PunWrapper<impl::MaskTexelDef, &impl::MaskTexelDef::arr>
		{
			using sae::PunWrapper<impl::MaskTexelDef, &impl::MaskTexelDef::arr>::PunWrapper;
		};

	};

	// Single alpha value for use in masking textures
	using MaskTexel = tx::Texel<tx::TEXTURE_ENCODING_E::R_8>;

	// Alpha mask texture
	using MaskTexture = tx::Texture<tx::TEXTURE_ENCODING_E::R_8>;






};
