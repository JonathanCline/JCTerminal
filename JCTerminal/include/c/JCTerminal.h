#pragma once
#ifndef JCTERMINAL_H
#define JCTERMINAL_H

struct jcTerminal;

extern "C"
{

	jcTerminal* jcTerminalOpen(int _cellsX, int _cellsY, const char* _title);
	void jcTerminalRefresh(jcTerminal* _terminal);
	void jcTerminalClose(jcTerminal** _terminal);


	void jcTerminalDraw(jcTerminal* _terminal);
	void jcTerminalDrawUntilEvent(jcTerminal* _terminal, double _timeoutSeconds = 0.100);


	void jcTerminalGetCellSize(const jcTerminal* _terminal, int* _width, int* _height);
	void jcTerminalSetCellSize(jcTerminal* _terminal, int _width, int _height);

	void jcTerminalGetWindowSize(const jcTerminal* _terminal, int* _widthCells, int* _heightCells);
	void jcTerminalSetWindowSize(jcTerminal* _terminal, int _widthCells, int _heightCells);



	int jcTerminalGetMaxTextureCount();

	int jcTerminalLoadFont(jcTerminal* _terminal, const char* _fontPath, unsigned short _startIndex = 0, unsigned short _glyphs = 256);
	int jcTerminalLoadPNG(jcTerminal* _terminal, const char* _path, unsigned short _setIndex);






	
	

	struct jcTerminal_Color
	{
		unsigned char r = 0;
		unsigned char g = 0;
		unsigned char b = 0;
		unsigned char a = 0;	
	};

	void jcTerminalSetColor(jcTerminal* _terminal, int _x, int _y, jcTerminal_Color _color);
	jcTerminal_Color jcTerminalGetColor(const jcTerminal* _terminal, int _x, int _y);

	void jcTerminalSetBackgroundColor(jcTerminal* _terminal, int _x, int _y, jcTerminal_Color _color);
	jcTerminal_Color jcTerminalGetBackgroundColor(const jcTerminal* _terminal, int _x, int _y);

	void jcTerminalPut(jcTerminal* _terminal, int _x, int _y, unsigned short _tindex);
	unsigned short jcTerminalGet(const jcTerminal* _terminal, int _x, int _y);


	void jcTerminalSetDefaultTextColor(jcTerminal* _terminal, jcTerminal_Color _textCol, jcTerminal_Color _background);

	// If _len is set to 0, string length will be calculated using strlen()
	void jcTerminalPrint(jcTerminal* _terminal, int* _x, int* _y, const char* _string, int _len = 0);

	// Same as jcTerminalPrint but does not provide a way of seeing where the cursor finished
	void jcTerminalPrintString(jcTerminal* _terminal, int _x, int _y, const char* _string, int _len = 0);



	void jcTerminalFillRect(jcTerminal* _terminal, int _x0, int _y0, int _x1, int _y1, jcTerminal_Color _color);



	void jcTerminalClear(jcTerminal* _terminal);




	typedef void(*jcTerminal_CloseCallback)(jcTerminal* _terminal);
	void jcTerminalSetCloseCallback(jcTerminal* _terminal, jcTerminal_CloseCallback _callback);

	typedef void(*jcTerminal_TextCallback)(jcTerminal* _terminal, unsigned short _codepoint);
	void jcTerminalSetTextCallback(jcTerminal* _terminal, jcTerminal_TextCallback _callback);



	enum jcTerminal_Key
	{
		JCT_KEY_0 = 48,
		JCT_KEY_1,
		JCT_KEY_2,
		JCT_KEY_3,
		JCT_KEY_4,
		JCT_KEY_5,
		JCT_KEY_6,
		JCT_KEY_7,
		JCT_KEY_8,
		JCT_KEY_9 = 57,

		JCT_KEY_W = 87,
		JCT_KEY_A = 65,
		JCT_KEY_S = 83,
		JCT_KEY_D = 68,

		JCT_KEY_LEFT_BRACKET = 91,
		JCT_KEY_RIGHT_BRACKET = 93,

		JCT_KEY_ESCAPE = 256,
		JCT_KEY_ENTER = 257,
		JCT_KEY_TAB = 258,
		JCT_KEY_BACKSPACE = 259,

		JCT_KEY_LEFT = 263,
		JCT_KEY_RIGHT = 262,
		JCT_KEY_UP = 265,
		JCT_KEY_DOWN = 264,

		JCT_KEY_CAPS_LOCK = 280,

		JCT_KEY_LEFT_SHIFT = 340,
		JCT_KEY_LEFT_CONTROL = 341,
		JCT_KEY_RIGHT_SHIFT = 344,
		JCT_KEY_RIGHT_CONTROL = 345,

	};



	enum jcTerminal_Action
	{
		JCT_RELEASE = 0,
		JCT_PRESS = 1,
		JCT_REPEAT = 2
	};

	typedef void(*jcTerminal_KeyCallback)(jcTerminal* _terminal, jcTerminal_Key _key, jcTerminal_Action _action);
	void jcTerminalSetKeyCallback(jcTerminal* _terminal, jcTerminal_KeyCallback _callback);

	// Returns true if key is currently pressed
	bool jcTerminalGetKey(jcTerminal* _terminal, jcTerminal_Key _key);




};

#endif