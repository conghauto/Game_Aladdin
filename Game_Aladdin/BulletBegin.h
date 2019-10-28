#pragma once
#include "GameObject.h"



class BulletBegin : public CGameObject
{

public:
	BulletBegin()
	{
		this->time_start_shoot = GetTickCount();
	}
	int countCol;
	float time_start_shoot;
	float time_end_start;
	virtual void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
};