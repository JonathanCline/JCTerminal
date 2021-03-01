#pragma once

#include "Interface/Interface.h"

namespace jct
{
	struct CallbackLedger
	{
		jcTerminal_CloseCallback close_callback{};
		jcTerminal_KeyCallback key_callback{};
		jcTerminal_TextCallback text_callback{};
	};
};
