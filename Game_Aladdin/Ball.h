#pragma once
#include "Weapon.h"

class Ball : public Weapon
{

public:
	//Bullet(Captain *captain, float max_width = SCREEN_WIDTH) { this->captain = captain; this->max_width = max_width; }
	Ball(Soldier *soldier, float max_width = SCREEN_WIDTH) { this->soldier = soldier; this->max_width = max_width; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
};