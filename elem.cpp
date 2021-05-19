#include "elem.h"

Elem::Elem(const ColorName& name, const position& place) {
	color = colors::pallete[name];
	pos = place;
}

Elem::Elem(const rgb& newColor, const position& place) {
	color = newColor;
	pos = place;
}


void Elem::DrawFrame() {
	glLineWidth(2);
	glBegin(GL_LINE_STRIP);
	glColor3f(1.0f, 0.5f, 0.0f);
	glVertex2f(0, 0);
	glVertex2f(0, 0.98f);
	glVertex2f(0.98f, 0.98f);
	glVertex2f(0.98f, 0);
	glVertex2f(0, 0);
	glEnd();
}


rgb Elem::GetColor() {
	return { 0,0,0 };
}

