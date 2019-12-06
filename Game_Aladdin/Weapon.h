#pragma once
#include "Item.h"
#include "Zombie.h"
#include "Aladdin.h"
#include "Soldier.h"
#include "BossSnake.h"
#include "BossJafar.h"

class Weapon : public Item
{

public:
	float max_width;
	float max_x;
	Aladdin *aladdin;
	Zombie *zombie;
	Soldier *soldier;
	BossSnake *snake;
	BossJafar *jafar;
	bool isActivate;
	DWORD firstCast;

	Weapon() {}

	Weapon(Aladdin *aladdin, float max_width = SCREEN_WIDTH) { this->aladdin = aladdin; this->max_width = max_width; }
	Weapon(BossSnake *snake, float max_width = SCREEN_WIDTH) { this->snake = snake; this->max_width = max_width; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render();
};


