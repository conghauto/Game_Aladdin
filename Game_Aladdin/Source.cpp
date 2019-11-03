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
#include "Captain.h"
#include "Ground.h"
#include "define.h"
#include "Zombie.h"
#include "Item.h"
#include "Lantern.h"
#include "Rock.h"
#include "Map.h"
#include "CheckPoint.h"
#include "UI.h"
#include "Water.h"
#include "Grid.h"
#include "Soldier.h"
#include "Bullet.h"
#include "Ball.h"
#include "BossOne.h"
#include "BulletBegin.h"
#include "BossBullet.h"
BossBullet *bossbullet;
BulletBegin *beginBullet;
BossOne *trum;
CGame *game;
Captain * captain;
Item *item;
Effect *whipEffect;
Soldier *soldier;
Soldier *soldier1;
Map *map;
Zombie*zombie;
Zombie *zombie1;
Shield *knife;
Bullet *bullet;
Ball *ball;
//UI * ui;
CSprite *sprite;
//vector<LPGAMEOBJECT> objects;
//vector<int> willDeleteObjects;

Grid *grid;
vector<Cell*> currentCell;

bool lv1 = true;
bool lv2 = false;

bool boss = false;
// check scene lv2->lv2_1
bool checkScene = false;
bool check1 = false;
bool check = false;
//
//check scene lv2_1 ->lv2_2
bool checkScene1 = false;
bool check3 = false;
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
	if (captain->GetState() == SIMON_STATE_DIE) return;

	// Nhay
	if (KeyCode == DIK_Z)
	{
		if (captain->isJump == false && captain->isSit == false && captain->isAttack == false && captain->isOnStair == false)
			captain->SetAction(SIMON_ACTION_JUMP);
	}

	if (KeyCode == DIK_C)
	{
		if (captain->isAttack == false)
			captain->SetAction(SIMON_ACTION_ATTACK);
	}
	else
	if (KeyCode == DIK_X)
	{
		captain->SetCurrentWeapon(809);
		if (captain->isAttack == false) {
			int nx = captain->nx;
			captain->SetAction(SIMON_ACTION_THROW);
			switch (captain->currentWeapon)
			{
			case ITEM_KNIFE:
				knife = new Shield(captain, 2 * SCREEN_WIDTH / 3);
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
				knife->SetPosition(captain->x, captain->y);
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
	if (captain->GetState() == SIMON_STATE_DIE) return;

	// Len xuong cau thang
	if (KeyCode == DIK_UP)
	{
		if (captain->isHoldShield)
		{
			captain->SetState(SIMON_STATE_IDLE);
			captain->isHoldShield = false;
		}
	}

	if (KeyCode == DIK_Q) {
		captain->SetState(SIMON_STATE_IDLE);
		captain->isDashing = false;

	}
	// Ngoi
	if (KeyCode == DIK_DOWN)
	{

		 if (captain->isSit)
		{
			if (!captain->isAttack)
			{
				captain->isSit = false;
			}
			else
			{
				captain->isExitSit = true;
			}
		}
	}

	// Di bo
	else if (KeyCode == DIK_RIGHT || KeyCode == DIK_LEFT)
	{
		captain->isMoving = false;
		captain->vx = 0;
	}
}

void CSampleKeyHander::KeyState(BYTE *states)
{
	// Chet
	if (captain->GetState() == SIMON_STATE_DIE) return;

	// Len xuong cau thang
	if (game->IsKeyDown(DIK_UP))
	{
		//if (!captain->isOnCheckStairDown && !captain->isOnStair && !captain->isAttack && !captain->isJump)
		captain->SetState(SIMON_STATE_HOLD_SHIELD);
	}

	// Ngoi
	if (game->IsKeyDown(DIK_Q)) {
		captain->SetState(SIMON_STATE_DASHING);
	}
	if (game->IsKeyDown(DIK_DOWN))
	{
		 if (!captain->isAttack && !captain->isJump)
			captain->SetState(SIMON_STATE_SIT);
	}

	// Di bo
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (!captain->isSit && !captain->isAttack)
			captain->SetState(SIMON_STATE_WALK);
		if (!captain->isJump &&  !captain->isAttack)
			captain->nx = 1.0f;
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (!captain->isSit && !captain->isAttack)
			captain->SetState(SIMON_STATE_WALK);
		if (!captain->isJump && !captain->isAttack)
			captain->nx = -1.0f;
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
	textures->Add(ID_TEX_MARIO, L"Resources\\aladdin.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_LV1, L"Resources\\lv1.png", D3DCOLOR_XRGB(176, 224, 248));
	textures->Add(ID_TEX_LV1_2, L"Resources\\maplv2.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_ROCK, L"Resources\\rock.png", D3DCOLOR_XRGB(163,73, 164));
	textures->Add(ID_TEX_ZOMBIE, L"Resources\\enemy.png", D3DCOLOR_XRGB(173, 214, 214));
	textures->Add(ID_TEX_ZOMBIE_RIGHT, L"Resources\\enemy.png", D3DCOLOR_XRGB(173, 214, 214));
	textures->Add(ID_TEX_PANTHER, L"Resources\\PANTHER.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_PANTHER_RIGHT, L"Resources\\PANTHER_right.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_FIRE, L"Resources\\123.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_CANDLE, L"Resources\\1.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_WHIP, L"Resources\\WHIP.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_WHIP_2, L"Resources\\WHIP_left.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_TILESET, L"Resources\\tileset.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_TILESET_2, L"Resources\\tileset2.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_CANDLE, L"Resources\\1.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_BBOX, L"Resources\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	textures->Add(ID_TEX_STAIR_BOT, L"Resources\\stair_bottom.png", D3DCOLOR_XRGB(0, 0, 255));//
	textures->Add(ID_TEX_STAIR_TOP, L"Resources\\stair_top.png", D3DCOLOR_XRGB(0, 0, 255));
	textures->Add(ID_TEX_MERMAN_LEFT, L"Resources\\MERMAN.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_MERMAN_RIGHT, L"Resources\\MERMAN_right.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_BAT, L"Resources\\BAT.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_BOSS, L"Resources\\BossBay.png", D3DCOLOR_XRGB(173, 214, 214));

	textures->Add(ID_TEX_AXE, L"Resources\\UI\\AXE.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_BLACK, L"Resources\\UI\\black.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_CROSS, L"Resources\\UI\\CROSS.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_DOUBLE_SHOT_UI, L"Resources\\UI\\DOUBLE_SHOT_UI.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_DOUBLE_SHOT, L"Resources\\UI\\DOUBLE_SHOT.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_TRIPLE_SHOT_UI, L"Resources\\UI\\TRIPLE_SHOT_UI.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_TRIPLE_SHOT, L"Resources\\UI\\TRIPLE_SHOT.png", D3DCOLOR_XRGB(255, 255, 255));
	textures->Add(ID_TEX_ENEMYHP, L"Resources\\UI\\EnemyHP.png", D3DCOLOR_XRGB(0, 0, 255));//
	textures->Add(ID_TEX_HEART, L"Resources\\SMALL_HEART.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_HOLY_WATER, L"Resources\\UI\\HOLY_WATER.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_HP, L"Resources\\UI\\HP.png", D3DCOLOR_XRGB(255, 0, 255));
	//textures->Add(ID_TEX_KNIFE, L"Resources\\UI\\KNIFE.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_NOHP, L"Resources\\UI\\NoHP.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_STOP_WATCH, L"Resources\\UI\\STOP_WATCH.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_RECT, L"Resources\\rect.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_LANTERN, L"Resources\\lantern.png", D3DCOLOR_XRGB(255,0,255));


	textures->Add(ID_TEX_EFFECT1, L"Resources\\0.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_EFFECT2, L"Resources\\DEAD.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_WHIP_VIP, L"Resources\\WHIP_VIP.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_MONEY, L"Resources\\money_bag_red.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_SIT, L"Resources\\sitstate.png", D3DCOLOR_XRGB(173, 214, 214));
	textures->Add(ID_TEX_ITEM1, L"Resources\\item1.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_ITEM2, L"Resources\\item2.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_ITEM3, L"Resources\\item3.png", D3DCOLOR_XRGB(255, 0, 255));

	LPDIRECT3DTEXTURE9 texShield = textures->Get(ID_TEX_KNIFE);
	LPDIRECT3DTEXTURE9 texCaptain = textures->Get(ID_TEX_MARIO);
	LPDIRECT3DTEXTURE9 texCaptainSit = textures->Get(ID_TEX_SIT);
	
	#pragma region Addsprite
	sprites->Add(10001, 1122, 6, 1162, 58, texCaptain);		// đứng im phải

	sprites->Add(10002, 1186, 1215, 1229, 1275, texCaptain);		// đi phải
	sprites->Add(10003, 1239, 1216, 1280, 1273, texCaptain);
	sprites->Add(11002, 1290, 1215, 1333, 1275, texCaptain);
	sprites->Add(11003, 1340, 1215, 1394, 1275, texCaptain);
	sprites->Add(11004, 1397, 1215, 1449, 1275, texCaptain);
	sprites->Add(11005, 1451, 1215, 1496, 1275, texCaptain);
	sprites->Add(11006, 1505, 1215, 1547, 1275, texCaptain);
	sprites->Add(11007, 1560, 1215, 1593, 1275, texCaptain);

	sprites->Add(10004, 1125, 324, 1173, 387, texCaptain);		// đánh phải
	sprites->Add(10005, 1174, 324, 1227, 387, texCaptain);					
	sprites->Add(10006, 1232, 324, 1280, 387, texCaptain);
	sprites->Add(10007, 1289, 324, 1373, 387, texCaptain);
	sprites->Add(10008, 1377, 324, 1431, 387, texCaptain);

	sprites->Add(10025, 1182, 618, 1235, 673, texCaptain);		// ngồi đánh phải			
	sprites->Add(10026, 1235, 618, 1307, 673, texCaptain);
	sprites->Add(10027, 1311, 618, 1404, 673, texCaptain);
	sprites->Add(10028, 1416, 618, 1502, 673, texCaptain);
	sprites->Add(10029, 1235, 618, 1307, 673, texCaptain);


	sprites->Add(10018, 1129, 618, 1176, 673, texCaptain); // ngồi phải

	sprites->Add(12001, 1196, 820, 1248, 890, texCaptain); // nhảy phải
	sprites->Add(12002, 1258, 820, 1313, 890, texCaptain);
	sprites->Add(12003, 1321, 820, 1377, 890, texCaptain);
	sprites->Add(12004, 1506, 820, 1545, 890, texCaptain);
	sprites->Add(12005, 1681, 810, 1713, 907, texCaptain);

	sprites->Add(10054, 436, 67, 471, 130, texCaptain);//bị đánh từ bên trái

	sprites->Add(13001, 1124, 225, 1170, 290, texCaptain); //ném táo phải
	sprites->Add(13002, 1178, 225, 1217, 290, texCaptain);
	sprites->Add(13003, 1227, 225, 1270, 290, texCaptain);
	sprites->Add(13004, 1282, 225, 1330, 290, texCaptain);
	sprites->Add(13005, 1339, 225, 1382, 290, texCaptain);

	sprites->Add(13021, 1127, 562, 1169, 615, texCaptain); // ngồi ném táo phải
	sprites->Add(13022, 1176, 562, 1218, 615, texCaptain);
	sprites->Add(13023, 1225, 562, 1278, 615, texCaptain);
	sprites->Add(13024, 1284, 562, 1365, 615, texCaptain);
	sprites->Add(13025, 1378, 562, 1436, 615, texCaptain);

	LPDIRECT3DTEXTURE9 texCaptain2 = textures->Get(ID_TEX_MARIO);
	sprites->Add(10011, 1079, 6, 1120, 59, texCaptain2);		// đứng im trái

	sprites->Add(10012, 1008, 1215, 1057, 1275, texCaptain2);		// đi trái
	sprites->Add(10013, 959, 1215, 1003, 1275, texCaptain2);
	sprites->Add(11012, 907, 1215, 952, 1275, texCaptain2);
	sprites->Add(11013, 848, 1215, 904, 1275, texCaptain2);
	sprites->Add(11014, 793, 1215, 846, 1275, texCaptain2);
	sprites->Add(11015, 742, 1215, 789, 1275, texCaptain2);
	sprites->Add(11016, 692, 1215, 736, 1275, texCaptain2);
	sprites->Add(11017, 643, 1215, 685, 1275, texCaptain2);

	sprites->Add(10014, 1070, 324, 1115, 387, texCaptain2);		// đánh trái				
	sprites->Add(10015, 1013, 324, 1068, 387, texCaptain2);
	sprites->Add(10016, 956, 324, 1009, 387, texCaptain2);
	sprites->Add(10017, 867, 324, 954, 387, texCaptain2);
	sprites->Add(10009, 808, 324, 866, 387, texCaptain2);

	sprites->Add(10031, 1005, 618, 1060, 673, texCaptain);		// ngồi đánh trái				
	sprites->Add(10032, 927, 618, 1009, 673, texCaptain);
	sprites->Add(10033, 835, 618, 930, 673, texCaptain);
	sprites->Add(10034, 737, 618, 825, 673, texCaptain);
	sprites->Add(10035, 927, 618, 1009, 673, texCaptain);

	sprites->Add(10019, 1064, 618, 1115, 673, texCaptain); // ngồi trái

	sprites->Add(12011, 994, 820, 1047, 890, texCaptain);//nhảy trái
	sprites->Add(12012, 929, 820, 986, 890, texCaptain);
	sprites->Add(12013, 801, 820, 851, 890, texCaptain);
	sprites->Add(12014, 694, 820, 728, 890, texCaptain);
	sprites->Add(12015, 522, 820, 558, 890, texCaptain);

	sprites->Add(13011, 1070, 225, 1115, 290, texCaptain); // ném táo trái
	sprites->Add(13012, 1022, 225, 1067, 290, texCaptain);
	sprites->Add(13013, 973, 225, 1014, 290, texCaptain);
	sprites->Add(13014, 912, 225, 928, 290, texCaptain);
	sprites->Add(13015, 861, 225, 902, 290, texCaptain);

	sprites->Add(13031, 1073, 562, 1115, 615, texCaptain); //ngồi ném táo trái
	sprites->Add(13032, 1022, 562, 1065, 615, texCaptain);
	sprites->Add(13033, 962, 562, 1015, 615, texCaptain);
	sprites->Add(13034, 877, 562, 956, 615, texCaptain);
	sprites->Add(13035, 808, 562, 866, 615, texCaptain);

	sprites->Add(10055, 12, 70, 41, 130, texCaptain2);//bị đánh từ bên phải

	sprites->Add(10099, 180, 237, 240, 264, texCaptain);		// chết 

	sprites->Add(10110, 106, 144, 149, 189, texCaptainSit);//nhay danh phai

	sprites->Add(10111, 59, 144, 102, 189, texCaptainSit); // nhay danh trai

	sprites->Add(10120, 282, 96, 302, 136, texCaptain); // double nhay 
	sprites->Add(10121, 305, 96, 327, 136, texCaptain);
	sprites->Add(10122, 327, 96, 349, 136, texCaptain);

	sprites->Add(10130, 237, 79, 276, 123, texCaptainSit);// dashing left
	sprites->Add(10131, 310, 79, 348, 123, texCaptainSit);// dashing right

	sprites->Add(10140, 233, 224, 258, 268, texCaptain); // knifeing right
	sprites->Add(10141, 202, 224, 227, 268, texCaptain);	//knifeing left
	sprites->Add(1200, 1482, 19, 1511, 71, texShield);	//khien
	#pragma endregion

	#pragma region ENEMY

	LPDIRECT3DTEXTURE9 whipR = textures->Get(ID_TEX_WHIP);
	sprites->Add(10022, 570, 200, 554, 267, whipR);			//roi lv2 phải
	sprites->Add(10023, 342, 260, 312, 316, whipR);
	sprites->Add(10024, 342, 328, 458, 364, whipR); // 116

	sprites->Add(10062, 570, 200, 554, 267, whipR);			//roi lv1 phải	
	sprites->Add(10063, 342, 260, 312, 316, whipR);
	sprites->Add(10064, 112, 65, 189, 100, whipR); 
	
	sprites->Add(10056, 570, 0, 553, 67, whipR);			//roi lv0 phải	
	sprites->Add(10057, 346, 0, 312, 49, whipR);
	sprites->Add(10058, 114, 0, 188, 30, whipR); // 74
	

	LPDIRECT3DTEXTURE9 whipL = textures->Get(ID_TEX_WHIP_2);
	sprites->Add(10065, 105, 70, 165, 133, whipL);			//roi lv1 trái	
	sprites->Add(10066, 340, 65, 406, 117, whipL);
	sprites->Add(10067, 572, 65, 530, 100, whipL); 

	sprites->Add(10059, 110, 0, 166, 66, whipL);			//roi lv0 trái
	sprites->Add(10060, 349, 0, 408, 52, whipL);
	sprites->Add(10061, 585, 0, 529, 30, whipL);


	LPDIRECT3DTEXTURE9 texEnemy = textures->Get(ID_TEX_ZOMBIE);
	sprites->Add(30001, 135, 12, 161, 56, texEnemy); // đi trái 
	sprites->Add(30002, 104, 12, 127, 56, texEnemy);
	sprites->Add(30003, 72, 12, 97, 56, texEnemy);

	sprites->Add(30007, 623, 20, 650, 56, texEnemy); //chet right



	LPDIRECT3DTEXTURE9 texEnemy0 = textures->Get(ID_TEX_ZOMBIE_RIGHT);
	sprites->Add(30004, 719, 12, 743, 56, texEnemy0); // đi Phải 
	sprites->Add(30005, 753, 12, 775, 56, texEnemy0);
	sprites->Add(30006, 783, 12, 808, 56, texEnemy0);

	sprites->Add(30008, 231, 21, 257, 56, texEnemy0);//chet left

		//Soldier
	sprites->Add(33000, 720, 66, 744, 112, texEnemy); // di phai
	sprites->Add(33001, 752, 66, 775, 112, texEnemy);
	sprites->Add(33002, 783, 66, 809, 112, texEnemy);

	sprites->Add(33003,	137, 66, 161, 112, texEnemy); //di trai
	sprites->Add(33004, 104, 66, 126, 112, texEnemy);
	sprites->Add(33005, 71, 66, 96, 112, texEnemy);

	sprites->Add(34000, 653, 66, 681, 112, texEnemy); //chet phai

	sprites->Add(34001, 200,66, 224,112, texEnemy); //chet trai

	LPDIRECT3DTEXTURE9 texBoss = textures->Get(ID_TEX_BOSS); //boss bay phai
	sprites->Add(41000, 454, 17, 486, 63, texBoss);
	sprites->Add(41001, 491, 17, 514, 63, texBoss);
	sprites->Add(41002, 518, 17, 551, 63, texBoss);

	sprites->Add(41003, 139, 17, 170, 63, texBoss); //boss di trai
	sprites->Add(41004, 109, 17, 131, 63, texBoss);
	sprites->Add(41005, 73, 17, 109, 63, texBoss);

	//dan bay luc dau
	sprites->Add(40099, 363,158, 378,171, texBoss);

	//boss dung trai
	sprites->Add(44010, 9, 16, 33, 64, texBoss);

	//boss dung phai
	sprites->Add(44011, 590, 16, 615, 64, texBoss);

	//boss drop trai
	sprites->Add(44020, 45, 130, 79, 181, texBoss);

	//boss drop phai
	sprites->Add(44021, 545, 130, 575, 181, texBoss);

	LPDIRECT3DTEXTURE9 texEnemy1 = textures->Get(ID_TEX_PANTHER);
	sprites->Add(30011, 0, 0, 50, 30, texEnemy1); // báo
	sprites->Add(30012, 64, 0, 120, 30, texEnemy1); // báo chạy trái
	sprites->Add(30013, 126, 0, 188, 30, texEnemy1);
	sprites->Add(30014, 188, 0, 256, 30, texEnemy1);

	LPDIRECT3DTEXTURE9 texEnemy10 = textures->Get(ID_TEX_PANTHER_RIGHT);	
	sprites->Add(30015, 136, 0, 190, 30, texEnemy10); //báo  chạy phải
	sprites->Add(30016, 67, 0, 130, 30, texEnemy10);
	sprites->Add(30017, 0, 0, 64, 30, texEnemy10);

	LPDIRECT3DTEXTURE9 texEnemy11 = textures->Get(ID_TEX_BAT);
	sprites->Add(30018, 0, 0, 32, 32, texEnemy11); // dơi // nằm chờ
	sprites->Add(30019, 32, 0, 64, 32, texEnemy11);
	sprites->Add(30020, 64, 0, 96, 32, texEnemy11);
	sprites->Add(30021, 96, 0, 128, 32, texEnemy11);

	LPDIRECT3DTEXTURE9 texEnemy12 = textures->Get(ID_TEX_BOSS);
	sprites->Add(30022, 27, 0, 67, 38, texEnemy12); // boss // nằm chờ
	sprites->Add(30023, 98, 0, 200, 45, texEnemy12);
	sprites->Add(30024, 213, 0, 281, 45, texEnemy12);
	
	LPDIRECT3DTEXTURE9 texEnemy13 = textures->Get(ID_TEX_MERMAN_LEFT);
	sprites->Add(30025, 0, 0, 32, 64, texEnemy13); //người cá trái
	sprites->Add(30026, 32, 0, 64, 64, texEnemy13);
	sprites->Add(30027, 64, 0, 96, 64, texEnemy13);

	LPDIRECT3DTEXTURE9 texEnemy14 = textures->Get(ID_TEX_MERMAN_RIGHT);
	sprites->Add(30028, 64, 0, 96, 64, texEnemy14); // người cá phải
	sprites->Add(30029, 32, 0, 64, 64, texEnemy14);
	sprites->Add(30030, 0, 0, 32, 64, texEnemy14);

	LPDIRECT3DTEXTURE9 texEnemy2 = textures->Get(ID_TEX_FIRE);
	sprites->Add(40011, 0, 0, 32, 64, texEnemy2);
	sprites->Add(40012, 32, 0, 64, 64, texEnemy2);

	LPDIRECT3DTEXTURE9 texEnemy3 = textures->Get(ID_TEX_CANDLE);
	sprites->Add(40013, 0, 0, 16, 32, texEnemy3);
	sprites->Add(40014, 16, 0, 32, 32, texEnemy3);

	LPDIRECT3DTEXTURE9 texMisc3 = textures->Get(ID_TEX_LADDER);
	sprites->Add(40015, 0, 0, 32, 32, texMisc3);

	LPDIRECT3DTEXTURE9 texMisc4 = textures->Get(ID_TEX_LADDER_LEFT);
	sprites->Add(40016, 0, 0, 32, 32, texMisc4);

	LPDIRECT3DTEXTURE9 texMisc5 = textures->Get(ID_TEX_STAIR_BOT);
	sprites->Add(40017, 0, 0, 32, 32, texMisc5);

	LPDIRECT3DTEXTURE9 texMisc6 = textures->Get(ID_TEX_STAIR_TOP);
	sprites->Add(40018, 0, 0, 32, 32, texMisc6);

	LPDIRECT3DTEXTURE9 texMisc7 = textures->Get(ID_TEX_HEART);
	sprites->Add(40019, 0, 0, 16, 16, texMisc7);

	LPDIRECT3DTEXTURE9 texMisc8 = textures->Get(ID_TEX_EFFECT1);
	sprites->Add(40020, 0, 0, 16, 20, texMisc8);

	LPDIRECT3DTEXTURE9 texMisc9 = textures->Get(ID_TEX_EFFECT2);
	sprites->Add(40021, 0, 10, 42, 37, texMisc9);
	sprites->Add(41021, 42, 10, 84, 37, texMisc9);
	sprites->Add(42021, 84, 10, 126, 37, texMisc9);

	LPDIRECT3DTEXTURE9 texMisc10 = textures->Get(ID_TEX_WHIP_VIP);
	sprites->Add(40022, 0, 0, 32, 32, texMisc10);

	LPDIRECT3DTEXTURE9 texMisc11 = textures->Get(ID_TEX_KNIFE);
	sprites->Add(40023, 0, 0, 32, 18, texMisc11);

	LPDIRECT3DTEXTURE9 texMisc12 = textures->Get(ID_TEX_MONEY);
	sprites->Add(40024, 0, 0, 30, 30, texMisc12);

	LPDIRECT3DTEXTURE9 texMisc13 = textures->Get(ID_TEX_LANTERN);
	sprites->Add(40025, 0, 0, 16, 16, texMisc13);

	LPDIRECT3DTEXTURE9 texMisc14 = textures->Get(ID_TEX_WATER);
	sprites->Add(40026, 0, 0, 16, 16, texMisc14);
	sprites->Add(40027, 16, 0, 32, 32, texMisc14);

	LPDIRECT3DTEXTURE9 texMisc15 = textures->Get(ID_TEX_ITEM1);
	sprites->Add(40028, 0, 0, 18, 20, texMisc15);

	LPDIRECT3DTEXTURE9 texMisc16 = textures->Get(ID_TEX_ITEM2);
	sprites->Add(40029, 0, 0,18,21, texMisc16);

	LPDIRECT3DTEXTURE9 texMisc17 = textures->Get(ID_TEX_ITEM3);
	sprites->Add(40030, 0, 0, 16, 24, texMisc17);

	LPDIRECT3DTEXTURE9 texRock1 = textures->Get(ID_TEX_ROCK); // rock
	sprites->Add(40031, 0, 4, 30, 20, texRock1);
	sprites->Add(40032, 35, 4, 66, 20, texRock1);
	sprites->Add(40033, 72, 4, 105, 21, texRock1);
	sprites->Add(40034, 111,4, 147, 24, texRock1);
	sprites->Add(40035, 154, 4, 194, 26, texRock1);

	LPDIRECT3DTEXTURE9 texBullet = textures->Get(ID_TEX_ZOMBIE); //bullet of soldier
	sprites->Add(44444, 262, 30, 273, 40, texBullet);

	LPDIRECT3DTEXTURE9 texBall = textures->Get(ID_TEX_ZOMBIE); //bullet of soldier
	sprites->Add(45555, 229, 70, 250, 84, texBall);
	
	#pragma endregion


	LPANIMATION ani;

	#pragma region CaptainAnimation

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

	ani = new CAnimation(100); //zombie đi trái
	ani->Add(30001);
	ani->Add(30002);
	ani->Add(30003);
	animations->Add(602, ani);

	ani = new CAnimation(100); //zombie đi phải
	ani->Add(30004);
	ani->Add(30005);
	ani->Add(30006);
	animations->Add(604, ani);

	ani = new CAnimation(100);//zombie die right
	ani->Add(30007);
	animations->Add(850, ani);

	ani = new CAnimation(100);//zombie die left
	ani->Add(30008);
	animations->Add(851, ani);


	ani = new CAnimation(100); //soldier đi phải
	ani->Add(33000);
	ani->Add(33001);
	ani->Add(33002);
	animations->Add(900, ani);

	ani = new CAnimation(100); //soldier đi trái
	ani->Add(33003);
	ani->Add(33004);
	ani->Add(33005);
	animations->Add(901, ani);


	ani = new CAnimation(100); //soldier chet phai
	ani->Add(34000);
	animations->Add(902, ani);
	
	ani = new CAnimation(100); //soldier chet trai
	ani->Add(34001);
	animations->Add(903, ani);

	ani = new CAnimation(100);	//đất2
	ani->Add(20002);
	animations->Add(603, ani);

	ani = new CAnimation(100); //panther nằm chờ
	ani->Add(30011);
	animations->Add(605, ani);

	ani = new CAnimation(100); //panther chạy trái
	ani->Add(30012);
	ani->Add(30013);
	ani->Add(30014);
	animations->Add(606, ani);

	ani = new CAnimation(100); //panther chạy phải
	ani->Add(30015);
	ani->Add(30016);
	ani->Add(30017);
	animations->Add(607, ani);

	ani = new CAnimation(100); //panther phóng
	ani->Add(30014);
	animations->Add(608, ani);

	ani = new CAnimation(100); //boss dơi nằm chờ
	ani->Add(30022);
	animations->Add(609, ani);

	ani = new CAnimation(100); //boosss dơi bay
	ani->Add(30023);
	ani->Add(30024);
	animations->Add(610, ani);

	ani = new CAnimation(100); //fire
	ani->Add(40011);
	ani->Add(40012);
	animations->Add(700, ani);

	ani = new CAnimation(100); //candle
	ani->Add(40013);
	ani->Add(40014);
	animations->Add(800, ani);

	ani = new CAnimation(0); //STAIR RIHGT
	ani->Add(40015);
	animations->Add(801, ani);

	ani = new CAnimation(0); //STAIR LEFT
	ani->Add(40016);
	animations->Add(802, ani);

	ani = new CAnimation(0); //STAIR BOT
	ani->Add(40017);
	animations->Add(803, ani);

	ani = new CAnimation(0); //STAIR TOP
	ani->Add(40018);
	animations->Add(804, ani);

	ani = new CAnimation(0); //heart
	ani->Add(40019);
	animations->Add(805, ani);

	ani = new CAnimation(150); //hieu ung' dau' sao
	ani->Add(40020);
	animations->Add(806, ani);

	ani = new CAnimation(50); //hieu ung toe' lua
	ani->Add(40021);
	ani->Add(41021);
	ani->Add(42021);
	animations->Add(807, ani);

	ani = new CAnimation(0); //whip item nang cap
	ani->Add(40022);
	animations->Add(808, ani);

	ani = new CAnimation(0); //knife
	ani->Add(40023);
	animations->Add(809, ani);

	ani = new CAnimation(0); //money
	ani->Add(40024);
	animations->Add(810, ani);

	ani = new CAnimation(0); //đèn lồng
	ani->Add(40025);
	animations->Add(811, ani);

	ani = new CAnimation(0); //item 1
	ani->Add(40028);
	animations->Add(813, ani);

	ani = new CAnimation(0); //item 2
	ani->Add(40029);
	animations->Add(814, ani);

	ani = new CAnimation(0); //item 3 
	ani->Add(40030);
	animations->Add(815, ani);

	ani = new CAnimation(0); // bậc thang
	ani->Add(40031);
	ani->Add(40032);
	ani->Add(40033);
	ani->Add(40034);
	ani->Add(40035);
	animations->Add(999, ani);

	#pragma endregion

	#pragma region captain
	captain = new Captain();
	captain->AddAnimation(400);	// đứng phải	
	captain->AddAnimation(401);	//đứng trái	
	captain->AddAnimation(500);	// đi phải
	captain->AddAnimation(501);	//đi trái
	captain->AddAnimation(402);	//đánh phải 
	captain->AddAnimation(403);	//đánh trái
	captain->AddAnimation(404);	//ngồi phải
	captain->AddAnimation(405);	//ngồi trái
	captain->AddAnimation(406);	//nhảy phải 
	captain->AddAnimation(407);	//nhảy trái
	captain->AddAnimation(410);	//ngồi đánh phải 
	captain->AddAnimation(411);	//ngồi đánh trái
	captain->AddAnimation(412);	//trên thang phải đánh phải
	captain->AddAnimation(413);	//trên thang phải đánh trái
	captain->AddAnimation(414);	//trên thang trái đánh phải
	captain->AddAnimation(415);	//trên thang trái đánh trái
	captain->AddAnimation(416);	//lên thang phải
	captain->AddAnimation(417);	//xuống thang phải
	captain->AddAnimation(418);	//lên thang trái
	captain->AddAnimation(419);	//xuống thang trái
	captain->AddAnimation(420);	//đứng im bên phải thang phải
	captain->AddAnimation(421);	//đứng im bên phải thang trái
	captain->AddAnimation(422);	//đứng im bên trái thang phải
	captain->AddAnimation(423);	//đứng im bên trái thang trái
	captain->AddAnimation(424);	//bị đánh từ bên trái
	captain->AddAnimation(425);	//bị đánh từ bên phải
	captain->AddAnimation(599);	//chết
	captain->AddAnimation(1600); // nhay danh phai
	captain->AddAnimation(1601); // nhay danh trai
	captain->AddAnimation(1602); // double nhay
	captain->AddAnimation(1603); // dashing right
	captain->AddAnimation(1604); // dashing left
	captain->AddAnimation(1605);	//knifeing right
	captain->AddAnimation(1606);	//knifeing left
	captain->AddAnimation(1610); //ném táo phải
	captain->AddAnimation(1611); //ném táo trái
	captain->AddAnimation(1612);	//ngồi ném táo phải
	captain->AddAnimation(1613); // ngồi ném táo trái

	captain->SetPosition(40, 700);
	//objects.push_back(captain);

	// khởi tạo grid
	grid->InitList(MAX_WIDTH_LV1,MAX_HEIGHT_LV1);
	//grid->AddObject(captain);

	#pragma endregion

	#pragma region Ground
	// nền di chuyển
	for (int i = 0; i < 134; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(0 + i * 16.0f,990);
		//objects.push_back(ground);
		grid->AddObject(ground);
	}
	#pragma endregion

	#pragma region Zombie
		zombie = new Zombie();
		zombie->AddAnimation(602);
		zombie->AddAnimation(604);
		zombie->SetPosition(100, 200);
		zombie->SetState(ZOMBIE_STATE_WALKING);
		//objects.push_back(zombie);
		grid->AddObject(zombie);


		zombie1 = new Zombie();
		zombie1->vx = -1;
		zombie1->AddAnimation(602);
		zombie1->AddAnimation(604);
		zombie1->SetPosition(450, 200);
		zombie1->SetState(ZOMBIE_STATE_WALKING);
		//objects.push_back(zombie1);
		grid->AddObject(zombie1);

	#pragma endregion

	#pragma region Soldier
		soldier = new Soldier();
		soldier->AddAnimation(901);
		soldier->AddAnimation(900);
		soldier->SetPosition(800, 200);
		soldier->SetState(SOLDIER_STATE_WALKING);
		//objects.push_back(zombie1);
		grid->AddObject(soldier);

		soldier1 = new Soldier();
		soldier1->vx = -1;
		soldier1->AddAnimation(901);
		soldier1->AddAnimation(900);
		soldier1->SetPosition(1400, 200);
		soldier1->SetState(SOLDIER_STATE_WALKING);
		//objects.push_back(zombie1);
		grid->AddObject(soldier1);
	#pragma endregion

	#pragma region Rock
		Rock*rock = new Rock();
		rock->AddAnimation(999);
		rock->SetPosition(200,950);
		grid->AddObject(rock);


		//Lantern *lantern = new Lantern();
		//lantern->AddAnimation(811);
		//lantern->SetPosition(64, 80);
		////objects.push_back(lantern);
		//grid->AddObject(lantern);

		//lantern = new Lantern();
		//lantern->AddAnimation(811);
		//lantern->SetPosition(128, 180);
		////objects.push_back(lantern);
		//grid->AddObject(lantern);

	
	#pragma endregion

	#pragma region CheckPoint
		//CheckPoint *checkPoint;
		//checkPoint = new CheckPoint();
		//checkPoint->SetType(CHECKPOINT_LEVELUP);
		//checkPoint->SetPosition(2000, 374);
		////objects.push_back(checkPoint);
		//grid->AddObject(checkPoint);
	#pragma endregion
}

void Update(DWORD dt)
{
	float x, y;
	captain->GetPosition(x, y);
	#pragma region Resource
		if (lv1 == true)
		{
			// Lên cấp
			if (captain->isLevelUp) {
				//captain->SetState(SIMON_STATE_WALK);

				grid->ReleaseList();

				lv2 = true;
				lv1 = false;
				/*captain->isLevelUp = false;
				captain->SetState(SIMON_STATE_IDLE);*/

			}
		}

		if (lv2 == true) {

			//if (countLoadResourceLv2 == false)
			//{

			//	game->mCamera->setX(0);
			//	grid->InitList(MAX_WIDTH_LV2);
			//	countLoadResourceLv2 = true;
			//	captain->SetPosition(10, 20);
			//	captain->GetPosition(x, y);

			//	LoadResourceLv1();

			//	beginBullet = new BulletBegin();
			//	beginBullet->AddAnimation(WEAPON_ANI_BALLBEGIN);
			//	beginBullet->AddAnimation(WEAPON_ANI_BALLBEGIN);
			//	//beginBullet->time_start_shoot = GetTickCount();
			//	beginBullet->SetPosition(100, 215);
			//	beginBullet->SetState(SOLDIER_STATE_WALKING);
			//	//objects.push_back(zombie1);
			//	grid->AddObject(beginBullet);		


			//}
		}
	#pragma endregion

	#pragma region Collision
		vector<LPGAMEOBJECT> coObjects;
		if (captain->x < 0)
			currentCell = grid->GetCurrentCells(0, captain->y);
		else
			currentCell = grid->GetCurrentCells(captain->x, captain->y);

		for (int i = 0; i < currentCell.size(); i++)
		{
			vector<LPGAMEOBJECT> listObjects = currentCell[i]->GetListObject();
			int listObjectSize = listObjects.size();
			for (int j = 0; j < listObjectSize; j++)
			{
				coObjects.push_back(listObjects[j]);
			}
		}

		captain->Update(dt, &coObjects);
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
	if (lv1 == true)
	{
		if (captain->nx > 0) {
			if (zombie->vx > 0)
			{
				if (captain->x < zombie->x)
				{
					zombie->vx = -zombie->vx;
					//soldier->nx = -1.0f;
				}
				if (GetTickCount() - zombie->time_start_shoot > 2000)
				{
					zombie->time_start_shoot = GetTickCount();
					bullet = new Bullet(zombie, 2 * SCREEN_WIDTH / 3);
					bullet->SetSpeed(0.1f, 0);
					bullet->AddAnimation(WEAPON_ANI_BULLET);
					//knife->SetType(ITEM_KNIFE);
					bullet->SetPosition(zombie->x, zombie->y);
					//objects.push_back(knife);
					grid->AddObject(bullet);
				}
			}
			else
			{
				if (GetTickCount() - zombie->time_start_shoot > 2000)
				{
					zombie->time_start_shoot = GetTickCount();
					bullet = new Bullet(zombie, 2 * SCREEN_WIDTH / 3);
					bullet->SetSpeed(-0.1f, 0);
					bullet->AddAnimation(WEAPON_ANI_BULLET);
					//knife->SetType(ITEM_KNIFE);
					bullet->SetPosition(zombie->x, zombie->y);
					//objects.push_back(knife);
					grid->AddObject(bullet);
				}
			}
		}
		else if (captain->nx < 0)
		{
			if (zombie->vx < 0) {
				if (captain->x > zombie->x)
				{
					zombie->vx = -zombie->vx;
					//soldier->nx = -1.0f;
				}
				if (GetTickCount() - zombie->time_start_shoot > 2000)
				{
					zombie->time_start_shoot = GetTickCount();
					bullet = new Bullet(zombie, 2 * SCREEN_WIDTH / 3);
					bullet->SetSpeed(-0.1f, 0);
					bullet->AddAnimation(WEAPON_ANI_BULLET);
					//knife->SetType(ITEM_KNIFE);
					bullet->SetPosition(zombie->x, zombie->y);
					//objects.push_back(knife);
					grid->AddObject(bullet);
				}
			}
			else
			{
				if (GetTickCount() - zombie->time_start_shoot > 2000)
				{
					zombie->time_start_shoot = GetTickCount();
					bullet = new Bullet(zombie, 2 * SCREEN_WIDTH / 3);
					bullet->SetSpeed(0.1f, 0);
					bullet->AddAnimation(WEAPON_ANI_BULLET);
					//knife->SetType(ITEM_KNIFE);
					bullet->SetPosition(zombie->x, zombie->y);
					bullet->appearTime = GetTickCount();
					//objects.push_back(knife);
					grid->AddObject(bullet);
				}
			}
		}


		if (zombie1->vx > 0)
		{
			//if (captain->x < zombie1->x)
			//{
			//	zombie1->vx = -zombie1->vx;
			//	//soldier->nx = -1.0f;
			//}
			if (GetTickCount() - zombie1->time_start_shoot > 2000)
			{
				zombie1->time_start_shoot = GetTickCount();
				bullet = new Bullet(zombie1, 2 * SCREEN_WIDTH / 3);
				bullet->SetSpeed(0.1f, 0);
				bullet->AddAnimation(WEAPON_ANI_BULLET);
				//knife->SetType(ITEM_KNIFE);
				bullet->SetPosition(zombie1->x, zombie1->y);
				//objects.push_back(knife);
				grid->AddObject(bullet);
			}
		}
		else
		{
			if (GetTickCount() - zombie1->time_start_shoot > 2000)
			{
				zombie1->time_start_shoot = GetTickCount();
				bullet = new Bullet(zombie1, 2 * SCREEN_WIDTH / 3);
				bullet->SetSpeed(-0.1f, 0);
				bullet->AddAnimation(WEAPON_ANI_BULLET);
				//knife->SetType(ITEM_KNIFE);
				bullet->SetPosition(zombie1->x, zombie1->y);
				//objects.push_back(knife);
				grid->AddObject(bullet);
			}
		}

		if (zombie1->vx < 0) {
			//if (captain->x > zombie1->x)
			//{
			//	zombie1->vx = -zombie1->vx;
			//	//soldier->nx = -1.0f;
			//}
			if (GetTickCount() - zombie1->time_start_shoot > 2000)
			{
				zombie1->time_start_shoot = GetTickCount();
				bullet = new Bullet(zombie1, 2 * SCREEN_WIDTH / 3);
				bullet->SetSpeed(-0.1f, 0);
				bullet->AddAnimation(WEAPON_ANI_BULLET);
				//knife->SetType(ITEM_KNIFE);
				bullet->SetPosition(zombie1->x, zombie1->y);
				//objects.push_back(knife);
				grid->AddObject(bullet);
			}
		}
		else
		{
			if (GetTickCount() - zombie1->time_start_shoot > 2000)
			{
				zombie1->time_start_shoot = GetTickCount();
				bullet = new Bullet(zombie1, 2 * SCREEN_WIDTH / 3);
				bullet->SetSpeed(0.1f, 0);
				bullet->AddAnimation(WEAPON_ANI_BULLET);
				//knife->SetType(ITEM_KNIFE);
				bullet->SetPosition(zombie1->x, zombie1->y);
				bullet->appearTime = GetTickCount();
				//objects.push_back(knife);
				grid->AddObject(bullet);
			}
		}


		//soldier

		if (captain->nx > 0) {
			if (soldier->vx > 0)
			{
				if (captain->x < soldier->x)
				{
					soldier->vx = -soldier->vx;
					//soldier->nx = -1.0f;
				}
				if (GetTickCount() - soldier->time_start_shoot > 2000)
				{
					soldier->time_start_shoot = GetTickCount();
					ball = new Ball(soldier, 2 * SCREEN_WIDTH / 3);
					ball->SetSpeed(0.1f, 0);
					ball->AddAnimation(WEAPON_ANI_BALL);
					//knife->SetType(ITEM_KNIFE);
					ball->SetPosition(soldier->x, soldier->y);
					//objects.push_back(knife);
					grid->AddObject(ball);
				}
			}
			else
			{
				if (GetTickCount() - soldier->time_start_shoot > 2000)
				{
					soldier->time_start_shoot = GetTickCount();
					ball = new Ball(soldier, 2 * SCREEN_WIDTH / 3);
					ball->SetSpeed(-0.1f, 0);
					ball->AddAnimation(WEAPON_ANI_BALL);
					//knife->SetType(ITEM_KNIFE);
					ball->SetPosition(soldier->x, soldier->y);
					//objects.push_back(knife);
					grid->AddObject(ball);
				}
			}
		}
		else if (captain->nx < 0)
		{
			if (soldier->vx < 0) {
				if (captain->x > soldier->x)
				{
					soldier->vx = -soldier->vx;
					//soldier->nx = -1.0f;
				}
				if (GetTickCount() - soldier->time_start_shoot > 2000)
				{
					soldier->time_start_shoot = GetTickCount();
					ball = new Ball(soldier, 2 * SCREEN_WIDTH / 3);
					ball->SetSpeed(-0.1f, 0);
					ball->AddAnimation(WEAPON_ANI_BALL);
					//knife->SetType(ITEM_KNIFE);
					ball->SetPosition(soldier->x, soldier->y);
					//objects.push_back(knife);
					grid->AddObject(ball);
				}
			}
			else
			{
				if (GetTickCount() - soldier->time_start_shoot > 2000)
				{
					soldier->time_start_shoot = GetTickCount();
					ball = new Ball(soldier, 2 * SCREEN_WIDTH / 3);
					ball->SetSpeed(0.1f, 0);
					ball->AddAnimation(WEAPON_ANI_BALL);
					//knife->SetType(ITEM_KNIFE);
					ball->SetPosition(soldier->x, soldier->y);
					ball->appearTime = GetTickCount();
					//objects.push_back(knife);
					grid->AddObject(ball);
				}
			}
		}


		if (soldier1->vx > 0)
		{
			//if (captain->x < zombie1->x)
			//{
			//	zombie1->vx = -zombie1->vx;
			//	//soldier->nx = -1.0f;
			//}
			if (GetTickCount() - soldier1->time_start_shoot > 2000)
			{
				soldier1->time_start_shoot = GetTickCount();
				ball = new Ball(soldier1, 2 * SCREEN_WIDTH / 3);
				ball->SetSpeed(0.1f, 0);
				ball->AddAnimation(WEAPON_ANI_BALL);
				//knife->SetType(ITEM_KNIFE);
				ball->SetPosition(soldier1->x, soldier1->y);
				//objects.push_back(knife);
				grid->AddObject(ball);
			}
		}
		else
		{
			if (GetTickCount() - soldier1->time_start_shoot > 2000)
			{
				soldier1->time_start_shoot = GetTickCount();
				ball = new Ball(soldier1, 2 * SCREEN_WIDTH / 3);
				ball->SetSpeed(-0.1f, 0);
				ball->AddAnimation(WEAPON_ANI_BALL);
				//knife->SetType(ITEM_KNIFE);
				ball->SetPosition(soldier1->x, soldier1->y);
				//objects.push_back(knife);
				grid->AddObject(ball);
			}
		}

		if (soldier1->vx < 0) {
			//if (captain->x > zombie1->x)
			//{
			//	zombie1->vx = -zombie1->vx;
			//	//soldier->nx = -1.0f;
			//}
			if (GetTickCount() - soldier1->time_start_shoot > 2000)
			{
				soldier1->time_start_shoot = GetTickCount();
				ball = new Ball(soldier1, 2 * SCREEN_WIDTH / 3);
				ball->SetSpeed(-0.1f, 0);
				ball->AddAnimation(WEAPON_ANI_BALL);
				//knife->SetType(ITEM_KNIFE);
				ball->SetPosition(soldier1->x, soldier1->y);
				//objects.push_back(knife);
				grid->AddObject(ball);
			}
		}
		else
		{
			if (GetTickCount() - zombie1->time_start_shoot > 2000)
			{
				soldier1->time_start_shoot = GetTickCount();
				ball = new Ball(soldier1, 2 * SCREEN_WIDTH / 3);
				ball->SetSpeed(0.1f, 0);
				ball->AddAnimation(WEAPON_ANI_BALL);
				//knife->SetType(ITEM_KNIFE);
				ball->SetPosition(soldier1->x, soldier1->y);
				ball->appearTime = GetTickCount();
				//objects.push_back(knife);
				grid->AddObject(ball);
			}
		}
	}

	if (check) {
		if (captain->nx > 0) {
			if (trum->vx > 0)
			{
				if (captain->x < trum->x)
				{
					trum->vx = -trum->vx;
					//soldier->nx = -1.0f;
				}
				if (GetTickCount() - trum->time_start_shoot > 2000)
				{
					trum->time_start_shoot = GetTickCount();
					bossbullet = new BossBullet(trum, 2 * SCREEN_WIDTH / 3);
					bossbullet->SetSpeed(0.1f, 0);
					bossbullet->AddAnimation(WEAPON_ANI_BULLET);
					//knife->SetType(ITEM_KNIFE);
					bossbullet->SetPosition(trum->x, trum->y);
					//objects.push_back(knife);
					grid->AddObject(bossbullet);
				}
			}
			else
			{
				if (GetTickCount() - trum->time_start_shoot > 2000)
				{
					trum->time_start_shoot = GetTickCount();
					bossbullet = new BossBullet(trum, 2 * SCREEN_WIDTH / 3);
					bossbullet->SetSpeed(-0.1f, 0);
					bossbullet->AddAnimation(WEAPON_ANI_BULLET);
					//knife->SetType(ITEM_KNIFE);
					bossbullet->SetPosition(trum->x, trum->y);
					//objects.push_back(knife);
					grid->AddObject(bossbullet);
				}
			}
		}
		else if (captain->nx < 0)
		{
			if (trum->vx < 0) {
				if (captain->x > trum->x)
				{
					trum->vx = -trum->vx;
					//soldier->nx = -1.0f;
				}
				if (GetTickCount() - trum->time_start_shoot > 2000)
				{
					trum->time_start_shoot = GetTickCount();
					bossbullet = new BossBullet(trum, 2 * SCREEN_WIDTH / 3);
					bossbullet->SetSpeed(-0.1f, 0);
					bossbullet->AddAnimation(WEAPON_ANI_BULLET);
					//knife->SetType(ITEM_KNIFE);
					bossbullet->SetPosition(trum->x, trum->y);
					//objects.push_back(knife);
					grid->AddObject(bossbullet);
				}
			}
			else
			{
				if (GetTickCount() - trum->time_start_shoot > 2000)
				{
					trum->time_start_shoot = GetTickCount();
					bossbullet = new BossBullet(trum, 2 * SCREEN_WIDTH / 3);
					bossbullet->SetSpeed(0.1f, 0);
					bossbullet->AddAnimation(WEAPON_ANI_BULLET);
					//knife->SetType(ITEM_KNIFE);
					bossbullet->SetPosition(trum->x, trum->y);
					bossbullet->appearTime = GetTickCount();
					//objects.push_back(knife);
					grid->AddObject(bossbullet);
				}
			}
		}
	}
	#pragma region Remove Object
		vector<LPGAMEOBJECT> listRemoveObjects;
		for (int i = 0; i < coObjects.size(); i++)
		{
			if (dynamic_cast<Zombie *>(coObjects.at(i)))
			{
				Zombie *zombie = dynamic_cast<Zombie *>(coObjects.at(i));

				if (zombie->GetState() == ZOMBIE_STATE_DIE)
				{
					listRemoveObjects.push_back(zombie);
					item = new Item();
					item->SetPosition(zombie->x, zombie->y);
					item->SetSpeed(0, -0.1);
					coObjects.push_back(item);
					grid->AddObject(item);
					srand(time(NULL));
					int random_portion = rand() % 100;

					// Heart
					if (random_portion < 30)
					{
						item->AddAnimation(ITEM_HEART);
						item->SetType(ITEM_HEART);
					}
					// Item 1
					else if (random_portion >= 30 && random_portion < 50)
					{
						item->AddAnimation(ITEM_1);
						item->SetType(ITEM_1);
					}
					// Item 2
					else if (random_portion >= 50 && random_portion < 80)
					{
						item->AddAnimation(ITEM_2);
						item->SetType(ITEM_2);
					}
					// Item 3
					else
					{
						item->AddAnimation(ITEM_3);
						item->SetType(ITEM_3);
					}
				}
			}
			else if (dynamic_cast<Soldier *>(coObjects.at(i)))
			{
				Soldier *soldier = dynamic_cast<Soldier *>(coObjects.at(i));

				if (soldier->GetState() == SOLDIER_STATE_DIE)
				{
					listRemoveObjects.push_back(soldier);
					item = new Item();
					item->SetPosition(soldier->x, soldier->y);
					item->SetSpeed(0, -0.1);
					coObjects.push_back(item);
					grid->AddObject(item);
					srand(time(NULL));
					int random_portion = rand() % 100;

					// Heart
					if (random_portion < 30)
					{
						item->AddAnimation(ITEM_HEART);
						item->SetType(ITEM_HEART);
					}
					// Item 1
					else if (random_portion >= 30 && random_portion < 50)
					{
						item->AddAnimation(ITEM_1);
						item->SetType(ITEM_1);
					}
					// Item 2
					else if (random_portion >= 50 && random_portion < 80)
					{
						item->AddAnimation(ITEM_2);
						item->SetType(ITEM_2);
					}
					// Item 3
					else
					{
						item->AddAnimation(ITEM_3);
						item->SetType(ITEM_3);
					}
				}
			}

			else if (dynamic_cast<Lantern *>(coObjects.at(i)))
			{
				Lantern *lantern = dynamic_cast<Lantern *>(coObjects.at(i));
				if (lantern->isHitted)
				{
					listRemoveObjects.push_back(lantern);
					float lantern_x, lantern_y, lantern_right, lantern_bottom;
					lantern->GetBoundingBox(lantern_x, lantern_y, lantern_right, lantern_bottom);

					item = new Item();
					item->SetPosition(lantern_x, lantern_y);
					item->SetSpeed(0, -0.1);
					coObjects.push_back(item);
					grid->AddObject(item);
					// Whip item
					if (captain->whip->level < 2)
					{
						item->AddAnimation(ITEM_2);
						item->SetType(ITEM_2);
					}
					else
					{
						srand(time(NULL));
						int random_portion = rand() % 100;

						// Heart
						if (random_portion < 30)
						{
							item->AddAnimation(ITEM_HEART);
							item->SetType(ITEM_HEART);
						}
						// Item 1
						else if (random_portion >= 30 && random_portion < 50)
						{
							item->AddAnimation(ITEM_1);
							item->SetType(ITEM_1);
						}
						// Item 2
						else if (random_portion >= 50 && random_portion < 80)
						{
							item->AddAnimation(ITEM_2);
							item->SetType(ITEM_2);
						}
						// Item 3
						else
						{
							item->AddAnimation(ITEM_3);
							item->SetType(ITEM_3);
						}
					}

				}
			}
			else if (dynamic_cast<Item *>(coObjects.at(i)))
			{
				Item *item = dynamic_cast<Item *>(coObjects.at(i));

				if (item->GetEaten())
				{
					//objects.erase(objects.begin() + i);
					//delete item;
					listRemoveObjects.push_back(item);
				}
			}
			else if (dynamic_cast<Effect *>(coObjects.at(i)))
			{
				Effect *effect = dynamic_cast<Effect *>(coObjects.at(i));

				if (effect->GetExposed())
				{
					//objects.erase(objects.begin() + i);
					//delete effect;
					listRemoveObjects.push_back(effect);
				}
			}
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
			if (y > SCREEN_HEIGHT / 2&&y < MAX_HEIGHT_LV1 - SCREEN_HEIGHT / 2)
			{
				game->mCamera->setX(game->mCamera->getX());
				game->mCamera->setY(y-SCREEN_HEIGHT/2);
			}
			/*else if(y<MAX_HEIGHT-SCREEN_HEIGHT/2)
			{
				game->mCamera->setX(game->mCamera->getX());
				//game->mCamera->setY(SCREEN_HEIGHT-MAX_HEIGHT);
			
			}*/
			else
			{
				game->mCamera->setX(game->mCamera->getX());
				game->mCamera->setY(game->mCamera->getY());
			}
			if (x > SCREEN_WIDTH / 2 && x < MAX_WIDTH_LV1 - SCREEN_WIDTH / 2)
			{

				game->mCamera->setX(x - SCREEN_WIDTH / 2);
				game->mCamera->setY(game->mCamera->getY());
			}
			/*else if (x > MAX_WIDTH_LV1 - SCREEN_WIDTH / 2)
			{
				game->mCamera->setX(MAX_WIDTH_LV1 - SCREEN_WIDTH);
				game->mCamera->setY(game->mCamera->getY());
			}*/
			else
			{
				game->mCamera->setX(game->mCamera->getX());
				game->mCamera->setY(game->mCamera->getY());
			}
		}
		/*else if (lv2 == true)
		{
			if (x > SCREEN_WIDTH - SCREEN_WIDTH / 2 && x < MAX_WIDTH_LV2 - SCREEN_WIDTH / 2)
			{
				game->mCamera->setX(x - SCREEN_WIDTH / 2);
				game->mCamera->setY(0);
			}
		}*/
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
		captain->GetPosition(x, y);

		LPDIRECT3DTEXTURE9 tileset = textures->Get(ID_TEX_TILESET);
		LPDIRECT3DTEXTURE9 tileset1 = textures->Get(ID_TEX_TILESET_2);
		LPDIRECT3DTEXTURE9 tileset2 = textures->Get(ID_TEX_TILESET_3);
		if (lv2 == true&&check==false) {
			if (beginBullet->time_end_start - beginBullet->time_start_shoot > 5000) {
				beginBullet->time_start_shoot = GetTickCount();
				grid->RemoveObject(beginBullet);
				delete beginBullet;
				trum = new BossOne();
				trum->AddAnimation(667);
				trum->AddAnimation(666);
				trum->AddAnimation(670);
				trum->AddAnimation(671);
				trum->AddAnimation(672);
				trum->AddAnimation(673);
				trum->SetPosition(300, 100);
				trum->SetState(BOSS_STATE_WALKING);
				//objects.push_back(zombie1);
				grid->AddObject(trum);
				check = true;
			
			}
			}
		if (lv1 == true)
		{
			
			map = new	Map (/*48, 10,*/ tileset, 16, 16); 
			map->LoadMatrixMap("Resources\\Mapstate.txt");
			//map->Draw(game->x_cam, game->y_cam);
		}
		if (lv2 == true)
		{
			map = new	Map(/*48, 10,*/ tileset1, 16, 16);
			map->LoadMatrixMap("Resources\\Mapstate1.txt");
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

		captain->Render();
		
		//ui->Render(game->x_cam,game->y_cam,captain);
		/*RECT newRect;
		SetRect(&newRect, 0, 0, 30, 30);
		D3DXVECTOR3 p(0, 0, 0);
		spriteHandler->Draw(textures->Get(ID_TEX_AXE), &newRect, NULL, &p, D3DCOLOR_ARGB(255, 255, 255, 255));*/
		
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