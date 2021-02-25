#include "Freetype.h"

namespace jct::text
{

	FTLibWrapper::FTLibWrapper()
	{
		auto _err = FT_Init_FreeType(&this->ftlib_);
		if (_err)
		{
#ifndef NDEBUG
			std::terminate();
#endif
		};
	};
	FTLibWrapper::~FTLibWrapper()
	{
		FT_Done_FreeType(this->ftlib_);
	};

	FT_Library& get_freetype_lib()
	{
		return sae::get_singleton_thread_local<FTLibWrapper>();
	};


}
