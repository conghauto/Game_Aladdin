#include "Bone.h"
#include "define.h"
#include "Aladdin.h"

void Bone::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + BONE_BBOX_WIDTH;

	if (state == BONE_STATE_DIE)
		bottom = y + GUARDIAN_BBOX_HEIGHT_DIE;
	else
		bottom = y + BAT_BBOX_HEIGHT;
}

void Bone::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;
	if (state == BONE_STATE_DIE)
	{
		SetSpeed(0.0f, 0.0f);
		return;
	}

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
				this->SetState(BONE_STATE_DIE);
			}
		}
	}

}

void Bone::Render()
{
	int ani;

	if (state == BONE_STATE_DIE) {
		vx = 0;
		vy = 0;
		return;
	}
	if (state == BONE_STATE_ATTACK)
		ani = BONE_ANI_ATTACK;


	animations[ani]->Render(x, y);
	RenderBoundingBox();
}

void Bone::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BONE_STATE_DIE:
		vx = 0;
		vy = 0;
		break;
	case BONE_STATE_ATTACK:
		float vx = this->vx;
		float vy = this->vy;
		break;
	}

}

