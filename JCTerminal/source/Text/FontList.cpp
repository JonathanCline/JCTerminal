#include "FontList.h"

#include "Font.h"
#include "Texture/TextureLoader.h"

#include <unordered_map>

namespace jct
{
	void FontHandle::release() noexcept
	{
		this->font_ = nullptr;
		this->index_ = 0;
	};
	void FontHandle::reset()
	{
		if (this->good())
		{
			delete this->get();
			this->release();
		};
	};
	FontHandle::pointer FontHandle::extract() noexcept
	{
		auto _out = this->get();
		this->release();
		return _out;
	};

	FontHandle& FontHandle::operator=(FontHandle&& other) noexcept
	{
		this->reset();
		this->font_ = other.extract();
		this->index_ = std::exchange(other.index_, 0);
		return *this;
	};

	FontHandle::~FontHandle()
	{
		this->reset();
	};




	
	namespace impl
	{	
		extern inline std::optional<FontLedger> FONT_LEDGER_V{ std::nullopt };
		auto& get_font_ledger()
		{
			auto& _lopt = FONT_LEDGER_V;
			if (_lopt.has_value()) [[likely]]
			{
				return _lopt.value();
			}
			else 
			{
				_lopt.emplace();
				return get_font_ledger();
			};
		};

		extern inline FontIndex FONT_COUNTER_V{ 0 };

		FontIndex new_font_index() noexcept
		{
			auto _index = ++FONT_COUNTER_V;
			return _index;
		};
	};



	FontHandle load_font_from_disk(const FontPath& _path, const FontSize* const  _sizeDataBegin, const FontSize* const  _sizeDataEnd, unsigned short _glyphs)
	{
		const auto _index = impl::new_font_index();
		FontHandle _fontHandle{ _index,  new text::Font{} };

		auto& _font = *_fontHandle.get();

		for (auto _sizePtr = _sizeDataBegin; _sizePtr != _sizeDataEnd; ++_sizePtr)
		{
			auto _faceOpt = text::load_typeface(_path, text::pixels{ *_sizePtr }, _glyphs);
			assert(_faceOpt);
			_font.insert(std::move(*_faceOpt));
		};

		return _fontHandle;
	};




	FontLedger::container_type& FontLedger::get_container() noexcept { return this->fonts_; };
	const FontLedger::container_type& FontLedger::get_container() const noexcept { return this->fonts_; };

	void FontLedger::insert(value_type _font)
	{
		assert(!this->contains(_font.handle));
		this->get_container().insert(std::move(_font));
	};
	void FontLedger::insert(FontHandle _font, const FontPath& _path)
	{
		assert(!this->contains(_font));
		this->insert(value_type{ std::move(_font), _path });

	};


	unsigned short load_font_into_texture(FontHandle _fontHandle, Texture& _texture, int _layerWidth, int _layerHeight, unsigned short _offset)
	{		
		auto _gcount = _offset;
		
		auto& _font = *_fontHandle.get();

		int32_t _maxGlyphHeight = 0;

		for (auto& g : _font[0])
		{
			auto _yoffset = (_layerHeight * (int)_gcount);
			++_gcount;

			const auto _twidth = std::min<size_t>(g.texture.width(), _layerWidth);
			const auto _theight = std::min<size_t>(g.texture.height(), _layerHeight);

			Texture _gtex{ _twidth, _theight, { 0, 0, 0, 0 } };
			//_font[0].metrics().descender
			auto _texIter = _gtex.begin();
			const auto _texEndIter = _gtex.end();
			for (auto& px : g.texture)
			{
				_texIter->fill(px);
				++_texIter;
				if (_texIter == _texEndIter)
				{
					break;
				};
			};
			_maxGlyphHeight = std::max(text::pixels{ g.texture_height }.count(), _maxGlyphHeight);

			const auto _fixBaselineY = _yoffset + (_layerHeight - (int)_theight) - jct::text::pixels(g.bearingY - g.height).count();
			
			const auto _coff_y = _fixBaselineY + text::pixels{ _font[0].metrics().descender }.count();
			const auto _coff_x =(_layerWidth - (int)_twidth) / 2;

			_texture.fill((size_t)_coff_x, _coff_y, _gtex);
		};



		//save_texture_png(_texture, "C:\\Users\\jonat\\source\\repos\\JonathanCline\\JCTerminal\\FONTREEEEE.png");

		return _gcount - _offset;

	};

}