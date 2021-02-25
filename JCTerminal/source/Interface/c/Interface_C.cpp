#pragma once

#include "c/JCTerminal.h"

#include "Settings/Settings.h"
#include "Callback/Callback.h"
#include "Terminal/Terminal.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cassert>

jcTerminal* jcTerminal_open(int _widthCells, int _heightCells, const char* _title)
{
	auto _res = glfwInit();
	assert(_res == 1);

	auto* _terminal = new jcTerminal{};
	assert(_terminal);

	auto& _st = _terminal->settings();
	_st.cells_x = _widthCells;
	_st.cells_y = _heightCells;
	_st.title = _title;

	auto& _window = _terminal->window();
	_window = glfwCreateWindow(_st.cell_width * _st.cells_x, _st.cell_height * _st.cell_height, _st.title.c_str(), NULL, NULL);
	assert(_window);

	glfwMakeContextCurrent(_window);
	_res = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	assert(_res == 1);

	return _terminal;
};
void jcTerminal_refresh(jcTerminal* _terminal)
{
	assert(_terminal);

	auto& _window = _terminal->window();
	assert(_window);

	glfwSwapBuffers(_window);
	glfwPollEvents();

};
void jcTerminal_close(jcTerminal** _terminalPtr)
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

void jcTerminal_getCellSize(jcTerminal* _terminal, int* _width, int* _height)
{
	assert(_terminal);
	auto& _st = _terminal->settings();
	*_width = _st.cell_width;
	*_height = _st.cell_height;
};
void jcTerminal_setCellSize(jcTerminal* _terminal, int _width, int _height)
{
	assert(_terminal);
	auto& _st = _terminal->settings();
	_st.cell_width = _width;
	_st.cell_height = _height;
};

void jcTerminal_getWindowSize(jcTerminal* _terminal, int* _widthCells, int* _heightCells)
{
	assert(_terminal);
	auto& _st = _terminal->settings();
	*_widthCells = _st.cells_x;
	*_heightCells = _st.cells_y;
};
void jcTerminal_setWindowSize(jcTerminal* _terminal, int _widthCells, int _heightCells)
{
	assert(_terminal);
	auto& _st = _terminal->settings();
	_st.cells_x = _widthCells;
	_st.cells_y = _heightCells;
};

void jcTerminal_setCloseCallback(jcTerminal* _terminal, jcTerminal_CloseCallback _callback)
{
	assert(_terminal);
	auto& _cbl = _terminal->callbacks();
	_cbl.close_callback = _callback;
};
