#include "Interface/Interface.h"

#include "cpp/JCTerminal.hpp"



namespace jct
{

	jcTerminal* open(int _cellsX, int _cellsY, const char* _title)
	{
		return jcTerminalOpen(_cellsX, _cellsY, _title);
	};
	void refresh(jcTerminal* _terminal)
	{
		jcTerminalRefresh(_terminal);
	};
	void close(jcTerminal*& _terminal)
	{
		jcTerminalClose(&_terminal);
	};

	CellSize getCellSize(jcTerminal* _terminal)
	{
		CellSize _csize{};
		jcTerminalGetCellSize(_terminal, &_csize.width, &_csize.height);
		return _csize;
	};
	void setCellSize(jcTerminal* _terminal, CellSize _csize)
	{
		setCellSize(_terminal, _csize.width, _csize.height);
	};
	void setCellSize(jcTerminal* _terminal, int _width, int _height)
	{
		jcTerminalSetCellSize(_terminal, _width, _height);
	};

	WindowSize getWindowSize(jcTerminal* _terminal)
	{
		WindowSize _csize{};
		jcTerminalGetWindowSize(_terminal, &_csize.width, &_csize.height);
		return _csize;
	};
	void setWindowSize(jcTerminal* _terminal, int _widthCells, int _heightCells)
	{
		jcTerminalSetWindowSize(_terminal, _widthCells, _heightCells);
	};
	void setWindowSize(jcTerminal* _terminal, WindowSize _size)
	{
		setWindowSize(_terminal, _size.width, _size.height);
	};


}
