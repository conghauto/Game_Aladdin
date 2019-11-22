#pragma once
#include "GameObject.h"
#include"define.h"


class Dumbbell : public CGameObject
{
private:
	DWORD firstRenderTime;
public:
	bool isHitted;
	float width, height;
	Dumbbell() :CGameObject() {
		isHitted = false;
		width = 0;
		height = 0;
	}
	void SetBound();
	void SetHit();
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

