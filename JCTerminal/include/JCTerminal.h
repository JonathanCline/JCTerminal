#pragma once
#ifndef JCTERMINAL_H
#define JCTERMINAL_H



#ifdef __cplusplus
namespace jct
{
#endif

	int terminal_open(int _widthCells, int _heightCells, const char* _title);
	void terminal_refresh();
	void terminal_close();



#ifdef __cplusplus
};
#endif

#endif