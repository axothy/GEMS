#pragma once
#include "gametable.h"

class Bomb : public Elem {
public:
	Bomb(position place);
	void DrawElem();
	//virtual rgb GetColor() = 0;
};