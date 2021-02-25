#pragma once
#ifndef JCTERMINAL_H
#define JCTERMINAL_H

int  jcTerminal_open(int _cellsX, int _cellsY, const char* _title);
void jcTerminal_refresh();
void jcTerminal_close();

void jcTerminal_getCellSize(int* _width, int* _height);
void jcTerminal_setCellSize(int _width, int _height);

void jcTerminal_getWindowSize(int* _widthCells, int* _heightCells);
void jcTerminal_setWindowSize(int _widthCells, int _heightCells);

#endif