#include "Zombie.h"
#include "define.h"
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
	if (checkX < 100 && checkY < 5 )
		this->SetState(GUARDIAN_STATE_ATTACK);
	if (checkX > 0 && state != GUARDIAN_STATE_WALK) {
		nx = -1;
		//if (state == GUARDIAN_STATE_WALK) {
		//	vx = -SKELETON_WALKING_SPEED;
		//}
	}
	else if (checkX <= 0 && state != GUARDIAN_STATE_WALK){
		nx = 1;
		//if (state == GUARDIAN_STATE_WALK)
		//	vx = SKELETON_WALKING_SPEED;
	}
	if (this->x < 1150 && y == 945) {
		x = 1150;
		nx = -nx;
		this->SetState(GUARDIAN_STATE_WALK);
	}
	else if (this->x > 1260 && this->y == 945) {
		this->x = 1250;
		this->nx = -nx;
		this->SetState(GUARDIAN_STATE_WALK);
	}

	if (this->x < 1550 && y == 944) {
		x = 1550;
		nx = -nx;
		this->SetState(GUARDIAN_STATE_WALK);
	}
	else if (this->x > 1900 && this->y == 944) {
		this->x = 1900;
		this->nx = -nx;
		this->SetState(GUARDIAN_STATE_WALK);
	}
	//if (check < 0)
	//{
	//	nx = -nx;
	//	this->SetState(GUARDIAN_STATE_IDLE);
	//}

}

void Zombie::Render()
{
	int ani;

	if (GetHP() <= 0)
	{

		return;
	}
	if (isHurt)
	{
		if (nx > 0) ani = GUARDIAN_ANI_HURT_RIGHT;
		else
			ani = GUARDIAN_ANI_HURT_LEFT;
	}
	if (state == GUARDIAN_STATE_IDLE)
	{
		if (nx > 0) ani = GUARDIAN_ANI_IDLE_RIGHT;
		else
			ani = GUARDIAN_ANI_IDLE_LEFT;
	}
	else
		if (isAttack) {
			if (nx > 0)
				ani = GUARDIAN_ANI_ATTACKING_RIGHT;
			else
				ani = GUARDIAN_ANI_ATTACKING_LEFT;

		}
	if (state == GUARDIAN_STATE_WALK) {
		if (nx > 0) ani = GUARDIAN_ANI_WALKING_RIGHT;
		else
			ani = GUARDIAN_ANI_WALKING_LEFT;
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
		isAttack = false;
		isHurt = false;
		break;
	case GUARDIAN_STATE_IDLE:
		vx = 0;
		vy = 0;
		isHurt = false;
		isAttack = false;
		break;
	case GUARDIAN_STATE_HURT:
		DescHP();
		isHurt = true;
		vx = 0;
		vy = 0;
		isAttack = false;
		hurtTime = GetTickCount();
		break;
	case GUARDIAN_STATE_ATTACK:
		isAttack = true;
		vx = 0;
		isHurt = false;
		break;
	case GUARDIAN_STATE_WALK:
		isAttack = false;
		isHurt = false;
		if (nx > 0)
			vx = SKELETON_WALKING_SPEED;
		else
			vx = -SKELETON_WALKING_SPEED;
		break;
	}

}