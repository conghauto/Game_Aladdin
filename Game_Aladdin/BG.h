#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include "define.h"
#include <string>
#include "Sprites.h"
#include <vector>
#include "Aladdin.h"
#include "define.h"
#include "Textures.h"
#include "Sprites.h"
class BG
{
private:
	ID3DXFont* font;
	RECT rect_life, rect_spend, rect_apple;
	Aladdin* aladdin;
	D3DXFONT_DESC desc;

	std::string information;
	std::string number_life;
	std::string number_apples;
	std::string number_spend;

	std::vector<LPSPRITE> simonHPList;
	std::vector<LPSPRITE> enemyHPList;
	std::vector<LPSPRITE> noHPList;

	CSprite* heart;
	CSprite* weapon;
	CSprite* pinkrect;

	CSprite* holyWater;
	CSprite* cross;
	CSprite* knife;
	CSprite* stopWatch;

	CSprite* upgrade1;
	CSprite* upgrade2;

	CSprite* pillar1;
	CSprite* pillar2;
	CSprite* pillar3;
	CSprite* pillar4;
	CSprite* gate;
	CSprite* apple;
	CSprite* spend;
	CSprite* aladdin_life;
	CSprite* god_light;

	ID3DXFont* numberofapples;

	int simonHP;
	int bossHP;
	int uiHP;

	int score;
	int time;
	int item;
	int energy;
	int life;
	int stage;
	int subWeapon;

public:
	BG();
	~BG();
	bool Initialize(LPDIRECT3DDEVICE9 d3ddv);
	//void Update(int bossHP, int time, int life, int stage);
	void Update(int time, int stage, Aladdin* aladdin);
	void Render(float x, float y, Aladdin* aladdin);
	void RenderText(float x, float y, Aladdin* aladdin);
	LPDIRECT3DTEXTURE9 LoadTexture(char* file, D3DCOLOR);
	void drawFont(ID3DXFont* font, float x, float y, float width, float height, int value);
	CSprite* axe;
};

