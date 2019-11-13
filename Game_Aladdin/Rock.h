#pragma once
#include "GameObject.h"
#include"define.h"


class Rock : public CGameObject
{
private:
	DWORD firstRenderTime;
public:
	bool isHitted;
	float width, height;
	Rock() :CGameObject() {
		isHitted = false;
		width = 0;
		height = 0;
	}
	void SetBound();
	void SetHit();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

