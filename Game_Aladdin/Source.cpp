#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "Shield.h"
#include "Aladdin.h"
#include "Ground.h"
#include "define.h"
#include "Item.h"
#include "Pillar.h"
#include "Rock.h"
#include "Map.h"
#include "CheckPoint.h"
#include "Water.h"
#include "Grid.h"
#include "Bullet.h"
#include "Ball.h"
#include "BossOne.h"
#include "BulletBegin.h"
#include "BossBullet.h"
BossBullet *bossbullet;
BulletBegin *beginBullet;
BossOne *trum;
CGame *game;
Aladdin * aladdin;
Item *item;
Effect *whipEffect;
Soldier *soldier;
Soldier *soldier1;
Map *map;
Shield *knife;
Bullet *bullet;
Ball *ball;
//UI * ui;
CSprite *sprite;

Grid *grid;
vector<Cell*> currentCell;

bool lv1 = true;
bool lv2 = false;

bool boss = false;
// check scene lv2->lv2_1
bool checkScene = false;
bool check1 = false;
bool check = false;
vector<ObjectTile*> listObjectsInMap;
ListObject*lsObjs;

//
//check scene lv2_1 ->lv2_2
bool checkScene1 = false;
bool countLoadResourceLv2 = false;

DWORD timer; // load enemy lv2
DWORD timer2;//load enemy boss

DWORD gameTime = 999000;
CSprites * sprites = CSprites::GetInstance();
CAnimations * animations = CAnimations::GetInstance();
CTextures * textures = CTextures::GetInstance();

class CSampleKeyHander : public CKeyEventHandler
{
public:
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander * keyHandler;

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	// Chet
	if (aladdin->GetState() == SIMON_STATE_DIE) return;

	// Nhay
	if (KeyCode == DIK_Z)
	{
		if (aladdin->isJump == false && aladdin->isSit == false && aladdin->isAttack == false && aladdin->isOnStair == false)
			aladdin->SetAction(SIMON_ACTION_JUMP);
	}

	if (KeyCode == DIK_C)
	{
		if (aladdin->isAttack == false)
			aladdin->SetAction(SIMON_ACTION_ATTACK);
	}
	else
	if (KeyCode == DIK_X)
	{
		aladdin->SetCurrentWeapon(809);
		if (aladdin->isAttack == false) {
			int nx = aladdin->nx;
			aladdin->SetAction(SIMON_ACTION_THROW);
			switch (aladdin->currentWeapon)
			{
			case ITEM_KNIFE:
				knife = new Shield(aladdin, 2 * SCREEN_WIDTH / 3);
				if (nx > 0)
				{
					knife->SetSpeed(KNIFE_SPEED, 0);
					knife->AddAnimation(WEAPON_ANI_KNIFE);
				}
				else if (nx < 0)
				{
					knife->SetSpeed(-KNIFE_SPEED, 0);
					knife->AddAnimation(WEAPON_ANI_KNIFE);
				}
				//knife->SetType(ITEM_KNIFE);
				knife->SetPosition(aladdin->x, aladdin->y);
				knife->appearTime = GetTickCount();
				knife->firstCast = GetTickCount();
				//objects.push_back(knife);
				grid->AddObject(knife);
				break;
			}
		}
	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	// Chet
	if (aladdin->GetState() == SIMON_STATE_DIE) return;

	// Len xuong cau thang
	if (KeyCode == DIK_UP)
	{
		if (aladdin->isHoldShield)
		{
			aladdin->SetState(SIMON_STATE_IDLE);
			aladdin->isHoldShield = false;
		}
	}

	if (KeyCode == DIK_Q) {
		aladdin->SetState(SIMON_STATE_IDLE);
		aladdin->isDashing = false;

	}
	// Ngoi
	if (KeyCode == DIK_DOWN)
	{

		 if (aladdin->isSit)
		{
			if (!aladdin->isAttack)
			{
				aladdin->isSit = false;
			}
			else
			{
				aladdin->isExitSit = true;
			}
		}
	}

	// Di bo
	else if (KeyCode == DIK_RIGHT || KeyCode == DIK_LEFT)
	{
		aladdin->isMoving = false;
		aladdin->vx = 0;
	}
}

void CSampleKeyHander::KeyState(BYTE *states)
{
	// Chet
	if (aladdin->GetState() == SIMON_STATE_DIE) return;

	// Len xuong cau thang
	if (game->IsKeyDown(DIK_UP))
	{
		//if (!aladdin->isOnCheckStairDown && !aladdin->isOnStair && !aladdin->isAttack && !aladdin->isJump)
		aladdin->SetState(SIMON_STATE_HOLD_SHIELD);
	}

	// Ngoi
	if (game->IsKeyDown(DIK_Q)) {
		aladdin->SetState(SIMON_STATE_DASHING);
	}
	if (game->IsKeyDown(DIK_DOWN))
	{
		 if (!aladdin->isAttack && !aladdin->isJump)
			aladdin->SetState(SIMON_STATE_SIT);
	}

	// Di bo
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (!aladdin->isSit && !aladdin->isAttack)
			aladdin->SetState(SIMON_STATE_WALK);
		if (!aladdin->isJump &&  !aladdin->isAttack)
			aladdin->nx = 1.0f;
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (!aladdin->isSit && !aladdin->isAttack)
			aladdin->SetState(SIMON_STATE_WALK);
		if (!aladdin->isJump && !aladdin->isAttack)
			aladdin->nx = -1.0f;
	}

