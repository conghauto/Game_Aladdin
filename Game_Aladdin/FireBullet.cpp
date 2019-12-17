#include "FireBullet.h"
#include "define.h"
#include "Game.h"
#include "Aladdin.h"

void FireBullet::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + FIRE_BULLET_BBOX_WIDTH;
	bottom = y + 50;
}

void FireBullet::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;
	if (GetHP() == 0)
	{
		SetSpeed(0.0f, 0.0f);
		return;
	}
	
	if (this->x > 960 || x <= 0) SetState(FIRE_BULLET_STATE_DIE);
	

	//vector<LPCOLLISIONEVENT> coEvents;
	//vector<LPCOLLISIONEVENT> coEventsResult;

	//CalcPotentialCollisions(coObjects, coEvents);


	//// No collision occured, proceed normally
	//if (coEvents.size() == 0)
	//{

	//}
	//else
	//{
	//	float min_tx, min_ty, nx = 0, ny;

	//	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

	//	for (UINT i = 0; i < coEventsResult.size(); i++)
	//	{
	//		LPCOLLISIONEVENT e = coEventsResult[i];

	//		if (dynamic_cast<Aladdin *>(e->obj))
	//		{
	//			Aladdin *aladdin = dynamic_cast<Aladdin *>(e->obj);
	//			aladdin->SetState(SIMON_STATE_HURT);
	//		}
	//	}
	//}
}

void FireBullet::Render()
{
	int ani;

	if (GetState() == FIRE_BULLET_STATE_DIE)
	{

		return;
	}
	else
		if (isAttack) {
			if (nx > 0)
				ani = FIRE_BULLET_ATTACK_RIGHT;
			else
				ani = FIRE_BULLET_ATTACK_LEFT;
		}
	animations[ani]->Render(x, y);
	RenderBoundingBox();
}

void FireBullet::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FIRE_STATE_DIE:
		vx = 0;
		vy = 0;
		isAttack = false;
		break;
	case FIRE_BULLET_STATE_ATTACK:
		if (nx > 0) vx = FIRE_BULLET_WALKING_SPEED;
		else vx = -FIRE_BULLET_WALKING_SPEED;
		vy = 0;
		isAttack = true;
		break;
	}
}

