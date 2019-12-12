#pragma once
#include "GameObject.h"
#include"define.h"


class ItemHeart : public CGameObject
{
public:
	bool isEaten;
public:
	ItemHeart() {
		isEaten = false;
	}
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

