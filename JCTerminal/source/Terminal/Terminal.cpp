#include "Terminal.h"

#include <GLFW/glfw3.h>

#include <cassert>

jcTerminal::~jcTerminal()
{
	auto& _window = this->window();
	if (_window != nullptr)
	{
		glfwDestroyWindow(_window);
		_window = nullptr;
	};
	assert(this->window() == nullptr);
};
