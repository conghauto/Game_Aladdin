#include "Dumbbell.h"

void Dumbbell::Render()
{
	animations[0]->RenderDumbbell(x, y);
	RenderBoundingBox();
}

void Dumbbell::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + DUMBBELL_BBOX_WIDTH;
	b = y + DUMBBELL_BBOX_HEIGHT;
}