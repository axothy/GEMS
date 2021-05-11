#pragma once
#include "gametable.h"

class Quad : public Elem
{
public:
	Quad(ColorName name, position place);
	Quad(rgb newColor, position place);
	void DrawElem();
};
