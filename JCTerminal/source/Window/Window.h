#pragma once

struct jcTerminal;

struct GLFWwindow;

namespace jct
{


	void glfw_close_callback(GLFWwindow* _window);




	void register_glfw_callbacks(GLFWwindow* _window);





	namespace impl
	{
		void set_glfw_user_pointer(GLFWwindow* _window, void* _userPtr);
		void* get_glfw_user_pointer(GLFWwindow* _window);
	};

	void set_window_terminal(GLFWwindow* _window, jcTerminal* _userPtr);
	jcTerminal* get_window_terminal(GLFWwindow* _window);






};
