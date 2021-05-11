#pragma once
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

struct rgb {
	float red, green, blue;
};

enum ColorName {
	BLACK, WHITE, RED,
	GREEN, BLUE, YELLOW,
	PINK, CYAN
};

struct position {
	int x, y;
};


class Elem {
public:
	rgb color;
	position pos;
	bool select = false;
	Elem(ColorName name, position place);
	Elem(rgb newColor, position place);
	void DrawFrame();
	virtual void DrawElem() = 0;
	virtual rgb GetColor();

};

class Quad : public Elem
{
public:
	Quad(ColorName name, position place);
	Quad(rgb newColor, position place);
	void DrawElem();
};

class reColour : public Elem {
public:
	rgb lastColor;
	reColour(rgb prevCol, position place);
	void DrawElem();
	rgb GetColor();
};

class Bomb : public Elem {
public:
	Bomb(position place);
	void DrawElem();
	//virtual rgb GetColor() = 0;
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
