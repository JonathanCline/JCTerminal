#pragma once
#ifndef JCTERMINAL_HPP
#define JCTERMINAL_HPP

struct jcTerminal;

namespace jct
{
	jcTerminal* open(int _cellsX, int _cellsY, const char* _title);
	void refresh(jcTerminal* _terminal);
	void close(jcTerminal*& _terminal);


	struct CellSize
	{
		int width;
		int height;
	};

	CellSize getCellSize(jcTerminal* _terminal);
	void setCellSize(jcTerminal* _terminal, CellSize _size);
	void setCellSize(jcTerminal* _terminal, int _width, int _height);


	struct WindowSize
	{
		int width;
		int height;
	};

	WindowSize getWindowSize(jcTerminal* _terminal);
	void setWindowSize(jcTerminal* _terminal, int _widthCells, int _heightCells);
	void setWindowSize(jcTerminal* _terminal, WindowSize _size);


	



};

#endif