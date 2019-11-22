#include "Dumbbell.h"

void Dumbbell::Render()
{

	if (!isHitted)
	{
		if (name == "dumbbell1")
		{
			if (animations[0]->currentFrame > 0)
			{
				SetBound();
			}
			else
			{
				width = 0;
					height = 0;
			}
			animations[0]->RenderDumbbell(x, y, 255);
		}
		if (name == "dumbbell3")
		{
			if (animations[0]->currentFrame != 14)
			{
				SetBound();
			}
			else
			{
				width = 0;
				height = 0;
			}
			animations[0]->RenderDumbbell(x, y, 255);
		}
		if (name == "dumbbell2" )
		{
			if (animations[0]->currentFrame !=21)
			{
				SetBound();
			}
			else
			{
				width = 0;
				height = 0;
			}
			animations[0]->RenderDumbbell(x, y, 255);
		}

	}
}
void Dumbbell::SetHit()
{
	isHitted = true;
}

void Dumbbell::SetBound()
{

	this->width = x + DUMBBELL_BBOX_WIDTH;
	this->height = y + DUMBBELL_BBOX_HEIGHT;
}
void Dumbbell::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = width;
	b = height;
}