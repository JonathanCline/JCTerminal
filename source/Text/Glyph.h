#pragma once

#include "TextUnits.h"
#include "Texture/Texture.h"

#include <SAELib_Decorator.h>

namespace sf::client::text
{

	struct Glyph_BBox
	{
		int32_t xMin = 0;
		int32_t xMax = 0;
		int32_t yMin = 0;
		int32_t yMax = 0;
	};

	class Glyph_Texture : public sae::Decorate_Iterator<Glyph_Texture>
	{
	private:
		friend sae::Decorate_Iterator<Glyph_Texture>;

		using container_type = Texture<R_8>;

		container_type& get_container() noexcept { return this->texture_; };
		const container_type& get_container() const noexcept { return this->texture_; };

	public:
		using value_type = Texel<R_8>;

		using iterator = typename container_type::iterator;
		using const_iterator = typename container_type::const_iterator;
		using reverse_iterator = typename container_type::reverse_iterator;
		using const_reverse_iterator = typename container_type::const_reverse_iterator;

		using size_type = typename container_type::size_type;

		size_type width() const noexcept { return this->get_container().width(); };
		size_type height() const noexcept { return this->get_container().height(); };

		void resize(size_type _width, size_type _height, value_type _fillVal)
		{
			this->get_container().resize(_width, _height, _fillVal);
		};
		size_type size() const noexcept { return this->get_container().size(); };

		auto data() const noexcept { return this->get_container().data(); };

		template <typename IterT>
		void assign(size_type _width, size_type _height, IterT _begin, IterT _end)
		{
			this->get_container().assign(_width, _height, _begin, _end);
		};

		Glyph_Texture() = default;
		Glyph_Texture(size_type _width, size_type _height) :
			texture_{ _width, _height }
		{};
	
		template <typename IterT>
		Glyph_Texture(size_type _width, size_type _height, IterT _begin, const IterT _end) :
			Glyph_Texture{ _width, _height }
		{
			this->assign(_width, _height, _begin, _end);
		};

	private:
		Texture<R_8> texture_{};

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

		Glyph_Texture texture{};
		pixels texture_width = 0_px;
		pixels texture_height = 0_px;

	};

}