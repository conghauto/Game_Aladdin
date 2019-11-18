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
	if (state == SOLDIER_STATE_DIE)
	{
		SetSpeed(0.0f, 0.0f);
		return;
	}
	float check = this->x - Aladdin::XforGet;
	if (check < 100)
		this->SetState(SOLDIER_ANI_ATTACK_RIGHT);
	if (check < 0)
	{
		nx = -nx;
		this->SetState(SOLDIER_STATE_IDLE);
	}
}

void Soldier::Render()
{
	int ani;

	if (state == SOLDIER_STATE_DIE) {

		return;
	}
	if (state == SOLDIER_STATE_IDLE)
	{
		if (nx > 0) ani = SOLDIER_ANI_IDLE_RIGHT;
		else
			ani = SOLDIER_ANI_IDLE_LEFT;
	}
	else {
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
		break;
	case SOLDIER_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case SOLDIER_STATE_ATTACK:
		vx = 0;
		break;
		
	}

}

