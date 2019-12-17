#include <algorithm>
#include "debug.h"
#include "Rock.h"
#include "Aladdin.h"
#include "Game.h"
#include "Item.h"
#include "Zombie.h"
#include "Ground.h"
#include "CheckPoint.h"
#include "Soldier.h"
#include "Spike.h"
#include "Rope.h"
#include "Dumbbell.h"
#include "Skeleton.h"
#include "Bat.h"
#include "BossJafar.h"
#include "BossSnake.h"
#include "Fire.h"
#include "FireBullet.h"
#include "Bone.h"
#include "Stars.h"
#include "Sound.h"
#include "ItemApple.h"
#include "ItemHeart.h"
#include "ItemSpend.h"
#include "GenieFace.h"
#include "GenieJar.h"
int Aladdin::preHP = 5;
int Aladdin::score = 0;
int Aladdin::life = 3;
int Aladdin::numberapples = 10;
int Aladdin::numberspend = 15;
int Aladdin::heartsAmount = 5;
/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void Aladdin::CalcPotentialCollisions(
	vector<LPGAMEOBJECT> *coObjects,
	vector<LPCOLLISIONEVENT> &coEvents)
{
	bool isCollideWithCheckBox = false;

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}
	if (!isOnRope)
	{
		isClimbDown = false;
		isClimbUp = false;
		ny = 0;
	}
	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void Aladdin::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	if (isAttack) {
		SetBound(200);
	}
	else SetBound(35);

	if (preHP <= 0) {
		if (life > 0) {
			life -= 1;
			preHP = 5;
		}
		else
		{
			x += 10;
			life = 4;
			preHP = 5;
		}

	}

	if (isThrow == true && GetTickCount() - attackTime >= SIMON_TIMER_ATTACK)
	{
		isThrow = false;
		if (isExitSit)
		{
			isSit = false;
			y -= SIMON_SIT_TO_STAND;
			isExitSit = false;
		}
	}
	// Simple fall down
	if (!isOnRope)
	vy += SIMON_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != SIMON_STATE_DIE || state != SIMON_STATE_HURT)
		CalcPotentialCollisions(coObjects, coEvents);

	// turn off isOnCheckStair


	// Set idle state
	if (!isSit && !isMoving && !isJump && !isAttack && !isHurt && !isOnRope)
	{
		SetState(SIMON_STATE_IDLE);
	}

	// reset untouchable timer if untouchable time has passed
	if (untouchable == 1 && GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}


	// Handle Aladdin go over screen camera
	float leftCorner = CGame::GetInstance()->mCamera->getX();
	float rightCorner = leftCorner + SCREEN_WIDTH - SIMON_STAND_BBOX_WIDTH;
	// Left corner
	if (x < leftCorner)
	{
		x = 0;
	}
	// Right corner
	else if (x + 32 > rightCorner)
	{
		x = rightCorner-32;
	}

	if (level == 1 && (x >= 1365 && x < 1400 && y>700)) {
		x = 1365;
	}

	else {
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
		bool willHurt = false;
		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		/*
		 * Handle collision here
		 */
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Zombie *>(e->obj))
			{
				Zombie *zombie = dynamic_cast<Zombie *>(e->obj);
				if (this->isAttack == true && zombie->GetState() != GUARDIAN_STATE_DIE) {
				
					zombie->SetState(GUARDIAN_STATE_HURT);
				}
				else
				if (zombie->GetState() != GUARDIAN_STATE_DIE && untouchable == 0 && this->isAttack == false) {
					// Đặt hướng hurt
					if (e->nx > 0)
						this->nx = -1;
					else if (e->nx < 0)
						this->nx = 1;

					SetState(SIMON_STATE_HURT);
					willHurt = true;
					StartUntouchable();
				}

			}
			else
			if (dynamic_cast<Skeleton *>(e->obj))
			{
				Skeleton *skeleton = dynamic_cast<Skeleton *>(e->obj);
				if (this->isAttack == true && skeleton->GetState() != SKELETON_STATE_DIE) {
					skeleton->SetState(SKELETON_STATE_HURT);
				}
				else
				if (skeleton->GetState() != GUARDIAN_STATE_DIE && untouchable == 0 && this->isAttack == false) {
					// Đặt hướng hurt
					if (e->nx > 0)
						this->nx = -1;
					else if (e->nx < 0)
						this->nx = 1;

					SetState(SIMON_STATE_HURT);
					willHurt = true;
					StartUntouchable();
				}
			}
			else if (dynamic_cast<Soldier *>(e->obj))
			{
				Soldier *soldier = dynamic_cast<Soldier *>(e->obj);
				if (soldier->GetState() != SOLDIER_STATE_DIE && untouchable == 0) {
					// Đặt hướng hurt
					if (e->nx > 0)
						this->nx = -1;
					else if (e->nx < 0)
						this->nx = 1;

					SetState(SIMON_STATE_HURT);
					willHurt = true;
					StartUntouchable();
				}
			}
			else if (dynamic_cast<BossJafar *>(e->obj))
			{
				BossJafar *jafar = dynamic_cast<BossJafar *>(e->obj);
				if (jafar->GetHP() != 0 && untouchable == 0) {
					// Đặt hướng hurt
					if (e->nx > 0)
						this->nx = -1;
					else if (e->nx < 0)
						this->nx = 1;

					SetState(SIMON_STATE_HURT);
					willHurt = true;
					StartUntouchable();
				}
			}
			else if (dynamic_cast<BossSnake *>(e->obj))
			{
				BossSnake *snake = dynamic_cast<BossSnake *>(e->obj);
				if (snake->GetHP() != 0 && untouchable == 0) {
					// Đặt hướng hurt
					if (e->nx > 0)
						this->nx = -1;
					else if (e->nx < 0)
						this->nx = 1;

					SetState(SIMON_STATE_HURT);
					willHurt = true;
					StartUntouchable();
				}
			}
			else if (dynamic_cast<Bat *>(e->obj))
			{
				Bat *bat = dynamic_cast<Bat *>(e->obj);
				if (bat->GetState() != BAT_STATE_DIE && untouchable == 0) {
					// Đặt hướng hurt
					if (e->nx > 0)
						this->nx = -1;
					else if (e->nx < 0)
						this->nx = 1;

					SetState(SIMON_STATE_HURT);
					willHurt = true;
					StartUntouchable();
				}
			}
			else if (dynamic_cast<Bone *>(e->obj))
			{
				Bone *bone = dynamic_cast<Bone *>(e->obj);
				if (bone->GetState() != BONE_STATE_DIE && untouchable == 0) {
					// Đặt hướng hurt
					if (e->nx > 0)
						this->nx = -1;
					else if (e->nx < 0)
						this->nx = 1;

					SetState(SIMON_STATE_HURT);
					willHurt = true;
					StartUntouchable();
					bone->SetState(BONE_STATE_DIE);
				}
			}
			else if (dynamic_cast<Fire *>(e->obj))
			{
				Fire *fire = dynamic_cast<Fire *>(e->obj);
				if (untouchable == 0) {
					//// Đặt hướng hurt
					//if (e->nx > 0)
					//	this->nx = -1;
					//else if (e->nx < 0)
					//	this->nx = 1;

					SetState(SIMON_STATE_HURT);
					willHurt = true;
					StartUntouchable();
				}
			}
			else if (dynamic_cast<ItemApple *>(e->obj))
			{
			ItemApple *itemapple = dynamic_cast<ItemApple *>(e->obj);
			if (untouchable == 0) {
				Sound::getInstance()->loadSound(APPLE_MUSIC, "apple");
				Sound::getInstance()->play("apple", false, 1);
				itemapple->isEaten = true;
				IncreScore(2000);
				itemapple->isDisappear = true;
				IncrNumberApples();
			}
			}
			else if (dynamic_cast<ItemSpend *>(e->obj))
			{
			ItemSpend *itemSpend = dynamic_cast<ItemSpend *>(e->obj);
			if (untouchable == 0) {
				Sound::getInstance()->loadSound(APPLE_MUSIC, "apple");
				Sound::getInstance()->play("apple", false, 1);
				IncrNumberSpend();
				itemSpend->isEaten = true;			
			}
			}
			else if (dynamic_cast<GenieFace *>(e->obj))
			{
			GenieFace *item = dynamic_cast<GenieFace *>(e->obj);
			if (untouchable == 0) {
				Sound::getInstance()->loadSound(APPLE_MUSIC, "apple");
				Sound::getInstance()->play("apple", false, 1);
				IncLife();
				item->isEaten = true;
			}
			}
			else if (dynamic_cast<GenieJar *>(e->obj))
			{
			GenieJar *item = dynamic_cast<GenieJar *>(e->obj);
			if (untouchable == 0) {
				Sound::getInstance()->loadSound(APPLE_MUSIC, "apple");
				Sound::getInstance()->play("apple", false, 1);
				IncreScore(2000);
				item->isEaten = true;
			}
			}
			else if (dynamic_cast<ItemHeart *>(e->obj))
			{
			ItemHeart *item = dynamic_cast<ItemHeart *>(e->obj);
			if (untouchable == 0) {
				Sound::getInstance()->loadSound(APPLE_MUSIC, "apple");
				Sound::getInstance()->play("apple", false, 1);
				item->isEaten = true;
				IncrNumberHeart();
			}
			}
			else if (dynamic_cast<FireBullet *>(e->obj))
			{
			FireBullet *firebullet = dynamic_cast<FireBullet *>(e->obj);
			if (untouchable == 0) {
				SetState(SIMON_STATE_HURT);
				willHurt = true;
				StartUntouchable();
			}
			}
			else if (dynamic_cast<Stars *>(e->obj))
			{
			Stars *star = dynamic_cast<Stars *>(e->obj);
			if (untouchable == 0) {
				if (star->nx > 0) this->x -= 10;
				else
					this->x += 10;
				star->SetState(STAR_STATE_DIE);
			}
			}
			else if (dynamic_cast<Rock*>(e->obj))
			{
				// Da cham dat
				// Khong va cham theo phuong ngang
				if (isJump && e->nx == 0 && e->ny < 0)
					isJump = false;
				if (isHurt)
					isHurt = false;
				if (isOnRope)
					isOnRope = false;
				// Xét va chạm cứng
				if (nx != 0) vx = 0;
				if (ny != 0) vy = 0;
			}
			else if (dynamic_cast<Dumbbell*>(e->obj))
			{
				// Da cham dat
				// Khong va cham theo phuong ngang
				if (isJump && e->nx == 0 && e->ny < 0)
					isJump = false;
				if (isHurt)
					isHurt = false;
				if (isOnRope)
					isOnRope = false;
				// Xét va chạm cứng
				if (nx != 0) vx = 0;
				if (ny != 0) vy = 0;
			}
			else if (dynamic_cast<Rope*>(e->obj))
			{
				// Da cham dat
				// Khong va cham theo phuong ngang
				y -= 2;
				ny = -1;
				x = e->obj->x-16;
				//vy = -0.5;
				/*vy= -1.0;*/
				SetState(SIMON_STATE_ONROPE);
			}
			else if (dynamic_cast<Ground *>(e->obj) && !willHurt)
			{
				// Da cham dat
				// Khong va cham theo phuong ngang
				if (isJump && e->nx == 0 && e->ny < 0)
					isJump = false;
				if (isHurt)
					isHurt = false;
				if (isOnRope)
					isOnRope = false;
				isClimbDown = false;
				isClimbUp = false;
				// Xét va chạm cứng
				if (nx != 0) vx = 0;
				if (ny != 0) vy = 0;
			}
			else if (dynamic_cast<CheckPoint *>(e->obj))
			{
				CheckPoint *checkpoint = dynamic_cast<CheckPoint *>(e->obj);
				int type = checkpoint->GetType();

				switch (type)
				{
				case CHECKPOINT_BONUS:
					isBonus = true;
					checkpoint->SetType(CHECKPOINT_BONUS);
					break;
				case CHECKPOINT_LEVELUP:
					if (!isLevelUp)
					{
						x -= nx * 1.0f;
						isLevelUp = true;
						onCheckPointTime = GetTickCount();
					}
					break;
				}
			}
			else if (dynamic_cast<Item *>(e->obj))
			{
				Item *item = dynamic_cast<Item *>(e->obj);
				item->SetEaten();

				int type = item->GetType();
				switch (type)
				{
				case ITEM_HEART:
					IncHeart(5);
					break;
				case ITEM_KNIFE:
					SetCurrentWeapon(ITEM_KNIFE);
					break;
				case ITEM_MONEY:
					IncScore(1000);
					break;
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	// Has completed attack animation
	if (isAttack == true && GetTickCount() - attackTime >= SIMON_TIMER_ATTACK)
	{
		isAttack = false;
		if (isExitSit)
		{
			isSit = false;
			y -= SIMON_SIT_TO_STAND;
			isExitSit = false;
		}
	}
}

void Aladdin::Render()
{
	int ani = -1, aniWhip = -1;

	if (state == SIMON_STATE_DIE) {
		//if (GetLife() >= 0) {
		//	DesLife();
		//	x = 0;
		//	y = 0;
		//}
		//else return;
	}
	else
	{
		if (isHurt)
		{
			if (nx > 0)
				ani = SIMON_ANI_HURT_RIGHT;
			else if (nx < 0)
				ani = SIMON_ANI_HURT_LEFT;
		}
		else
			if (isOnRope) {
				if (isClimbUp) {
					ani = SIMON_ANI_CLIMB_RIGHT;
				}
				else
					if (isClimbDown) {
						ani = SIMON_ANI_CLIMB_LEFT;
					}
					else
						if (nx > 0)
						{
							ani = SIMON_ANI_ONROPE_IDLE_RIGHT;
						}
						else
							ani = SIMON_ANI_ONROPE_IDLE_LEFT;
			}
		if (isAttack)
		{
			if (nx > 0)
			{
				if (isSit)
				{
					ani = SIMON_ANI_SIT_ATTACK_RIGHT;
				}
				else
				{
					ani = SIMON_ANI_ATTACK_RIGHT;
				}
			}
			else
			{
				if (isSit)
				{
					ani = SIMON_ANI_SIT_ATTACK_LEFT;
				}
				else
				{
					ani = SIMON_ANI_ATTACK_LEFT;
				}
			}
		}
		else if (isThrow)
		{

			
			if (nx > 0)
			{
				if (isSit)
				{
					ani = SIMON_ANI_SIT_THROW_RIGHT;
				}
				else
				{
					ani = SIMON_ANI_THROW_RIGHT;
				}
			}
			else
			{
				if (isSit)
				{
					ani = SIMON_ANI_SIT_THROW_LEFT;
				}
				else
				{
					ani = SIMON_ANI_THROW_LEFT;
				}
			}
		}
		
		else if (isJump)
		{
			if (nx > 0)
				ani = SIMON_ANI_JUMP_RIGHT;
			else
				ani = SIMON_ANI_JUMP_LEFT;
		}
		// Right direction
		else if (nx > 0)
		{
			switch (state)
			{
			case SIMON_STATE_SIT:
				if (isThrow)
					ani = SIMON_ANI_SIT_THROW_RIGHT;
				else
					if (isAttack)
						ani = SIMON_ANI_SIT_ATTACK_RIGHT;
					else
						ani = SIMON_ANI_SIT_RIGHT;
				break;
			case SIMON_STATE_WALK:
				ani = SIMON_ANI_WALKING_RIGHT;
				break;
			case SIMON_STATE_DASHING:
				ani = SIMON_ANI_DASHING_RIGHT;
				break;
			case SIMON_STATE_IDLE:
				ani = SIMON_ANI_IDLE_RIGHT;
				break;
			}
		}
		// Left direction
		else if (nx < 0)
		{
			switch (state)
			{
			case SIMON_STATE_SIT:
				if (isThrow)
					ani = SIMON_ANI_SIT_THROW_LEFT;
				else
					if (isAttack)
						ani = SIMON_ANI_SIT_ATTACK_LEFT;
					else
						ani = SIMON_ANI_SIT_LEFT;
				break;
			case SIMON_STATE_WALK:
				ani = SIMON_ANI_WALKING_LEFT;
				break;
			case SIMON_STATE_IDLE:
				ani = SIMON_ANI_IDLE_LEFT;
				break;
			}
		}
	}

	int alpha = 255;
	if (untouchable) alpha = 0;

	if (ani != -1)
	{
		switch (ani)
		{
		case SIMON_ANI_ATTACK_RIGHT:
		case SIMON_ANI_SIT_ATTACK_RIGHT:
			animations[ani]->Render(x - 16, y, alpha);
			break;
		case SIMON_ANI_HITUP_RLADDER:
			animations[ani]->Render(x - 18, y, alpha);
			break;
		case SIMON_ANI_ATTACK_LEFT:
		case SIMON_ANI_SIT_ATTACK_LEFT:
			animations[ani]->Render(x - 12, y, alpha);
			break;
		default:
			animations[ani]->Render(x, y, alpha);
			break;
		}
	}

	RenderBoundingBox();
}

void Aladdin::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_DIE:
		isDead = true;
		vy = -SIMON_DIE_DEFLECT_SPEED;
		break;
	case SIMON_STATE_HURT:
		isHurt = true;
		Sound::getInstance()->loadSound(HURT_MUSIC, "hurt");
		Sound::getInstance()->play("hurt", false, 1);
		DesHP();
		vy = -SIMON_DIE_DEFLECT_SPEED;
		if (nx > 0)
		{

			vx = -SIMON_DIE_DEFLECT_SPEED;
		}
		else if (nx < 0)
		{
			vx = SIMON_DIE_DEFLECT_SPEED;
		}

		isAttack = false;
		isJump = false;
		isMoving = false;
		isOnStair = false;
		isClimbDown = false;
		isClimbUp = false;
		isSit = false;
		isExitSit = false;
		isOnRope = false;
		break;
	case SIMON_STATE_ONROPE:
		isOnRope = true;
		isClimbDown = false;
		isClimbUp = false;
		isJump = false;
		isHurt = false;
		vx = 0;
		vy = 0;
		break;
	case SIMON_STATE_ONROPE_CLIMB:
		isClimbUp = true;
		vy = -SIMON_CLIMBING_SPEED_Y;
		break;
	case SIMON_STATE_ONROPE_CLIMB_DOWN:
		isClimbDown = true;
		vy = SIMON_CLIMBING_SPEED_Y;
		break;

	case SIMON_STATE_SIT:
		isOnRope = false;
		isSit = true;
		vx = 0;
		break;
	case SIMON_STATE_WALK:
		isOnRope = false;
		isMoving = true;
		if (nx == 1.0f)
		{
			vx = SIMON_WALKING_SPEED;
		}
		else if (nx == -1.0f)
		{
			vx = -SIMON_WALKING_SPEED;
		}
		break;
	case SIMON_STATE_IDLE:
		isOnRope = false;
		isClimbDown = false;
		isClimbUp = false;
		vx = 0;
		isMoving = false;
		break;
	}
}

