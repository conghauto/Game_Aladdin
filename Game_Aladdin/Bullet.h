#pragma once
#include "Weapon.h"

class Bullet : public Weapon
{

public:
	//Bullet(Aladdin *aladdin, float max_width = SCREEN_WIDTH) { this->aladdin = aladdin; this->max_width = max_width; }
	Bullet(Zombie *zombie, float max_width = SCREEN_WIDTH) { this->zombie = zombie; this->max_width = max_width; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
};