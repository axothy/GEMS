#pragma once
#include "gametable.h"

class Elem {
public:
	rgb color;
	position pos;
	bool select = false;
	Elem(const ColorName& name, const position& place);
	Elem(const rgb& newColor, const position& place);
	void DrawFrame();
	virtual void DrawElem() = 0;
	virtual rgb GetColor();

};
