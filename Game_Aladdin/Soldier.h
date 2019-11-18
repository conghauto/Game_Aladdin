#pragma once
#include "GameObject.h"



class Soldier : public CGameObject
{

public:
	Soldier()
	{
	}
	virtual void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
};