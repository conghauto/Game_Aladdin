#include "JafarBullet.h"
#include "Aladdin.h"

void JafarBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isActivate)
	{
		if (GetTickCount() - firstCast > 2000)
		{
			float x = jafar->x;
			float y = jafar->y;

			nx = jafar->nx;

			if (nx > 0)
			{
				this->max_x = x + max_width;
			}
			else if (nx < 0)
			{
				this->max_x = x - max_width;
			}

			SetPosition(x, y + 10);
			appearTime = GetTickCount();

			isActivate = true;
		}
	}

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
					Aladdin *captain = dynamic_cast<Aladdin *>(e->obj);
					captain->SetState(SIMON_STATE_HURT);
					this->isEaten = true;
				}
			}
		}
	}
}