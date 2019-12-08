#pragma once
#include "Weapon.h"

class JafarBullet : public Weapon
{

public:
	//Bullet(Captain *captain, float max_width = SCREEN_WIDTH) { this->captain = captain; this->max_width = max_width; }
	JafarBullet(BossJafar *jafar, float max_width = SCREEN_WIDTH) { this->jafar = jafar; this->max_width = max_width; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
};