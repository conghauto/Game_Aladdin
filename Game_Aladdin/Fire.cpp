#include "Fire.h"
#include "define.h"
#include "Game.h"
#include "Aladdin.h"

void Fire::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + FIRE_BBOX_WIDTH;
	bottom = y + FIRE_BBOX_HEIGHT;
}

void Fire::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{

	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;
	if (GetHP() == 0)
	{
		SetSpeed(0.0f, 0.0f);
		return;
	}
	if (GetTickCount() - timeAppear > 1000) {
		SetState(FIRE_STATE_DIE);
	}
}

void Fire::Render()
{
	int ani;

	if (GetState() == FIRE_STATE_DIE)
	{

		return;
	}
	else
	if (isAttack) {
	
		ani = FIRE_ANI_BURNING;
	}
	animations[ani]->Render(x, y);
	RenderBoundingBox();
}

void Fire::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FIRE_STATE_DIE:
		vx = 0;
		vy = 0;
		isAttack = false;
		break;
	case FIRE_STATE_BURNING:
		vx = 0;
		vy = 0;
		isAttack = true;
		break;
	}
}

