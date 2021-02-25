#pragma once

#include "Glyph.h"
#include "TextUnits.h"

#include <SAELib_Decorator.h>
#include <SAELib_Algorithm.h>

#include <vector>
#include <initializer_list>
#include <ranges>

namespace sf::client::text
{

	struct GlyphSet : public sae::Decorate_Vector<GlyphSet, const Glyph*>
	{
	private:
		friend sae::Decorate_Vector<GlyphSet, const Glyph*>;
		using container_type = std::vector<const Glyph*>;
		container_type& get_container() noexcept { return this->glyphs_; };
		const container_type& get_container() const noexcept { return this->glyphs_; };

	public:
		using iterator = typename container_type::iterator;
		using const_iterator = typename container_type::const_iterator;
		using reverse_iterator = typename container_type::reverse_iterator;
		using const_reverse_iterator = typename container_type::const_reverse_iterator;

		GlyphSet() = default;
		GlyphSet(std::initializer_list<const Glyph*> _glyphs) noexcept :
			glyphs_{ _glyphs }
		{};

	private:
		std::vector<const Glyph*> glyphs_;

	};

	using Glyph_Iterator = GlyphSet::iterator;
	using Glyph_ConstIterator = GlyphSet::const_iterator;

	template <typename T>
	concept cx_glyph_view = requires(T a)
	{
		{ a.begin() } -> sae::cx_one_of<Glyph_Iterator, Glyph_ConstIterator>;
		{ a.end() } -> sae::cx_one_of<Glyph_Iterator, Glyph_ConstIterator>;
	};


	struct Glyph_View
	{
	public:
		using iterator = Glyph_Iterator;
		using const_iterator = Glyph_ConstIterator;

		iterator begin() const { return this->begin_; };
		const_iterator cbegin() const { return this->begin_; };
		iterator end() const { return this->end_; };
		const_iterator cend() const { return this->end_; };

		Glyph_View(Glyph_Iterator _begin, Glyph_Iterator _end) :
			begin_{ _begin }, end_{ _end }
		{};

		template <cx_glyph_view GlyphViewT>
		Glyph_View(GlyphViewT& _container) :
			Glyph_View{ _container.begin(), _container.end() }
		{}
		template <cx_glyph_view GlyphViewT>
		Glyph_View& operator=(GlyphViewT& _container)
		{
			this->begin_ = _container.begin();
			this->end_ = _container.end();
			return *this;
		};

	private:
		iterator begin_;
		iterator end_;

	};



	static frac26_pixels width(Glyph_ConstIterator _begin, const Glyph_ConstIterator _end)
	{
		return std::accumulate(_begin, _end, 0_fpx26, [](frac26_pixels _last, const Glyph* _at)
			{
				return _last + _at->advance;
			});
	};
	template <cx_glyph_view T>
	static frac26_pixels width(const T& _glyphView)
	{
		return width(_glyphView.begin(), _glyphView.end());
	};


	template <sae::cx_one_of<Glyph_Iterator, Glyph_ConstIterator> IterT>
	static auto find(IterT _begin, const IterT _end, Glyph::character_type _codepoint)
	{
		return std::find_if(_begin, _end, [_codepoint](const Glyph*& _g) -> bool
			{
				return _g->codepoint == _codepoint;
			});
	};
	template <cx_glyph_view T>
	static auto find(T& _view, Glyph::character_type _codepoint)
	{
		return find(_view.begin(), _view.end(), _codepoint);
	};

	template <sae::cx_one_of<Glyph_Iterator, Glyph_ConstIterator> IterT>
	static auto rfind(IterT _begin, const IterT _end, Glyph::character_type _codepoint)
	{
		return sae::rfind_if(_begin, _end, [_codepoint](const Glyph*& _g) -> bool
			{
				return _g->codepoint == _codepoint;
			});
	};
	template <cx_glyph_view T>
	static auto rfind(T& _view, Glyph::character_type _codepoint)
	{
		return rfind(_view.begin(), _view.end(), _codepoint);
	};

	template <sae::cx_one_of<Glyph_Iterator, Glyph_ConstIterator> IterT>
	static auto line_end(IterT _at, IterT _end)
	{
		return find(_at, _end, (Glyph::character_type)'\n');
	};
	template <cx_glyph_view GlyphViewT>
	static auto line_end(GlyphViewT& _view)
	{
		return line_end(_view.begin(), _view.end());
	};

	template <sae::cx_one_of<Glyph_Iterator, Glyph_ConstIterator> IterT>
	static IterT line_begin(IterT _at, IterT _end)
	{
		return rfind(_at, _end, (Glyph::character_type)'\n');
	};
	template <cx_glyph_view GlyphViewT>
	static auto line_begin(GlyphViewT& _view)
	{
		return line_begin(_view.begin(), _view.end());
	};

}
