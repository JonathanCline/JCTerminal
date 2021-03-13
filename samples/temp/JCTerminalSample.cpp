
#include <c/JCTerminal.h>

#include <SAELib_Timer.h>

#include <chrono>
#include <iostream>
#include <cassert>


constexpr auto hack_font_path_v = SOURCE_ROOT "/../../assets/fonts/Hack/Hack-Regular.ttf";

bool keep_refreshing_terminal_v = true;

const int screen_width = 96;
const int screen_height = 48;

const jcTerminal_Color text_color{ 40, 200, 40, 255 };
const jcTerminal_Color text_background_color{ 0, 0, 0, 255 };




std::string user_input = "";
const int user_input_x = 4;
const int user_input_y = 4;

int cursor_x = user_input_x;
int cursor_y = user_input_y;

void tclose_callback(jcTerminal* _terminal)
{
	keep_refreshing_terminal_v = false;
};
void tkey_callback(jcTerminal* _terminal, jcTerminal_Key _key, jcTerminal_Action _action)
{
	switch (_key)
	{
	case JCT_KEY_BACKSPACE:
		switch (_action)
		{
		case JCT_REPEAT: [[fallthrough]];
		case JCT_PRESS:
			if (!user_input.empty())
			{
				--cursor_x;
				jcTerminalPut(_terminal, cursor_x, cursor_y, 0);
				user_input.pop_back();
			};
			break;
		default:
			break;
		};
		break;
	default:
		break;
	};
};

void ttext_callback(jcTerminal* _terminal, unsigned short _codepoint)
{
	user_input.push_back(_codepoint);
	cursor_x = user_input_x;
	cursor_y = user_input_y;
	jcTerminalPrint(_terminal, &cursor_x, &cursor_y, user_input.c_str(), user_input.size());
	jcTerminalRefresh(_terminal);
};


int main()
{
	auto _terminal = jcTerminalOpen(screen_width, screen_height, "Welcome to Chillis");
	jcTerminalSetCellSize(_terminal, 10, 18);

	jcTerminalSetCloseCallback(_terminal, tclose_callback);
	jcTerminalSetKeyCallback(_terminal, tkey_callback);
	jcTerminalSetTextCallback(_terminal, ttext_callback);

	auto _hackIndex = jcTerminalLoadFont(_terminal, hack_font_path_v, 0, 512);
	jcTerminalLoadPNG(_terminal, SOURCE_ROOT "/Tree.png", 1420);
	jcTerminalRefresh(_terminal);

	jcTerminalSetDefaultTextColor(_terminal, text_color, text_background_color);
	jcTerminalPrintString(_terminal, 1, 1, "Makin Pancakes got \033[0;33mbig gay");
	jcTerminalRefresh(_terminal);

	jcTerminalPut(_terminal, 10, 10, 1420);
	jcTerminalPut(_terminal, 12, 10, 1420);
	jcTerminalPut(_terminal, 14, 10, 1420);
	jcTerminalRefresh(_terminal);

	while (keep_refreshing_terminal_v) 
	{
		jcTerminalDrawUntilEvent(_terminal, 0.5);
		jcTerminalRefresh(_terminal);
	};

	jcTerminalClose(&_terminal);
	assert(_terminal == nullptr);

	return 0;
};