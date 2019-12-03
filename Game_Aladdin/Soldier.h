#pragma once
#include "GameObject.h"



class Soldier : public CGameObject
{
	int HP;
public:
	DWORD hurtTime;
	bool isHurt;
	bool isDead;
	int untouchable;
	bool isAttack;
public:
	Soldier()
	{
		HP = 12;
	}
	virtual void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	int GetHP() { return HP; }
	void DescHP() { HP = HP - 1; }
};