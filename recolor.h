#pragma once
#include "gametable.h"

class reColour : public Elem {
public:
	rgb lastColor;
	reColour(rgb prevCol, position place);
	void DrawElem();
	rgb GetColor();
};
