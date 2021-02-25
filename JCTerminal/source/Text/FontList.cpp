#include "FontList.h"

#include "Font.h"

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



	FontIndex load_font_from_disk(const FontPath& _path, const FontSize* const  _sizeDataBegin, const FontSize* const  _sizeDataEnd)
	{
		const auto _index = impl::new_font_index();
		FontHandle _fontHandle{ _index,  new text::Font{} };

		auto& _font = *_fontHandle.get();

		for (auto _sizePtr = _sizeDataBegin; _sizePtr != _sizeDataEnd; ++_sizePtr)
		{
			auto _faceOpt = text::load_typeface(_path, text::pixels{ *_sizePtr });
			assert(_faceOpt);
			_font.insert(std::move(*_faceOpt));
		};

		auto& _ledger = impl::get_font_ledger();
		_ledger.insert( std::move(_fontHandle), _path );
		return _index;
	};




	FontLedger::container_type& FontLedger::get_container() noexcept { return this->fonts_; };
	const FontLedger::container_type& FontLedger::get_container() const noexcept { return this->fonts_; };

	void FontLedger::insert(value_type _font)
	{
		assert(!this->contains(_font.handle));
	};
	void FontLedger::insert(FontHandle _font, const FontPath& _path)
	{
		assert(!this->contains(_font));
		this->insert(value_type{ std::move(_font), _path });

	};

}