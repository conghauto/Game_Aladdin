#pragma once
#include "GameObject.h"

class Stars : public CGameObject
{
public:
	Stars()
	{
	}
	virtual void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void SetSpeed(float vx, float vy)
	{
		this->vx = vx;
		this->vy = vy;
	};
};

