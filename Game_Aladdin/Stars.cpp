#include "Stars.h"
#include "define.h"
#include "Aladdin.h"

void Stars::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + STAR_BBOX_WIDTH;
	bottom = y + STAR_BBOX_HEIGHT;
}

void Stars::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;
	if (state == STAR_STATE_DIE)
	{
		SetSpeed(0.0f, 0.0f);
		return;
	}
	float checkVy = Aladdin::VyfoGet;
	if (checkVy >= 0) vy = 0.00f;
	else if (checkVy < 0) vy = -0.005f;
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
				if (this->nx > 0) aladdin->DesX();
				else aladdin->InsX();
				this->SetState(STAR_STATE_DIE);
			}
		}
	}
	if (this->x > 960 || x <= 0) SetState(STAR_STATE_DIE);
}

void Stars::Render()
{
	int ani;

	if (state == STAR_STATE_DIE) {
		vx = 0;
		vy = 0;
		return;
	}
	if (state == STAR_STATE_ATTACK)
		ani = STAR_ANI_ATTACK;


	animations[ani]->Render(x, y);
	RenderBoundingBox();
}

void Stars::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case STAR_STATE_DIE:
		vx = 0;
		vy = 0;
		break;
	case STAR_STATE_ATTACK:
		if (nx > 0) vx = STAR_SPEED_ATTACK_X;
		else vx = -STAR_SPEED_ATTACK_X;
		vy = 0;
		break;
	}

}

