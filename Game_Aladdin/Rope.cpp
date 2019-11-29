#include "Rope.h"

void Rope::Render()
{
	//animations[0]->Render(x, y);
	RenderBoundingBox();
}

void Rope::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x;
	b = y + height;
}
