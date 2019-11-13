#include "Rock.h"

void Rock::Render()
{
	if (!isHitted)
	{
		if (animations[0]->currentFrame != 0)
			SetBound();
		else
		{
			width = 0;
			height = 0;
		}
		animations[0]->Render(x, y, 255);
	}
}

void Rock::SetHit()
{
	isHitted = true;
}

void Rock::SetBound()
{
	this->width = x + BRICK_BBOX_WIDTH;
	this->height = y + BRICK_BBOX_HEIGHT;
}

void Rock::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = width;
	b = height;
}