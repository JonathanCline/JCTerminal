#pragma once
#ifndef JCTERMINAL_HPP
#define JCTERMINAL_HPP

namespace jct
{
	int open(int _cellsX, int _cellsY, const char* _title);
	void refresh();
	void close();

	struct CellSize
	{
		int width;
		int height;
	};

	CellSize getCellSize();
	void setCellSize(CellSize _size);
	void setCellSize(int _width, int _height);

	struct WindowSize
	{
		int width;
		int height;
	};

	WindowSize getWindowSize();
	void setWindowSize(int _widthCells, int _heightCells);
	void setWindowSize(WindowSize _size);

};

#endif