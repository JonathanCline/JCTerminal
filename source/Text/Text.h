#pragma once

#include "Font.h"
#include "GlyphString.h"
#include "GlyphIterator.h"

#include "Window/WindowObject.h"

#include "Texture/GLTexture.h"
#include "Shader/GLShader.h"

#include "Projection/Projection.h"

#include "Panel/Panel.h"

#include <cstdint>
#include <cstdlib>

namespace sf::client::impl
{

	class UIText
	{
	public:
		using Font = text::Font;
		using zlayer_type = uint16_t;

		text::GlyphString& text() noexcept { return this->text_; };
		const text::GlyphString& text() const noexcept { return this->text_; };

		auto& texture() noexcept { return this->texture_; };
		const auto& texture() const noexcept { return this->texture_; };
		
		auto& shader() noexcept { return this->shader_; };
		const auto& shader() const noexcept { return this->shader_; };

	private:
		void set_texture(GLTexture&& _texture)
		{
			this->texture_ = std::move(_texture);
		};
		
		struct GlyphVertex
		{
			union
			{
				struct
				{
					float_t x;
					float_t y;
					float_t z;
					float_t u;
					float_t v;
					float_t layer;
					uint8_t r;
					uint8_t g;
					uint8_t b;
					uint8_t a;
					uint8_t bg_r;
					uint8_t bg_g;
					uint8_t bg_b;
					uint8_t bg_a;
				};
				struct
				{
					std::array<float_t, 3> pos;
					std::array<float_t, 3> uvs;
					std::array<uint8_t, 4> col;
					std::array<uint8_t, 4> bg_col;
				};
			};
		};

		text::GlyphSet generate_glyph_set(text::GlyphString& _string);



		struct GlyphVertexDataDef
		{
			std::array<GlyphVertex, 6> arr{};
		};
		using GlyphVertexData = sae::PunWrapper<GlyphVertexDataDef, &GlyphVertexDataDef::arr>;

		struct GlyphVerticesDef
		{
			union
			{
				struct
				{
					GlyphVertex ll;
					GlyphVertex ul;
					GlyphVertex lr;
					GlyphVertex ur;
				};
				std::array<GlyphVertex, 4> arr{};
			};
		};
		using GlyphVertices = sae::PunWrapper<GlyphVerticesDef, &GlyphVerticesDef::arr>;



		static GlyphVertices make_glyph_vertices(const text::Glyph& _glyph, const text::TypeFace& _tface, zlayer_type _zlayer = 0.0f);
		GlyphVertexData make_glyph_vertex_data(const text::GlyphInstance& _glyph);
		
		static const text::Glyph& get_glyph(const Font* _font, const text::GlyphInstance& _glyph);
		const text::Glyph& get_glyph(const text::GlyphInstance& _glyph) const;
		

	protected:
		static GLTexture make_font_texture(const Font* _font);
		static Shader get_default_shader();

	private:
		void reupload_data_nobind();

		bool check_needs_refresh_flag() const noexcept { return this->needs_refresh_; };
		void set_needs_refresh_flag() { this->needs_refresh_ = true; };
		void clear_needs_refresh_flag() { this->needs_refresh_ = false; };

		struct Cursor
		{
			text::pixels x{};
			text::pixels y{};
		};
		Cursor cursor_begin();

	public:	

		using LineView = text::Glyph_View;

		LineView line_begin();
		LineView next_line(LineView _line);




		enum JUSTIFICATION_HORIZONTAL_E
		{
			jhLeft,
			jhCenter,
			jhRight
		};
		enum JUSTIFICATION_VERTICAL_E
		{
			jvTop,
			jvCenter,
			jvBottom
		};

		auto& justify_h() noexcept { return this->justify_h_; };
		const auto& justify_h() const noexcept { return this->justify_h_; };
		
		auto& justify_v() noexcept { return this->justify_v_; };
		const auto& justify_v() const noexcept { return this->justify_v_; };

		void justify_center() noexcept;



		const Font* font() const 
		{
			return font_;
		};
		void set_font(const Font* _font) 
		{
			this->font_ = _font;
			this->set_texture(this->make_font_texture(_font)); 
		};

		using bounds_type = UIRect;
		using zlayer_type = uint16_t;

		auto& bounds() noexcept { return this->bounds_; };
		const auto& bounds() const noexcept { return this->bounds_; };
		
		auto& zlayer() noexcept { return this->zlayer_; };
		const auto& zlayer() const noexcept { return this->zlayer_; };


		void refresh();
		void draw(const glm::mat4& _projectionMatrix);

		UIText(GLShader _shader, const Font* _font, const text::GlyphString& _string);

		~UIText();

	private:
		JUSTIFICATION_HORIZONTAL_E justify_h_ = JUSTIFICATION_HORIZONTAL_E::jhLeft;
		JUSTIFICATION_VERTICAL_E justify_v_ = JUSTIFICATION_VERTICAL_E::jvTop;

		bool needs_refresh_ = false;

		const Font* font_ = nullptr;

		UIRect bounds_;
		zlayer_type zlayer_;

		GLTexture texture_;
		GLShader shader_;

		uint32_t u_projection_matrix_ = 0;
		uint32_t u_text_box_position_ = 0;

		text::GlyphString text_;
		text::GlyphSet glyphs_{};

		uint32_t vao_ = 0;
		uint32_t vbo_ = 0;

	};


}

namespace sf::client
{
	class UIText : public impl::UIText, public window::WindowResource
	{
	public:

		void update() override { impl::UIText::draw(this->projection_.mat()); };
		void refresh() override { impl::UIText::refresh(); };
		
		void grow(int _dw, int _dh) override
		{
			this->bounds().grow(_dw, _dh);
		};
		void shift(int _dx, int _dy, int _dz) override 
		{ 
			this->bounds().shift(_dx, _dy);
			this->zlayer() += (zlayer_type)_dz;
		};



		UIText(const text::Font* _font, const text::GlyphString& _gstring) : 
			impl::UIText{ std::shared_ptr<Shader>{ new Shader{ std::move(get_default_shader()) }}, _font, _gstring }
		{};

		
	private:
		UI_OrthoProjectionMatrix projection_{};

	};
}

