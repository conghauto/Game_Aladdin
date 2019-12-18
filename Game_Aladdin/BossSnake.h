#pragma once
#include "GameObject.h"



class BossSnake : public CGameObject
{
	int HP;
public:
	DWORD hurtTime;
	bool isHurt;
	bool isDead;
	int untouchable;
	bool isAttack;
	float time_start_shoot;
public:
	BossSnake()
	{
		this->time_start_shoot = GetTickCount();
		HP = 20;
	}
	virtual void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	int GetHP() { return HP; }
	void DescHP() { HP = HP - 1; }
};

