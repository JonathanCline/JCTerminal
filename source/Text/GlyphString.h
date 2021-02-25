#pragma once

#include "Glyph.h"
#include "Color/Color.h"

#include "GlyphIterator.h"

#include <SAELib_Concepts.h>
#include <SAELib_Decorator.h>

#include <cstdint>
#include <vector>
#include <initializer_list>
#include <utility>
#include <numeric>
#include <string>

namespace sf::client::text
{

	enum GLYPH_STYLE_FLAGS_E : uint8_t
	{
		gStyleNone = 0x00,
		gStyleBold = 0x01,
		gStyleItalic = 0x02
	};

	class GlyphStyleFlags
	{
	public:
		using value_type = uint8_t;
		using GLYPH_STYLE_FLAGS_E = GLYPH_STYLE_FLAGS_E;

		GlyphStyleFlags& set(GLYPH_STYLE_FLAGS_E _flag) noexcept
		{
			this->bits_ |= (value_type)_flag;
			return *this;
		};
		GlyphStyleFlags& clear(GLYPH_STYLE_FLAGS_E _flag) noexcept
		{
			this->bits_ &= ~(value_type)_flag;
			return *this;
		};
		constexpr bool check(GLYPH_STYLE_FLAGS_E _flag) const noexcept
		{
			return (this->bits_ & (value_type)_flag) != 0;
		};

		friend constexpr inline GlyphStyleFlags operator|(const GlyphStyleFlags& lhs, GLYPH_STYLE_FLAGS_E _flag) noexcept
		{
			GlyphStyleFlags _out{ lhs };
			_out.bits_ |= (value_type)_flag;
			return _out;
		};
		friend inline GlyphStyleFlags& operator|=(GlyphStyleFlags& lhs, GLYPH_STYLE_FLAGS_E _flag) noexcept
		{
			lhs.set(_flag);
			return lhs;
		};

		friend constexpr inline GlyphStyleFlags operator|(const GlyphStyleFlags& lhs, const GlyphStyleFlags& rhs) noexcept
		{
			GlyphStyleFlags _out{ lhs };
			_out.bits_ |= rhs.bits_;
			return _out;
		};
		friend inline GlyphStyleFlags& operator|=(GlyphStyleFlags& lhs, const GlyphStyleFlags& rhs) noexcept
		{
			lhs.bits_ |= rhs.bits_;
			return lhs;
		};

	private:
		static constexpr value_type fold_initializer_list(const std::initializer_list<GLYPH_STYLE_FLAGS_E>& _ilist) noexcept
		{
			value_type _out = 0x00;
			for (const auto& f : _ilist)
			{
				_out |= (value_type)f;
			};
			return _out;
		};

	public:
		constexpr GlyphStyleFlags() noexcept = default;

		constexpr explicit GlyphStyleFlags(value_type _bits) noexcept :
			bits_{ _bits }
		{};
		constexpr GlyphStyleFlags(std::initializer_list<GLYPH_STYLE_FLAGS_E> _flags) noexcept :
			bits_{ fold_initializer_list(_flags) }
		{};

	private:
		value_type bits_ = 0x00;

	};

	struct GlyphStyle
	{
		using color_type = ColorRGBA_8;
		GlyphStyleFlags style{};
		uint16_t size = 16;
		color_type color{ 255, 255, 255, 255 };
		color_type background_color{ 0, 0, 0, 0 };
	};

	struct GlyphInstance
	{
		using character_type = Glyph::character_type;

		constexpr operator character_type() const noexcept { return this->codepoint; };

		character_type codepoint;
		GlyphStyle style;

	};

	class GlyphString : public sae::Decorate_Iterator<GlyphString>
	{
	public:
		using value_type = GlyphInstance;
		using pointer = value_type*;
		using reference = value_type&;
		using const_pointer = const value_type*;
		using const_reference = const value_type&;

		using character_type = value_type::character_type;

	private:
		using container_type = std::vector<value_type>;

		friend sae::Decorate_Iterator<GlyphString>;
		container_type& get_container() noexcept { return this->glyphs_; };
		const container_type& get_container() const noexcept { return this->glyphs_; };

		static reference set_glyph_style(reference _glyph, const GlyphStyle& _style) noexcept
		{
			_glyph.style = _style;
			return _glyph;
		};
		static value_type make_glyph(character_type _codepoint, const GlyphStyle& _style)
		{
			value_type _out{ .codepoint = _codepoint };
			set_glyph_style(_out, _style);
			return _out;
		};

	public:
		using iterator = typename container_type::iterator;
		using const_iterator = typename container_type::const_iterator;
		using reverse_iterator = typename container_type::reverse_iterator;
		using const_reverse_iterator = typename container_type::const_reverse_iterator;

		GlyphStyle& active_style() noexcept { return this->active_style_; };
		const GlyphStyle& active_style() const noexcept { return this->active_style_; };

		std::basic_string<character_type> string() const noexcept
		{
			std::basic_string<character_type> _out{ this->begin(), this->end() };
			return _out;
		};

		using size_type = typename container_type::size_type;

		size_type capacity() const noexcept { return this->get_container().capacity(); };
		void reserve(size_type _n) { this->get_container().reserve(_n); };

		size_type size() const noexcept { return this->get_container().size(); };
		void resize(size_type _n) { this->get_container().resize(_n); };

		void clear() noexcept { this->get_container().clear(); };



		iterator erase(const_iterator _at)
		{
			return this->get_container().erase(_at);
		};
		iterator erase(const_iterator _begin, const_iterator _end)
		{
			return this->get_container().erase(_begin, _end);
		};

