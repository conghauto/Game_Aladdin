#include "Apple.h"
#include "Pillar.h"
#include "Soldier.h"
#include "Skeleton.h"
#include "Bat.h"
#include "BossJafar.h"
#include "BossSnake.h"
#include "Ground.h"
void Apple::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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

				if (dynamic_cast<Zombie *>(e->obj))
				{
					Zombie *zombie = dynamic_cast<Zombie *>(e->obj);
					zombie->SetState(GUARDIAN_STATE_HURT);
					this->isEaten = true;
				}
				else if (dynamic_cast<Soldier *>(e->obj))
				{
					Soldier *soldier = dynamic_cast<Soldier *>(e->obj);
					soldier->SetState(SOLDIER_STATE_HURT);
					this->isEaten = true;
				}
				else if (dynamic_cast<Skeleton *>(e->obj))
				{
					Skeleton *skeleton = dynamic_cast<Skeleton *>(e->obj);
					skeleton->SetState(SKELETON_STATE_DIE);
					this->isEaten = true;
				}
				else if (dynamic_cast<Bat *>(e->obj))
				{
					Bat *bat = dynamic_cast<Bat *>(e->obj);
					bat->SetState(BAT_STATE_DIE);
					this->isEaten = true;
				}
				else if (dynamic_cast<BossJafar *>(e->obj))
				{
					BossJafar *jafar = dynamic_cast<BossJafar *>(e->obj);
					jafar->SetState(JAFAR_STATE_HURT);
					this->isEaten = true;
				}
				else if (dynamic_cast<BossSnake *>(e->obj))
				{
					BossSnake *snake = dynamic_cast<BossSnake *>(e->obj);
					snake->SetState(SNAKE_STATE_HURT);
					this->isEaten = true;
				}
				else if (dynamic_cast<Pillar *>(e->obj))
				{
					Pillar *pillar = dynamic_cast<Pillar *>(e->obj);
					pillar->isHitted = true;
					this->isEaten = true;
				}
				else if (dynamic_cast<Ground *>(e->obj))
				{
					this->isEaten = true;
				}
			}
		}
	}
}