#include "Window.h"

#include "Terminal/Terminal.h"

#include <GLFW/glfw3.h>
#include <cassert>

namespace jct
{
	void glfw_close_callback(GLFWwindow* _window)
	{
		assert(_window);
		auto _terminal = get_window_terminal(_window);
		assert(_terminal);
		
		const auto& _cb = _terminal->callbacks();
		if (_cb.close_callback)
		{
			std::invoke(_cb.close_callback, _terminal);
		};

	};








	void register_glfw_callbacks(GLFWwindow* _window)
	{
		glfwSetWindowCloseCallback(_window, glfw_close_callback);
	};




	namespace impl
	{
		void set_glfw_user_pointer(GLFWwindow* _window, void* _userPtr)
		{
			glfwSetWindowUserPointer(_window, _userPtr);
		};
		void* get_glfw_user_pointer(GLFWwindow* _window)
		{
			return glfwGetWindowUserPointer(_window);
		};
	};

	void set_window_terminal(GLFWwindow* _window, jcTerminal* _userPtr)
	{
		impl::set_glfw_user_pointer(_window, static_cast<void*>(_userPtr));
	};
	jcTerminal* set_window_terminal(GLFWwindow* _window)
	{
		auto _ptr = impl::get_glfw_user_pointer(_window);
		assert(_ptr);
		return static_cast<jcTerminal*>(_ptr);
	};

};