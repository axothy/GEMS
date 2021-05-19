#include "gametable.h"


GameTable::GameTable(int mapH, int mapW) {
	pos = new position * [mapH];
	block = new std::shared_ptr<Elem>*[mapH];
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
			block[i][j] = std::make_shared<Quad>(Quad(rng, pos[i][j]));
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

void GameTable::BOOM() {
	int x1, y1;
	for (int i = 0; i < 5; i++) {
		x1 = rand() % mapW;
		y1 = rand() % mapH;
		BrokeElem({ x1, y1 });
	}
}

void GameTable::KaBOOM() {
	for (int x = 0; x < mapW; x++) {
		for (int y = 0; y < mapH; y++) {
			if (block[x][y]->select && IsColorEqColor(colors::pallete[BLACK], { x,y })) {
				block[x][y]->select = false;
				PushUpAndRecolour({x, y});
				BOOM();
			}

		}
	}
}

inline void GetPosForReColor(int x, int y, int& rng1, int& rng2) {
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
}

void GameTable::REColorBonus() {
	for (int x = 0; x < mapW; x++) {
		for (int y = 0; y < mapH; y++) {
			if (block[x][y]->select && IsColorEqColor(colors::pallete[WHITE], { x,y })) {
				rgb recolorSave;
				block[x][y]->select = false;
				recolorSave = block[x][y]->GetColor();
				block[x][y].reset(new Quad(recolorSave, pos[x][y]));
				for (int i = 0; i < 2; i++) {
					int rng1 = 100, rng2 = 100;
					GetPosForReColor(x, y, rng1, rng2);
					block[x + rng1][y + rng2]->color = recolorSave;
				}
			}
		}
	}
}

void GameTable::PushUpAndRecolour(const position& posToUp)
{
	int x = posToUp.x, y = posToUp.y;
	
	for (; y < mapH - 1; y++)
	{
		std::swap(block[x][y], block[x][y + 1]);
	}
	block[x][y]->color = colors::pallete[ColorName(RED + ColorName(rand()) % (CYAN - RED + 1))];
}

const int BonusChance = 10;

const int BombChance = 2;

void GameTable::CreateBonus(const position& posToBonus) {
	int x = posToBonus.x, y = posToBonus.y;
	if (y == mapH - 1) {
		rgb recolorSave = block[x][y]->color;
		if (rand() % BombChance == 1) {
			block[x][y].reset(new Bomb({ x,y }));
		}
		else
			block[x][y].reset(new reColour(recolorSave, { x, y }));
		return;
	}
	else {
		rgb recolorSave = block[x][y]->color;
		if (rand() % BombChance == 1) {
			block[x][y + 1].reset(new Bomb({ x,y + 1 }));
		}
		else
			block[x][y + 1].reset(new reColour(recolorSave, { x, y + 1 }));
	}
}

void GameTable::BrokeElem(const position& posToUp) {
	std::shared_ptr<Elem> swap;
	int x = posToUp.x, y = posToUp.y;
	if (rand() % BonusChance == 1) {
		CreateBonus({ x,y });
	}
	PushUpAndRecolour({ x,y });
}

inline void GameTable::PushStack(std::stack<position>& stackToUp) {
	if (stackToUp.size() >= 3) {
		while (!stackToUp.empty()) {
			BrokeElem(stackToUp.top());
			stackToUp.pop();
		}
	}
}

inline void GameTable::FreeStack(std::stack<position>& stackToFree) {
	while (!stackToFree.empty()) {
		stackToFree.pop();
	}
}

bool GameTable::IsColorEqColor(const rgb& currentColor, const position& posElem) {
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
