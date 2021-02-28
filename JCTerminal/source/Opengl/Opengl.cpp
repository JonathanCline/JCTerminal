
#include "Opengl.h"

#include "Terminal/Terminal.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace jct::gl
{

	namespace impl
	{

		

	}


	int max_texture_layers()
	{
		GLint _maxLayers = 0;
		glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &_maxLayers);
		return _maxLayers;
	};
	int max_texture_size()
	{
		GLint _maxLayers = 0;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &_maxLayers);
		return _maxLayers;
	};


	std::pair<int, int> max_texture_layer_size(jcTerminal* _terminal)
	{
		std::pair<int, int> _out
		{
			_terminal->settings().cell_width,
			_terminal->settings().cell_height
		};

		const auto _maxSize = max_texture_size();
		_out.second = std::min(_out.second, _maxSize / _out.first);
		
		return _out;
	};


}