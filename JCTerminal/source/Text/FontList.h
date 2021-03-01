#pragma once

#include "Texture/Texture.h"

#include <filesystem>
#include <set>
#include <optional>
#include <compare>

namespace jct::text
{
	class Font;
};

namespace jct
{
	using FontIndex = int;
	using FontPath = std::filesystem::path;

	class FontHandle
	{
	public:
		using pointer = text::Font*;
		using const_pointer = const text::Font*;

		constexpr pointer get() const noexcept { return this->font_; };

		constexpr bool good() const noexcept { return this->get() != nullptr; };
		constexpr explicit operator bool() const noexcept { return this->good(); };

		void release() noexcept;
		void reset();
		pointer extract() noexcept;

		constexpr FontIndex index() const noexcept { return this->index_; };

		constexpr explicit FontHandle(FontIndex _index, pointer _font) noexcept : 
			index_{ _index }, font_{ _font }
		{};
		
		FontHandle(const FontHandle& other) = delete;
		FontHandle& operator=(const FontHandle& other) = delete;

		constexpr FontHandle(FontHandle&& other) noexcept : 
			index_{ std::exchange(other.index_, 0) }, 
			font_{ std::exchange(other.font_, nullptr) }
		{};
		FontHandle& operator=(FontHandle&& other) noexcept;

		~FontHandle();

	private:
		FontIndex index_ = 0;
		pointer font_ = nullptr;

	};
	
	using FontSize = int;
	FontHandle load_font_from_disk(const FontPath& _path, const FontSize* const  _sizeDataBegin, const FontSize* const _sizeDataEnd, unsigned short _glyphs);

	class FontLedger
	{
	public:
		struct FontData
		{
			friend inline bool operator<(const FontData& lhs, const FontData& rhs) noexcept { return lhs.handle.index() < rhs.handle.index(); };

			friend inline bool operator<(const FontData& lhs, const FontHandle& rhs) noexcept { return lhs.handle.index() < rhs.index(); };
			friend inline bool operator<(const FontHandle& lhs, const FontData& rhs) noexcept { return lhs.index() < rhs.handle.index(); };

			friend inline bool operator<(const FontData& lhs, const FontIndex& rhs) noexcept { return lhs.handle.index() < rhs; };
			friend inline bool operator<(const FontIndex& lhs, const FontData& rhs) noexcept { return lhs < rhs.handle.index(); };

			FontHandle handle;
			FontPath path{};
		};
		using value_type = FontData;

	private:
		using container_type = std::set<value_type, std::less<>>;

		container_type& get_container() noexcept;
		const container_type& get_container() const noexcept;

	public:
		using iterator = typename container_type::iterator;
		using const_iterator = typename container_type::const_iterator;

		iterator begin() noexcept { return this->get_container().begin(); };
		const_iterator begin() const noexcept { return this->get_container().cbegin(); };
		const_iterator cbegin() const noexcept { return this->get_container().cbegin(); };

		iterator end() noexcept { return this->get_container().end(); };
		const_iterator end() const noexcept { return this->get_container().cend(); };
		const_iterator cend() const noexcept { return this->get_container().cend(); };

		void insert(value_type _font);
		void insert(FontHandle _font, const FontPath& _path);

		auto find(const FontHandle& _font) { return this->get_container().find(_font); };
		auto find(const FontHandle& _font) const { return this->get_container().find(_font); };
		bool contains(const FontHandle& _font) const { return this->find(_font) != this->end(); };

		auto find(FontIndex _index) { return this->get_container().find(_index); };
		auto find(FontIndex _index) const { return this->get_container().find(_index); };
		bool contains(FontIndex _index) const { return this->find(_index) != this->end(); };

	private:
		container_type fonts_{};

	};

	// Returns number of glyphs written into texture
	unsigned short load_font_into_texture(FontHandle _index, Texture& _texture, int _layerWidth, int _layerHeight, unsigned short _offset);

}