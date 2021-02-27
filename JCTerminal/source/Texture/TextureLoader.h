#pragma once

#include "Texture.h"

#include <optional>
#include <filesystem>

namespace jct
{


	const std::filesystem::path& png_file_extension();


	// Returns true if
	//		File exists
	//		File has extension
	//		File extension is equal to png_file_extension()
	//		
	bool is_png_file(const std::filesystem::path& _path);


	std::optional<Texture> load_texture_png(const std::filesystem::path& _path);




}