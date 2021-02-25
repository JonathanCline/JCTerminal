#pragma once

#include <vector>
#include <cassert>
#include <cstdint>
#include <algorithm>
#include <numeric>
#include <initializer_list>

namespace sf::client
{
	enum TEXTURE_ENCODING_E
	{
		R_8,
		RGB_8,
		RGBA_8
	};
	
	template <TEXTURE_ENCODING_E Encoding>
	struct Texel;

	namespace impl
	{
		template <TEXTURE_ENCODING_E Encoding>
		struct TexelTypeHelper
		{
			constexpr static TEXTURE_ENCODING_E encoding = Encoding;
			constexpr static size_t values_per_pixel = []() -> size_t
			{
				switch (Encoding)
				{
				case TEXTURE_ENCODING_E::R_8:
					return 1u;
					break;
				case TEXTURE_ENCODING_E::RGB_8:
					return 3u;
					break;
				case TEXTURE_ENCODING_E::RGBA_8:
					return 4u;
					break;
				default:
					assert(false);
					return 0u;
					break;
				};
			}();
			constexpr static size_t pixel_width = []() -> size_t
			{
				switch (Encoding)
				{
				case TEXTURE_ENCODING_E::R_8:
					return values_per_pixel * 1u;
					break;
				case TEXTURE_ENCODING_E::RGB_8:
					return values_per_pixel * 1u;
					break;
				case TEXTURE_ENCODING_E::RGBA_8:
					return values_per_pixel * 1u;
					break;
				default:
					assert(false);
					return 0u;
					break;
				};
			}();
		};
	};

	template <TEXTURE_ENCODING_E Encoding>
	class Texture
	{
	private:
		constexpr static TEXTURE_ENCODING_E encoding_v = Encoding;
	public:
		constexpr static TEXTURE_ENCODING_E encoding() noexcept { return encoding_v; };

		using value_type = Texel<encoding_v>;
		using pointer = value_type*;
		using reference = value_type&;
		using const_pointer = const value_type*;
		using const_reference = const value_type&;

		using allocator_type = std::allocator<value_type>;

		using size_type = size_t;

	private:
		using container_type = std::vector<value_type, allocator_type>;

		void raw_set_width(size_type _width) noexcept
		{
			this->width_ = _width;
		};
		void raw_set_height(size_type _height) noexcept
		{
			this->height_ = _height;
		};

	public:
		using iterator = typename container_type::iterator;
		using const_iterator = typename container_type::const_iterator;
		using reverse_iterator = typename container_type::reverse_iterator;
		using const_reverse_iterator = typename container_type::const_reverse_iterator;

		iterator begin() noexcept { return this->data_.begin(); };
		const_iterator begin() const noexcept { return this->data_.cbegin(); };
		const_iterator cbegin() const noexcept { return this->data_.cbegin(); };

		iterator end() noexcept { return this->data_.end(); };
		const_iterator end() const noexcept { return this->data_.cend(); };
		const_iterator cend() const noexcept { return this->data_.cend(); };

		reverse_iterator rbegin() noexcept { return this->data_.begin(); };
		const_reverse_iterator rbegin() const noexcept { return this->data_.cbegin(); };
		const_reverse_iterator crbegin() const noexcept { return this->data_.cbegin(); };

		reverse_iterator rend() noexcept { return this->data_.rend(); };
		const_reverse_iterator rend() const noexcept { return this->data_.crend(); };
		const_reverse_iterator crend() const noexcept { return this->data_.crend(); };

		size_type size() const noexcept { return this->data_.size(); };
		
		size_type width() const noexcept { return this->width_; };
		size_type height() const noexcept { return this->height_; };

	private:
		constexpr static size_type position_to_index(size_type _imageWidth, size_type _x, size_type _y) noexcept
		{
			return (_y * _imageWidth) + _x;
		};
		size_type position_to_index(size_type _x, size_type _y) const noexcept
		{
			return this->position_to_index(this->width(), _x, _y);
		};

		size_type dim_size() const noexcept
		{
			return this->width() * this->height();
		};

