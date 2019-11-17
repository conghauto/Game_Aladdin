#include "Spike.h"

void Spike::Render()
{
	if (!isHitted)
	{
		if ((animations[0]->currentFrame > 0 && name == "spike1") || (animations[0]->currentFrame < 5 && name == "spike2"))
		{
			SetBound();
		}
		else
		{
			width = 0;
			height = 0;
		}
		animations[0]->RenderSpike(x, y, 255);

	}
}

void Spike::SetHit()
{
	isHitted = true;
}

void Spike::SetBound()
{

	this->width = x + BRICK_BBOX_WIDTH;
	this->height = y + BRICK_BBOX_HEIGHT;
}

void Spike::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = width;
	b = height;
}