	// Neu khong co gi xay ra se dung im
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Load all game resources
	In this example: load textures, sprites, animations and mario object

	TO-DO: Improve this function by loading texture,sprite,animation,object from file
*/
void LoadResources()
{
	textures->Add(ID_TEX_KNIFE, L"Resources\\aladdin.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_SIMON, L"Resources\\aladdin.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_ROCK, L"Resources\\rock.png", D3DCOLOR_XRGB(163,73, 164));
	textures->Add(ID_TEX_TILESET, L"Resources\\tileset.png", D3DCOLOR_XRGB(255, 0, 255));

	textures->Add(ID_TEX_PILLAR1, L"Resources\\pillar_1.png", D3DCOLOR_XRGB(163, 73, 164));
	textures->Add(ID_TEX_PILLAR2, L"Resources\\pillar_2.png", D3DCOLOR_XRGB(163, 73, 164));
	textures->Add(ID_TEX_PILLAR3, L"Resources\\pillar_3.png", D3DCOLOR_XRGB(163, 73, 164));
	textures->Add(ID_TEX_PILLAR4, L"Resources\\pillar_4.png", D3DCOLOR_XRGB(163, 73, 164));
	textures->Add(ID_TEX_GATE, L"Resources\\gate_exit.png", D3DCOLOR_XRGB(163, 73, 164));

	LPDIRECT3DTEXTURE9 texShield = textures->Get(ID_TEX_KNIFE);
	LPDIRECT3DTEXTURE9 texAladdin = textures->Get(ID_TEX_SIMON);
	LPDIRECT3DTEXTURE9 texAladdinSit = textures->Get(ID_TEX_SIT);
	
	#pragma region Addsprite
	sprites->Add(10001, 1122, 6, 1162, 58, texAladdin);		// đứng im phải

	sprites->Add(10002, 1186, 1215, 1229, 1275, texAladdin);		// đi phải
	sprites->Add(10003, 1239, 1216, 1280, 1273, texAladdin);
	sprites->Add(11002, 1290, 1215, 1333, 1275, texAladdin);
	sprites->Add(11003, 1340, 1215, 1394, 1275, texAladdin);
	sprites->Add(11004, 1397, 1215, 1449, 1275, texAladdin);
	sprites->Add(11005, 1451, 1215, 1496, 1275, texAladdin);
	sprites->Add(11006, 1505, 1215, 1547, 1275, texAladdin);
	sprites->Add(11007, 1560, 1215, 1593, 1275, texAladdin);

	sprites->Add(10004, 1125, 324, 1173, 387, texAladdin);		// đánh phải
	sprites->Add(10005, 1174, 324, 1227, 387, texAladdin);					
	sprites->Add(10006, 1232, 324, 1280, 387, texAladdin);
	sprites->Add(10007, 1289, 324, 1373, 387, texAladdin);
	sprites->Add(10008, 1377, 324, 1431, 387, texAladdin);

	sprites->Add(10025, 1182, 618, 1235, 673, texAladdin);		// ngồi đánh phải			
	sprites->Add(10026, 1235, 618, 1307, 673, texAladdin);
	sprites->Add(10027, 1311, 618, 1404, 673, texAladdin);
	sprites->Add(10028, 1416, 618, 1502, 673, texAladdin);
	sprites->Add(10029, 1235, 618, 1307, 673, texAladdin);


	sprites->Add(10018, 1129, 618, 1176, 673, texAladdin); // ngồi phải

	sprites->Add(12001, 1196, 820, 1248, 890, texAladdin); // nhảy phải
	sprites->Add(12002, 1258, 820, 1313, 890, texAladdin);
	sprites->Add(12003, 1321, 820, 1377, 890, texAladdin);
	sprites->Add(12004, 1506, 820, 1545, 890, texAladdin);
	sprites->Add(12005, 1681, 810, 1713, 907, texAladdin);

	sprites->Add(10054, 436, 67, 471, 130, texAladdin);//bị đánh từ bên trái

	sprites->Add(13001, 1124, 225, 1170, 290, texAladdin); //ném táo phải
	sprites->Add(13002, 1178, 225, 1217, 290, texAladdin);
	sprites->Add(13003, 1227, 225, 1270, 290, texAladdin);
	sprites->Add(13004, 1282, 225, 1330, 290, texAladdin);
	sprites->Add(13005, 1339, 225, 1382, 290, texAladdin);

	sprites->Add(13021, 1127, 562, 1169, 615, texAladdin); // ngồi ném táo phải
	sprites->Add(13022, 1176, 562, 1218, 615, texAladdin);
	sprites->Add(13023, 1225, 562, 1278, 615, texAladdin);
	sprites->Add(13024, 1284, 562, 1365, 615, texAladdin);
	sprites->Add(13025, 1378, 562, 1436, 615, texAladdin);

	LPDIRECT3DTEXTURE9 texAladdin2 = textures->Get(ID_TEX_SIMON);
	sprites->Add(10011, 1079, 6, 1120, 59, texAladdin2);		// đứng im trái

	sprites->Add(10012, 1008, 1215, 1057, 1275, texAladdin2);		// đi trái
	sprites->Add(10013, 959, 1215, 1003, 1275, texAladdin2);
	sprites->Add(11012, 907, 1215, 952, 1275, texAladdin2);
	sprites->Add(11013, 848, 1215, 904, 1275, texAladdin2);
	sprites->Add(11014, 793, 1215, 846, 1275, texAladdin2);
	sprites->Add(11015, 742, 1215, 789, 1275, texAladdin2);
	sprites->Add(11016, 692, 1215, 736, 1275, texAladdin2);
	sprites->Add(11017, 643, 1215, 685, 1275, texAladdin2);

	sprites->Add(10014, 1070, 324, 1115, 387, texAladdin2);		// đánh trái				
	sprites->Add(10015, 1013, 324, 1068, 387, texAladdin2);
	sprites->Add(10016, 956, 324, 1009, 387, texAladdin2);
	sprites->Add(10017, 867, 324, 954, 387, texAladdin2);
	sprites->Add(10009, 808, 324, 866, 387, texAladdin2);

	sprites->Add(10031, 1005, 618, 1060, 673, texAladdin);		// ngồi đánh trái				
	sprites->Add(10032, 927, 618, 1009, 673, texAladdin);
	sprites->Add(10033, 835, 618, 930, 673, texAladdin);
	sprites->Add(10034, 737, 618, 825, 673, texAladdin);
	sprites->Add(10035, 927, 618, 1009, 673, texAladdin);

	sprites->Add(10019, 1064, 618, 1115, 673, texAladdin); // ngồi trái

	sprites->Add(12011, 994, 820, 1047, 890, texAladdin);//nhảy trái
	sprites->Add(12012, 929, 820, 986, 890, texAladdin);
	sprites->Add(12013, 801, 820, 851, 890, texAladdin);
	sprites->Add(12014, 694, 820, 728, 890, texAladdin);
	sprites->Add(12015, 522, 820, 558, 890, texAladdin);

	sprites->Add(13011, 1070, 225, 1115, 290, texAladdin); // ném táo trái
	sprites->Add(13012, 1022, 225, 1067, 290, texAladdin);
	sprites->Add(13013, 973, 225, 1014, 290, texAladdin);
	sprites->Add(13014, 912, 225, 928, 290, texAladdin);
	sprites->Add(13015, 861, 225, 902, 290, texAladdin);

	sprites->Add(13031, 1073, 562, 1115, 615, texAladdin); //ngồi ném táo trái
	sprites->Add(13032, 1022, 562, 1065, 615, texAladdin);
	sprites->Add(13033, 962, 562, 1015, 615, texAladdin);
	sprites->Add(13034, 877, 562, 956, 615, texAladdin);
	sprites->Add(13035, 808, 562, 866, 615, texAladdin);

	sprites->Add(10055, 12, 70, 41, 130, texAladdin2);//bị đánh từ bên phải

	sprites->Add(10099, 180, 237, 240, 264, texAladdin);		// chết 

	sprites->Add(10110, 106, 144, 149, 189, texAladdinSit);//nhay danh phai

	sprites->Add(10111, 59, 144, 102, 189, texAladdinSit); // nhay danh trai

	sprites->Add(10120, 282, 96, 302, 136, texAladdin); // double nhay 
	sprites->Add(10121, 305, 96, 327, 136, texAladdin);
	sprites->Add(10122, 327, 96, 349, 136, texAladdin);

	sprites->Add(10130, 237, 79, 276, 123, texAladdinSit);// dashing left
	sprites->Add(10131, 310, 79, 348, 123, texAladdinSit);// dashing right

	sprites->Add(10140, 233, 224, 258, 268, texAladdin); // knifeing right
	sprites->Add(10141, 202, 224, 227, 268, texAladdin);	//knifeing left
	sprites->Add(1200, 1482, 19, 1511, 71, texShield);	//khien
	#pragma endregion

	#pragma region ENEMY

	LPDIRECT3DTEXTURE9 texMisc11 = textures->Get(ID_TEX_KNIFE);
	sprites->Add(40023, 0, 0, 32, 18, texMisc11);

	LPDIRECT3DTEXTURE9 texRock1 = textures->Get(ID_TEX_ROCK); // rock
	sprites->Add(40031, 0, 4, 30, 20, texRock1);
	sprites->Add(40032, 35, 4, 66, 20, texRock1);
	sprites->Add(40033, 72, 4, 105, 21, texRock1);
	sprites->Add(40034, 111,4, 147, 24, texRock1);
	sprites->Add(40035, 154, 4, 194, 26, texRock1);

	LPDIRECT3DTEXTURE9 texPillar1 = textures->Get(ID_TEX_PILLAR1); //pillar 1
	sprites->Add(410036, 0, 0, 40, 343, texPillar1);

	LPDIRECT3DTEXTURE9 texPillar2 = textures->Get(ID_TEX_PILLAR2); //pillar 2
	sprites->Add(410037, 0, 0, 30, 160, texPillar2);

	LPDIRECT3DTEXTURE9 texPillar3 = textures->Get(ID_TEX_PILLAR3); //pillar 2
	sprites->Add(410038, 0, 5, 32, 749, texPillar3);

	LPDIRECT3DTEXTURE9 texPillar4 = textures->Get(ID_TEX_PILLAR4); //pillar 4
	sprites->Add(410039, 0, 0, 32, 193, texPillar4);

	LPDIRECT3DTEXTURE9 texGate = textures->Get(ID_TEX_GATE); //gate
	sprites->Add(410040, 0, 0, 78, 212, texGate);
	
	#pragma endregion

	LPANIMATION ani;

	#pragma region AladdinAnimation

	ani = new CAnimation(100);	//đứng phải
	ani->Add(10001);
	animations->Add(400, ani);

	ani = new CAnimation(100);	//đứng trái
	ani->Add(10011);
	animations->Add(401, ani);

	ani = new CAnimation(180);	//đi phải
	ani->Add(10002);
	ani->Add(10003);
	ani->Add(11002);
	ani->Add(11003);
	ani->Add(11004);
	ani->Add(11005);
	ani->Add(11006);
	ani->Add(11007);
	animations->Add(500, ani);

	ani = new CAnimation(180);	//đi trái
	ani->Add(10012);
	ani->Add(10013);
	ani->Add(11012);
	ani->Add(11013);
	ani->Add(11014);
	ani->Add(11015);
	ani->Add(11016);
	ani->Add(11017);
	animations->Add(501, ani);

	ani = new CAnimation(150); //đánh phải
	ani->Add(10004);
	ani->Add(10005);
	ani->Add(10006);
	ani->Add(10007);
	ani->Add(10008);
	animations->Add(402, ani);

	ani = new CAnimation(150);//đánh trái
	ani->Add(10014);
	ani->Add(10015);
	ani->Add(10016);
	ani->Add(10017);
	ani->Add(10009);
	animations->Add(403, ani);

	ani = new CAnimation(100); //ngồi phải
	ani->Add(10018);
	animations->Add(404, ani);

	ani = new CAnimation(100);//ngồi trái
	ani->Add(10019);
	animations->Add(405, ani);

	ani = new CAnimation(100);//nhảy phải
	ani->Add(12001);
	ani->Add(12002);
	ani->Add(12003);
	ani->Add(12004);
	ani->Add(12005);
	animations->Add(406, ani);

	ani = new CAnimation(100);//nhảy trái
	ani->Add(12011);
	ani->Add(12012);
	ani->Add(12013);
	ani->Add(12014);
	ani->Add(12015);
	animations->Add(407, ani);

	ani = new CAnimation(150);//ngồi đánh phải
	ani->Add(10025);
	ani->Add(10026);
	ani->Add(10027);
	ani->Add(10028);
	ani->Add(10029);
	animations->Add(410, ani);

	ani = new CAnimation(150);//ngồi đánh trái
	ani->Add(10031);
	ani->Add(10032);
	ani->Add(10033);
	ani->Add(10034);
	ani->Add(10035);
	animations->Add(411, ani);

	ani = new CAnimation(150);//trên thang phải đánh phải
	ani->Add(10034);
	ani->Add(10035);
	ani->Add(10036);
	animations->Add(412, ani);

	ani = new CAnimation(150);//trên thang phải đánh trái
	ani->Add(10037);
	ani->Add(10038);
	ani->Add(10039);
	animations->Add(413, ani);

	ani = new CAnimation(150);//trên thang trái đánh phải
	ani->Add(10040);
	ani->Add(10041);
	ani->Add(10042);
	animations->Add(414, ani);

	ani = new CAnimation(150);//trên thang trái đánh trái
	ani->Add(10043);
	ani->Add(10044);
	ani->Add(10045);
	animations->Add(415, ani);


	ani = new CAnimation(100);//lên thang phải
	ani->Add(10046);
	ani->Add(10047);
	animations->Add(416, ani);

	ani = new CAnimation(100);//xuống thang phải
	ani->Add(10048);
	ani->Add(10049);
	animations->Add(417, ani);

	ani = new CAnimation(100);//lên thang trái
	ani->Add(10050);
	ani->Add(10051);
	animations->Add(418, ani);

	ani = new CAnimation(100);//xuống thang trái
	ani->Add(10052);
	ani->Add(10053);
	animations->Add(419, ani);

	ani = new CAnimation(100);//đứng im bên phải thang phải
	ani->Add(10047);
	animations->Add(420, ani);

	ani = new CAnimation(100);//đứng im bên phải thang trái
	ani->Add(10049);
	animations->Add(421, ani);

	ani = new CAnimation(100);//đứng im bên trái thang phải
	ani->Add(10051);
	animations->Add(422, ani);

	ani = new CAnimation(100);//đứng im bên trái thang phải
	ani->Add(10053);
	animations->Add(423, ani);

	ani = new CAnimation(100);//bị đánh từ bên trái
	ani->Add(10054);	
	animations->Add(424, ani);

	ani = new CAnimation(100);//bị đánh từ bên phải
	ani->Add(10055);	
	animations->Add(425, ani);

	ani = new CAnimation(150);//roi lv0 phải
	ani->Add(10056);
	ani->Add(10057);
	ani->Add(10058);
	animations->Add(426, ani);

	ani = new CAnimation(150);//roi lv0 trái
	ani->Add(10059);
	ani->Add(10060);
	ani->Add(10061);
	animations->Add(427, ani);

	ani = new CAnimation(150);//roi lv1 phải
	ani->Add(10062);
	ani->Add(10063);
	ani->Add(10064);
	animations->Add(428, ani);

	ani = new CAnimation(150);//roi lv1 trái
	ani->Add(10065);
	ani->Add(10066);
	ani->Add(10067);
	animations->Add(429, ani);

	ani = new CAnimation(100);	//chết	
	ani->Add(10099);
	animations->Add(599, ani);


	ani = new CAnimation(100); //nhay danh phai
	ani->Add(10110);
	animations->Add(1600, ani);

	ani = new CAnimation(100); //nhay danh trai
	ani->Add(10111);
	animations->Add(1601, ani);

	ani = new CAnimation(100); //double nhay
	ani->Add(10120);
	ani->Add(10121);
	ani->Add(10122);
	animations->Add(1602, ani);

	ani = new CAnimation(100); //dashing right
	ani->Add(10131);
	animations->Add(1603, ani);

	ani = new CAnimation(100); //dashing left
	ani->Add(10130);
	animations->Add(1604, ani);
		

	ani = new CAnimation(100); //knifeing right
	ani->Add(10140);
	animations->Add(1605, ani);

	ani = new CAnimation(100); //knifeing left
	ani->Add(10141);
	animations->Add(1606, ani);

	ani = new CAnimation(100);
	ani->Add(1200);
	animations->Add(1607, ani); //nem khien

	ani = new CAnimation(100);
	ani->Add(44444);
	animations->Add(1608, ani); //nem dan of zombie

	ani = new CAnimation(100);
	ani->Add(45555);
	animations->Add(1609, ani); //nem dan of soldier

	ani = new CAnimation(100); // ném táo phải
	ani->Add(13001);
	ani->Add(13002);
	ani->Add(13003);
	ani->Add(13004);
	ani->Add(13005);
	animations->Add(1610, ani);

	ani = new CAnimation(100); // ném táo trái
	ani->Add(13011);
	ani->Add(13012);
	ani->Add(13013);
	ani->Add(13014);
	ani->Add(13015);
	animations->Add(1611, ani);

	ani = new CAnimation(100); // ngồi ném táo phải
	ani->Add(13021);
	ani->Add(13022);
	ani->Add(13023);
	ani->Add(13024);
	ani->Add(13025);
	animations->Add(1612, ani);

	ani = new CAnimation(100); // ngồi ném táo trái
	ani->Add(13031);
	ani->Add(13032);
	ani->Add(13033);
	ani->Add(13034);
	ani->Add(13035);
	animations->Add(1613, ani);

	ani = new CAnimation(100);//Boss di phai
	ani->Add(41000);
	ani->Add(41001);
	ani->Add(41002);
	animations->Add(666, ani);

	ani = new CAnimation(100);//Boss di trai
	ani->Add(41003);
	ani->Add(41004);
	ani->Add(41005);
	animations->Add(667, ani);

	ani = new CAnimation(100);//dan bay
	ani->Add(40099);
	animations->Add(668, ani);

	ani = new CAnimation(100);//boss dung trai
	ani->Add(44010);
	animations->Add(670, ani);

	ani = new CAnimation(100);//boss dung phai
	ani->Add(44011);
	animations->Add(671, ani);

	ani = new CAnimation(100);//boss drop trai
	ani->Add(44020);
	animations->Add(672, ani);

	ani = new CAnimation(100);//boss drop phai
	ani->Add(44021);
	animations->Add(673, ani);
	#pragma endregion

	#pragma region ObjectAnimation

	ani = new CAnimation(100);	//đất1
	ani->Add(20001);
	animations->Add(601, ani);

	ani = new CAnimation(0); //knife
	ani->Add(40023);
	animations->Add(809, ani);

	ani = new CAnimation(0); // bậc thang 1
	ani->Add(40031);
	ani->Add(40032);
	ani->Add(40033);
	ani->Add(40034);
	ani->Add(40035);
	animations->Add(990, ani);


	ani = new CAnimation(0); // bậc thang 2
	ani->Add(40033);
	ani->Add(40034);
	ani->Add(40035);
	ani->Add(40031);
	ani->Add(40032);
	animations->Add(991, ani);

	ani = new CAnimation(0); // pillar 1
	ani->Add(410036);
	animations->Add(1000, ani);

	ani = new CAnimation(0); // pillar 2
	ani->Add(410037);
	animations->Add(1001, ani);

	ani = new CAnimation(0); // pillar 3
	ani->Add(410038);
	animations->Add(1002, ani);

	ani = new CAnimation(0); // pillar 4
	ani->Add(410039);
	animations->Add(1003, ani);

	#pragma endregion

	#pragma region aladdin
	aladdin = new Aladdin();
	aladdin->AddAnimation(400);	// đứng phải	
	aladdin->AddAnimation(401);	//đứng trái	
	aladdin->AddAnimation(500);	// đi phải
	aladdin->AddAnimation(501);	//đi trái
	aladdin->AddAnimation(402);	//đánh phải 
	aladdin->AddAnimation(403);	//đánh trái
	aladdin->AddAnimation(404);	//ngồi phải
	aladdin->AddAnimation(405);	//ngồi trái
	aladdin->AddAnimation(406);	//nhảy phải 
	aladdin->AddAnimation(407);	//nhảy trái
	aladdin->AddAnimation(410);	//ngồi đánh phải 
	aladdin->AddAnimation(411);	//ngồi đánh trái
	aladdin->AddAnimation(412);	//trên thang phải đánh phải
	aladdin->AddAnimation(413);	//trên thang phải đánh trái
	aladdin->AddAnimation(414);	//trên thang trái đánh phải
	aladdin->AddAnimation(415);	//trên thang trái đánh trái
	aladdin->AddAnimation(416);	//lên thang phải
	aladdin->AddAnimation(417);	//xuống thang phải
	aladdin->AddAnimation(418);	//lên thang trái
	aladdin->AddAnimation(419);	//xuống thang trái
	aladdin->AddAnimation(420);	//đứng im bên phải thang phải
	aladdin->AddAnimation(421);	//đứng im bên phải thang trái
	aladdin->AddAnimation(422);	//đứng im bên trái thang phải
	aladdin->AddAnimation(423);	//đứng im bên trái thang trái
	aladdin->AddAnimation(424);	//bị đánh từ bên trái
	aladdin->AddAnimation(425);	//bị đánh từ bên phải
	aladdin->AddAnimation(599);	//chết
	aladdin->AddAnimation(1600); // nhay danh phai
	aladdin->AddAnimation(1601); // nhay danh trai
	aladdin->AddAnimation(1602); // double nhay
	aladdin->AddAnimation(1603); // dashing right
	aladdin->AddAnimation(1604); // dashing left
	aladdin->AddAnimation(1605);	//knifeing right
	aladdin->AddAnimation(1606);	//knifeing left
	aladdin->AddAnimation(1610); //ném táo phải
	aladdin->AddAnimation(1611); //ném táo trái
	aladdin->AddAnimation(1612);	//ngồi ném táo phải
	aladdin->AddAnimation(1613); // ngồi ném táo trái

	aladdin->SetPosition(40, 660);
	//objects.push_back(aladdin);

	// khởi tạo grid
	grid->InitList(MAX_WIDTH_LV1,MAX_HEIGHT_LV1);
	//grid->AddObject(aladdin);

	#pragma endregion

	#pragma region Ground
	// nền di chuyển
	//for (int i = 0; i < 134; i++)
	//{
	//	Ground *ground = new Ground();
	//	ground->SetPosition(0 + i * 16.0f,990);
	//	//objects.push_back(ground);
	//	grid->AddObject(ground);
	//}


	listObjectsInMap = lsObjs->getObjectsInFile("Resources\\objects.txt");

	if (listObjectsInMap.size()>0) {
		for (int i = 0; i < listObjectsInMap.size(); i++) {
			Ground *ground = new Ground();
			if (listObjectsInMap[i]->name == "Ground") {
				ground->SetPosition(listObjectsInMap[i]->x, listObjectsInMap[i]->y);
				grid->AddObject(ground);
			}
		}
	}

	//for (int i = 0; i < 6; i++)
	//{
	//	Ground *ground = new Ground();
	//	ground->SetPosition(1056 + i * 16.0f, 820);
	//	//objects.push_back(ground);
	//	grid->AddObject(ground);
	//}

	//for (int i = 0; i < 5; i++)
	//{
	//	Ground *ground = new Ground();
	//	ground->SetPosition(1328 + i * 16.0f, 840);
	//	//objects.push_back(ground);
	//	grid->AddObject(ground);
	//}

	//for (int i = 0; i < 4; i++)
	//{
	//	Ground *ground = new Ground();
	//	ground->SetPosition(1350 + i * 16.0f, 780);
	//	//objects.push_back(ground);
	//	grid->AddObject(ground);
	//}
	#pragma endregion

	#pragma region Zombie
	//	zombie = new Zombie();
	//	zombie->AddAnimation(602);
	//	zombie->AddAnimation(604);
	//	zombie->SetPosition(100, 200);
	//	zombie->SetState(ZOMBIE_STATE_WALKING);
	//	//objects.push_back(zombie);
	//	grid->AddObject(zombie);


	//	zombie1 = new Zombie();
	//	zombie1->vx = -1;
	//	zombie1->AddAnimation(602);
	//	zombie1->AddAnimation(604);
	//	zombie1->SetPosition(450, 200);
	//	zombie1->SetState(ZOMBIE_STATE_WALKING);
	//	//objects.push_back(zombie1);
	//	grid->AddObject(zombie1);

	//#pragma endregion

	//#pragma region Soldier
	//	soldier = new Soldier();
	//	soldier->AddAnimation(901);
	//	soldier->AddAnimation(900);
	//	soldier->SetPosition(800, 200);
	//	soldier->SetState(SOLDIER_STATE_WALKING);
	//	//objects.push_back(zombie1);
	//	grid->AddObject(soldier);

	//	soldier1 = new Soldier();
	//	soldier1->vx = -1;
	//	soldier1->AddAnimation(901);
	//	soldier1->AddAnimation(900);
	//	soldier1->SetPosition(1400, 200);
	//	soldier1->SetState(SOLDIER_STATE_WALKING);
	//	//objects.push_back(zombie1);
	//	grid->AddObject(soldier1);
	#pragma endregion

	#pragma region Rock
		Rock*rock = new Rock();
		rock->AddAnimation(990);
		rock->SetPosition(1232,928);
		grid->AddObject(rock);

		rock = new Rock();
		rock->AddAnimation(990);
		rock->SetPosition(1248, 864);
		grid->AddObject(rock);

		Rock*rock1 = new Rock();
		rock1->AddAnimation(991);
		rock1->SetPosition(1155, 890);
		grid->AddObject(rock1);
	#pragma endregion

	#pragma region CheckPoint
		//CheckPoint *checkPoint;
		//checkPoint = new CheckPoint();
		//checkPoint->SetType(CHECKPOINT_LEVELUP);
		//checkPoint->SetPosition(2000, 374);
		////objects.push_back(checkPoint);
		//grid->AddObject(checkPoint);
	#pragma endregion

	#pragma region Pillar
		Pillar*pillar1 = new Pillar();
		pillar1->AddAnimation(1000);
		pillar1->SetPosition(160, 704);
		grid->AddObject(pillar1);

		Pillar*pillar2 = new Pillar();
		pillar2->AddAnimation(1001);
		pillar2->SetPosition(462, 864);
		grid->AddObject(pillar2);

		Pillar*pillar3 = new Pillar();
		pillar3->AddAnimation(1002);
		pillar3->SetPosition(765, 660);
		grid->AddObject(pillar3);

		Pillar*pillar4 = new Pillar();
		pillar4->AddAnimation(1003);
		pillar4->SetPosition(1098, 832);
		grid->AddObject(pillar4);
	#pragma endregion
}

void Update(DWORD dt)
{
	float x, y;
	aladdin->GetPosition(x, y);
	#pragma region Resource
		if (lv1 == true)
		{
			// Lên cấp
			if (aladdin->isLevelUp) {
				//aladdin->SetState(SIMON_STATE_WALK);

				grid->ReleaseList();

				lv2 = true;
				lv1 = false;
				/*aladdin->isLevelUp = false;
				aladdin->SetState(SIMON_STATE_IDLE);*/

			}
		}

	#pragma endregion

	#pragma region Collision
		vector<LPGAMEOBJECT> coObjects;
		if (aladdin->x < 0)
			currentCell = grid->GetCurrentCells(0,y);
		else
			currentCell = grid->GetCurrentCells(x,y);

		for (int i = 0; i < currentCell.size(); i++)
		{
			vector<LPGAMEOBJECT> listObjects = currentCell[i]->GetListObject();
			int listObjectSize = listObjects.size();
			for (int j = 0; j < listObjectSize; j++)
			{
				coObjects.push_back(listObjects[j]);
			}
		}

		aladdin->Update(dt, &coObjects);
		if(boss == false)
			for (int i = 0; i < coObjects.size(); i++)
			{
					coObjects[i]->Update(dt+200, &coObjects);
			}
		else if(boss == true)
		{
			for (int i = 0; i < coObjects.size(); i++)
			{
					coObjects[i]->Update( dt, &coObjects);
					grid->UpdateObjectInCell(coObjects[i]);
			}
		}

	
	#pragma endregion	

	//if (check) {
	//	if (aladdin->nx > 0) {
	//		if (trum->vx > 0)
	//		{
	//			if (aladdin->x < trum->x)
	//			{
	//				trum->vx = -trum->vx;
	//				//soldier->nx = -1.0f;
	//			}
	//			if (GetTickCount() - trum->time_start_shoot > 2000)
	//			{
	//				trum->time_start_shoot = GetTickCount();
	//				bossbullet = new BossBullet(trum, 2 * SCREEN_WIDTH / 3);
	//				bossbullet->SetSpeed(0.1f, 0);
	//				bossbullet->AddAnimation(WEAPON_ANI_BULLET);
	//				//knife->SetType(ITEM_KNIFE);
	//				bossbullet->SetPosition(trum->x, trum->y);
	//				//objects.push_back(knife);
	//				grid->AddObject(bossbullet);
	//			}
	//		}
	//		else
	//		{
	//			if (GetTickCount() - trum->time_start_shoot > 2000)
	//			{
	//				trum->time_start_shoot = GetTickCount();
	//				bossbullet = new BossBullet(trum, 2 * SCREEN_WIDTH / 3);
	//				bossbullet->SetSpeed(-0.1f, 0);
	//				bossbullet->AddAnimation(WEAPON_ANI_BULLET);
	//				//knife->SetType(ITEM_KNIFE);
	//				bossbullet->SetPosition(trum->x, trum->y);
	//				//objects.push_back(knife);
	//				grid->AddObject(bossbullet);
	//			}
	//		}
	//	}
	//	else if (aladdin->nx < 0)
	//	{
	//		if (trum->vx < 0) {
	//			if (aladdin->x > trum->x)
	//			{
	//				trum->vx = -trum->vx;
	//				//soldier->nx = -1.0f;
	//			}
	//			if (GetTickCount() - trum->time_start_shoot > 2000)
	//			{
	//				trum->time_start_shoot = GetTickCount();
	//				bossbullet = new BossBullet(trum, 2 * SCREEN_WIDTH / 3);
	//				bossbullet->SetSpeed(-0.1f, 0);
	//				bossbullet->AddAnimation(WEAPON_ANI_BULLET);
	//				//knife->SetType(ITEM_KNIFE);
	//				bossbullet->SetPosition(trum->x, trum->y);
	//				//objects.push_back(knife);
	//				grid->AddObject(bossbullet);
	//			}
	//		}
	//		else
	//		{
	//			if (GetTickCount() - trum->time_start_shoot > 2000)
	//			{
	//				trum->time_start_shoot = GetTickCount();
	//				bossbullet = new BossBullet(trum, 2 * SCREEN_WIDTH / 3);
	//				bossbullet->SetSpeed(0.1f, 0);
	//				bossbullet->AddAnimation(WEAPON_ANI_BULLET);
	//				//knife->SetType(ITEM_KNIFE);
	//				bossbullet->SetPosition(trum->x, trum->y);
	//				bossbullet->appearTime = GetTickCount();
	//				//objects.push_back(knife);
	//				grid->AddObject(bossbullet);
	//			}
	//		}
	//	}
	//}
	#pragma region Remove Object
		vector<LPGAMEOBJECT> listRemoveObjects;
		for (int i = 0; i < coObjects.size(); i++)
		{
			//if (dynamic_cast<Zombie *>(coObjects.at(i)))
			//{
			//	Zombie *zombie = dynamic_cast<Zombie *>(coObjects.at(i));

			//	if (zombie->GetState() == ZOMBIE_STATE_DIE)
			//	{
			//		listRemoveObjects.push_back(zombie);
			//		item = new Item();
			//		item->SetPosition(zombie->x, zombie->y);
			//		item->SetSpeed(0, -0.1);
			//		coObjects.push_back(item);
			//		grid->AddObject(item);
			//		srand(time(NULL));
			//		int random_portion = rand() % 100;

			//		// Heart
			//		if (random_portion < 30)
			//		{
			//			item->AddAnimation(ITEM_HEART);
			//			item->SetType(ITEM_HEART);
			//		}
			//		// Item 1
			//		else if (random_portion >= 30 && random_portion < 50)
			//		{
			//			item->AddAnimation(ITEM_1);
			//			item->SetType(ITEM_1);
			//		}
			//		// Item 2
			//		else if (random_portion >= 50 && random_portion < 80)
			//		{
			//			item->AddAnimation(ITEM_2);
			//			item->SetType(ITEM_2);
			//		}
			//		// Item 3
			//		else
			//		{
			//			item->AddAnimation(ITEM_3);
			//			item->SetType(ITEM_3);
			//		}
			//	}
			//}
		}

		// Remove lần lượt từng object từ listRemoveObjects trong grid
		for (int i = 0; i < listRemoveObjects.size(); i++)
		{
			grid->RemoveObject(listRemoveObjects[i]);
			delete listRemoveObjects[i];
		}
	#pragma endregion	

	#pragma region Camera
		if (lv1 == true)
		{
			// Oy
			if (y > SCREEN_HEIGHT / 2&&y < MAX_HEIGHT_LV1 - SCREEN_HEIGHT / 2)
			{
				game->mCamera->setX(game->mCamera->getX());
				game->mCamera->setY(y-SCREEN_HEIGHT/2);
			}
			else
			{
				game->mCamera->setX(game->mCamera->getX());
				game->mCamera->setY(game->mCamera->getY());
			}

			// Ox
			if (x > SCREEN_WIDTH / 2 && x < MAX_WIDTH_LV1 - SCREEN_WIDTH / 2)
			{

				game->mCamera->setX(x - SCREEN_WIDTH / 2);
				game->mCamera->setY(game->mCamera->getY());
			}
			else
			{
				game->mCamera->setX(game->mCamera->getX());
				game->mCamera->setY(game->mCamera->getY());
			}
		}
	#pragma endregion
}

void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
		CTextures * textures = CTextures::GetInstance();
		LPDIRECT3DTEXTURE9 tex = textures->Get(ID_TEX_LV1);
		LPDIRECT3DTEXTURE9 tex2 = textures->Get(ID_TEX_LV1_2);		
		float x, y;
		aladdin->GetPosition(x, y);

		LPDIRECT3DTEXTURE9 tileset = textures->Get(ID_TEX_TILESET);
		LPDIRECT3DTEXTURE9 tileset1 = textures->Get(ID_TEX_TILESET_2);
		LPDIRECT3DTEXTURE9 tileset2 = textures->Get(ID_TEX_TILESET_3);
		if (lv1 == true)
		{
			
			map = new	Map (/*48, 10,*/ tileset, 16, 16); 
			map->LoadMatrixMap("Resources\\Mapstate.txt");
			//map->Draw(game->x_cam, game->y_cam);
		}

		map->Draw(game->mCamera->getX(), game->mCamera->getY());
		for (int i = 0; i < currentCell.size(); i++)
		{
			vector<LPGAMEOBJECT> listObject = currentCell[i]->GetListObject();
			int listObjectSize = listObject.size();

			for (int j = 0; j < listObjectSize; j++)
			{	
					listObject[j]->Render();
			}
		}

		aladdin->Render();

		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();

			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();
	game->Init(hWnd);

	keyHandler = new CSampleKeyHander();
	game->InitKeyboard(keyHandler);
	
	grid = Grid::GetInstance();

	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}