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

	int jcTerminalLoadFont(jcTerminal* _terminal, const char* _fontPath);
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


	void jcTerminalFillRect(jcTerminal* _terminal, int _x0, int _y0, int _x1, int _y1, jcTerminal_Color _color);







	typedef void(*jcTerminal_CloseCallback)(jcTerminal* _terminal);
	void jcTerminalSetCloseCallback(jcTerminal* _terminal, jcTerminal_CloseCallback _callback);




	enum jcTerminal_Key
	{
		JCT_KEY_W = 87,
		JCT_KEY_A = 65,
		JCT_KEY_S = 83,
		JCT_KEY_D = 68
	};

	enum jcTerminal_Action
	{
		JCT_RELEASE = 0,
		JCT_PRESS = 1,
		JCT_REPEAT = 2
	};

	typedef void(*jcTerminal_KeyCallback)(jcTerminal* _terminal, jcTerminal_Key _key, jcTerminal_Action _action);
	void jcTerminalSetKeyCallback(jcTerminal* _terminal, jcTerminal_KeyCallback _callback);




};

#endif