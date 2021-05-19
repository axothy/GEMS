#pragma once
#include "gametable.h"

class Quad : public Elem
{
public:
	Quad(const ColorName& name, const position& place);
	Quad(const rgb& newColor, const position& place);
	void DrawElem();
};
