#include "CellBuffer.h"

namespace
{

	constexpr auto CELL_BUFFER_VERTEX_SHADER_V = R"(

#version 330 core

layout(location = 0) in vec4 base_pos_and_uvs;

layout(location = 1) in vec4 instance_pos_and_layer;
layout(location = 2) in vec4 instance_foreground_color;
layout(location = 3) in vec4 instance_background_color;

out vec3 uvs;
out vec4 foreground_col;
out vec4 background_col;

uniform mat4 ProjectionMatrix;

void main() 
{
	uvs = vec3(base_pos_and_uvs.zw, instance_pos_and_layer.w);

	foreground_col = instance_foreground_color;
	background_col = instance_background_color;
	
	vec3 ipos = vec3(instance_pos_and_layer.xyz);
	vec3 bpos = vec3(base_pos_and_uvs.xy, 0.0);

	vec3 adjusted_ipos = vec3(ipos.x + bpos.x, ipos.y + bpos.y, ipos.z + bpos.z);
	
	gl_Position = ProjectionMatrix * vec4(adjusted_ipos.xyz, 1.0);
};

)";

	constexpr auto CELL_BUFFER_FRAGMENT_SHADER_V = R"(
#version 330 core

in vec3 uvs;
in vec4 foreground_col;
in vec4 background_col;

out vec4 color;

// Regular texture sheet

uniform sampler2DArray textureSheet;

void main() 
{
	vec4 tsheetTexel = texture(textureSheet, uvs).rgba;
	vec4 fore = foreground_col * tsheetTexel;
	color = mix(background_col, fore, fore.a);
};
)";

};

namespace jct
{
	namespace
	{
		GLuint build_shader_program_from_source(const std::string& VertexShaderCode, const std::string& FragmentShaderCode)
			{
				// Create the shaders
				GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
				GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

				GLint Result = GL_FALSE;
				int InfoLogLength;

				// Compile Vertex Shader
				char const* VertexSourcePointer = VertexShaderCode.c_str();
				glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
				glCompileShader(VertexShaderID);

				// Check Vertex Shader
				glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
				glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
				if (InfoLogLength > 0) {
					std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
					glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
					printf("%s\n", &VertexShaderErrorMessage[0]);

					return 0;
				}

				// Compile Fragment Shader
				//printf("Compiling shader : %s\n", fragment_file_path);
				char const* FragmentSourcePointer = FragmentShaderCode.c_str();
				glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
				glCompileShader(FragmentShaderID);

				// Check Fragment Shader
				glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
				glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
				if (InfoLogLength > 0) {
					std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
					glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
					printf("%s\n", &FragmentShaderErrorMessage[0]);

					return 0;
				}

				// Link the program
				GLuint ProgramID = glCreateProgram();
				glAttachShader(ProgramID, VertexShaderID);
				glAttachShader(ProgramID, FragmentShaderID);
				glLinkProgram(ProgramID);

				// Check the program
				glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
				glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
				if (InfoLogLength > 0) {
					std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
					glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
					printf("%s\n", &ProgramErrorMessage[0]);

					return 0;

				}

				glDetachShader(ProgramID, VertexShaderID);
				glDetachShader(ProgramID, FragmentShaderID);

				glDeleteShader(VertexShaderID);
				glDeleteShader(FragmentShaderID);

				return ProgramID;
			}

		constexpr std::array<float_t, 4 * 4> CELL_VERTEX_DATA_V
		{
			// pos			// uvs

			0.0f, 0.0f, 	0.0f, 0.0f,
			0.0f, 1.0f,		0.0f, 1.0f,
			1.0f, 1.0f,		1.0f, 1.0f,
			1.0f, 0.0f,		1.0f, 0.0f

		};

		constexpr std::array<uint8_t, 6> CELL_INDICES_V
		{
			0, 1, 2,
			0, 2, 3
		};
	};
	
