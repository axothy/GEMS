#pragma once
#include "gametable.h"

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
