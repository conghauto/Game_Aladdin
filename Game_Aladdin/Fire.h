#pragma once
#include "GameObject.h"



class Fire : public CGameObject
{
	int HP;
public:
	DWORD hurtTime;
	bool isHurt;
	bool isDead;
	int untouchable;
	bool isAttack;
public:
	Fire()
	{
		HP = 5;
	}
	virtual void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	int GetHP() { return HP; }
	void SetHP(int hp) { HP = hp; }
	void DescHP() { HP = HP - 1; }
};

