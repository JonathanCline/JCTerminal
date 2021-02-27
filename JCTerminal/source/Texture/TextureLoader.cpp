#include "TextureLoader.h"

#include <lodepng.h>

#include <cassert>



namespace jct
{
	namespace
	{
		static const std::filesystem::path PNG_FILE_EXTENSION_V{ ".png" };
	};

	const std::filesystem::path& png_file_extension()
	{
		return PNG_FILE_EXTENSION_V;
	};

	bool is_png_file(const std::filesystem::path& _path)
	{
		const auto _exists = std::filesystem::exists(_path);
		if (!_exists) [[unlikely]]
		{
			return false;
		};

		const auto _hasExtension = _path.has_extension();
		if (!_hasExtension) [[unlikely]]
		{
			return false;
		};

		const auto& _pngExtension = png_file_extension();
		const auto _extension = _path.extension();
		if (_extension != _pngExtension) [[unlikely]]
		{
			return false;
		};

		// Good file
		return true;
	};




	std::optional<Texture> load_texture_png(const std::filesystem::path& _path)
	{
		constexpr size_t PIXEL_VALUES_PER_TEXEL_V = 4;

		if (!is_png_file(_path)) [[unlikely]]
		{
			return std::nullopt;
		};

		std::vector<unsigned char> _pixels{};
		unsigned int _width = 0;
		unsigned int _height = 0;

		const auto _result = lodepng::decode(_pixels, _width, _height, _path.string());
		if (_result) [[unlikely]]
		{
			return std::nullopt;
		};

		Texture _tex{};

		auto _pxBegin = (Texel*)_pixels.data();
		const auto _pxEnd = _pxBegin + (_pixels.size() / PIXEL_VALUES_PER_TEXEL_V);
		_tex.assign((size_t)_width, (size_t)_height, _pxBegin, _pxEnd);

		return _tex;
	};





};

