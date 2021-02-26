#pragma once

#include "c/JCTerminal.h"

#include "Settings/Settings.h"
#include "Callback/Callback.h"
#include "Terminal/Terminal.h"
#include "Window/Window.h"
#include "Text/FontList.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cassert>
#include <array>
#include <numeric>

namespace
{
	constexpr auto DEFAULT_FONT_SIZES_V = []() -> std::array<int, 6>
	{
		std::array<int, 6> _out{};
		_out[0] = 8;
		std::iota(_out.begin(), _out.end(), 6);
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

	auto& _cellBuffer = _terminal->cell_buffer();
	_cellBuffer.resize(_st.cells_x, _st.cells_y);
	auto _goodInit = _cellBuffer.initialize();
	assert(_goodInit);

	return _terminal;
};
void jcTerminalRefresh(jcTerminal* _terminal)
{
	assert(_terminal);

	auto& _window = _terminal->window();
	assert(_window);

	glClear(GL_COLOR_BUFFER_BIT);

	auto& _cellBuffer = _terminal->cell_buffer();
	_cellBuffer.draw();

	glfwSwapBuffers(_window);
	glfwPollEvents();

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

void jcTerminalGetCellSize(jcTerminal* _terminal, int* _width, int* _height)
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

void jcTerminalGetWindowSize(jcTerminal* _terminal, int* _widthCells, int* _heightCells)
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



int jcTerminalLoadFont(jcTerminal* _terminal, const char* _fontPath)
{
	assert(_terminal);
	auto& _fonts = _terminal->fonts();

	jct::FontPath _path{ _fontPath };
	constexpr auto _fontSizes = DEFAULT_FONT_SIZES_V;
	auto _fontIndex = jct::load_font_from_disk(_path, _fontSizes.data(), _fontSizes.data() + _fontSizes.size());
	return _fontIndex;
};







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

	_cellBuffer.update();
};
jcTerminal_Color jcTerminalGetColor(jcTerminal* _terminal, int _x, int _y)
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

	_cellBuffer.update();
};
jcTerminal_Color jcTerminalGetBackgroundColor(jcTerminal* _terminal, int _x, int _y)
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

	_cb.update();

};







void jcTerminalSetCloseCallback(jcTerminal* _terminal, jcTerminal_CloseCallback _callback)
{
	assert(_terminal);
	auto& _cbl = _terminal->callbacks();
	_cbl.close_callback = _callback;
};
void jcTerminalKeyCallback(jcTerminal* _terminal, jcTerminal_KeyCallback _callback)
{
	assert(_terminal);
	auto& _cb = _terminal->callbacks();
	_cb.key_callback = _callback;
};