		size_type erase(size_type _offset, size_type _count)
		{
			const auto _begin = std::next(this->begin(), _offset);
			return std::distance(this->begin(), this->erase(_begin, std::next(_begin, _count)));
		};
		size_type erase(size_type _offset)
		{
			return this->erase(_offset, 1);
		};

		template <sae::cx_forward<value_type> _GlyphT>
		iterator insert(const_iterator _at, _GlyphT&& _glyph)
		{
			return this->get_container().insert(_at, std::forward<_GlyphT>(_glyph));
		};

		template <typename IterT>
		iterator insert(const_iterator _at, IterT _begin, IterT _end)
		{
			return this->get_container().insert(_at, _begin, _end);
		};

		template <sae::cx_forward<value_type> _GlyphT>
		size_type insert(size_type _offset, _GlyphT&& _val)
		{
			return std::distance(this->begin(), this->insert(std::next(this->begin(), _offset), std::forward<_GlyphT>(_val)));
		};


		template <sae::cx_character CharT>
		iterator insert(const_iterator _at, CharT _char, GlyphStyle _style)
		{
			return this->insert(_at, make_glyph(_char, _style));
		};
		template <sae::cx_character CharT>
		iterator insert(const_iterator _at, CharT _char)
		{
			return this->insert(_at, make_glyph(_char, this->active_style()));
		};

		template <sae::cx_character CharT>
		iterator insert(const_iterator _at, const CharT* _cString, size_type _len, GlyphStyle _style)
		{
			iterator _out{};
			for (size_type n = 0; n < _len; ++n)
			{
				_out = this->insert(this->end(), _cString[n], _style);
			};
			return _out;
		};
		template <sae::cx_character CharT>
		iterator insert(const_iterator _at, const CharT* _cString, size_type _len)
		{
			return this->insert(_at, _cString, _len, this->active_style());
		};

		template <sae::cx_character CharT>
		iterator insert(const_iterator _at, const CharT* _cString, GlyphStyle _style)
		{
			return this->insert(_at, _cString, sae::strlen(_cString), _style);
		};
		template <sae::cx_character CharT>
		iterator insert(const_iterator _at, const CharT* _cString)
		{
			return this->insert(_at, _cString, sae::strlen(_cString), this->active_style());
		};

		template <sae::cx_character CharT, typename CharTraits = std::char_traits<CharT>, typename CharAlloc = std::allocator<CharT>>
		iterator insert(const_iterator _at, const std::basic_string<CharT, CharTraits, CharAlloc>& _string, GlyphStyle _style)
		{
			return this->insert(_at, _string.c_str(), _string.size(), _style);
		};
		template <sae::cx_character CharT, typename CharTraits = std::char_traits<CharT>, typename CharAlloc = std::allocator<CharT>>
		iterator insert(const_iterator _at, const std::basic_string<CharT, CharTraits, CharAlloc>& _string)
		{
			return this->insert(_at, _string, this->active_style());
		};



		template <sae::cx_character CharT>
		void push_back(CharT _character, GlyphStyle _style)
		{
			this->insert(this->end(), _character, _style);
		};
		template <sae::cx_character CharT>
		void push_back(CharT _character)
		{
			this->push_back(_character, this->active_style());
		};


		template <sae::cx_character CharT, typename CharTraits = std::char_traits<CharT>, typename CharAlloc = std::allocator<CharT>>
		void append(const std::basic_string<CharT, CharTraits, CharAlloc>& _string, GlyphStyle _style)
		{
			this->insert(this->end(), _string, _style);
		};
		template <sae::cx_character CharT, typename CharTraits = std::char_traits<CharT>, typename CharAlloc = std::allocator<CharT>>
		void append(const std::basic_string<CharT, CharTraits, CharAlloc>& _string)
		{
			this->append(_string, this->active_style());
		};

		template <sae::cx_character CharT>
		void append(const CharT* _cstr, GlyphStyle _style)
		{
			this->insert(this->end(), _cstr, _style);
		};
		template <sae::cx_character CharT>
		void append(const CharT* _cstr)
		{
			this->append(_cstr, this->active_style());
		};


		template <sae::cx_character CharT, typename CharTraits = std::char_traits<CharT>, typename CharAlloc = std::allocator<CharT>>
		void assign(const std::basic_string<CharT, CharTraits, CharAlloc>& _string, GlyphStyle _style)
		{
			this->clear();
			this->append(_string, _style);
		};
		template <sae::cx_character CharT, typename CharTraits = std::char_traits<CharT>, typename CharAlloc = std::allocator<CharT>>
		void assign(const std::basic_string<CharT, CharTraits, CharAlloc>& _string)
		{
			this->assign(_string, this->active_style());
		};

		template <sae::cx_character CharT>
		void assign(const CharT* _cstr, GlyphStyle _style)
		{
			this->clear();
			this->append(_cstr, _style);
		};
		template <sae::cx_character CharT>
		void assign(const CharT* _cstr)
		{
			this->assign(_cstr, this->active_style());
		};



		reference at(size_type _idx)
		{
			return this->get_container().at(_idx);
		};
		const_reference at(size_type _idx) const
		{
			return this->get_container().at(_idx);
		};

		reference operator[](size_type _idx)
		{
			return this->at(_idx);
		};
		const_reference operator[](size_type _idx) const
		{
			return this->at(_idx);
		};

		void pop_back()
		{
			this->erase(std::next(this->end(), -1));
		};

		GlyphString() = default;

		GlyphString( GlyphStyle _activeStyle) :
			active_style_{ _activeStyle }
		{};

	private:
		GlyphStyle active_style_{};
		container_type glyphs_{};

	};
}