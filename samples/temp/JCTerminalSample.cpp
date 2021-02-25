
#include <c/JCTerminal.h>

#include <SAELib_Timer.h>

#include <chrono>
#include <iostream>
#include <cassert>


constexpr auto hack_font_path_v = SOURCE_ROOT "/../../assets/fonts/Hack/Hack-Regular.ttf";

bool keep_refreshing_terminal_v = true;

const int screen_width = 48;
const int screen_height = 32;

int pos_x = 0;
int pos_y = 0;

const ColorRGBA default_color{ 255, 255, 255, 255 };
const ColorRGBA square_color{ 255, 0, 0, 255 };

void tclose_callback(jcTerminal* _terminal)
{
	keep_refreshing_terminal_v = false;
};
void tkey_callback(jcTerminal* _terminal, jcTerminal_Key _key, jcTerminal_Action _action)
{
	if (_action == JCT_RELEASE) { return; };
	switch (_key)
	{
	case JCT_KEY_W:
		if (pos_y > 0)
		{
			jcTerminalSetColor(_terminal, pos_x, pos_y, default_color);
			--pos_y;
			jcTerminalSetColor(_terminal, pos_x, pos_y, square_color);
		};
		break;
	case JCT_KEY_A:
		if (pos_x > 0)
		{
			jcTerminalSetColor(_terminal, pos_x, pos_y, default_color);
			--pos_x;
			jcTerminalSetColor(_terminal, pos_x, pos_y, square_color);
		};
		break;
	case JCT_KEY_S:
		if (pos_y + 1 < screen_height)
		{
			jcTerminalSetColor(_terminal, pos_x, pos_y, default_color);
			++pos_y;
			jcTerminalSetColor(_terminal, pos_x, pos_y, square_color);
		};
		break;
	case JCT_KEY_D:
		if (pos_x + 1 < screen_width)
		{
			jcTerminalSetColor(_terminal, pos_x, pos_y, default_color);
			++pos_x;
			jcTerminalSetColor(_terminal, pos_x, pos_y, square_color);
		};
		break;
	default:
		break;
	};
};

int main()
{
	auto _terminal = jcTerminalOpen(screen_width, screen_height, "nosegay");
	jcTerminalSetCellSize(_terminal, 16, 16);

	jcTerminalSetCloseCallback(_terminal, tclose_callback);
	jcTerminalKeyCallback(_terminal, tkey_callback);

	auto _hackIndex = jcTerminalLoadFont(_terminal, hack_font_path_v);

	jcTerminalRefresh(_terminal);
	jcTerminalSetColor(_terminal, pos_x, pos_y, square_color);

	while (keep_refreshing_terminal_v)
	{
		jcTerminalRefresh(_terminal);
	};

	jcTerminalClose(&_terminal);
	assert(_terminal == nullptr);

	return 0;
};