	bool CellBuffer::initialize()
	{
		auto _err = glGetError();
		if (_err != GL_NO_ERROR)
		{
			assert(false);
		};

		this->shader_ = build_shader_program_from_source(CELL_BUFFER_VERTEX_SHADER_V, CELL_BUFFER_FRAGMENT_SHADER_V);

		_err = glGetError();
		if (_err != GL_NO_ERROR)
		{
			assert(false);
		};

		this->projection_uniform_ = glGetUniformLocation(this->shader_, "ProjectionMatrix");

		_err = glGetError();
		if (_err != GL_NO_ERROR)
		{
			assert(false);
		};

		auto& _vao = this->vao_;
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		_err = glGetError();
		if (_err != GL_NO_ERROR)
		{
			assert(false);
		};

		auto& _vbo = this->vbo_;
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		const auto& _vertexData = CELL_VERTEX_DATA_V;
		glEnableVertexAttribArray(0);
		glBufferData(GL_ARRAY_BUFFER, _vertexData.size() * sizeof(float_t), _vertexData.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);


		_err = glGetError();
		if (_err != GL_NO_ERROR)
		{
			assert(false);
		};

		auto& _ibo = this->ibo_;
		glGenBuffers(1, &_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
		
		const auto& _indiceData = CELL_INDICES_V;
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indiceData.size() * sizeof(GL_UNSIGNED_BYTE), _indiceData.data(), GL_STATIC_DRAW);

		auto& _instanceVBO = this->instance_buffer_id_;
		glGenBuffers(1, &_instanceVBO);
		glBindBuffer(GL_ARRAY_BUFFER, _instanceVBO);

		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		glBufferData(GL_ARRAY_BUFFER, sizeof(InstanceData) * this->size(), this->get_instance_data().data(), GL_STATIC_DRAW);

		glVertexAttribPointer(1, 4, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(InstanceData), 0);
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(InstanceData), (void*)(sizeof(uint16_t) * 4));
		glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(InstanceData), (void*)((sizeof(uint16_t) * 4) + (sizeof(uint8_t) * 4)));



		glVertexAttribDivisor(1, 1);
		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);

		glBindVertexArray(0);
		this->update_projection_matrix();
		

		_err = glGetError();
		if (_err != GL_NO_ERROR)
		{
			assert(false);
		};

		return true;
	};
	void CellBuffer::rebuffer_data()
	{
		auto _err = glGetError();
		if (_err != GL_NO_ERROR)
		{
			assert(false);
		};

		auto& _instanceVBO = this->instance_buffer_id_;
		
		glBindBuffer(GL_ARRAY_BUFFER, _instanceVBO);
		
		_err = glGetError();
		if (_err != GL_NO_ERROR)
		{
			assert(false);
		};

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(InstanceData) * this->size(), this->get_instance_data().data());
		
		_err = glGetError();
		if (_err != GL_NO_ERROR)
		{
			assert(false);
		};
	

		_err = glGetError();
		if (_err != GL_NO_ERROR)
		{
			assert(false);
		};

	};

	void CellBuffer::destroy()
	{
		glDeleteBuffers(1, &this->vbo_);
		glDeleteBuffers(1, &this->ibo_);
		glDeleteBuffers(1, &this->instance_buffer_id_);
		glDeleteVertexArrays(1, &this->vao_);
		glDeleteProgram(this->shader_);
	};

	void CellBuffer::update_projection_matrix()
	{
		auto _projectionMat = glm::ortho(0.0f, (float_t)(this->width()), (float_t)(this->height()), 0.0f);
		this->projection_ = _projectionMat;
	};


	void CellBuffer::update()
	{
		glBindVertexArray(this->vao_);
		this->rebuffer_data();
		glBindVertexArray(0);
	};
	void CellBuffer::draw()
	{
		glUseProgram(this->shader_);

		glBindVertexArray(this->vao_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_);
		glUniformMatrix4fv(this->projection_uniform_, 1, GL_FALSE, &this->projection_[0][0]);

		this->gl_texture().bind();

		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0, this->size());

		this->gl_texture().unbind();
		glBindVertexArray(0);
	};



	void CellBuffer::resize(size_t _cellsX, size_t _cellsY)
	{
		const auto _newSize = _cellsX * _cellsY;
		this->width_ = _cellsX;
		this->height_ = _cellsY;
		this->get_instance_data().resize(_newSize);

		for (size_t y = 0; y < this->height(); ++y)
		{
			for (size_t x = 0; x < this->width(); ++x)
			{
				auto& _instance = this->at(x, y);
				_instance.x = (uint16_t)x;
				_instance.y = (uint16_t)y;
			};
		};

		if (this->vao_ != 0)
		{

			glBindVertexArray(this->vao_);
			this->rebuffer_data();
			glBindVertexArray(0);

		};

		this->update_projection_matrix();
	};
	void CellBuffer::set_cell_size(size_t _width, size_t _height)
	{
		this->cell_width_ = _width;
		this->cell_height_ = _height;

		this->update_projection_matrix();
	};


	CellBuffer::CellBuffer()
	{};
	CellBuffer::~CellBuffer()
	{
		this->destroy();
	};


}