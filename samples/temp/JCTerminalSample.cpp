
#include <c/JCTerminal.h>

#include <SAELib_Timer.h>

#include <chrono>
#include <iostream>
#include <cassert>


constexpr auto hack_font_path_v = SOURCE_ROOT "/../../assets/fonts/Hack/Hack-Regular.ttf";

bool keep_refreshing_terminal_v = true;

const int screen_width = 32;
const int screen_height = 24;

int pos_x = 0;
int pos_y = 0;

const jcTerminal_Color default_color{ 255, 255, 255, 255 };
const jcTerminal_Color square_color{ 255, 0, 0, 255 };
const jcTerminal_Color dared_color{ 80, 10, 0, 255 };

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
			jcTerminalPut(_terminal, pos_x, pos_y, 0);
			--pos_y;
			jcTerminalPut(_terminal, pos_x, pos_y, 1);
		};
		break;
	case JCT_KEY_A:
		if (pos_x > 0)
		{
			jcTerminalPut(_terminal, pos_x, pos_y, 0);
			--pos_x;
			jcTerminalPut(_terminal, pos_x, pos_y, 1);
		};
		break;
	case JCT_KEY_S:
		if (pos_y + 1 < screen_height)
		{
			jcTerminalPut(_terminal, pos_x, pos_y, 0);
			++pos_y;
			jcTerminalPut(_terminal, pos_x, pos_y, 1);
		};
		break;
	case JCT_KEY_D:
		if (pos_x + 1 < screen_width)
		{
			jcTerminalPut(_terminal, pos_x, pos_y, 0);
			++pos_x;
			jcTerminalPut(_terminal, pos_x, pos_y, 1);
		};
		break;
	default:
		break;
	};
	jcTerminalRefresh(_terminal);
};

int main()
{
	auto _terminal = jcTerminalOpen(screen_width, screen_height, "nosegay");
	jcTerminalSetCellSize(_terminal, 32, 32);

	jcTerminalSetCloseCallback(_terminal, tclose_callback);
	jcTerminalSetKeyCallback(_terminal, tkey_callback);

	auto _hackIndex = jcTerminalLoadFont(_terminal, hack_font_path_v);

	jcTerminalPut(_terminal, pos_x, pos_y, 1);

	jcTerminalFillRect(_terminal, 4, 4, 12, 16, dared_color);
	jcTerminalRefresh(_terminal);

	auto _lpngResult = jcTerminalLoadPNG(_terminal, SOURCE_ROOT "/Untitled.png", 1);
	assert(_lpngResult == 0);

	while (keep_refreshing_terminal_v) 
	{
		jcTerminalDrawUntilEvent(_terminal);
	};

	jcTerminalClose(&_terminal);
	assert(_terminal == nullptr);

	return 0;
};