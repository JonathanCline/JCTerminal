#include "Font.h"

#include "Freetype/Freetype.h"

#include <cassert>

namespace jct::text
{
	namespace
	{
		frac26_pixels to_frac26(font_units _val, FT_Fixed _scale) noexcept
		{
			return frac26_pixels{ (frac26_pixels::value_type)FT_MulFix(_val.count(), _scale) };
		};

		frac26_pixels to_frac26_x(font_units _val, const FT_Face& _face) noexcept
		{
			return frac26_pixels{ (frac26_pixels::value_type)FT_MulFix(_val.count(), _face->size->metrics.x_scale) };
		};
		frac26_pixels to_frac26_y(font_units _val, const FT_Face& _face) noexcept
		{
			return frac26_pixels{ (frac26_pixels::value_type)FT_MulFix(_val.count(), _face->size->metrics.y_scale) };
		};

		TypeFaceMetrics make_face_metrics(const FT_Face& _face)
		{
			const auto& _metrics = _face->size->metrics;

			TypeFaceMetrics _out
			{
				.ascender = frac26_pixels{ _metrics.ascender },
				.descender = frac26_pixels{ _metrics.descender },
				.height = frac26_pixels{ _metrics.height },
				.max_advance = frac26_pixels{ _metrics.max_advance },
				.glyph_max_width = _out.max_advance,
				.glyph_max_height = _out.height
			};
			
			auto _h = pixels{ _out.height };
			auto _w = _h;
			return _out;
		};

		std::optional<GlyphTexture> make_glyph_texture(const FT_Face& _face)
		{
			auto _err = FT_Render_Glyph(_face->glyph, FT_RENDER_MODE_NORMAL);
			if (_err) [[unlikely]]
			{
				return std::nullopt;
			};

			const auto _width = (GlyphTexture::size_type)_face->glyph->bitmap.width;
			const auto _height = (GlyphTexture::size_type)_face->glyph->bitmap.rows;
			const auto _begin = _face->glyph->bitmap.buffer;
			const auto _end = _begin + (_width * _height);

			GlyphTexture _out{};
			_out.resize(_width, _height, GlyphTexture::value_type{});
			auto _texIter = _out.begin();
			std::copy(_begin, _end, _texIter);

			return _out;
		};

		std::optional<Glyph> make_glyph_horizontal(Glyph::character_type _codepoint, const FT_Face& _face) noexcept
		{
			auto _idx = FT_Get_Char_Index(_face, (FT_ULong)_codepoint);
			auto _err = FT_Load_Glyph(_face, _idx, 0);
			if (_err)
			{
				return std::nullopt;
			};

			Glyph _glyph{};

			const auto _ftGlyph = _face->glyph;
			const auto _ftMet = _ftGlyph->metrics;

			_glyph.codepoint = (Glyph::character_type)_codepoint;

			_glyph.advance = frac26_pixels{ _ftMet.horiAdvance };
			_glyph.bearingX = frac26_pixels{ _ftMet.horiBearingX };
			_glyph.bearingY = frac26_pixels{ _ftMet.horiBearingY };
			_glyph.width = frac26_pixels{ _ftMet.width };
			_glyph.height = frac26_pixels{ _ftMet.height };

			auto _gTexOpt = make_glyph_texture(_face);
			if (!_gTexOpt) [[unlikely]]
			{
				return std::nullopt;
			}
			else
			{
				_glyph.texture = *_gTexOpt;
			};

			_glyph.texture_width = pixels{ (pixels::value_type) _glyph.texture.width() };
			_glyph.texture_height = pixels{ (pixels::value_type)_glyph.texture.height() };

			return _glyph;
		};
		std::optional<Glyph> make_glyph_vertical(Glyph::character_type _codepoint, const FT_Face& _face) noexcept
		{
			auto _idx = FT_Get_Char_Index(_face, (FT_ULong)_codepoint);
			auto _err = FT_Load_Glyph(_face, _idx, 0);
			if (_err)
			{
				return std::nullopt;
			};

			Glyph _glyph{};

			const auto _ftGlyph = _face->glyph;
			const auto _ftMet = _ftGlyph->metrics;

			_glyph.codepoint = (Glyph::character_type)_codepoint;

			_glyph.advance = frac26_pixels{ _ftMet.vertAdvance };
			_glyph.bearingX = frac26_pixels{ _ftMet.vertBearingX };
			_glyph.bearingY = frac26_pixels{ _ftMet.vertBearingY };
			_glyph.width = frac26_pixels{ _ftMet.width };
			_glyph.height = frac26_pixels{ _ftMet.height };

			auto _gTexOpt = make_glyph_texture(_face);
			if (!_gTexOpt) [[unlikely]]
			{
				return std::nullopt;
			}
			else
			{
				_glyph.texture = std::move(*_gTexOpt);
			};

			return _glyph;
		};


	};

	constexpr pixels::pixels(frac26_pixels _fontUnits) noexcept :
		pixels{ (pixels::value_type)(_fontUnits.count() >> 6 ) }
	{};

	std::optional<TypeFace> load_typeface(const std::filesystem::path& _path, pixels _heightPx)
	{
		FT_Face _face{};

		auto _err = FT_New_Face(get_freetype_lib(), _path.string().c_str(), 0, &_face);
		assert(!_err);

		_err = FT_Set_Pixel_Sizes(_face, 0, _heightPx.count());
		assert(!_err);

		const auto _faceMets = make_face_metrics(_face);
		const auto _fHeight = pixels{ _faceMets.height };

		const auto _nGlyphs = _face->num_glyphs;

		TypeFace _tface{};
		auto& _tfMets = _tface.metrics();
		_tfMets = _faceMets;

		size_t _gNum = 0;
		for (Glyph::character_type c = 0; c < _nGlyphs; ++c)
		{
			const auto _glyphOpt = make_glyph_horizontal(c, _face);
			assert(_glyphOpt);

			_tface.insert(*_glyphOpt);

			_gNum++;
		};

		_err = FT_Done_Face(_face);
		assert(!_err);

		return _tface;
	};

	

};

namespace jct::text
{

	void TypeFace::insert(Glyph _glyph)
	{
		this->glyphs_.push_back(GlyphEntry{ _glyph });
	};
	
	void TypeFace::clear() noexcept
	{
		this->glyphs_.clear();
	};


	TypeFace::size_type TypeFace::get_character_index(character_type _char) const noexcept
	{
		size_type _out = npos;
		auto _iter = this->find(_char);
		if (_iter != this->end())
		{
			_out = std::distance(this->begin(), _iter);
		};
		return _out;
	};

	TypeFace::reference TypeFace::at(size_type _idx) { return this->get_container().at(_idx); };
	TypeFace::const_reference TypeFace::at(size_type _idx) const { return this->get_container().at(_idx); };

	TypeFace::reference TypeFace::at(character_type _char) { return this->at(this->get_character_index(_char)); };
	TypeFace::const_reference TypeFace::at(character_type _char) const { return this->get_container().at(this->get_character_index(_char)); };

	TypeFace::reference TypeFace::operator[](size_type _idx) { return this->at(_idx); };
	TypeFace::const_reference TypeFace::operator[](size_type _idx) const { return this->at(_idx); };

	TypeFace::reference TypeFace::operator[](character_type _char) { return this->at(_char); };
	TypeFace::const_reference TypeFace::operator[](character_type _char) const { return this->at(_char); };





}
