
#include <c/JCTerminal.h>

#include <SAELib_Timer.h>

#include <chrono>
#include <iostream>
#include <cassert>


constexpr auto hack_font_path_v = SOURCE_ROOT "/../../assets/fonts/Hack/Hack-Regular.ttf";

bool keep_refreshing_terminal_v = true;

const int screen_width = 96;
const int screen_height = 48;

const jcTerminal_Color text_color{ 255, 0, 100, 255 };
const jcTerminal_Color text_background_color{ 0, 0, 0, 255 };

void tclose_callback(jcTerminal* _terminal)
{
	keep_refreshing_terminal_v = false;
};
void tkey_callback(jcTerminal* _terminal, jcTerminal_Key _key, jcTerminal_Action _action)
{
	
};


std::string user_input = "";
int user_input_x = 4;
int user_input_y = 4;

void ttext_callback(jcTerminal* _terminal, unsigned short _codepoint)
{
	user_input.push_back(_codepoint);
	jcTerminalPrint(_terminal, user_input_x, user_input_y, user_input.c_str(), user_input.size());
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
	jcTerminalPrint(_terminal, 1, 1, "Makin Pancakes got big gay");
	jcTerminalRefresh(_terminal);

	jcTerminalPut(_terminal, 10, 10, 1420);
	jcTerminalPut(_terminal, 12, 10, 1420);
	jcTerminalPut(_terminal, 14, 10, 1420);
	jcTerminalRefresh(_terminal);

	while (keep_refreshing_terminal_v) 
	{
		jcTerminalDrawUntilEvent(_terminal);
	};

	jcTerminalClose(&_terminal);
	assert(_terminal == nullptr);

	return 0;
};