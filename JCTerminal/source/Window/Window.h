#pragma once

struct jcTerminal;

struct GLFWwindow;

namespace jct
{


	void glfw_close_callback(GLFWwindow* _window);
	void glfw_resize_callback(GLFWwindow* _window, int _width, int _height);
	void glfw_key_callback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods);




	void register_glfw_callbacks(GLFWwindow* _window);





	namespace impl
	{
		void set_glfw_user_pointer(GLFWwindow* _window, void* _userPtr);
		void* get_glfw_user_pointer(GLFWwindow* _window);
	};

	void set_window_terminal(GLFWwindow* _window, jcTerminal* _userPtr);
	jcTerminal* get_window_terminal(GLFWwindow* _window);




	bool open_terminal_window(jcTerminal* _terminal, int _width, int _height, const char* _title);
	bool open_terminal_window(jcTerminal* _terminal);

	void refresh_window_size(jcTerminal* _terminal);

};
