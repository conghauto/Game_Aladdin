#include "BG.h"


BG::BG()
{
	simonHP = NULL;
	simonHP = NULL;
}


BG::~BG()
{

}

bool BG::Initialize(LPDIRECT3DDEVICE9 d3ddv)
{

	ZeroMemory(&desc, sizeof(D3DXFONT_DESC));
	desc.Height = 20;
	desc.Width = 10;
	desc.Weight = 200;
	desc.MipLevels = D3DX_DEFAULT;
	desc.Italic = false;
	desc.CharSet = 0;
	desc.OutputPrecision = 0;
	desc.Quality = 0;
	desc.PitchAndFamily = 0;
	
	//Aladdin::GetInstance()->createFont(d3ddev, desc);
	D3DXCreateFontIndirect(d3ddv, &desc, &numberofapples);

	CTextures* textures = CTextures::GetInstance();
	pillar1 = new CSprite(1, 0, 0, 40, 346, textures->Get(ID_TEX_PILLAR_1));
	pillar2 = new CSprite(2, 0, 0, 30, 150, textures->Get(ID_TEX_PILLAR_2));
	pillar3 = new CSprite(3, 0, 0, 30, 680, textures->Get(ID_TEX_PILLAR_3));
	pillar4 = new CSprite(4, 0, 0, 30, 173, textures->Get(ID_TEX_PILLAR_4));
	gate = new CSprite(5, 0, 0, 75, 200, textures->Get(ID_TEX_GATE));
	apple = new CSprite(6, 0, 0, 18, 17, textures->Get(ITEM_BG_APPLE));
	spend = new CSprite(7, 0, 0, 21, 21, textures->Get(ITEM_BG_SPEND));
	aladdin_life = new CSprite(8, 0, 0, 20, 22, textures->Get(ALADDIN_LIFE));
	god_light = new CSprite(9, 0, 0, 111, 38, textures->Get(ITEM_BG_GOD_LIGHT));

	/*for (int i = 0; i < 16; i++)
	{
		CSprite* sprite = new CSprite(8, 23, 37, 30, 50, textures->Get(ID_TEX_UI));

		simonHPList.push_back(sprite);
	}

	for (int i = 0; i < 16; i++)
	{
		CSprite* sprite = new CSprite(9, 37, 37, 44, 50, textures->Get(ID_TEX_UI));
		enemyHPList.push_back(sprite);
	}*/
	number_life = "1000000";
	font = NULL;
	HRESULT hr = D3DXCreateFont(d3ddv, 22, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE, L"Arial", &font);
	if (!SUCCEEDED(hr))
	{
		return FALSE;
	}
	SetRect(&rect, 0, 150, 500, 44);
	
	/*font = NULL;
	AddFontResourceEx(GAME_FONT, FR_PRIVATE, NULL);
	HRESULT result = D3DXCreateFont(
		d3ddv, 20, 0, FW_NORMAL, 1, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE, L"Press Start", &font);

	if (!SUCCEEDED(result))
		return false;
	SetRect(&rect1,50, 100, SCREEN_WIDTH, SCREEN_HEIGHT);
	number_life = std::to_string(1000000);

	/*SetRect(&rect1, 0, 20, SCREEN_WIDTH, SCREEN_HEIGHT);
	number_apples = std::to_string(aladdin->GetNumberapples());

	SetRect(&rect1, 0, 20, SCREEN_WIDTH, SCREEN_HEIGHT);
	number_spend = std::to_string(aladdin->GetNumberspend());*/
	return false;
}

//void BG::Update(int bossHP, int time, int life, int stage)
void BG::Update(int time, int stage, Aladdin* aladdin)
{
	this->time = time;
	this->stage = stage;


	std::string scoreString = std::to_string(aladdin->GetScore());
	while (scoreString.length() < 6)
		scoreString = "0" + scoreString;


	std::string timeString = std::to_string(this->time);
	while (timeString.length() < 4)
		timeString = "0" + timeString;


	std::string stageString = std::to_string(this->stage);
	while (stageString.length() < 2)
		stageString = "0" + stageString;

	information = "SCORE_000000 TIME 0000 STAGE 00\n";
	information += "PLAYER                =62\n";
	information += "ENEMY                 P=3\n";

	number_life = "1000000";
	/*number_life = std::to_string(1000000);
	number_apples = std::to_string(aladdin->GetNumberapples());
	number_spend = std::to_string(aladdin->GetNumberspend());*/
}



void BG::Render(float x, float y, Aladdin* aladdin)
{
	
	
	/*for (int i = 0; i < simonHPList.size(); i++)
	{
		simonHPList[i]->Draw(x + 130 + 11 * i, 42, 255);
	}
	for (int i = 0; i < enemyHPList.size(); i++)
	{
		enemyHPList[i]->Draw(x + 130 + 11 * i, 62, 255);
	}*/
	pillar1->Draw(160, 680, 255);
	pillar2->Draw(463, 863, 255);
	pillar3->Draw(767, 336, 255);
	pillar4->Draw(1100, 835, 255);
	gate->Draw(2070, 60, 255);
	god_light->Draw(x+10, y+10, 255);
	aladdin_life->Draw(x + 10, y + SCREEN_HEIGHT - 80, 255);
	spend->Draw(x + SCREEN_WIDTH - 110, y + SCREEN_HEIGHT - 80,255);
	apple->Draw(x + SCREEN_WIDTH - 60, y + SCREEN_HEIGHT - 80, 255);
	drawFont(numberofapples, 100, 500, 135, 525, 1000);
	//heart->Draw(x + SCREEN_WIDTH / 2 , y+SCREEN_HEIGHT/2, 255);
	//pinkrect->Draw(x + SCREEN_WIDTH / 2, 40, 255);
	if (aladdin->currentWeapon != NULL)
	{
		int weapon = aladdin->currentWeapon;
		{
			switch (weapon)
			{
			/*case ITEM_KNIFE:
				knife->Draw(x + SCREEN_WIDTH / 2 + 10, 50, 255);
				break;*/
			/*case ITEM_AXE:
				axe->Draw(x + SCREEN_WIDTH / 2 + 10, 50, 255);
				break;
			case ITEM_HOLYWATER:
				holyWater->Draw(x + SCREEN_WIDTH / 2 + 10, 50, 255);
				break;
			case ITEM_CROSS:
				cross->Draw(x + SCREEN_WIDTH / 2 + 10, 50, 255);
				break;
			case ITEM_CLOCK:
				stopWatch->Draw(x + SCREEN_WIDTH / 2 + 10, 50, 255);
				break;*/
			}
		}
	}
}
void BG::drawFont(ID3DXFont* font, float x, float y, float width, float height, int value)
{
	//set position of font 
	RECT rect;
	rect.top = (int)y;
	rect.left = (int)x;
	rect.bottom = (int)y + height;
	rect.right = (int)x + width;

	char intStr[20];
	//_itoa_s(value, intStr, 10);
	std::string number_life1 = std::to_string(value);
	font->DrawTextA(NULL, number_life1.c_str(), -1, &rect, DT_TOP | DT_LEFT, D3DCOLOR_XRGB(255, 0, 255));
	//strcpy_s(intStr, status.c_str());
	/*if (font != score)
		font->DrawText(NULL, intStr, -1, &rect, DT_TOP | DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
	else
		font->DrawText(NULL, intStr, -1, &rect, DT_TOP | DT_RIGHT, D3DCOLOR_XRGB(255, 255, 255));*/
}

