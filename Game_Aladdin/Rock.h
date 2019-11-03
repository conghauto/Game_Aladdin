#pragma once
#include "GameObject.h"
#include"define.h"


class Rock : public CGameObject
{
private:
	DWORD firstRenderTime;
public:
	Rock() { objectRock = true; }
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

