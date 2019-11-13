#include "Spike.h"

void Spike::Render()
{
	animations[0]->RenderSpike(x, y);
	RenderBoundingBox();
}

void Spike::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + SPIKE_BBOX_WIDTH;
	b = y + SPIKE_BBOX_HEIGHT;
}