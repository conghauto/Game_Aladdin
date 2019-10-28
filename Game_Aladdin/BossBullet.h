#pragma once
#include "Weapon.h"

class BossBullet : public Weapon
{

public:
	//Bullet(Captain *captain, float max_width = SCREEN_WIDTH) { this->captain = captain; this->max_width = max_width; }
	BossBullet(BossOne *bossone, float max_width = SCREEN_WIDTH) { this->bossone = bossone; this->max_width = max_width; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
};