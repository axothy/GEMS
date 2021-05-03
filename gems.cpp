#include "gems.hpp"

namespace colors {
	rgb pallete[8] = {
		{0.0f, 0.0f, 0.0f},
		{1.0f, 1.0f, 1.0f},
		{1.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f},
		{1.0f, 1.0f, 0.0f},
		{1.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 1.0f},
	};
};

Elem::Elem(ColorName name, position place) {
	color = colors::pallete[name];
	pos = place;
}

Elem::Elem(rgb newColor, position place) {
	color = newColor;
	pos = place;
}

Quad::Quad(rgb newColor, position place) : Elem(newColor, place) {}

Quad::Quad(ColorName name, position place) : Elem(name, place) {}

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

reColour::reColour(rgb prevCol, position place) : Elem(WHITE, place) {
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


GameTable::GameTable(int mapH, int mapW) {
	pos = new position * [mapH];
	block = new std::shared_ptr<Elem> * [mapH];
	for (int i = 0; i < mapH; i++) {
		pos[i] = new position[mapW];
		block[i] = new std::shared_ptr<Elem>[mapW];
	}

	for (int i = 0; i < mapW; i++)
	{
		for (int j = 0; j < mapH; j++)
		{
			ColorName rng = ColorName(RED + ColorName(rand()) % (CYAN - RED + 1));
			pos[i][j] = { i, j };
			block[i][j] = std::shared_ptr<Elem>(new Quad(rng, pos[i][j]));
		}
	}
}

void GameTable::BuildTable()
{
	glLoadIdentity();
	glScalef(2.0f / mapW, 2.0f / mapH, 1);
	glTranslatef(-mapW / 2.0f, -mapH / 2.0f, 0);

	for (int i = 0; i < mapW; i++)
	{
		for (int j = 0; j < mapH; j++)
		{
			glPushMatrix();
			glTranslatef(float(i), float(j), 0);
			block[i][j]->DrawElem();
			if (block[i][j]->select) {
				block[i][j]->DrawFrame();
			}
			glPopMatrix();
		}
	}
}

void GameTable::ReplaceElem()
{
	int dx = 1;
	int dy = 1;

	for (int i = 0; i < mapW; i++) {
		for (int j = 0; j < mapH; j++) {
			if (block[i][j]->select) {
				if (i != mapH - 1 && block[i + dx][j]->select)
				{
					std::shared_ptr<Elem> swap = block[i][j];
					block[i][j] = block[i + dx][j];
					block[i][j]->select = false;
					block[i + dx][j] = swap;
					block[i + dx][j]->select = false;
				}
				else if (j != mapH - 1 && block[i][j + dy]->select)
				{
					std::shared_ptr<Elem> swap = block[i][j];
					block[i][j] = block[i][j + dy];
					block[i][j]->select = false;
					block[i][j + dy] = swap;
					block[i][j + dy]->select = false;
				}
			}
		}
	}
}

void GameTable::KaBOOM() {
	std::shared_ptr<Elem> swap;
	int x1, y1, y2;
	for (int x = 0; x < mapW; x++) {
		for (int y = 0; y < mapH; y++) {
			if (block[x][y]->select && IsColorEqColor(colors::pallete[BLACK], { x,y })) {
				block[x][y]->select = false;
				for (y2 = y; y2 < mapH - 1; y2++)
				{
					swap = block[x][y2];
					block[x][y2] = block[x][y2 + 1];
					block[x][y2 + 1] = swap;
				}
				block[x][y2]->~Elem();
				block[x][y2] = std::shared_ptr<Elem>(new Quad(ColorName(RED + ColorName(rand()) % (CYAN - RED + 1)), pos[x][y2]));
				for (int i = 0; i < 5; i++) {
					x1 = rand() % mapW;
					y1 = rand() % mapH;
					PushUpAndRecolour({ x1, y1 });
				}
			}

		}
	}
}

rgb Elem::GetColor() {
	return { 0,0,0 };
}

rgb reColour::GetColor() {
	return lastColor;
}

void GameTable::REColorBonus() {
	for (int x = 0; x < mapW; x++) {
		for (int y = 0; y < mapH; y++) {
			if (block[x][y]->select && IsColorEqColor(colors::pallete[WHITE], { x,y })) {
				rgb recolorSave;
				int x1 = x, y1 = y;
				block[x][y]->select = false;
				recolorSave = block[x][y]->GetColor();
				block[x][y]->~Elem();
				block[x][y] = std::shared_ptr<Elem>(new Quad(recolorSave, pos[x][y]));
				for (int i = 0; i < 2; i++) {
					int rng1 = 100, rng2 = 100;
					while (x + rng1 >= mapW || x + rng1 < 0 || y + rng2 >= mapH || y + rng2 < 0) {
						rng1 = 1 + rand() % 2;
						rng2 = 1 + rand() % 2;
						if (rand() % 2) {
							rng1 = -rng1;
						}
						if (rand() % 2) {
							rng2 = -rng2;
						}
					}
					block[x + rng1][y + rng2]->color = recolorSave;
				}
			}
		}
	}
}

Elem::~Elem() {
}


void GameTable::PushUpAndRecolour(position posToUp) {
	std::shared_ptr<Elem> swap;
	int x = posToUp.x, y = posToUp.y;
	if (IsColorEqColor(colors::pallete[BLACK], { x,y })) {
		block[x][y]->select = true;
		KaBOOM();
	}
	if (rand() % 10 == 1) {
		if (y == mapH - 1) {
			rgb recolorSave = block[x][y]->color;
			block[x][y]->~Elem();
			if (rand() % 2 == 1) {
				block[x][y] = std::shared_ptr<Elem>(new Bomb({ x,y }));
			}
			else
				block[x][y] = std::shared_ptr <Elem>(new reColour(recolorSave, { x, y }));
			return;
		}
		else {
			rgb recolorSave = block[x][y]->color;
			block[x][y + 1]->~Elem();
			if (rand() % 2 == 1) {
				block[x][y + 1] = std::shared_ptr<Elem>(new Bomb({ x,y + 1 }));
			}
			else
				block[x][y + 1] = std::shared_ptr <Elem>(new reColour(recolorSave, { x, y + 1 }));
		}
	}
	for (; y < mapH - 1; y++)
	{
		swap = block[x][y];
		block[x][y] = block[x][y + 1];
		block[x][y + 1] = swap;
	}
	block[x][y]->color = colors::pallete[ColorName(RED + ColorName(rand()) % (CYAN - RED + 1))];
}

inline void GameTable::PushStack(std::stack<position>& stackToUp) {
	if (stackToUp.size() >= 3) {
		while (!stackToUp.empty()) {
			PushUpAndRecolour(stackToUp.top());
			stackToUp.pop();
		}
	}
}

inline void GameTable::FreeStack(std::stack<position>& stackToFree) {
	while (!stackToFree.empty()) {
		stackToFree.pop();
	}
}

bool GameTable::IsColorEqColor(rgb currentColor, position posElem) {
	return (currentColor.red == block[posElem.x][posElem.y]->color.red &&
		currentColor.green == block[posElem.x][posElem.y]->color.green &&
		currentColor.blue == block[posElem.x][posElem.y]->color.blue);
}

void GameTable::CheckColumnsTable() {
	std::stack<position> posToUp;
	rgb currentColor;
	for (int x = 0; x < mapW; x++) {
		PushStack(posToUp);
		FreeStack(posToUp);
		currentColor = block[x][0]->color;
		posToUp.push({ x,0 });
		for (int y = 1; y < mapH; y++) {
			if (IsColorEqColor(currentColor, { x,y }))
			{
				posToUp.push({ x,y });
			}
			else
			{
				PushStack(posToUp);
				FreeStack(posToUp);
				currentColor = block[x][y]->color;
				posToUp.push({ x,y });
			}
		}
	}
	PushStack(posToUp);
}

void GameTable::CheckRowsTable()
{
	std::stack<position> posToUp;
	rgb currentColor;
	for (int y = 0; y < mapH; y++) {
		PushStack(posToUp);
		FreeStack(posToUp);

		currentColor = block[0][y]->color;
		posToUp.push({ 0,y });
		for (int x = 1; x < mapW; x++) {
			if (IsColorEqColor(currentColor, { x,y }))
			{
				posToUp.push({ x,y });
			}
			else
			{
				PushStack(posToUp);
				FreeStack(posToUp);

				currentColor = block[x][y]->color;
				posToUp.push({ x,y });
			}
		}
	}
	PushStack(posToUp);
}
