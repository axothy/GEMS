#pragma once
#include "bomb.h"
#include "recolor.h"
#include "pallete.h"
#include "elem.h"
#include "quad.h"
#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <stack>
#include <windows.h>
#include <gl/gl.h>
#include <math.h>

const int mapW = 10;
const int mapH = 10;



struct position {
	int x, y;
};


class GameTable
{
public:
	std::shared_ptr<Elem>** block;
	position** pos;

	GameTable(int mapH, int mapW);
	void PushUpAndRecolour(position posToUp);
	inline void PushStack(std::stack<position>& stackToUp);
	inline void FreeStack(std::stack<position>& stackToFree);
	bool IsColorEqColor(rgb currentColor, position posElem);
	void KaBOOM();
	void REColorBonus();
	void BuildTable();
	void CheckRowsTable();
	void CheckColumnsTable();
	void ReplaceElem();
};