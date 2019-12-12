#include "ItemHeart.h"

void ItemHeart::Render()
{
	if (isEaten) return;
	animations[0]->Render(x, y, 255);
	RenderBoundingBox();
}

void ItemHeart::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + ITEM_GENIEFACE_BBOX_WIDTH;
	b = y + ITEM_GENIEFACE_BBOX_HEIGHT;
}
