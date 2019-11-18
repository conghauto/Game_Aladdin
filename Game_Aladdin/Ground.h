#pragma once
#include "GameObject.h"
#include"define.h"


class Ground : public CGameObject
{
public:
	float width, height;
	bool checkCollision=true;
	//Ground() :CGameObject() {
	//	width = 32;
	//	height = 1;
	//	/*SetBound();*/
	//}


	void SetBound();
	/*void SetBoundNoCollision() { width = 0; height = 0; }*/
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

