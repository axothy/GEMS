#include "quad.h"

Quad::Quad(const rgb& newColor, const position& place) : Elem(newColor, place) {}

Quad::Quad(const ColorName& name, const position& place) : Elem(name, place) {}

void Quad::DrawElem() {
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0, 0);
	glColor3f(0.5f * color.red, 0.5f * color.green, 0.5f * color.blue);
	glVertex2f(1, 0);
	glColor3f(color.red, color.green, color.blue);
	glVertex2f(1, 1);
	glColor3f(0.5f * color.red, 0.5f * color.green, 0.5f * color.blue);
	glVertex2f(0, 1);
	glEnd();
}

