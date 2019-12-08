#include "BossJafar.h"
#include "define.h"
#include "Game.h"
#include "Aladdin.h"

void BossJafar::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + JAFAR_BBOX_WIDTH;

	if (state == JAFAR_STATE_DIE)
		bottom = y + JAFAR_BBOX_HEIGHT_DIE;
	else
		bottom = y + JAFAR_BBOX_HEIGHT;
}

void BossJafar::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;
	if (GetHP() == 0)
	{
		SetSpeed(0.0f, 0.0f);
		return;
	}

	if (isHurt && GetTickCount() - hurtTime >= 600)
	{
		isHurt = false;
		isAttack = true;
	}


	float checkX = this->x - Aladdin::XforGet;
	float checkY = -this->y + Aladdin::YforGet;
	if (checkX < 100)
		this->SetState(JAFAR_STATE_ATTACK);
	if (checkX > 0) nx = -1;
	else nx = 1;
	////if (check < 0)
	////{
	////	nx = -nx;
	////	this->SetState(JAFAR_STATE_IDLE);
	////}

}

void BossJafar::Render()
{
	int ani;

	if (GetHP() == 0)
	{

		return;
	}
	if (isHurt)
	{
		if (nx > 0) ani = JAFAR_ANI_HURT_LEFT;
		else
			ani = JAFAR_ANI_HURT_LEFT;
	}
	if (state == JAFAR_STATE_IDLE)
	{
		if (nx > 0) ani = JAFAR_ANI_IDLE_RIGHT;
		else
			ani = JAFAR_ANI_IDLE_LEFT;
	}
	else
		if (isAttack) {
			if (nx > 0)
				ani = JAFAR_ANI_ATTACKING_RIGHT;
			else
				ani = JAFAR_ANI_ATTACKING_LEFT;

		}
	animations[ani]->Render(x, y);
	RenderBoundingBox();
}

void BossJafar::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case JAFAR_STATE_DIE:
		y += JAFAR_BBOX_HEIGHT - JAFAR_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		isAttack = false;
		isHurt = false;
		break;
	case JAFAR_STATE_IDLE:
		vx = 0;
		vy = 0;
		isHurt = false;
		isAttack = false;
		break;
	case JAFAR_STATE_HURT:
		DescHP();
		isHurt = true;
		vx = 0;
		vy = 0;
		isAttack = false;
		hurtTime = GetTickCount();
		break;
	case JAFAR_STATE_ATTACK:
		isAttack = true;
		vx = 0;
		isHurt = false;
		break;
	}

}