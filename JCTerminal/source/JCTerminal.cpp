#include "JCTerminal.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cassert>

#ifdef __cplusplus
namespace jct
{
#endif

	namespace impl
	{
		extern inline GLFWwindow* terminal_window_v = nullptr;
	};

	int terminal_open(int _widthCells, int _heightCells, const char* _title)
	{
		auto _res = glfwInit();
		assert(_res == 1);

		impl::terminal_window_v = glfwCreateWindow(_widthCells * 16, _heightCells * 16, _title, NULL, NULL);
		assert(impl::terminal_window_v);

		glfwMakeContextCurrent(impl::terminal_window_v);
		_res = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		assert(_res == 1);

		return 1;
	};

	void terminal_refresh()
	{
		assert(impl::terminal_window_v);
		glfwSwapBuffers(impl::terminal_window_v);
	};

	void terminal_close()
	{
		if (impl::terminal_window_v)
		{
			glfwDestroyWindow(impl::terminal_window_v);
		};
	};

#ifdef __cplusplus
};
#endif

