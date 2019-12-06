#include "BossSnake.h"
#include "define.h"
#include "Game.h"
#include "Aladdin.h"

void BossSnake::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + SNAKE_BBOX_WIDTH;

	if (state == SNAKE_STATE_DIE)
		bottom = y + SNAKE_BBOX_HEIGHT_DIE;
	else
		bottom = y + SNAKE_BBOX_HEIGHT;
}

void BossSnake::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
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
	if (checkX > 0) nx = -1;
	else nx = 1;

}

void BossSnake::Render()
{
	int ani;

	if (GetHP() == 0)
	{

		return;
	}
	if (isHurt)
	{
		if (nx > 0) ani = SNAKE_ANI_HURT_LEFT;
		else
			ani = SNAKE_ANI_HURT_LEFT;
	}
		if (isAttack) {
			if (nx > 0)
				ani = SNAKE_ANI_ATTACKING_RIGHT;
			else
				ani = SNAKE_ANI_ATTACKING_LEFT;

		}
	animations[ani]->Render(x, y);
	RenderBoundingBox();
}

void BossSnake::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SNAKE_STATE_DIE:
		y += SNAKE_BBOX_HEIGHT - SNAKE_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		isAttack = false;
		isHurt = false;
		break;
	//case SNAKE_STATE_IDLE:
	//	vx = 0;
	//	vy = 0;
	//	isHurt = false;
	//	isAttack = false;
	//	break;
	case SNAKE_STATE_HURT:
		DescHP();
		isHurt = true;
		vx = 0;
		vy = 0;
		isAttack = false;
		hurtTime = GetTickCount();
		break;
	case SNAKE_STATE_ATTACK:
		isAttack = true;
		vx = 0;
		isHurt = false;
		break;
	}

}

