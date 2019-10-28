#pragma once
#include "GameObject.h"



class BossOne : public CGameObject
{

public:
	BossOne(){
		this->time_start_shoot = GetTickCount();
	}
	bool isMoving;
	bool isDropping;
	bool isFlying;
	float time_start_shoot;
	float time_start_walking;
	virtual void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	virtual void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
};