void Aladdin::SetAction(int action)
{
	switch (action)
	{
	case SIMON_ACTION_ATTACK:
		// Ngat tat ca trang thai (tru ngoi)
		// Neu dang nhay toi thi de nguyen van toc, neu khong thi dung lai va danh
		isAttack = true;
		if (!isJump)
			vx = 0;
		isJump = false;
		isMoving = false;
		attackTime = GetTickCount();
		break;
		// Don gian la cho nhay, khong ngat bat ki trang thai nao
		// Them van toc nhay
	case SIMON_ACTION_THROW:
		isThrow = true;
		if (!isJump)
			vx = 0;
		isJump = false;
		isMoving = false;
		attackTime = GetTickCount();
		break;

	case SIMON_ACTION_JUMP:
		isJump = true;
		vy = -SIMON_JUMP_SPEED_Y;
		break;
	case SIMON_ACTION_JUMP_ONROPE:
		isJump = true;
		vy = -0.03f;
		if (nx > 0) {
			vx = 0.01f;
		}
		else
			vx = -0.01f;
		break;
	}
}
void Aladdin::SetBound(float dis) {
	width = x + dis;
}
void Aladdin::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	
	top = y;
	left = x;
	right = width;
	bottom = y + SIMON_STAND_BBOX_HEIGHT;

}


