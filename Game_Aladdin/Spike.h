#pragma once
#include "GameObject.h"
#include"define.h"


class Spike : public CGameObject
{
private:
	DWORD firstRenderTime;
public:
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

