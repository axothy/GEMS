#include "recolor.h"

reColour::reColour(const rgb& prevCol, const position& place) : Elem(WHITE, place) {
	lastColor = prevCol;
}


void reColour::DrawElem() {
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1, 1, 1);
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
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.25f, 0.25f);
	glColor3f(0.5f * lastColor.red, 0.5f * lastColor.green, 0.5f * lastColor.blue);
	glVertex2f(0.75f, 0.25f);
	glColor3f(lastColor.red, lastColor.green, lastColor.blue);
	glVertex2f(0.75f, 0.75f);
	glColor3f(0.5f * lastColor.red, 0.5f * lastColor.green, 0.5f * lastColor.blue);
	glVertex2f(0.25f, 0.75f);
	glEnd();
}

rgb reColour::GetColor() {
	return lastColor;
}