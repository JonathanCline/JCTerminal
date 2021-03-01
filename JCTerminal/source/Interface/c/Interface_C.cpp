#pragma once

#include "c/JCTerminal.h"

#include "Settings/Settings.h"
#include "Callback/Callback.h"
#include "Terminal/Terminal.h"
#include "Window/Window.h"
#include "Text/FontList.h"
#include "Texture/TextureLoader.h"
#include "Opengl/Opengl.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cassert>
#include <array>
#include <numeric>

namespace
{
	constexpr auto DEFAULT_FONT_SIZES_V = []() -> auto
	{
		std::array<int, 1> _out{ 0 };
		return _out;
	}();
};


jcTerminal* jcTerminalOpen(int _widthCells, int _heightCells, const char* _title)
{
	auto _res = glfwInit();
	assert(_res == 1);

	auto* _terminal = new jcTerminal{};
	assert(_terminal);


	auto& _st = _terminal->settings();
	_st.cells_x = _widthCells;
	_st.cells_y = _heightCells;
	_st.title = _title;


	auto _result = jct::open_terminal_window(_terminal);
	assert(_result);


	auto& _window = _terminal->window();
	assert(_window);
	glfwMakeContextCurrent(_window);
	

	_res = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	assert(_res == 1);

	glEnable(GL_BLEND);
	//glBlendFunc(GL_)


	auto& _tsheet = _terminal->texture_sheet();
	const auto _texLayers = jct::gl::max_texture_layers();
	const auto _texLayerSize = jct::gl::max_texture_layer_size(_terminal);
	_tsheet.resize(_texLayerSize.first, _texLayerSize.second * _texLayers, { 0, 0, 0, 0 });



	auto& _cellBuffer = _terminal->cell_buffer();
	_cellBuffer.resize(_st.cells_x, _st.cells_y);
	auto _goodInit = _cellBuffer.initialize();
	assert(_goodInit);



	auto& _glTex = _cellBuffer.gl_texture();
	const auto _goodTexture = _glTex.initialize();
	assert(_goodTexture);


	_glTex.assign(_tsheet, _texLayers);

	return _terminal;
};
void jcTerminalRefresh(jcTerminal* _terminal)
{
	assert(_terminal);

	auto& _cb = _terminal->cell_buffer();

	// Update gl texture
	if (_terminal->texture_needs_update_v)
	{
		const auto& _textureSheet = _terminal->texture_sheet();
		_cb.gl_texture().assign(_textureSheet, jct::gl::max_texture_layers());
		_terminal->texture_needs_update_v = false;
	};

	// Update cell buffer
	_cb.update();

};
void jcTerminalClose(jcTerminal** _terminalPtr)
{
	if (_terminalPtr && *_terminalPtr)
	{
		auto& _terminal = *_terminalPtr;
		
		auto& _window = _terminal->window();
		if(_window)
		{
			glfwDestroyWindow(_window);
			_window = nullptr;
		};

		delete _terminal;
		_terminal = nullptr;
	};
};

namespace
{


	void draw_terminal(jcTerminal* _terminal)
	{
		assert(_terminal);

		auto& _window = _terminal->window();
		assert(_window);

		glClear(GL_COLOR_BUFFER_BIT);

		auto& _cellBuffer = _terminal->cell_buffer();
		_cellBuffer.draw();

		glfwSwapBuffers(_window);
	};



};



void jcTerminalDraw(jcTerminal* _terminal)
{
	assert(_terminal);
	draw_terminal(_terminal);
	glfwPollEvents();
};
void jcTerminalDrawUntilEvent(jcTerminal* _terminal, double _timeoutSeconds)
{
	assert(_terminal);
	draw_terminal(_terminal);
	glfwWaitEventsTimeout(_timeoutSeconds);
};



void jcTerminalGetCellSize(const jcTerminal* _terminal, int* _width, int* _height)
{
	assert(_terminal);
	auto& _st = _terminal->settings();
	*_width = _st.cell_width;
	*_height = _st.cell_height;
};
void jcTerminalSetCellSize(jcTerminal* _terminal, int _width, int _height)
{
	assert(_terminal);
	auto& _st = _terminal->settings();
	_st.cell_width = _width;
	_st.cell_height = _height;
	jct::refresh_window_size(_terminal);
};

