#include "Rock.h"

void Rock::Render()
{
	//animations[0]->Render(x, y);
	RenderBoundingBox();
}

void Rock::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + ROCK_BBOX_WIDTH;
	b = y + ROCK_BBOX_HEIGHT;
}