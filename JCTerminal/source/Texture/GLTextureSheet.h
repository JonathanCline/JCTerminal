#pragma once

#include "Texture.h"
#include "Opengl/Opengl.h"

#include <cstdint>

namespace jct::gl
{

	using GLTextureID = unsigned int;

	class GLTextureSheet
	{
	public:
		GLTextureID& get() noexcept;
		const GLTextureID& get() const noexcept;

		bool good() const noexcept;
		explicit operator bool() const noexcept;
		
		bool initialize();

		void release() noexcept;
		void reset();

		void bind() const noexcept;
		void unbind() const noexcept;

		void resize(size_t _layerWidth, size_t _layerHeight, size_t _layers);

		constexpr size_t layer_width() const noexcept { return this->layerWidth_; };
		constexpr size_t layer_height() const noexcept { return this->layerHeight_; };
		constexpr size_t layers() const noexcept { return this->layers_; };

		// Fills a layer with data from _tex
		void fill(size_t _layer, const Texture& _tex);
		
		// Loads texture data from _tex as full texture sheet
		void assign(const Texture& _tex, size_t _layers);

		constexpr explicit GLTextureSheet(GLTextureID _id) noexcept :
			id_{ _id }
		{};
		constexpr explicit GLTextureSheet() noexcept :
			GLTextureSheet{ 0 }
		{};

		GLTextureSheet(size_t _layerWidth, size_t _layerHeight, size_t _layers);

		~GLTextureSheet();

	private:
		GLTextureID id_ = 0;

		size_t layerWidth_ = 0;
		size_t layerHeight_ = 0;
		size_t layers_ = 0;

	};

};
