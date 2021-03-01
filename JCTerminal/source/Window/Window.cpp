#include "Window.h"

#include "Terminal/Terminal.h"
#include "Opengl/Opengl.h"

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
	void glfw_resize_callback(GLFWwindow* _window, int _width, int _height)
	{
		assert(_window);
		auto _terminal = get_window_terminal(_window);
		assert(_terminal);

		

	};
	void glfw_key_callback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
	{
		assert(_window);
		auto _terminal = get_window_terminal(_window);
		assert(_terminal);

		const auto& _cb = _terminal->callbacks();
		if (_cb.key_callback)
		{
			std::invoke(_cb.key_callback, _terminal, (jcTerminal_Key)_key, (jcTerminal_Action)_action);
		};

	};
	void glfw_text_callback(GLFWwindow* _window, unsigned int _codepoint)
	{
		assert(_window);
		auto _terminal = get_window_terminal(_window);
		assert(_terminal);

		const auto& _cb = _terminal->callbacks();
		if (_cb.text_callback)
		{
			std::invoke(_cb.text_callback, _terminal, (unsigned short)(_codepoint & 0xFFFF));
		};

	};






	void register_glfw_callbacks(GLFWwindow* _window)
	{
		glfwSetWindowCloseCallback(_window, glfw_close_callback);
		glfwSetKeyCallback(_window, glfw_key_callback);
		glfwSetCharCallback(_window, glfw_text_callback);
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
	jcTerminal* get_window_terminal(GLFWwindow* _window)
	{
		auto _ptr = impl::get_glfw_user_pointer(_window);
		assert(_ptr);
		return static_cast<jcTerminal*>(_ptr);
	};




	bool open_terminal_window(jcTerminal* _terminal, int _width, int _height, const char* _title)
	{
		auto& _window = _terminal->window();
		assert(!_window);

		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		_window = glfwCreateWindow(_width, _height, _title, NULL, NULL);
		assert(_window);
		assert(_terminal->window());

		glfwMakeContextCurrent(_window);

		set_window_terminal(_window, _terminal);
		register_glfw_callbacks(_window);

		return (_terminal->window() != nullptr);
	};
	bool open_terminal_window(jcTerminal* _terminal)
	{
		const auto& _settings = _terminal->settings();
		
		const auto _width = _settings.cells_x * _settings.cell_width;
		const auto _height = _settings.cells_y * _settings.cell_height;
		const auto _title = _settings.title.c_str();

		return open_terminal_window(_terminal, _width, _height, _title);
	};

	void refresh_window_size(jcTerminal* _terminal)
	{
		assert(_terminal);

		const auto& _st = _terminal->settings();
		const auto _width = _st.cell_width * _st.cells_x;
		const auto _height = _st.cell_height * _st.cells_y;

		glViewport(0, 0, _width, _height);

		auto& _window = _terminal->window();

		glfwSetWindowSize(_window, (int)_width, (int)_height);

		auto& _tsheet = _terminal->texture_sheet();
		_tsheet.resize(_st.cell_width, _st.cell_height * gl::max_texture_layers(), { 0, 0, 0, 0 });

		
		
	};


};