#include "GenieJar.h"

void GenieJar::Render()
{
	animations[0]->Render(x, y, 255);
	RenderBoundingBox();
}

void GenieJar::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + ITEM_APPLE_BBOX_WIDTH;
	b = y + ITEM_APPLE_BBOX_HEIGHT;
}