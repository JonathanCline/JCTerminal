#include "Interface/Interface.h"

#include "cpp/JCTerminal.hpp"

namespace jct
{

	int open(int _cellsX, int _cellsY, const char* _title)
	{
		return jcTerminal_open(_cellsX, _cellsY, _title);
	};
	void refresh()
	{
		jcTerminal_refresh();
	};
	void close()
	{
		jcTerminal_close();
	};

	CellSize getCellSize()
	{
		CellSize _csize{};
		jcTerminal_getCellSize(&_csize.width, &_csize.height);
		return _csize;
	};
	void setCellSize(CellSize _csize)
	{
		setCellSize(_csize.width, _csize.height);
	};
	void setCellSize(int _width, int _height)
	{
		jcTerminal_setCellSize(_width, _height);
	};

	WindowSize getWindowSize()
	{
		WindowSize _csize{};
		jcTerminal_getWindowSize(&_csize.width, &_csize.height);
		return _csize;
	};
	void setWindowSize(int _widthCells, int _heightCells)
	{
		jcTerminal_setWindowSize(_widthCells, _heightCells);
	};
	void setWindowSize(WindowSize _size)
	{
		setWindowSize(_size.width, _size.height);
	};


}