void jcTerminalGetWindowSize(const jcTerminal* _terminal, int* _widthCells, int* _heightCells)
{
	assert(_terminal);
	auto& _st = _terminal->settings();
	*_widthCells = _st.cells_x;
	*_heightCells = _st.cells_y;
};
void jcTerminalSetWindowSize(jcTerminal* _terminal, int _widthCells, int _heightCells)
{
	assert(_terminal);
	auto& _st = _terminal->settings();
	_st.cells_x = _widthCells;
	_st.cells_y = _heightCells;

	auto& _cellBuffer = _terminal->cell_buffer();
	_cellBuffer.resize(_st.cells_x, _st.cells_y);

	jct::refresh_window_size(_terminal);
};



int jcTerminalGetMaxTextureCount()
{
	return jct::gl::max_texture_layers();
};



int jcTerminalLoadFont(jcTerminal* _terminal, const char* _fontPath, unsigned short _startIndex, unsigned short _glyphs)
{
	assert(_terminal);
	auto& _fonts = _terminal->fonts();

	jct::FontPath _path{ _fontPath };
	auto _fontSizes = DEFAULT_FONT_SIZES_V;
	for (auto& f : _fontSizes)
	{
		f += (_terminal->settings().cell_height - 2);
	};
	auto _fontIndex = jct::load_font_from_disk(_path, _fontSizes.data(), _fontSizes.data() + _fontSizes.size(), _glyphs);

	const auto& _st = _terminal->settings();
	const auto _glyphCount = jct::load_font_into_texture(std::move(_fontIndex), _terminal->texture_sheet(), _st.cell_width, _st.cell_height, _startIndex);

	_terminal->texture_needs_update_v = true;

	return _glyphCount;
};
int jcTerminalLoadPNG(jcTerminal* _terminal, const char* _path, unsigned short _setIndex)
{
	const auto _fpath = std::filesystem::path{ _path };
	const auto _isPNG = jct::is_png_file(_fpath);
	if (!_isPNG) [[unlikely]]
	{
		return -1;
	};

	auto _texOpt = jct::load_texture_png(_fpath);
	if (!_texOpt) [[unlikely]]
	{
		return -2;
	};


	/*
		Write the loaded texture into the texture sheet at the index requested
	*/
	
	auto& _texture = *_texOpt;
	auto& _textureSheet = _terminal->texture_sheet();

	const auto _cellWidth = _terminal->settings().cell_width;
	const auto _cellHeight = _terminal->settings().cell_height;

	const auto _y0 = _cellHeight * _setIndex;

	if (_texture.height() > _cellHeight || _texture.width() > _cellWidth) [[unlikely]]
	{
		_texture.resize(_cellHeight, _cellHeight);
	};

	_textureSheet.fill(0, _y0, _texture);

	_terminal->texture_needs_update_v = true;

	return 0;
};

namespace
{
	jct::ColorRGBA convert_color(jcTerminal_Color _color)
	{
		jct::ColorRGBA _col{};
		_col.r = _color.r;
		_col.g = _color.g;
		_col.b = _color.b;
		_col.a = _color.a;
		return _col;
	};

}


void jcTerminalSetColor(jcTerminal* _terminal, int _x, int _y, jcTerminal_Color _color)
{
	assert(_terminal);
	auto& _cellBuffer = _terminal->cell_buffer();
	auto& _cell = _cellBuffer.at((uint16_t)_x, (uint16_t)_y);

	auto& _col = _cell.foreground; 
	_col.r = _color.r;
	_col.g = _color.g;
	_col.b = _color.b;
	_col.a = _color.a;

};
jcTerminal_Color jcTerminalGetColor(const jcTerminal* _terminal, int _x, int _y)
{
	assert(_terminal);
	auto& _cellBuffer = _terminal->cell_buffer();
	auto& _cell = _cellBuffer.at((uint16_t)_x, (uint16_t)_y);

	jcTerminal_Color _col{};

	auto& _color = _cell.foreground;
	_col.r = _color.r;
	_col.g = _color.g;
	_col.b = _color.b;
	_col.a = _color.a;

	return _col;
};

void jcTerminalSetBackgroundColor(jcTerminal* _terminal, int _x, int _y, jcTerminal_Color _color)
{
	assert(_terminal);
	auto& _cellBuffer = _terminal->cell_buffer();
	auto& _cell = _cellBuffer.at((uint16_t)_x, (uint16_t)_y);

	auto& _col = _cell.background;
	_col.r = _color.r;
	_col.g = _color.g;
	_col.b = _color.b;
	_col.a = _color.a;

};
jcTerminal_Color jcTerminalGetBackgroundColor(const jcTerminal* _terminal, int _x, int _y)
{
	assert(_terminal);
	auto& _cellBuffer = _terminal->cell_buffer();
	auto& _cell = _cellBuffer.at((uint16_t)_x, (uint16_t)_y);

	jcTerminal_Color _col{};

	auto& _color = _cell.background;
	_col.r = _color.r;
	_col.g = _color.g;
	_col.b = _color.b;
	_col.a = _color.a;

	return _col;
};



