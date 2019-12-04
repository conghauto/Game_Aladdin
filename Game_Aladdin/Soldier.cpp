#include "Soldier.h"
#include "define.h"
#include "Aladdin.h"
void Soldier::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + SOLDIER_BBOX_WIDTH;

	if (state == SOLDIER_STATE_DIE)
		bottom = y + SOLDIER_BBOX_HEIGHT_DIE;
	else
		bottom = y + SOLDIER_BBOX_HEIGHT;
}

void Soldier::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;
	if (GetHP() == 0)
	{
		SetSpeed(0.0f, 0.0f);
		return;
	}
	if (isHurt && GetTickCount() - hurtTime >= 600) {
		isHurt = false;
		isAttack = true;
	}
	float checkX = this->x - Aladdin::XforGet;
	float checkY = -this->y + Aladdin::YforGet;
	if (checkX < 50 && checkY < 10)
		this->SetState(SOLDIER_STATE_ATTACK);
	//if (check < 0)
	//{
	//	nx = -nx;
	//	this->SetState(SOLDIER_STATE_IDLE);
	//}
}

void Soldier::Render()
{
	int ani;

	if (GetHP() == 0) {

		return;
	}
	if (isHurt)
	{
		if (nx > 0) ani = SOLDIER_ANI_HURT_RIGHT;
		else
			ani = SOLDIER_ANI_HURT_LEFT;
	}
		if (state == SOLDIER_STATE_IDLE)
		{
			if (nx > 0) ani = SOLDIER_ANI_IDLE_RIGHT;
			else
				ani = SOLDIER_ANI_IDLE_LEFT;
		}
		else
	if (isAttack){
		if (nx > 0)
			ani = SOLDIER_ANI_ATTACK_RIGHT;
		else
			ani = SOLDIER_ANI_ATTACK_LEFT;

	}
	animations[ani]->Render(x, y);
	RenderBoundingBox();
}

void Soldier::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SOLDIER_STATE_DIE:
		y += SOLDIER_BBOX_HEIGHT - SOLDIER_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		isHurt = false;
		isAttack = false;
		break;
	case SOLDIER_STATE_HURT:
		DescHP();
		isHurt = true;
		vx = 0;
		vy = 0;
		isAttack = false;
		hurtTime = GetTickCount();
		break;
	case SOLDIER_STATE_IDLE:
		vx = 0;
		vy = 0;
		isHurt = false;
		isAttack = false;
		break;
	case SOLDIER_STATE_ATTACK:
		vx = 0;
		isAttack = true;
		isHurt = false;
		break;	
	}

}

