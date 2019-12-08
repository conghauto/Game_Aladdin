#include "Sprites.h"
#include "Game.h"
#include "debug.h"

CSprite::CSprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;
}

CSprites * CSprites::__instance = NULL;

CSprites *CSprites::GetInstance()
{
	if (__instance == NULL) __instance = new CSprites();
	return __instance;
}

void CSprite::Draw(float x, float y, int alpha)
{
	CGame * game = CGame::GetInstance();
	game->Draw(x, y, texture, left, top, right, bottom, alpha);
}

void CSprites::Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	LPSPRITE s = new CSprite(id, left, top, right, bottom, tex);
	sprites[id] = s;
}

LPSPRITE CSprites::Get(int id)
{
	return sprites[id];
}



void CAnimation::Add(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);
	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t);
	frames.push_back(frame);
}

void CAnimation::Render(float x, float y, int alpha)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
		}

	}

	frames[currentFrame]->GetSprite()->Draw(x, y, alpha);
	
}

void CAnimation::RenderRock(float x, float y, int alpha)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame++;
		lastFrameTime = now;
		t = frames[currentFrame]->GetTime() + 2500;
	}
	else
	{
		if (now - lastFrameTime > t&& status == 0)
		{

			t = frames[currentFrame]->GetTime() + 100;
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size())
			{
				status = 1;
				currentFrame = frames.size() - 1;
				t = frames[currentFrame]->GetTime() + 5000;
			}
		}
		if (now - lastFrameTime > t&& status == 1)
		{

			t = frames[currentFrame]->GetTime() + 100;
			currentFrame--;
			lastFrameTime = now;
			if (currentFrame == -1)
			{
				status = 0;
				currentFrame = 0;
				t = frames[currentFrame]->GetTime() + 5000;
			}
		}
	}

	frames[currentFrame]->GetSprite()->Draw(x, y, alpha);
}

void CAnimation::RenderSpike(float x, float y, int alpha)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame++;
		lastFrameTime = now;
		t = frames[currentFrame]->GetTime() + 1000;
	}
	else
	{
		if (now - lastFrameTime > t&& status == 0)
		{

			t = frames[currentFrame]->GetTime() + 100;
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size())
			{
				status = 1;
				currentFrame = frames.size() - 1;
				t = frames[currentFrame]->GetTime() + 2000;
			}
		}
		if (now - lastFrameTime > t&& status == 1)
		{

			t = frames[currentFrame]->GetTime() + 100;
			currentFrame--;
			lastFrameTime = now;
			if (currentFrame == -1)
			{
				status = 0;
				currentFrame = 0;
				t = frames[currentFrame]->GetTime() + 2000;
			}
		}
	}

	frames[currentFrame]->GetSprite()->Draw(x, y, alpha);
}

void CAnimation::RenderDumbbell(float x, float y, int alpha)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t + 100)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
		}

	}
	frames[currentFrame]->GetSprite()->Draw(x, y, alpha);

}

CAnimations * CAnimations::__instance = NULL;

CAnimations * CAnimations::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimations();
	return __instance;
}

void CAnimations::Add(int id, LPANIMATION ani)
{
	animations[id] = ani;
}

LPANIMATION CAnimations::Get(int id)
{
	return animations[id];
}