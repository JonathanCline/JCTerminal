#pragma once

#include "TextUnits.h"
#include "Glyph.h"

#include "Texture/Texture.h"

#include <SAELib_Invariant.h>
#include <SAELib_Decorator.h>
#include <SAELib_Utility.h>
#include <SAELib_Concepts.h>

#include <cstdint>
#include <set>
#include <concepts>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <string>

namespace sf::client::text
{

	struct TypeFaceMetrics
	{
		frac26_pixels ascender = 0_fpx26;
		frac26_pixels descender = 0_fpx26;
		frac26_pixels height = 0_fpx26;
		frac26_pixels max_advance = 0_fpx26;

		pixels glyph_max_width = 0_px;
		pixels glyph_max_height = 0_px;

	};

	class TypeFace : public sae::Decorate_Iterator<TypeFace>
	{
	public:
		using character_type = Glyph::character_type;

		using value_type = Glyph;
		using pointer = value_type*;
		using reference = value_type&;
		using const_pointer = const value_type*;
		using const_reference = const value_type&;

	private:
		friend sae::Decorate_Iterator<TypeFace>;
		
	public:
		struct GlyphEntry : public Glyph
		{
			friend constexpr inline bool operator<(const GlyphEntry& _lhs, const GlyphEntry& _rhs) noexcept
			{
				return _lhs.codepoint < _rhs.codepoint;
			};
			friend constexpr inline bool operator==(const GlyphEntry& _lhs, const GlyphEntry& _rhs) noexcept
			{
				return _lhs.codepoint == _rhs.codepoint;
			};

			friend constexpr inline bool operator==(const GlyphEntry& _lhs, const Glyph& _rhs) noexcept
			{
				return _lhs.codepoint == _rhs.codepoint;
			};
			
			friend constexpr inline bool operator<(const GlyphEntry& _lhs, const Glyph& _rhs) noexcept
			{
				return _lhs.codepoint < _rhs.codepoint;
			};
			friend constexpr inline bool operator<(const Glyph& _lhs, const GlyphEntry& _rhs) noexcept
			{
				return _lhs.codepoint < _rhs.codepoint;
			};

			friend constexpr inline bool operator==(const GlyphEntry& _lhs, const character_type& _rhs) noexcept
			{
				return _lhs.codepoint == _rhs;
			};

			friend constexpr inline bool operator<(const GlyphEntry& _lhs, const character_type& _rhs) noexcept
			{
				return _lhs.codepoint < _rhs;
			};
			friend constexpr inline bool operator<(const character_type& _lhs, const GlyphEntry& _rhs) noexcept
			{
				return _lhs < _rhs.codepoint;
			};

			using Glyph::Glyph;
			
			GlyphEntry(Glyph _glyph) :
				Glyph{ std::move(_glyph) }
			{};

		};

	private:
		using container_type = std::vector<GlyphEntry>;

		container_type& get_container() noexcept { return this->glyphs_; };
		const container_type& get_container() const noexcept { return this->glyphs_; };

	public:
		using iterator = typename container_type::iterator;
		using const_iterator = typename container_type::const_iterator;
		using reverse_iterator = typename container_type::reverse_iterator;
		using const_reverse_iterator = typename container_type::const_reverse_iterator;

		using size_type = size_t;

		auto find(character_type _char) noexcept { return std::find(this->begin(), this->end(), _char); };
		auto find(character_type _char) const noexcept { return std::find(this->begin(), this->end(), _char); };
		bool contains(character_type _char) const noexcept { return this->find(_char) != this->end(); };
		
		TypeFaceMetrics& metrics() noexcept { return this->size_metrics_; };
		const TypeFaceMetrics& metrics() const noexcept { return this->size_metrics_; };

		void insert(Glyph _glyph);
		size_type size() const noexcept { return this->glyphs_.size(); };

		void clear() noexcept;

		constexpr static size_type npos = std::numeric_limits<size_type>::max();

		size_type get_character_index(character_type _char) const noexcept;

		reference at(size_type _idx);
		const_reference at(size_type _idx) const;

		reference at(character_type _char);
		const_reference at(character_type _char) const;

		reference operator[](size_type _idx);
		const_reference operator[](size_type _idx) const;

		reference operator[](character_type _char);
		const_reference operator[](character_type _char) const;

	private:
		TypeFaceMetrics size_metrics_{};		
		container_type glyphs_{};

	};

	std::optional<TypeFace> load_typeface(const std::filesystem::path& _path, pixels _heightPx);

	class Font : sae::Decorate_Iterator<Font>
	{
	private:
		friend sae::Decorate_Iterator<Font>;

		auto& get_container() noexcept { return this->styles_; };
		const auto& get_container() const noexcept { return this->styles_; };

	public:
		using character_type = Glyph::character_type;

		using value_type = TypeFace;
		using pointer = TypeFace*;
		using reference = TypeFace&;
		using const_pointer = const TypeFace*;
		using const_reference = const TypeFace&;
		
		using size_type = size_t;

		void clear() noexcept { this->get_container().clear(); };

		void insert(const_reference _tface)
		{
			this->get_container().push_back(_tface);
		};
		void insert(value_type&& _tface)
		{
			this->get_container().push_back(std::move(_tface));
		};

		size_type size() const noexcept { return this->get_container().size(); };
		
		reference at(size_type _idx) { return this->get_container().at(_idx); };
		const_reference at(size_type _idx) const { return this->get_container().at(_idx); };

		reference operator[](size_type _idx) { return this->at(_idx); };
		const_reference operator[](size_type _idx) const { return this->at(_idx); };

		Font() = default;
		Font(std::initializer_list<TypeFace> _styles) :
			styles_{ std::move(_styles) }
		{};

	private:
		std::vector<TypeFace> styles_{};

	};

}
