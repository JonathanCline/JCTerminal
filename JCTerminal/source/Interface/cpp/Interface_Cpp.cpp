#include "Interface/Interface.h"

#include "cpp/JCTerminal.hpp"



namespace jct
{

	jcTerminal* open(int _cellsX, int _cellsY, const char* _title)
	{
		return jcTerminal_open(_cellsX, _cellsY, _title);
	};
	void refresh(jcTerminal* _terminal)
	{
		jcTerminal_refresh(_terminal);
	};
	void close(jcTerminal*& _terminal)
	{
		jcTerminal_close(&_terminal);
	};

	CellSize getCellSize(jcTerminal* _terminal)
	{
		CellSize _csize{};
		jcTerminal_getCellSize(_terminal, &_csize.width, &_csize.height);
		return _csize;
	};
	void setCellSize(jcTerminal* _terminal, CellSize _csize)
	{
		setCellSize(_terminal, _csize.width, _csize.height);
	};
	void setCellSize(jcTerminal* _terminal, int _width, int _height)
	{
		jcTerminal_setCellSize(_terminal, _width, _height);
	};

	WindowSize getWindowSize(jcTerminal* _terminal)
	{
		WindowSize _csize{};
		jcTerminal_getWindowSize(_terminal, &_csize.width, &_csize.height);
		return _csize;
	};
	void setWindowSize(jcTerminal* _terminal, int _widthCells, int _heightCells)
	{
		jcTerminal_setWindowSize(_terminal, _widthCells, _heightCells);
	};
	void setWindowSize(jcTerminal* _terminal, WindowSize _size)
	{
		setWindowSize(_terminal, _size.width, _size.height);
	};


}
