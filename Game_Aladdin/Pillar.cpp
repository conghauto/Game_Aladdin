#include "Pillar.h"


void Pillar::Render()
{
	if (!isHitted)
		animations[0]->Render(x, y);
}
void Pillar::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}

void Pillar::SetShoot()
{
	isHitted = true;
}

void Pillar::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = 0;
	b = 0;
}
