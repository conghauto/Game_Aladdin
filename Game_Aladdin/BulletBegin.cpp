#include "BulletBegin.h"
#include "define.h"
#include "Captain.h"

void BulletBegin::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + SOLDIER_BBOX_WIDTH;

	if (state == SOLDIER_STATE_DIE)
		bottom = y + SOLDIER_BBOX_HEIGHT_DIE;
	else
		bottom = y + SOLDIER_BBOX_HEIGHT;
}

void BulletBegin::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;
	if (state == SOLDIER_STATE_DIE)
	{
		SetSpeed(0.0f, 0.0f);
		return;
	}



	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > 640) {
		x = 640;
		vx = -vx;
		this->countCol += 1;
	}
	this->time_end_start = GetTickCount();
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	CalcPotentialCollisions(coObjects, coEvents);

	x += dx;
	y += dy;

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

			if (dynamic_cast<Captain *>(e->obj))
			{
				Captain *captain = dynamic_cast<Captain *>(e->obj);
				captain->SetState(SIMON_STATE_DIE);
				//this->isEaten = true;
			}
		}
	}

}

void BulletBegin::Render()
{
	int ani;

	if (state == SOLDIER_STATE_DIE) {
		if (nx > 0) {
			ani = SOLDIER_ANI_DIE_RIGHT;
		}
		else
		{
			ani = SOLDIER_ANI_DIE_LEFT;
		}

		return;
	}
	else {
		if (vx < 0)
			ani = SOLDIER_ANI_WALKING_LEFT;
		else if (vx > 0)
			ani = SOLDIER_ANI_WALKING_RIGHT;

		animations[ani]->Render(x, y);
	}
	RenderBoundingBox();
}

void BulletBegin::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SOLDIER_STATE_DIE:
		y += SOLDIER_BBOX_HEIGHT - SOLDIER_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case SOLDIER_STATE_WALKING:
		vx = BALLBEGIN;

	}

}

