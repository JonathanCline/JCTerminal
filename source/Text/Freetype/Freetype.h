#pragma once

#include <SAELib_Singleton.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <optional>

namespace sf::client::text
{
	class FTLibWrapper
	{
	public:
		operator FT_Library& () noexcept { return this->ftlib_; };
		operator const FT_Library& () const noexcept { return this->ftlib_; };

		FTLibWrapper();
		~FTLibWrapper();

	private:

		FTLibWrapper(const FTLibWrapper& other) = delete;
		FTLibWrapper& operator=(const FTLibWrapper& other) = delete;
		FTLibWrapper(FTLibWrapper&& other) = delete;
		FTLibWrapper& operator=(FTLibWrapper&& other) = delete;

		FT_Library ftlib_;
	};

	

	FT_Library& get_freetype_lib();



};
