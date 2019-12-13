#include "Bat.h"
#include "define.h"
#include "Aladdin.h"

void Bat::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + BAT_BBOX_WIDTH;

	if (state == GUARDIAN_STATE_DIE)
		bottom = y + GUARDIAN_BBOX_HEIGHT_DIE;
	else
		bottom = y + BAT_BBOX_HEIGHT;
}

void Bat::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;
	if (state == BAT_STATE_DIE)
	{
		SetSpeed(0.0f, 0.0f);
		return;
	}

	if (vx < 0 && y > 950) {
		y = 950;
		vx = -vx;
	}
	if (vx < 0 && y == 950) {
		vy = -vy;
	}
	float check = this->x - Aladdin::XforGet;
	if (check < 300)
		this->SetState(BAT_STATE_ATTACK);


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

void Bat::Render()
{
	int ani;

	if (state == BAT_STATE_DIE) {
		vx = 0;
		vy = 0;
		return;
	}
	if (state == BAT_STATE_WAIT)
		ani = BOSSBAT_ANI_WAIT;
	else
		ani = BOSSBAT_ANI_ATTACK;


	animations[ani]->Render(x, y);
	RenderBoundingBox();
}

void Bat::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BAT_STATE_WAIT:
		vx = 0;
		vy = 0;
		break;
	case BAT_STATE_ATTACK:
		vx = -0.02f;
		vy = 0.02f;
		break;
	}

}

