#include "Ground.h"

void Ground::Render()
{
	//animations[0]->Render(x, y);
	if (checkCollision) {
		SetBound();
	}
	else {
		SetBoundNoCollision();
	}
	RenderBoundingBox();
}
void Ground::SetBound()
{
	this->width = x + BRICK_BBOX_WIDTH;
	this->height = y + BRICK_BBOX_HEIGHT;
}

void Ground::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	/*r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;*/
	r = width;
	b = height;
}
