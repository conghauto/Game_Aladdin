#include "BossOne.h"
#include "define.h"
#include "Ground.h"
#include <algorithm>
#include "Captain.h"
#include "Shield.h"

void BossOne::CalcPotentialCollisions(
	vector<LPGAMEOBJECT> *coObjects,
	vector<LPCOLLISIONEVENT> &coEvents)
{
	bool isCollideWithCheckBox = false;

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		// Captain se khong va cham voi nhung vat sau:
			LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

			if (e->t > 0 && e->t <= 1.0f)
				coEvents.push_back(e);
			else
				delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}
void BossOne::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + BOSS_BBOX_WIDTH;

	if (state == BOSS_STATE_DIE)
		bottom = y + BOSS_BBOX_HEIGHT_DIE;
	else
		bottom = y + BOSS_BBOX_HEIGHT;
}

void BossOne::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);
	vy += SIMON_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	//x += dx;
	//y += dy;
	if (state == BOSS_STATE_DIE)
	{
		SetSpeed(0.0f, 0.0f);
		return;
	}



	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > MAX_WIDTH_LV2) {
		x = MAX_WIDTH_LV2;
		vx = -vx;
	}
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		/*
		 * Handle collision here
		 */
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

		 if (dynamic_cast<Ground *>(e->obj))
		 {
				// Da cham dat
				// Khong va cham theo phuong ngang

				// Xét va chạm cứng
				if (nx != 0) vx = BOSS_WALKING_SPEED;
				if (ny != 0) vy =0;
				isMoving = true;
				//this->time_start_walking = GetTickCount();
		 }
		 else  if (dynamic_cast<Shield*>(e->obj))
		 {
			 // Da cham dat
			 // Khong va cham theo phuong ngang

			 // Xét va chạm cứng
			 if (nx != 0) vx = BOSS_WALKING_SPEED;
			 if (ny != 0) vy = -BOSS_WALKING_SPEED;
			 isMoving = true;
			// this->time_start_walking = GetTickCount();
		 }
		 
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	//this->time_start_walking = GetTickCount();
}

void BossOne::Render()
{
	int ani;
	//if (GetTickCount() - this->time_start_walking > 10000) {
	//	//vy = -0.02;
	//	vx = 0.05;
	//}
	/*if (y>=210) {
		vx = BOSS_WALKING_SPEED;
		vy = -0.05;
	}*/

	/*if (GetTickCount() - this->time_start_walking > 10000) {
		vy = -0.05;
		vx = 0.01;
	}

	if (GetTickCount() - this->time_start_walking > 12000) {
		vy = 0.05;
	}*/

	if (state == BOSS_STATE_DIE) {
		if (nx > 0) {
			ani = BOSS_ANI_DIE_RIGHT;
		}
		else
		{
			ani = BOSS_ANI_DIE_LEFT;
		}

		return;
	}
	else {
		if (isDropping) {
			if (nx < 0)
				ani = BOSS_ANI_IDLE_LEFT;
			else if (nx > 0)
				ani = BOSS_ANI_IDLE_RIGHT;

			isMoving = true;
		}
		else if (isMoving) {
			if (vx < 0)
				ani = BOSS_ANI_WALKING_LEFT;
			else if (vx > 0)
				ani = BOSS_ANI_WALKING_RIGHT;
			
		}
		else
		{
			if (nx < 0)
				ani = BOSS_ANI_IDLE_LEFT;
			else if (nx > 0)
				ani = BOSS_ANI_IDLE_RIGHT;
		}


		animations[ani]->Render(x, y);
	}
	RenderBoundingBox();
	
}

void BossOne::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOSS_STATE_IDLE:
		vx = 0;
		isMoving = false;
		isDropping = false;
		break;
	case BOSS_STATE_DIE:
		y += BOSS_BBOX_HEIGHT - BOSS_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		isDropping = false;
		isMoving = false;
		break;
	case BOSS_STATE_DROP:
		isDropping = true;
		vy = 0.6f;
		break;
	case BOSS_STATE_WALKING:
		vx = BOSS_WALKING_SPEED;
		isMoving = true;
		break;
	}

}

