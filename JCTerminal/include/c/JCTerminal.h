#pragma once
#ifndef JCTERMINAL_H
#define JCTERMINAL_H

struct jcTerminal;


jcTerminal* jcTerminal_open(int _cellsX, int _cellsY, const char* _title);

void jcTerminal_refresh(jcTerminal* _terminal);
void jcTerminal_close(jcTerminal** _terminal);

void jcTerminal_getCellSize(jcTerminal* _terminal, int* _width, int* _height);
void jcTerminal_setCellSize(jcTerminal* _terminal, int _width, int _height);

void jcTerminal_getWindowSize(jcTerminal* _terminal, int* _widthCells, int* _heightCells);
void jcTerminal_setWindowSize(jcTerminal* _terminal, int _widthCells, int _heightCells);




typedef void(*jcTerminal_CloseCallback)(jcTerminal* _terminal);

void jcTerminal_setCloseCallback(jcTerminal* _terminal, jcTerminal_CloseCallback _callback);




#endif