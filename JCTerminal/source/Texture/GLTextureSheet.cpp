#pragma once

#include "GLTextureSheet.h"

#include <glad/glad.h>

#include <algorithm>
#include <vector>
#include <memory>

namespace jct::gl
{

	GLTextureID& GLTextureSheet::get() noexcept
	{
		return this->id_;
	};
	const GLTextureID& GLTextureSheet::get() const noexcept
	{
		return this->id_;
	};

	bool GLTextureSheet::good() const noexcept
	{
		return this->get() != 0;
	};
	GLTextureSheet::operator bool() const noexcept
	{
		return this->good();
	};

	bool GLTextureSheet::initialize()
	{
		assert(!this->good());
		glGenTextures(1, &this->get());
		return this->good();
	};

	void GLTextureSheet::release() noexcept
	{
		this->get() = 0;
	};
	void GLTextureSheet::reset()
	{
		glDeleteTextures(1, &this->get());
		this->release();
	};
	
	void GLTextureSheet::bind() const noexcept
	{
		assert(this->good());
		glBindTexture(GL_TEXTURE_2D_ARRAY, this->get());
	};
	void GLTextureSheet::unbind() const noexcept
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	};

	void GLTextureSheet::resize(size_t _layerWidth, size_t _layerHeight, size_t _layers)
	{
		this->layerWidth_ = _layerWidth;
		this->layerHeight_ = _layerHeight;
		this->layers_ = _layers;

		this->reset();
		this->initialize();
		this->bind();

		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, (GLsizei)_layerWidth, (GLsizei)_layerHeight, (GLsizei)_layers);
		//glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, (GLsizei)_layerWidth, (GLsizei)_layerHeight, (GLsizei)_layers, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		auto _err = glGetError();
		if (_err != GL_NO_ERROR)
		{
			assert(false);
		};

		this->unbind();
	};

	void GLTextureSheet::fill(size_t _layer, const Texture& _tex)
	{
		assert(this->good());
		
		this->bind();

		const auto _layerWidth = this->layer_width();
		const auto _layerHeight = this->layer_height(); 

		glTexSubImage3D
		(
			GL_TEXTURE_2D_ARRAY, 0, 		// gl bind target
			0, 0, _layer,					// data offset
			_layerWidth, _layerHeight, 0,	// layer size
			GL_RGBA, GL_UNSIGNED_BYTE,		// formatting
			_tex.data()						// data
		);

		auto _err = glGetError();
		if (_err != GL_NO_ERROR)
		{
			assert(false);
		};

		this->unbind();
	};
	
	void GLTextureSheet::assign(const Texture& _tex, size_t _layers)
	{
		this->resize(_tex.width(), _tex.height() / _layers, _layers);
		
		this->bind();

		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0,
			(GLsizei)this->layer_width(), (GLsizei)this->layer_height(), (GLsizei)_layers, 
			GL_RGBA, GL_UNSIGNED_BYTE, _tex.data());

		auto _err = glGetError();
		if (_err != GL_NO_ERROR)
		{
			assert(false);
		};
		this->unbind();
	};

	GLTextureSheet::GLTextureSheet(size_t _layerWidth, size_t _layerHeight, size_t _layers)  :
		GLTextureSheet{ 0 }
	{
		this->initialize();
		this->resize(this->layer_width(), this->layer_height(), this->layers());
	};
	
	GLTextureSheet::~GLTextureSheet()
	{
		this->reset();
	};


};
