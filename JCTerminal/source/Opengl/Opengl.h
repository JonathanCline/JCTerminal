#pragma once

#include <utility>

struct jcTerminal;

namespace jct::gl
{

	int max_texture_size();
	int max_texture_layers();

	
	std::pair<int, int> max_texture_layer_size(jcTerminal* _terminal);


	
}