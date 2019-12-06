#pragma once
#include "Weapon.h"
#include "BossSnake.h"

class FireBullet : public Weapon
{

public:
	FireBullet(BossSnake *snake, float max_width = SCREEN_WIDTH) { this->snake = snake; this->max_width = max_width; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
};