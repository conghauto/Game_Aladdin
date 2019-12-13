#include "Skeleton.h"
#include "define.h"
#include "Game.h"
#include "Aladdin.h"

void Skeleton::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + SKELETON_BBOX_WIDTH;

	if (state == SKELETON_STATE_DIE)
		bottom = y + SKELETON_BBOX_HEIGHT_DIE;
	else
		bottom = y + SKELETON_BBOX_HEIGHT;
}

void Skeleton::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;
	preX = x;
	preY = y;
	if (GetHP() <= 0)
	{
		SetSpeed(0.0f, 0.0f);
		return;
	}



	float check = this->x - Aladdin::XforGet;
	float checkY = -this->y + Aladdin::YforGet;
	if (check < 200 && checkY < 20)
		this->SetState(SKELETON_STATE_WALK);
	if (this->x < 200 && this->y == 930) {
		this->x = 200;
		nx = -nx;
		this->SetState(SKELETON_STATE_WALK);
	}
	if (this->x > 500 && this->y == 930) {
		this->x = 500;
		nx = -nx;
		this->SetState(SKELETON_STATE_WALK);
	}

	if (this->x < 470 && this->y == 750) {
		this->x = 470;
		nx = -nx;
		this->SetState(SKELETON_STATE_WALK);
	}
	if (this->x > 830 && this->y == 750) {
		this->x = 830;
		nx = -nx;
		this->SetState(SKELETON_STATE_WALK);
	}

	//if (check < 0)
	//{
	//	nx = -nx;
	//	this->SetState(SKELETON_STATE_IDLE);
	//}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{

	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Aladdin *>(e->obj))
			{
				Aladdin *aladdin = dynamic_cast<Aladdin *>(e->obj);
				aladdin->SetState(SIMON_STATE_HURT);
			}
		}
	}

}

void Skeleton::Render()
{
	int ani;

	if (GetHP() <= 0) {
		return;
	}
	if (state == SKELETON_STATE_IDLE)
	{
		if (nx > 0) ani = SKELETON_ANI_IDLE_RIGHT;
		else
			ani = SKELETON_ANI_IDLE_LEFT;
	}
	else {
		if (nx > 0)
			ani = SKELETON_ANI_IDLE_RIGHT;
		else
			ani = SKELETON_ANI_IDLE_LEFT;

	}
	animations[ani]->Render(x, y);
	RenderBoundingBox();
}

void Skeleton::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SKELETON_STATE_DIE:
		y += SKELETON_BBOX_HEIGHT - SKELETON_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case SKELETON_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case SKELETON_STATE_HURT:
		DescHP();
		break;
	case SKELETON_STATE_WALK:
		if (nx > 0)
			vx = SKELETON_WALKING_SPEED;
		else
			vx = -SKELETON_WALKING_SPEED;
		break;
	}

}

