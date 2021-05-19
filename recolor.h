#pragma once
#include "gametable.h"

class reColour : public Elem {
public:
	rgb lastColor;
	reColour(const rgb& prevCol, const position& place);
	void DrawElem();
	rgb GetColor();
};
