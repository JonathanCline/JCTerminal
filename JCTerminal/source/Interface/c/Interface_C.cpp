#pragma once

#include "c/JCTerminal.h"

#include "Settings/Settings.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cassert>

namespace impl
{
	extern inline GLFWwindow* terminal_window_v = nullptr;
};

namespace
{
	auto& terminal_settings() { return jct::terminal_settings(); };
};

int jcTerminal_open(int _widthCells, int _heightCells, const char* _title)
{
	auto _res = glfwInit();
	assert(_res == 1);

	auto& _st = terminal_settings();
	_st.cells_x = _widthCells;
	_st.cells_y = _heightCells;
	_st.title = _title;

	auto& _window = impl::terminal_window_v;
	_window = glfwCreateWindow(_st.cell_width * _st.cells_x, _st.cell_height * _st.cell_height, _st.title.c_str(), NULL, NULL);
	assert(_window);

	glfwMakeContextCurrent(_window);
	_res = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	assert(_res == 1);

	return 1;
};
void jcTerminal_refresh()
{
	assert(impl::terminal_window_v);
	glfwSwapBuffers(impl::terminal_window_v);
	glfwPollEvents();
};
void jcTerminal_close()
{
	if (impl::terminal_window_v)
	{
		glfwDestroyWindow(impl::terminal_window_v);
		impl::terminal_window_v = nullptr;
	};
};

void jcTerminal_getCellSize(int* _width, int* _height)
{
	auto& _st = terminal_settings();
	*_width = _st.cell_width;
	*_height = _st.cell_height;
};
void jcTerminal_setCellSize(int _width, int _height)
{
	auto& _st = terminal_settings();
	_st.cell_width = _width;
	_st.cell_height = _height;
};

void jcTerminal_getWindowSize(int* _widthCells, int* _heightCells)
{
	auto& _st = terminal_settings();
	*_widthCells = _st.cells_x;
	*_heightCells = _st.cells_y;
};
void jcTerminal_setWindowSize(int _widthCells, int _heightCells)
{
	auto& _st = terminal_settings();
	_st.cells_x = _widthCells;
	_st.cells_y = _heightCells;
};

