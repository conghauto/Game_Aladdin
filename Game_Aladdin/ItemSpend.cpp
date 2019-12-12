#include "ItemSpend.h"

void ItemSpend::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects) {
	if (isEaten) {
		return;
	}
}
void ItemSpend::Render()
{
	int ani;
	if (isEaten) return;
	else
		ani = 0;
	animations[ani]->Render(x, y);
	RenderBoundingBox();
}

void ItemSpend::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + ITEM_GENIEFACE_BBOX_WIDTH;
	b = y + ITEM_GENIEFACE_BBOX_HEIGHT;
}