void jcTerminalPut(jcTerminal* _terminal, int _x, int _y, unsigned short _tindex)
{
	assert(_terminal);
	auto& _cell = _terminal->cell_buffer().at(_x, _y);
	_cell.foreground.fill(255);
	_cell.layer = _tindex;
};
unsigned short jcTerminalGet(const jcTerminal* _terminal, int _x, int _y)
{
	assert(_terminal);
	auto _index = _terminal->cell_buffer().at(_x, _y).layer;
	return _index;
};

namespace
{

	uint16_t get_tab_width()
	{
		return 4;
	};

};


void jcTerminalSetDefaultTextColor(jcTerminal* _terminal, jcTerminal_Color _textCol, jcTerminal_Color _background)
{
	assert(_terminal);
	auto& _st = _terminal->settings();
	_st.active_text_color = convert_color(_textCol);
	_st.active_text_background_color = convert_color(_background);
};

void jcTerminalPrint(jcTerminal* _terminal, int _x, int _y, const char* _string, int _len)
{
	assert(_terminal);
	const auto _strlen = ((_len == 0) ? std::strlen(_string) : _len);
	const auto _endPtr = _string + _strlen;
	
	const auto _beginX = _x;
	const auto _beginY = _y;

	const auto _color = _terminal->settings().active_text_color;
	const auto _bgcolor = _terminal->settings().active_text_background_color;

	auto& _cb = _terminal->cell_buffer();

	for (const char* c = _string; c != _endPtr; c++)
	{
		const auto _schar = *c;
		uint16_t _index = 0;
		switch (_schar)
		{
		case '\n':
			if (_y + 1 < _terminal->settings().cells_y)
			{
				++_y;
			};
			break;
		case '\t':
			if (_x + get_tab_width() < _terminal->settings().cells_x)
			{
				_x += get_tab_width();
			};
			break;
		case '\r':
			_x = _beginX;
			break;
		case '\b':
			if (_x > _beginX)
			{
				--_x;
				_cb.at(_x, _y).layer = 0;
			};
			break;
		default:
		{
			auto& _cref = _cb.at(_x, _y);
			_cref.layer = (uint16_t)_schar;
			_cref.foreground = _color;
			_cref.background = _bgcolor;
		};		
			if (_x + 1 < _terminal->settings().cells_x)
			{
				++_x;
			}
			else if (_y + 1 < _terminal->settings().cells_y)
			{
				++_y;
				_x = _beginX;
			};
			break;
		};
	};
};


void jcTerminalFillRect(jcTerminal* _terminal, int _x0, int _y0, int _x1, int _y1, jcTerminal_Color _color)
{
	assert(_terminal);

	if (_x0 > _x1) { std::swap(_x0, _x1); };
	if (_y0 > _y1) { std::swap(_y0, _y1); };
	
	auto& _cb = _terminal->cell_buffer();
	
	for (auto x = _x0; x != _x1; ++x)
	{
		for (auto y = _y0; y != _y1; ++y)
		{
			auto& _col = _cb.at(x, y).foreground;
			_col.r = _color.r;
			_col.g = _color.g;
			_col.b = _color.b;
			_col.a = _color.a;
		};
	};

};



void jcTerminalClear(jcTerminal* _terminal)
{
	assert(_terminal);
	auto& _cb = _terminal->cell_buffer();
	for (size_t x = 0; x < _cb.size(); ++x)
	{
		_cb.at(x).layer = 0;
	};
};





void jcTerminalSetCloseCallback(jcTerminal* _terminal, jcTerminal_CloseCallback _callback)
{
	assert(_terminal);
	auto& _cbl = _terminal->callbacks();
	_cbl.close_callback = _callback;
};
void jcTerminalSetKeyCallback(jcTerminal* _terminal, jcTerminal_KeyCallback _callback)
{
	assert(_terminal);
	auto& _cb = _terminal->callbacks();
	_cb.key_callback = _callback;
};
void jcTerminalSetTextCallback(jcTerminal* _terminal, jcTerminal_TextCallback _callback)
{
	assert(_terminal);
	_terminal->callbacks().text_callback = _callback;
};