	public:
		reference at(size_type _index) { return this->data_.at(_index); };
		const_reference at(size_type _index) const { return this->data_.at(_index); };

		reference at(size_type _x, size_type _y) { return this->at(this->position_to_index(_x, _y)); };
		const_reference at(size_type _x, size_type _y) const { return this->at(this->position_to_index(_x, _y)); };

		void assign(size_type _width, size_type _height, const_reference _value)
		{
			this->raw_set_width(_width);
			this->raw_set_height(_height);
			this->data_.assign(this->dim_size(), _value);
		};
		template <typename IterT>
		void assign(size_type _width, size_type _height, IterT _begin, IterT _end)
		{
			this->raw_set_width(_width);
			this->raw_set_height(_height);
			assert(this->dim_size() == std::distance(_begin, _end));
			this->data_.assign(_begin, _end);
		};
		void assign(size_type _width, size_type _height, std::initializer_list<value_type> _vals)
		{
			this->raw_set_width(_width);
			this->raw_set_height(_height);
			assert(this->dim_size() == _vals.size());
			this->assign(_vals.begin(), _vals.end());
		};

		void fill(const size_type _x, const size_type _y, const size_type _width, const size_type _height, const_reference _value)
		{
			assert(_x < this->width());
			assert(_y < this->height());
			assert(_width <= this->width());
			assert(_height <= this->height());

			const auto _endX = _x + _width;
			const auto _endY = _y + _height;

			assert(_endX <= this->width());
			assert(_endY <= this->height());

			auto _rowIter = this->begin() + this->position_to_index(_x, _y);
			for (size_type r = 0; r < _height; ++r)
			{
				std::fill_n(_rowIter + (this->width() * r), _width, _value);
			};

		};

		template <typename IterT>
		void fill(const size_type _x, const size_type _y, const size_type _w, const size_type _h, IterT _begin)
		{
			const auto _width = _w;
			const auto _height = _h;

			assert(_x < this->width());
			assert(_y < this->height());
			assert(_width <= this->width());
			assert(_height <= this->height());

			const auto _endX = _x + _width;
			const auto _endY = _y + _height;

			assert(_endX <= this->width());
			assert(_endY <= this->height());

			auto _rowIter = this->begin() + this->position_to_index(_x, _y);
			auto _inIter = _begin;

			for (size_type r = 0; r < _height; ++r)
			{
				std::copy_n(_inIter + (r * _width), _width, _rowIter + (r * this->width()));
			};

		};

		void fill(const size_type _x, const size_type _y, const Texture<Encoding>& _texture)
		{
			this->fill(_x, _y, _texture.width(), _texture.height(), _texture.begin());
		};

		void resize(const size_type _width, const size_type _height, const_reference _fillColor)
		{
			const auto _oldWidth = this->width();
			const auto _oldTexture{ this->data_ };

			auto _iter = _oldTexture.begin();
			const auto _endIter = _oldTexture.end();

			this->assign(_width, _height, _fillColor);
			assert(this->dim_size() == this->size());
			assert(this->dim_size() == (_width * _height));

			auto _outIter = this->begin();
#ifndef NDEBUG
			const auto _outEndIter = this->end();
#endif
			for (_iter; _iter < _endIter; _iter = std::next(_iter, _oldWidth), _outIter = std::next(_outIter, _width))
			{
				assert(_outIter < _outEndIter);
				std::copy_n(_iter, _oldWidth, _outIter);
			};

		};
		void resize(const size_type _width, const size_type _height)
		{
			this->resize(_width, _height, value_type{});
		};

		void clear() noexcept
		{
			this->width_ = 0;
			this->data_.clear();
		};
		bool empty() const noexcept { return this->data_.empty(); };

		pointer data() noexcept { return this->data_.data(); };
		const_pointer data() const noexcept { return this->data_.data(); };



		Texture() = default;

		explicit Texture(size_type _width, size_type _height) :
			width_{ _width }, height_{ _height }, data_(_width* _height)
		{};

	private:
		size_type width_ = 0;
		size_type height_ = 0;
		container_type data_{};

	};


};
