#pragma once
#include "GameObject.h"
#include "Whip.h"
#include "define.h"

class Aladdin : public CGameObject
{
	int action;
	int level;
	int untouchable;
	DWORD untouchable_start;

public:

	bool isDead;
	bool isHurt;
	bool isAttack;
	bool isDoubleJump;
	bool isHoldApple;
	bool isOnCheckStairUp;
	bool isOnCheckStairDown;
	bool isClimbUp;
	bool isClimbDown;
	bool isOnStair;
	bool isOnRope;
	bool isLeftToRight;
	bool isUseWhip;
	bool isSit;
	bool isExitSit;
	bool isDashing;
	bool isJump;
	bool isThrow;

	bool isMoving;

	DWORD attackTime;
	DWORD onCheckPointTime;
	int ny;

	int life;
	int preHP;
	bool isLevelUp;
	bool isBonus;
	float timePressJump;
	float distanceDash;
	float distanceJump;
	int xStartDash;
	int yStartJump;

public:
	static float XforGet;
	static int score;
	static int heartsAmount;
	int currentWeapon;
	Aladdin() : CGameObject()
	{
		level = 1;
		untouchable = 0;
		life = 3;
		preHP = 16;
		currentWeapon = 0;
		isUseWhip = true;
	}

	virtual void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void SetCurrentWeapon(int weapon) { currentWeapon = weapon; }
	int GetCurrentWeapon() { return currentWeapon; }

	void SetPosition(float x, float y) {
		this->x = x;
		this->y = y;
	}
	void IncHeart(int num) { heartsAmount += num; }
	void IncScore(int num) { score += num; }
	void SetState(int state);

	void SetAction(int action);
	int GetAction() { return action; }
	int GetDistanceDash() { return abs(this->x - xStartDash); }
	int GetDistanceJump() { return abs(this->y - yStartJump); }

	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	int GetScore() { return score; }
	int GetLife() { return life; }
	int GetHP() { return preHP; }
	int GetHeart() { return heartsAmount; }
	float GetX() { return XforGet; };
};