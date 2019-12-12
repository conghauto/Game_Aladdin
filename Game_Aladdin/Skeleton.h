#pragma once
#include "GameObject.h"



class Skeleton : public CGameObject
{
	int HP;
	float preX;
	float preY;
public:
	DWORD hurtTime;
	bool isHurt;
	bool isDead;
	int untouchable;
	bool isAttack;
public:
	Skeleton()
	{
		HP = 2;
	}
	virtual void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	int GetHP() { return HP; }
	float GetX() { return this->preX; };
	float GetY() { return this->preY; };
	void DescHP() { HP = HP - 1; }
};

