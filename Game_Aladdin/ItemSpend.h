#pragma once
#include "GameObject.h"
#include"define.h"


class ItemSpend : public CGameObject
{
public:
	bool isEaten;
public:
	ItemSpend(){
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

