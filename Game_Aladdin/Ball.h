#pragma once
#include "Weapon.h"

class Ball : public Weapon
{

public:
	//Bullet(Aladdin *aladdin, float max_width = SCREEN_WIDTH) { this->aladdin = aladdin; this->max_width = max_width; }
	Ball(Soldier *soldier, float max_width = SCREEN_WIDTH) { this->soldier = soldier; this->max_width = max_width; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
};