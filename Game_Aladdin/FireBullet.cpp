#include "FireBullet.h"
#include "BossSnake.h"
#include "Pillar.h"
#include "Aladdin.h"
void FireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Weapon::Update(dt);

	if (isActivate)
	{
		// Calculate dx, dy 
		CGameObject::Update(dt);

		// Outrange
		if (nx > 0)
		{
			if (this->x > this->max_x)
				isEaten = true;
		}
		else if (nx < 0)
		{
			if (this->x < this->max_x)
				isEaten = true;
		}

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

				if (dynamic_cast<Aladdin *>(e->obj))
				{
					Aladdin *aladdin = dynamic_cast<Aladdin *>(e->obj);
					aladdin->SetState(SIMON_STATE_HURT);
					this->isEaten = true;
				}
				else if (dynamic_cast<Pillar *>(e->obj))
				{
					Pillar *pillar = dynamic_cast<Pillar *>(e->obj);
					pillar->isHitted = true;
					this->isEaten = true;
				}
			}
		}
	}
}