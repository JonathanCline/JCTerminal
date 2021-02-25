#include "Text.h"

#include <glad/glad.h>

#include <SAELib_Cursor.h>

namespace sf::client::impl
{
	namespace
	{
		constexpr const char* TEXT_VERTEX_SHADER = R"(
#version 330 core

layout(location = 0) in vec3 raw_pos;
layout(location = 1) in vec3 raw_uvs;
layout(location = 2) in vec4 raw_col;
layout(location = 3) in vec4 raw_bg_col;

out vec3 uvs;
out vec4 col;
out vec4 bg_col;

uniform mat4 ProjectionMatrix;
uniform vec3 TextBoxPosition; 

void main() 
{
	uvs = raw_uvs;
	col = raw_col;
	bg_col = raw_bg_col;

	gl_Position = ProjectionMatrix * vec4(raw_pos.xy + TextBoxPosition.xy, (raw_pos.z + TextBoxPosition.z) / 128.0, 1.0);
};
)";
		constexpr const char* TEXT_FRAGMENT_SHADER = R"(
#version 330 core

out vec4 color;

in vec3 uvs;
in vec4 col;
in vec4 bg_col;

uniform sampler2DArray textureSheet;

void main() 
{
	float mixValue = texture(textureSheet, uvs).r;
	vec4 texel = mix(bg_col, col, mixValue);
	color = texel;
};

)";

	};



	void UIText::justify_center() noexcept
	{
		this->justify_v() = jvCenter;
		this->justify_h() = jhCenter;
	};
	


	Shader UIText::get_default_shader()
	{
		auto _outOpt = build_shader_program(TEXT_VERTEX_SHADER, TEXT_FRAGMENT_SHADER);
		assert(_outOpt);
		auto _out = std::move(*_outOpt);
		return _out;
	};
	GLTexture UIText::make_font_texture(const Font* _font)
	{
		Texture<R_8> _tex{};
		
		const auto& _tface = _font->at(0);
		const auto& _metrics = _tface.metrics();

		const auto _layerSize = _metrics.glyph_max_height.count();
		const auto _layerWidth = _layerSize;
		const auto _layerHeight = _layerSize;

		_tex.resize(_layerWidth, _layerHeight * _tface.size(), Texel<R_8>{ 0 });

		size_t _gcounter = 0;
		for (const auto& g : _tface)
		{
			_tex.fill(0, _gcounter * _layerHeight, g.texture.width(), g.texture.height(), g.texture.begin());
			++_gcounter;
		};

		save_texture_to_disk(PROJECT_ROOT "/font_draw_texture.png", _tex);
		
		GLTexture _out = new_gl_texture();

		const auto _id = _out.get();

		glBindTexture(GL_TEXTURE_2D_ARRAY, _id);
		
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_R8, _layerWidth, _layerHeight, _tface.size());
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, _layerWidth, _layerHeight, _tface.size(), GL_RED, GL_UNSIGNED_BYTE, _tex.data());

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

		return _out;
	};


	text::GlyphSet UIText::generate_glyph_set(text::GlyphString& _string)
	{
		text::GlyphSet _out{};
		for (auto& g : _string)
		{
			_out.push_back(&this->font()->at(0).at(g.codepoint));
		};
		return _out;
	};



	UIText::GlyphVertices UIText::make_glyph_vertices(const text::Glyph& _glyph, const text::TypeFace& _tface, zlayer_type _zlayer)
	{
		using namespace text;

		const auto _left = (float_t)pixels{ _glyph.bearingX }.count();
		const auto _right = (float_t)pixels{ _glyph.bearingX + _glyph.width }.count();
		const auto _top = (float_t)pixels{ -_glyph.bearingY  }.count();
		const auto _bottom =  (float_t)pixels{ -(_glyph.bearingY - _glyph.height) }.count();
		
		const auto _z = (float_t)_zlayer;

		const auto _maxW = _tface.metrics().glyph_max_height.count();
		const auto _maxH = _tface.metrics().glyph_max_height.count();

		const auto _tW = _glyph.texture_width.count();
		const auto _tH = _glyph.texture_height.count();

		const auto _uvWidth = (float_t)_tW / (float_t)_maxW;
		const auto _uvHeight = (float_t)_tH / (float_t)_maxH;

		const auto _uMin = 0.0f;
		const auto _uMax = _uvWidth;

		const auto _vMin = 0.0f;
		const auto _vMax = _uvHeight;

		GlyphVertices _out{};		
		_out.arr = 
		{
			GlyphVertex{ _left, _bottom, _z, _uMin, _vMax },
			GlyphVertex{ _left, _top, _z, _uMin, _vMin },
			GlyphVertex{ _right, _bottom, _z, _uMax, _vMax },
			GlyphVertex{ _right, _top, _z, _uMax, _vMin }
		};

		return  _out;
	};

	const text::Glyph& UIText::get_glyph(const Font* _font, const text::GlyphInstance& _glyph)
	{
		const auto& _fontRef = *_font;
		const auto& _gGlyph = _fontRef[0][_glyph.codepoint];
		return _gGlyph;
	};
	const text::Glyph& UIText::get_glyph(const text::GlyphInstance& _glyph) const
	{
		return this->get_glyph(this->font(), _glyph);
	};



	UIText::GlyphVertexData UIText::make_glyph_vertex_data(const text::GlyphInstance& _glyph)
	{
		auto _verts = this->make_glyph_vertices(this->get_glyph(_glyph), this->font()->at(0));
		
		const auto& _color = _glyph.style.color;
		const auto& _bgColor = _glyph.style.background_color;

		const auto  _codepoint = _glyph.codepoint;

		for (auto& v : _verts)
		{
			std::copy(_color.begin(), _color.end(), v.col.begin());
			std::copy(_bgColor.begin(), _bgColor.end(), v.bg_col.begin());
			v.layer = _codepoint;
		};

		UIText::GlyphVertexData _out{};
		
		_out[0] = _verts[0];
		_out[1] = _verts[1];
		_out[2] = _verts[3];
		
		_out[3] = _verts[0];
		_out[4] = _verts[3];
		_out[5] = _verts[2];

		return _out;
	};

	UIText::LineView UIText::line_begin()
	{
		return LineView{ this->glyphs_.begin(), text::line_end(this->glyphs_) };
	};
	UIText::LineView UIText::next_line(LineView _line)
	{
		auto _newLineBegin = text::line_begin(++_line.end(), this->glyphs_.end());
		return LineView{ _newLineBegin, text::line_end(_newLineBegin, this->glyphs_.end()) };
	};

	UIText::Cursor UIText::cursor_begin()
	{
		using namespace text::literals;

		Cursor _out{};
		switch (this->justify_h())
		{
		case jhLeft:
			_out.x = 0_px;
			break;

		case jhCenter:
			_out.x += text::pixels{ this->bounds().width() / 2 } - ( text::pixels{ text::width(this->line_begin()) } / 2 );
			break;

		case jhRight:
			std::terminate();
			break;
		};

		switch (this->justify_v())
		{
		case jvTop:
			_out.y = 0_px;
			break;

		case jvCenter:
		{
			_out.y += (text::pixels{ this->bounds().height() } / 2);
		
			auto _line = this->line_begin();

			auto _maxHeight = std::max_element(_line.begin(), _line.end(), [](const auto& lhs, const auto& rhs)
				{
					return lhs->height < rhs->height;
				});
			
			_out.y -= text::pixels{ (*_maxHeight)->height };
		
		};
		break;

		case jvBottom:
			std::terminate();
			break;
		};

		return _out;
	};



	void UIText::reupload_data_nobind()
	{
		this->glyphs_ = this->generate_glyph_set(this->text_);

		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GlyphVertexDataDef), NULL, GL_STATIC_DRAW);
	};

	void UIText::refresh()
	{
		glBindVertexArray(this->vao_);
		this->reupload_data_nobind();
		glBindVertexArray(0);
	};
	void UIText::draw(const glm::mat4& _projectionMat)
	{
		using namespace text;

		this->shader().bind();
		
		glBindVertexArray(this->vao_);
		glBindTexture(GL_TEXTURE_2D_ARRAY, this->texture().get());

		glUniformMatrix4fv(this->u_projection_matrix_, 1, GL_FALSE, &_projectionMat[0][0]);

		float_t _textBoxPos[3]
		{
			this->bounds().left(),
			this->bounds().top() + pixels{ this->font()->at(0).metrics().ascender }.count(),
			this->zlayer()
		};
		glUniform3fv(this->u_text_box_position_, 1, _textBoxPos);

		if (this->check_needs_refresh_flag())
		{
			this->reupload_data_nobind();
			this->clear_needs_refresh_flag();
		};

		const auto _left = 0u;
		const auto _top = 0u;

		const auto& _font = this->font();
		
		auto _cursor = this->cursor_begin();

		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_);

		auto _gCursor = sae::cursor{ this->glyphs_, this->glyphs_.begin() };
		for (const auto& _gInstance : this->text())
		{
			const auto& _gGlyph = this->get_glyph(_gInstance);
			auto _vdata = this->make_glyph_vertex_data(_gInstance);

			for (auto& v : _vdata)
			{
				v.x += _cursor.x.count();
				v.y += _cursor.y.count();
			};

			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GlyphVertexDataDef), _vdata.data());
			glDrawArrays(GL_TRIANGLES, 0, 6);

			_cursor.x += pixels{ _gGlyph.advance };

			if (_gGlyph.codepoint == '\n'_c32)
			{
				const auto& _tfMets = _font->at(0).metrics();
				auto _dy = pixels{ _tfMets.ascender + _tfMets.descender };
				_cursor.y += _dy;
				_cursor.x = 0_px;
			};

			_gCursor.next();

		};

		glBindVertexArray(0);

	};





	UIText::UIText(GLShader _shader, const Font* _font, const text::GlyphString& _string) : 
		shader_{ _shader }, text_{ _string }, font_{ _font }, texture_{ this->make_font_texture(_font) }
	{
		u_projection_matrix_ = glGetUniformLocation(this->shader()->get(), "ProjectionMatrix");
		u_text_box_position_ = glGetUniformLocation(this->shader()->get(), "TextBoxPosition");

		glGenVertexArrays(1, &this->vao_);
		glBindVertexArray(this->vao_);

		glGenBuffers(1, &this->vbo_);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		const auto _stride = sizeof(GlyphVertex);
		auto _offset = 0u;

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, _stride, (void*)_offset);
		_offset += sizeof(float_t) * 3;

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, _stride, (void*)_offset);
		_offset += sizeof(float_t) * 3;

		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, _stride, (void*)_offset);
		_offset += sizeof(uint8_t) * 4;

		glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, _stride, (void*)_offset);
		_offset += sizeof(uint8_t) * 4;

		assert(_offset == _stride);

		this->reupload_data_nobind();

		glBindVertexArray(0);

	};

	UIText::~UIText()
	{
		glDeleteVertexArrays(1, &this->vao_);
		glDeleteBuffers(1, &this->vbo_);
	};

}
