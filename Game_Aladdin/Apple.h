#pragma once
#include "Weapon.h"

class Apple : public Weapon
{

public:
	Apple(Aladdin *aladdin, float max_width = SCREEN_WIDTH) { this->aladdin = aladdin; this->max_width = max_width; }
	Apple(Soldier *soldier, float max_width = SCREEN_WIDTH) { this->soldier = soldier; this->max_width = max_width; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
};