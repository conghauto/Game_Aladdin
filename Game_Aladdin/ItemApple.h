#pragma once
#include "GameObject.h"
#include"define.h"


class ItemApple : public CGameObject
{
public :
	bool isEaten;
public:
	ItemApple() {
		isEaten = false;
	}
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

