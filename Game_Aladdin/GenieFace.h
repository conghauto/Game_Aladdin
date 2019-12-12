#pragma once
#include "GameObject.h"
#include"define.h"


class GenieFace : public CGameObject
{
public:
	bool isEaten;
public:
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

