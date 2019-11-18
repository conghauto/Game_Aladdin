#include "Zombie.h"
#include "define.h"
#include "Bullet.h"
#include "Game.h"
#include "Aladdin.h"

void Zombie::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + GUARDIAN_BBOX_WIDTH;

	if (state == GUARDIAN_STATE_DIE)
		bottom = y + GUARDIAN_BBOX_HEIGHT_DIE;
	else
		bottom = y + GUARDIAN_BBOX_HEIGHT;
}

void Zombie::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;
	if (state == GUARDIAN_STATE_DIE)
	{
		SetSpeed(0.0f, 0.0f);
		return;
	}

	

	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > 600) {
		x = 600; 
		vx = -vx;
	}
	float check = this->x - Aladdin::XforGet;
	if (check < 100)
		this->SetState(GUARDIAN_STATE_ATTACK);
	if (check < 0)
	{
		nx = -nx;
		this->SetState(GUARDIAN_STATE_IDLE);
	}

}

void Zombie::Render()
{
	int ani ;
	
	if (state == GUARDIAN_STATE_DIE) {
		if (nx > 0) {
			ani = GUARDIAN_ANI_DIE_RIGHT;
		}
		else
		{
			ani = GUARDIAN_ANI_DIE_LEFT;
		}

		return;
	}
	if (state == GUARDIAN_STATE_IDLE)
	{
		if (nx > 0) ani = GUARDIAN_ANI_IDLE_RIGHT;
		else
			ani = GUARDIAN_ANI_IDLE_LEFT;
	}
	else {
		if (nx > 0)
			ani = GUARDIAN_ANI_ATTACKING_RIGHT;
		else
			ani = GUARDIAN_ANI_ATTACKING_LEFT;
	
	}
	animations[ani]->Render(x, y);
	RenderBoundingBox();
}

void Zombie::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GUARDIAN_STATE_DIE:
		y += GUARDIAN_BBOX_HEIGHT - GUARDIAN_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case GUARDIAN_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case GUARDIAN_STATE_ATTACK:
		vx = 0;
		break;
	}

}

