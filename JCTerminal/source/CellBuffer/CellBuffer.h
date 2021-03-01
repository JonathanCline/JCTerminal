#pragma once

#include "Color/Color.h"
#include "Texture/GLTextureSheet.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include <cstdint>
#include <cstdlib>
#include <vector>

namespace jct
{

	class CellBuffer
	{
	private:

		auto& get_instance_data() noexcept { return this->instance_buffer_; };
		const auto& get_instance_data() const noexcept { return this->instance_buffer_; };

		void rebuffer_data();
		void destroy();

		void update_projection_matrix();

	public:

		bool initialize();

		struct InstanceData
		{
			uint16_t x;
			uint16_t y;
			uint16_t z;
			uint16_t layer;
			ColorRGBA foreground{ 0, 0, 0, 255 };
			ColorRGBA background{ 0, 0, 0, 255 };
		};

		size_t width() const noexcept { return this->width_; };
		size_t height() const noexcept { return this->height_; };
		size_t size() const noexcept { return this->width() * this->height(); };

		auto& gl_texture() noexcept { return this->texture_sheet_; };
		const auto& gl_texture() const noexcept { return this->texture_sheet_; };


		size_t to_index(uint16_t _x, uint16_t _y) const noexcept { return (_y * this->width()) + _x; };

		InstanceData& at(size_t _idx) { return this->get_instance_data().at(_idx); };
		InstanceData& at(uint16_t _x, uint16_t _y) { return this->at(this->to_index(_x, _y)); };

		const InstanceData& at(size_t _idx) const { return this->get_instance_data().at(_idx); };
		const InstanceData& at(uint16_t _x, uint16_t _y) const { return this->at(this->to_index(_x, _y)); };

		void resize(size_t _cellsX, size_t _cellsY);
		void set_cell_size(size_t _width, size_t _height);
		
		void update();
		void draw();

		//void clear(ColorRGBA _foreground)

		CellBuffer();
		~CellBuffer();

	private:
		GLuint shader_ = 0;

		gl::GLTextureSheet texture_sheet_{};

		GLuint projection_uniform_ = 0;
		glm::mat4 projection_{ 1.0f };

		size_t width_ = 0;
		size_t height_ = 0;

		size_t cell_width_ = 0;
		size_t cell_height_ = 0;

		GLuint vao_ = 0;
		GLuint vbo_ = 0;
		GLuint ibo_ = 0;

		std::vector<InstanceData> instance_buffer_{};
		GLuint instance_buffer_id_ = 0;



	};

};