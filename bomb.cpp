#include "bomb.h"

Bomb::Bomb(position place) : Elem(BLACK, place) {}

void Bomb::DrawElem() {
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0, 0, 0);
	float x, y;
	float cnt = 20;
	float r = 0.5;
	float alpha = 3.1415926f * 2.0f / cnt;
	for (int i = -1; i < cnt; i++) {
		x = sin(alpha * i) * r;
		y = cos(alpha * i) * r;
		glVertex2f(x + 0.5f, y + 0.5f);
	}
	glEnd();
}