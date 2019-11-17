#pragma once
#include "GameObject.h"



class Bat : public CGameObject
{
public:
	Bat()
	{
		this->time_start_shoot = GetTickCount();
	}
	float time_start_shoot;
	virtual void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
};

