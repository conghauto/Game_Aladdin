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
#include "Apple.h"
#include "Aladdin.h"
#include "Ground.h"
#include "define.h"
#include "Item.h"
#include "Pillar.h"
#include "Skeleton.h"
#include "Rock.h"
#include "Map.h"
#include "CheckPoint.h"
#include "Grid.h"
#include "Bullet.h"
#include "Ball.h"
#include "BossOne.h"
#include "BossBullet.h"
#include "Spike.h"
#include "Dumbbell.h"
#include "Rope.h"
#include "Bat.h"
BossBullet *bossbullet;
BossOne *trum;
CGame *game;
Aladdin * aladdin;
Item *item;
Effect *whipEffect;
Soldier *soldier;
Soldier *soldier1;
Map *map;
Apple *knife;
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
		if (aladdin->isJump == false && aladdin->isSit == false && aladdin->isAttack == false && aladdin->isOnRope == true)
			aladdin->SetAction(SIMON_ACTION_JUMP_ONROPE);
		else
		if (aladdin->isJump == false && aladdin->isSit == false && aladdin->isAttack == false && aladdin->isOnRope == false)
			aladdin->SetAction(SIMON_ACTION_JUMP);
	}

	if (KeyCode == DIK_C)
	{
		if (aladdin->isAttack == false && !aladdin->isOnRope)
			aladdin->SetAction(SIMON_ACTION_ATTACK);
	}
	else
		if (KeyCode == DIK_X)
		{

			aladdin->SetCurrentWeapon(809);
			if (aladdin->isAttack == false && !aladdin->isOnRope) {
				int nx = aladdin->nx;
				aladdin->SetAction(SIMON_ACTION_THROW);
				switch (aladdin->currentWeapon)
				{
				case ITEM_KNIFE:
					knife = new Apple(aladdin, 2 * SCREEN_WIDTH / 3);
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
					/*grid->AddObject(knife);*/
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
		if (aladdin->isOnRope)
		{
			aladdin->SetState(SIMON_STATE_ONROPE);
		}
		else
			aladdin->SetState(SIMON_STATE_IDLE);

	}

	// Ngoi
	if (KeyCode == DIK_DOWN)
	{
		if (aladdin->isOnRope) {
			aladdin->SetState(SIMON_STATE_ONROPE);
		}
		else
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
		if (aladdin->isOnRope)
		{
			aladdin->SetState(SIMON_STATE_ONROPE_CLIMB);
		}
	}

	// Ngoi
	if (game->IsKeyDown(DIK_DOWN))
	{
		if (aladdin->isOnRope ) {
			aladdin->SetState(SIMON_STATE_ONROPE_CLIMB_DOWN);
		}
		else
		if (!aladdin->isAttack && !aladdin->isJump)
			aladdin->SetState(SIMON_STATE_SIT);
	}

	// Di bo
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (!aladdin->isSit && !aladdin->isAttack && !aladdin->isOnRope)
			aladdin->SetState(SIMON_STATE_WALK);
		if (!aladdin->isJump && !aladdin->isAttack)
			aladdin->nx = 1.0f;
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (!aladdin->isSit && !aladdin->isAttack && !aladdin->isOnRope)
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
	textures->Add(ID_TEX_ALLADIN, L"Resources\\aladdin.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_BAT, L"Resources\\bat.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_SKELETON, L"Resources\\skeleton.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_ROCK, L"Resources\\rock.png", D3DCOLOR_XRGB(163, 73, 164));
	textures->Add(ID_TEX_TILESET, L"Resources\\tileset.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_GUARDIAN, L"Resources\\enemy.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_PILLAR1, L"Resources\\pillar_1.png", D3DCOLOR_XRGB(163, 73, 164));
	textures->Add(ID_TEX_PILLAR2, L"Resources\\pillar_2.png", D3DCOLOR_XRGB(163, 73, 164));
	textures->Add(ID_TEX_PILLAR3, L"Resources\\pillar_3.png", D3DCOLOR_XRGB(163, 73, 164));
	textures->Add(ID_TEX_PILLAR4, L"Resources\\pillar_4.png", D3DCOLOR_XRGB(163, 73, 164));
	textures->Add(ID_TEX_GATE, L"Resources\\gate_exit.png", D3DCOLOR_XRGB(163, 73, 164));
	textures->Add(ID_TEX_DUMBBELL, L"Resources\\dumbbell.png", D3DCOLOR_XRGB(163, 73, 164));
	textures->Add(ID_TEX_SPIKE, L"Resources\\spike.png", D3DCOLOR_XRGB(163, 73, 164));
	textures->Add(ID_TEX_TILESET_2, L"Resources\\tileset1.png", D3DCOLOR_XRGB(255, 0, 255));


	LPDIRECT3DTEXTURE9 texApple = textures->Get(ID_TEX_KNIFE);
	LPDIRECT3DTEXTURE9 texAladdin = textures->Get(ID_TEX_ALLADIN);
	LPDIRECT3DTEXTURE9 texAladdinSit = textures->Get(ID_TEX_SIT);
	LPDIRECT3DTEXTURE9 texBat = textures->Get(ID_TEX_BAT);
	LPDIRECT3DTEXTURE9 texSkeleton = textures->Get(ID_TEX_SKELETON);

	#pragma region Addsprite
		sprites->Add(10001, 1121, 0, 1162, 66, texAladdin);		// đứng im phải

		sprites->Add(14002, 1186, 1215, 1229, 1275, texAladdin);		// đi phải
		sprites->Add(14003, 1239, 1216, 1280, 1275, texAladdin);
		sprites->Add(14004, 1290, 1215, 1333, 1275, texAladdin);
		sprites->Add(14005, 1340, 1215, 1394, 1275, texAladdin);
		sprites->Add(14006, 1397, 1215, 1449, 1275, texAladdin);
		sprites->Add(14007, 1451, 1215, 1496, 1275, texAladdin);
		sprites->Add(14008, 1505, 1215, 1547, 1275, texAladdin);
		sprites->Add(14009, 1560, 1215, 1593, 1275, texAladdin);
		sprites->Add(14010, 1608, 1215, 1657, 1275, texAladdin);
		sprites->Add(14011, 1730, 1215, 1788, 1275, texAladdin);
		sprites->Add(14012, 1797, 1215, 1844, 1275, texAladdin);

		sprites->Add(14040, 1132, 1360, 1160, 1447, texAladdin);		// Leo dây phải
		sprites->Add(14041, 1174, 1360, 1200, 1447, texAladdin);
		sprites->Add(14042, 1214, 1360, 1242, 1447, texAladdin);
		sprites->Add(14043, 1252, 1360, 1295, 1447, texAladdin);
		sprites->Add(14044, 1305, 1360, 1338, 1447, texAladdin);
		sprites->Add(14045, 1350, 1360, 1382, 1447, texAladdin);
		sprites->Add(14046, 1392, 1360, 1424, 1447, texAladdin);
		sprites->Add(14047, 1432, 1360, 1468, 1447, texAladdin);
		sprites->Add(14048, 1476, 1360, 1518, 1447, texAladdin);
		sprites->Add(14049, 1525, 1360, 1555, 1447, texAladdin);

		sprites->Add(10004, 1125, 330, 1173, 387, texAladdin);		// đánh phải
		sprites->Add(10005, 1174, 330, 1227, 387, texAladdin);
		sprites->Add(10006, 1232, 330, 1280, 387, texAladdin);
		sprites->Add(10007, 1289, 330, 1373, 387, texAladdin);
		sprites->Add(10008, 1377, 330, 1431, 387, texAladdin);

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

		sprites->Add(13001, 1124, 232, 1170, 290, texAladdin); //ném táo phải
		sprites->Add(13002, 1178, 232, 1217, 290, texAladdin);
		sprites->Add(13003, 1227, 232, 1270, 290, texAladdin);
		sprites->Add(13004, 1282, 232, 1330, 290, texAladdin);
		sprites->Add(13005, 1339, 232, 1382, 290, texAladdin);

		sprites->Add(13021, 1127, 562, 1169, 615, texAladdin); // ngồi ném táo phải
		sprites->Add(13022, 1176, 562, 1218, 615, texAladdin);
		sprites->Add(13023, 1225, 562, 1278, 615, texAladdin);
		sprites->Add(13024, 1284, 562, 1365, 615, texAladdin);
		sprites->Add(13025, 1378, 562, 1436, 615, texAladdin);

		sprites->Add(14111, 1128, 2163, 1172, 2212, texAladdin); //bị thương bên phải
		sprites->Add(14112, 1178, 2163, 1234, 2212, texAladdin);
		sprites->Add(14113, 1241, 2163, 1300, 2212, texAladdin);
		sprites->Add(14114, 1310, 2163, 1389, 2212, texAladdin);
		sprites->Add(14115, 1402, 2163, 1464, 2212, texAladdin);
		sprites->Add(14116, 1474, 2163, 1530, 2212, texAladdin);

		LPDIRECT3DTEXTURE9 texAladdin2 = textures->Get(ID_TEX_ALLADIN);
		sprites->Add(10011, 1079, 0, 1120, 66, texAladdin2);		// đứng im trái

		sprites->Add(14022, 1008, 1215, 1057, 1275, texAladdin2);		// đi trái
		sprites->Add(14023, 959, 1215, 1003, 1275, texAladdin2);
		sprites->Add(14024, 907, 1215, 952, 1275, texAladdin2);
		sprites->Add(14025, 848, 1215, 904, 1275, texAladdin2);
		sprites->Add(14026, 793, 1215, 846, 1275, texAladdin2);
		sprites->Add(14027, 742, 1215, 789, 1275, texAladdin2);
		sprites->Add(14028, 692, 1215, 736, 1275, texAladdin2);
		sprites->Add(14029, 643, 1215, 685, 1275, texAladdin2);
		sprites->Add(14030, 583, 1215, 634, 1275, texAladdin2);
		sprites->Add(14031, 518, 1215, 575, 1275, texAladdin2);
		sprites->Add(14032, 455, 1215, 512, 1275, texAladdin2);
		sprites->Add(14033, 455, 1215, 512, 1275, texAladdin2);

		sprites->Add(14050, 1080, 1360, 1111, 1447, texAladdin2);		// Leo dây trái
		sprites->Add(14051, 1040, 1360, 1070, 1447, texAladdin2);
		sprites->Add(14052, 998, 1360, 1028, 1447, texAladdin2);
		sprites->Add(14053, 946, 1360, 988, 1447, texAladdin2);
		sprites->Add(14054, 904, 1360, 938, 1447, texAladdin2);
		sprites->Add(14055, 860, 1360, 890, 1447, texAladdin2);
		sprites->Add(14056, 820, 1360, 850, 1447, texAladdin2);
		sprites->Add(14057, 775, 1360, 810, 1447, texAladdin2);
		sprites->Add(14058, 722, 1360, 765, 1447, texAladdin2);
		sprites->Add(14059, 687, 1360, 715, 1447, texAladdin2);

		sprites->Add(10014, 1070, 330, 1115, 387, texAladdin2);		// đánh trái				
		sprites->Add(10015, 1013, 330, 1068, 387, texAladdin2);
		sprites->Add(10016, 956, 330, 1009, 387, texAladdin2);
		sprites->Add(10017, 867, 330, 954, 387, texAladdin2);
		sprites->Add(10009, 808, 330, 866, 387, texAladdin2);

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

		sprites->Add(13011, 1070, 232, 1115, 290, texAladdin); // ném táo trái
		sprites->Add(13012, 1023, 232, 1067, 290, texAladdin);
		sprites->Add(13013, 973, 232, 1014, 290, texAladdin);
		sprites->Add(13014, 912, 232, 959, 290, texAladdin);
		sprites->Add(13015, 861, 232, 902, 290, texAladdin);

		sprites->Add(13031, 1073, 562, 1115, 615, texAladdin); //ngồi ném táo trái
		sprites->Add(13032, 1022, 562, 1065, 615, texAladdin);
		sprites->Add(13033, 962, 562, 1015, 615, texAladdin);
		sprites->Add(13034, 877, 562, 956, 615, texAladdin);
		sprites->Add(13035, 808, 562, 866, 615, texAladdin);

		sprites->Add(14121, 1070, 2163, 1113, 2212, texAladdin); //bị thương bên trái
		sprites->Add(14122, 1008, 2163, 1062, 2212, texAladdin);
		sprites->Add(14123, 940, 2163, 998, 2212, texAladdin);
		sprites->Add(14124, 852, 2163, 928, 2212, texAladdin);
		sprites->Add(14125, 778, 2163, 838, 2212, texAladdin);
		sprites->Add(14126, 712, 2163, 768, 2212, texAladdin);

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
		sprites->Add(1200, 1482, 19, 1511, 71, texApple);	//khien
	#pragma endregion

	#pragma region ENEMY

	//Guardian Enemy
		LPDIRECT3DTEXTURE9 guardian = textures->Get(ID_TEX_GUARDIAN);
		sprites->Add(16006, 929, 572, 986, 626, guardian); // guardian tấn công phải
		sprites->Add(16005, 860, 572, 928, 626, guardian);
		sprites->Add(16004, 782, 572, 854, 626, guardian);
		sprites->Add(16003, 702, 572, 777, 626, guardian);
		sprites->Add(16002, 608, 572, 694, 626, guardian);
		sprites->Add(16001, 502, 572, 600, 626, guardian);
		sprites->Add(16007, 943, 643, 985, 702, guardian);
		sprites->Add(16008, 888, 643, 930, 702, guardian);
		sprites->Add(16009, 824, 643, 878, 702, guardian);
		sprites->Add(16010, 701, 643, 790, 702, guardian);


		sprites->Add(16011, 395, 572, 492, 626, guardian); // guardian tấn công trái
		sprites->Add(16012, 297, 572, 388, 626, guardian);
		sprites->Add(16013, 216, 572, 293, 626, guardian);
		sprites->Add(16014, 140, 572, 212, 626, guardian);
		sprites->Add(16015, 65, 572, 133, 626, guardian);
		sprites->Add(16016, 6, 572, 64, 626, guardian);
		sprites->Add(16017, 6, 572, 64, 626, guardian);
		sprites->Add(16018, 6, 572, 64, 626, guardian);
		sprites->Add(16019, 6, 572, 64, 626, guardian);
		sprites->Add(16020, 6, 572, 64, 626, guardian);

		sprites->Add(16021, 657, 237, 727, 270, guardian); //guardian chet phai
		sprites->Add(16022, 744, 237, 811, 270, guardian);
		sprites->Add(16023, 831, 237, 900, 270, guardian);
		sprites->Add(16024, 911, 237, 975, 980, guardian);

		sprites->Add(16031, 266, 237, 337, 270, guardian); //guardian chet trai
		sprites->Add(16032, 182, 237, 248, 270, guardian);
		sprites->Add(16033, 93, 237, 161, 270, guardian);
		sprites->Add(16034, 18, 237, 83, 270, guardian);

		sprites->Add(16041, 666, 448, 714, 504, guardian); //guardian đứng phải
		sprites->Add(16042, 721, 448, 770, 504, guardian);
		sprites->Add(16043, 775, 448, 835, 504, guardian);
		sprites->Add(16044, 838, 448, 894, 504, guardian);
		sprites->Add(16045, 896, 448, 936, 504, guardian);
		sprites->Add(16046, 942, 448, 986, 504, guardian);


		sprites->Add(16051, 277, 448, 325, 504, guardian); //guardian đứng trái
		sprites->Add(16052, 220, 448, 272, 504, guardian);
		sprites->Add(16053, 158, 448, 216, 504, guardian);
		sprites->Add(16054, 97, 448, 154, 504, guardian);
		sprites->Add(16055, 56, 448, 96, 504, guardian);
		sprites->Add(16056, 4, 448, 52, 504, guardian);
		//Soldier
		LPDIRECT3DTEXTURE9 soldierTex = textures->Get(ID_TEX_GUARDIAN); //solider danh phai
		sprites->Add(17001, 570, 92, 609, 156, soldierTex);
		sprites->Add(17002, 625, 92, 701, 156, soldierTex);
		sprites->Add(17003, 704, 92, 788, 156, soldierTex);
		sprites->Add(17004, 792, 92, 885, 156, soldierTex);
		sprites->Add(17005, 889, 92, 937, 156, soldierTex);
		sprites->Add(17006, 943, 92, 986, 156, soldierTex);

		sprites->Add(17011, 381, 92, 426, 156, soldierTex); //soldier danh trai
		sprites->Add(17012, 292, 92, 372, 156, soldierTex);
		sprites->Add(17013, 203, 92, 290, 156, soldierTex);
		sprites->Add(17014, 111, 92, 203, 156, soldierTex);
		sprites->Add(17015, 53, 92, 101, 156, soldierTex);
		sprites->Add(17016, 9, 92, 50, 156, soldierTex);

		sprites->Add(17021, 941, 166, 983, 233, soldierTex); // sodier đứng phải

		sprites->Add(17031, 6, 166, 52, 233, soldierTex); //sodier đứng trái
		//Bat enemy
		sprites->Add(17100, 4, 8, 18, 34, texBat); //bat đứng yên

		sprites->Add(17101, 8, 50, 34, 78, texBat); //bat tấn công
		sprites->Add(17102, 36, 50, 54, 78, texBat);
		sprites->Add(17103, 55, 50, 85, 78, texBat);
		sprites->Add(17104, 89, 57, 123, 86, texBat);
		sprites->Add(17105, 124, 64, 139, 94, texBat);
		sprites->Add(17106, 143, 50, 163, 78, texBat);
		sprites->Add(17107, 164, 60, 199, 88, texBat);

		sprites->Add(17201, 880, 124, 932, 204, texSkeleton); // skeleton đứng phải
		sprites->Add(17202, 943, 124, 995, 204, texSkeleton);
		sprites->Add(17203, 999, 124, 1062, 204, texSkeleton);
		sprites->Add(17204, 1072, 124, 1136, 204, texSkeleton);
		sprites->Add(17205, 1149, 124, 1222, 204, texSkeleton);
		sprites->Add(17206, 1248, 124, 1325, 204, texSkeleton);
		sprites->Add(17207, 1350, 124, 1440, 204, texSkeleton);
		sprites->Add(17208, 1460, 124, 1522, 204, texSkeleton);

		sprites->Add(17211, 806, 124, 856, 204, texSkeleton); //skeleton đứng trái
		sprites->Add(17212, 744, 124, 794, 204, texSkeleton);
		sprites->Add(17213, 674, 124, 736, 204, texSkeleton);
		sprites->Add(17214, 600, 124, 666, 204, texSkeleton);
		sprites->Add(17215, 514, 124, 586, 204, texSkeleton);
		sprites->Add(17216, 412, 124, 490, 204, texSkeleton);
		sprites->Add(17217, 294, 124, 386, 204, texSkeleton);
		sprites->Add(17218, 214, 124, 276, 204, texSkeleton);

		LPDIRECT3DTEXTURE9 texMisc11 = textures->Get(ID_TEX_KNIFE);
		sprites->Add(40023, 0, 0, 32, 18, texMisc11);

		LPDIRECT3DTEXTURE9 texRock1 = textures->Get(ID_TEX_ROCK); // rock
		sprites->Add(40031, 0, 4, 30, 20, texRock1);
		sprites->Add(40032, 36, 4, 66, 20, texRock1);
		sprites->Add(40033, 72, 4, 105, 21, texRock1);
		sprites->Add(40034, 111, 4, 147, 24, texRock1);
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

		LPDIRECT3DTEXTURE9 texSpike = textures->Get(ID_TEX_SPIKE); // spike - cây chông
		sprites->Add(40041, 1, 2, 25, 24, texSpike);
		sprites->Add(40042, 29, 2, 52, 24, texSpike);
		sprites->Add(40043, 57, 2, 81, 30, texSpike);
		sprites->Add(40044, 88, 2, 122, 34, texSpike);
		sprites->Add(40045, 126, 2, 170, 35, texSpike);
		sprites->Add(40046, 173, 2, 222, 38, texSpike);

		LPDIRECT3DTEXTURE9 texDumbbell = textures->Get(ID_TEX_DUMBBELL); // dumbbell - quả tạ
		sprites->Add(40050, 3, 2, 19, 69, texDumbbell);
		sprites->Add(40051, 23, 2, 38, 69, texDumbbell);
		sprites->Add(40052, 43, 2, 58, 69, texDumbbell);
		sprites->Add(40053, 63, 2, 78, 69, texDumbbell);
		sprites->Add(40054, 83, 2, 98, 69, texDumbbell);
		sprites->Add(40055, 103, 2, 120, 69, texDumbbell);
		sprites->Add(40056, 124, 2, 143, 69, texDumbbell);
		sprites->Add(40057, 147, 2, 169, 69, texDumbbell);
		sprites->Add(40058, 173, 2, 201, 69, texDumbbell);
		sprites->Add(40059, 204, 2, 233, 69, texDumbbell);
		sprites->Add(40060, 237, 2, 269, 69, texDumbbell);
		sprites->Add(40061, 272, 2, 305, 69, texDumbbell);
		sprites->Add(40062, 308, 2, 342, 69, texDumbbell);
		sprites->Add(40063, 345, 2, 380, 69, texDumbbell);
		sprites->Add(40064, 383, 2, 416, 69, texDumbbell);

	#pragma endregion

	LPANIMATION ani;

	#pragma region AladdinAnimation
		ani = new CAnimation(100);	//đứng phải
		ani->Add(10001);
		animations->Add(400, ani);

		ani = new CAnimation(100);	//đứng trái
		ani->Add(10011);
		animations->Add(401, ani);

	animations->Add(500, ani);
	animations->Add(501, ani);
		ani = new CAnimation(180);	//Leo phải
		ani->Add(14040);
		ani->Add(14041);
		ani->Add(14042);
		ani->Add(14043);
		ani->Add(14044);
		ani->Add(14045);
		ani->Add(14046);
		ani->Add(14047);
		ani->Add(14048);
		ani->Add(14049);
		animations->Add(502, ani);

		ani = new CAnimation(180);	//Leo trái
		ani->Add(14050);
		ani->Add(14051);
		ani->Add(14052);
		ani->Add(14053);
		ani->Add(14054);
		ani->Add(14055);
		ani->Add(14056);
		ani->Add(14057);
		ani->Add(14058);
		ani->Add(14059);
		animations->Add(503, ani);

	animations->Add(402, ani);

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

		ani = new CAnimation(100);//bị đánh từ bên phải
		ani->Add(14111);
		ani->Add(14112);
		ani->Add(14113);
		ani->Add(14114);
		ani->Add(14115);
		ani->Add(14116);
		animations->Add(424, ani);

		ani = new CAnimation(100);//bị đánh từ bên trái
		ani->Add(14121);
		ani->Add(14122);
		ani->Add(14123);
		ani->Add(14124);
		ani->Add(14125);
		ani->Add(14126);
		animations->Add(425, ani);

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

		ani = new CAnimation(100);	//Guardian tan cong phai
		ani->Add(16001);
		ani->Add(16002);
		ani->Add(16003);
		ani->Add(16004);
		ani->Add(16005);
		ani->Add(16006);
		ani->Add(16007);
		ani->Add(16008);
		ani->Add(16009);
		ani->Add(16010);
		animations->Add(551, ani);

		ani = new CAnimation(100);	//Guardian tan cong trai
		ani->Add(16011);
		ani->Add(16012);
		ani->Add(16013);
		ani->Add(16014);
		ani->Add(16015);
		ani->Add(16016);
		animations->Add(552, ani);

		ani = new CAnimation(100);	//Guardian chet phai
		ani->Add(16021);
		ani->Add(16022);
		ani->Add(16023);
		ani->Add(16024);
		animations->Add(553, ani);

		ani = new CAnimation(100);	//Guardian chet trai
		ani->Add(16031);
		ani->Add(16032);
		ani->Add(16033);
		ani->Add(16034);
		animations->Add(554, ani);

		ani = new CAnimation(100);	//Guardian đứng phải
		ani->Add(16041);
		ani->Add(16042);
		ani->Add(16043);
		ani->Add(16044);
		ani->Add(16045);
		ani->Add(16046);
		animations->Add(555, ani);

		ani = new CAnimation(100);	//Guardian đứng trái
		ani->Add(16051);
		ani->Add(16052);
		ani->Add(16053);
		ani->Add(16054);
		ani->Add(16055);
		ani->Add(16056);
		animations->Add(556, ani);

		ani = new CAnimation(100);	//soldier danh phai
		ani->Add(17001);
		ani->Add(17002);
		ani->Add(17003);
		ani->Add(17004);
		ani->Add(17005);
		ani->Add(17006);
		animations->Add(561, ani);

		ani = new CAnimation(100);	//soldier danh trai
		ani->Add(17011);
		ani->Add(17012);
		ani->Add(17013);
		ani->Add(17014);
		ani->Add(17015);
		ani->Add(17016);
		animations->Add(562, ani);

		ani = new CAnimation(100);	//soldier dung phai
		ani->Add(17021);
		animations->Add(563, ani);

		ani = new CAnimation(100);	//soldier dung trai
		ani->Add(17031);
		animations->Add(564, ani);

		ani = new CAnimation(100);	//bat đứng đợi
		ani->Add(17100);
		animations->Add(605, ani);

		ani = new CAnimation(100);	//bat tấn công
		ani->Add(17101);
		ani->Add(17102);
		ani->Add(17103);
		ani->Add(17104);
		ani->Add(17105);
		ani->Add(17106);
		ani->Add(17107);
		animations->Add(606, ani);

		ani = new CAnimation(100);	//skeleton dung phai
		ani->Add(17201);
		ani->Add(17202);
		ani->Add(17203);
		ani->Add(17204);
		ani->Add(17205);
		ani->Add(17206);
		ani->Add(17207);
		ani->Add(17208);
		animations->Add(607, ani);

		ani = new CAnimation(100);	//skeleton dung trai
		ani->Add(17211);
		ani->Add(17212);
		ani->Add(17213);
		ani->Add(17214);
		ani->Add(17215);
		ani->Add(17216);
		ani->Add(17217);
		ani->Add(17218);
		animations->Add(608, ani);

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
		ani->Add(40035);
		ani->Add(40034);
		ani->Add(40033);
		ani->Add(40032);
		ani->Add(40031);
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
	
		ani = new CAnimation(0); // gate
		ani->Add(410040);
		animations->Add(5555, ani); 

		ani = new CAnimation(0); // spike 1
		ani->Add(40041);
		ani->Add(40042);
		ani->Add(40043);
		ani->Add(40044);
		ani->Add(40045);
		ani->Add(40046);
		animations->Add(992, ani);

		ani = new CAnimation(0); // spike 2
		ani->Add(40046);
		ani->Add(40045);
		ani->Add(40044);
		ani->Add(40043);
		ani->Add(40042);
		ani->Add(40041);
		animations->Add(993, ani);

		ani = new CAnimation(0); // dumbbell 1
		ani->Add(40050);
		ani->Add(40051);
		ani->Add(40052);
		ani->Add(40053);
		ani->Add(40054);
		ani->Add(40055);
		ani->Add(40056);
		ani->Add(40057);
		ani->Add(40058);
		ani->Add(40059);
		ani->Add(40060);
		ani->Add(40061);
		ani->Add(40062);
		ani->Add(40063);
		ani->Add(40064);
		ani->Add(40063);
		ani->Add(40062);
		ani->Add(40061);
		ani->Add(40060);
		ani->Add(40059);
		ani->Add(40058);
		ani->Add(40057);
		ani->Add(40056);
		ani->Add(40055);
		ani->Add(40054);
		ani->Add(40053);
		ani->Add(40052);
		ani->Add(40051);
		animations->Add(994, ani);

		ani = new CAnimation(0); // dumbbell 1
		ani->Add(40057);
		ani->Add(40058);
		ani->Add(40059);
		ani->Add(40060);
		ani->Add(40061);
		ani->Add(40062);
		ani->Add(40063);
		ani->Add(40064);
		ani->Add(40063);
		ani->Add(40062);
		ani->Add(40061);
		ani->Add(40060);
		ani->Add(40059);
		ani->Add(40058);
		ani->Add(40057);
		ani->Add(40056);
		ani->Add(40055);
		ani->Add(40054);
		ani->Add(40053);
		ani->Add(40052);
		ani->Add(40051);
		ani->Add(40050);
		ani->Add(40051);
		ani->Add(40052);
		ani->Add(40053);
		ani->Add(40054);
		ani->Add(40055);
		ani->Add(40056);
		animations->Add(995, ani);

		ani = new CAnimation(0); // dumbbell 1
		ani->Add(40064);
		ani->Add(40063);
		ani->Add(40062);
		ani->Add(40061);
		ani->Add(40060);
		ani->Add(40059);
		ani->Add(40058);
		ani->Add(40057);
		ani->Add(40056);
		ani->Add(40055);
		ani->Add(40054);
		ani->Add(40053);
		ani->Add(40052);
		ani->Add(40051);
		ani->Add(40050);
		ani->Add(40051);
		ani->Add(40052);
		ani->Add(40053);
		ani->Add(40054);
		ani->Add(40055);
		ani->Add(40056);
		ani->Add(40057);
		ani->Add(40058);
		ani->Add(40059);
		ani->Add(40060);
		ani->Add(40061);
		ani->Add(40062);
		ani->Add(40063);
		animations->Add(996, ani);

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
		aladdin->AddAnimation(424);	//bị đánh từ bên phải
		aladdin->AddAnimation(425);	//bị đánh từ bên trái
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
		aladdin->AddAnimation(502);	//Leo dây phải
		aladdin->AddAnimation(503); // Leo dây trái
	aladdin->AddAnimation(1614);//dung day phai
	aladdin->AddAnimation(1615);//dung day trai

		//objects.push_back(aladdin);

		// khởi tạo grid
		grid->InitList(MAX_WIDTH_LV1, MAX_HEIGHT_LV1);
		aladdin->SetPosition(850, 50);
		//grid->AddObject(aladdin);

	#pragma endregion

	#pragma region Ground
		listObjectsInMap = lsObjs->getObjectsInFile("Resources\\objects.txt");
		grid->InitObjectsAtCell("Resources\\o_grid.txt");

		int count = 1;
		int countRocks = 1;
		int countSpikes = 1;
		int countDumbbells = 1;

		if (grid->listCells.size() > 0) {
			for (int j = 0; j < grid->listCells.size() > 0; j++) {
				if (grid->listCells[j]->listIdObject.size() > 0) {
					for (int k = 0; k < grid->listCells[j]->listIdObject.size(); k++) {
						if (listObjectsInMap.size() > 0) {
							for (int i = 0; i < listObjectsInMap.size(); i++)
							{
								int p = grid->listCells[j]->listIdObject[k];
								int l = listObjectsInMap[i]->id;
								if ((p) == l) {
									// Gắn đường đi - Ground
									Ground *ground = new Ground();
									if (listObjectsInMap[i]->name == "Ground")
									{
										ground->SetPosition(listObjectsInMap[i]->x, listObjectsInMap[i]->y);
										ground->cellNumber = grid->listCells[j]->getId();
										/*grid->AddObject(ground);*/
										grid->listCells[j]->AddObject(ground);
									}

									Rope *rope = new Rope();
									if (listObjectsInMap[i]->name == "Rope")
									{
										rope->SetPositionAndHeight(listObjectsInMap[i]->x, listObjectsInMap[i]->y, listObjectsInMap[i]->height);
										/*grid->AddObject(rope);*/
										rope->cellNumber = grid->listCells[j]->getId();
										grid->listCells[j]->AddObject(rope);
									}

									//Pillar*pillar = new Pillar();
									//if (listObjectsInMap[i]->name == "Pillar")
									//{
									//	if (count == 1) {
									//		pillar->AddAnimation(1000);
									//	}
									//	else if (count == 2) {
									//		pillar->AddAnimation(1001);
									//	}
									//	else if (count == 3) {
									//		pillar->AddAnimation(1002);
									//	}
									//	else if (count == 4) {
									//		pillar->AddAnimation(1003);
									//	}

									//	pillar->SetPosition(listObjectsInMap[i]->x, listObjectsInMap[i]->y);
									//	/*grid->AddObject(pillar);*/
									//	grid->listCells[j]->AddObject(pillar);
									//	count++;
									//}

									//// Gắn viên đá - Rock
									Rock*rock = new Rock();
									if (listObjectsInMap[i]->name == "Rock")
									{
										if ((countRocks == 1) || (countRocks == 2) || (countRocks == 6) || (countRocks == 8) || (countRocks == 10)
											|| (countRocks == 23) || (countRocks == 25) || (countRocks == 27) || (countRocks == 29) || (countRocks == 31))
										{
											rock->AddAnimation(990);
											rock->name = "rock1";
										}
										else if ((countRocks == 12) || (countRocks == 13) || (countRocks == 21)
											|| (countRocks == 15) || (countRocks == 17) || (countRocks == 18) || (countRocks == 19))
										{
											rock->AddAnimation(990);
											rock->name = "rock1";
										}
										else if ((countRocks == 4) || (countRocks == 3) || (countRocks == 5) || (countRocks == 7) || (countRocks == 16) || (countRocks == 22) || (countRocks == 26)
											|| (countRocks == 9) || (countRocks == 11) || (countRocks == 14) || (countRocks == 20) || (countRocks == 24)
											|| (countRocks == 28) || (countRocks == 30) || (countRocks == 32))
										{
											rock->AddAnimation(991);
											rock->name = "rock2";
										}
										else if (countRocks % 2 == 0) {
											rock->AddAnimation(991);
											rock->name = "rock2";
										}
										else
										{
											rock->AddAnimation(990);
											rock->name = "rock1";
										}

										rock->SetPosition(listObjectsInMap[i]->x, listObjectsInMap[i]->y);
										/*grid->AddObject(rock);*/
										rock->cellNumber = grid->listCells[j]->getId();
										grid->listCells[j]->AddObject(rock);
										countRocks++;
									}

									//// Gắn mũi nhọn - Spike
									Spike*spike = new Spike();
									if (listObjectsInMap[i]->name == "Spike")
									{
										if ((countSpikes == 1) || (countSpikes == 2)) {
											spike->AddAnimation(992);
											spike->name = "spike1";
										}
										else if ((countSpikes == 3) || (countSpikes == 4)) {
											spike->AddAnimation(993);
											spike->name = "spike2";
										}
										else {
											spike->AddAnimation(993);
											spike->name = "spike2";
										}

										spike->SetPosition(listObjectsInMap[i]->x, listObjectsInMap[i]->y);
										/*grid->AddObject(spike);*/
										spike->cellNumber = grid->listCells[j]->getId();
										grid->listCells[j]->AddObject(spike);
										countSpikes++;
									}

									//// Gắn quả tạ - Dumbbell
									Dumbbell*dumbbell = new Dumbbell();
									if (listObjectsInMap[i]->name == "Dumbbell")
									{
										if ((countDumbbells == 1) || (countDumbbells == 4) || (countDumbbells == 6)) {
											dumbbell->AddAnimation(994);
											dumbbell->name = "dumbbell1";
										}
										else if ((countDumbbells == 2) || (countDumbbells == 7)) {
											dumbbell->AddAnimation(995);
											dumbbell->name = "dumbbell2";
										}
										else if ((countDumbbells == 3) || (countDumbbells == 5)) {
											dumbbell->AddAnimation(996);
											dumbbell->name = "dumbbell3";
										}

										dumbbell->SetPosition(listObjectsInMap[i]->x, listObjectsInMap[i]->y);
										/*grid->AddObject(dumbbell);*/
										dumbbell->cellNumber = grid->listCells[j]->getId();
										grid->listCells[j]->AddObject(dumbbell);
										countDumbbells++;
									}

									/*CheckPoint*checkPoint = new CheckPoint();
									if (listObjectsInMap[i]->name == "Gate")
									{
										checkPoint->SetPosition(listObjectsInMap[i]->x, listObjectsInMap[i]->y);
										checkPoint->AddAnimation(5555);
										checkPoint->cellNumber = grid->listCells[j]->getId();
										grid->listCells[j]->AddObject(checkPoint);
									}*/
									break;
								}
							}
						}
					}
				}
			}
		}

	#pragma endregion

	grid->AddObject(zombie);

	#pragma endregion

#pragma region Skeleton
	grid->AddObject(soldier);
	#pragma endregion


	#pragma region CheckPoint
	CheckPoint *checkPoint;
	checkPoint = new CheckPoint();
	checkPoint->SetType(CHECKPOINT_LEVELUP);
	checkPoint->SetPosition(2071, 62);
	checkPoint->cellNumber = grid->listCells[6]->getId();
	grid->listCells[6]->AddObject(checkPoint);
	/*grid->AddObject(checkPoint);*/
#pragma endregion
}

void LoadResourceLv2()
{
	listObjectsInMap = lsObjs->getObjectsInFile("Resources\\objects_lv2.txt");
	grid->InitObjectsAtCell("Resources\\object_grid_lv2.txt");
	
	if (grid->listCells.size() > 0) {
		for (int j = 0; j < grid->listCells.size() > 0; j++) {
			if (grid->listCells[j]->listIdObject.size() > 0) {
				for (int k = 0; k < grid->listCells[j]->listIdObject.size(); k++) {
					if (listObjectsInMap.size() > 0) {
						for (int i = 0; i < listObjectsInMap.size(); i++)
						{
							int p = grid->listCells[j]->listIdObject[k];
							int l = listObjectsInMap[i]->id;
							if ((p) == l) {
								// Gắn đường đi - Ground
								Ground *ground = new Ground();
								if (listObjectsInMap[i]->name == "Ground")
								{
									ground->SetPosition(listObjectsInMap[i]->x, listObjectsInMap[i]->y);
									ground->cellNumber = grid->listCells[j]->getId();
									/*grid->AddObject(ground);*/
									grid->listCells[j]->AddObject(ground);
								}
								break;
							}
						}
					}
				}
			}
		}
	}
}
float Aladdin::XforGet = 0;
void Update(DWORD dt)
{

	float x, y;
	aladdin->GetPosition(x, y);
	Aladdin::XforGet = x;
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

		if (lv2 == true) {

			if (countLoadResourceLv2 == false)
			{
				game->mCamera->setX(0);
				grid->InitList(MAX_WIDTH_LV2, MAX_HEIGHT_LV2);
				aladdin->SetPosition(50, 50);
				LoadResourceLv2();
				countLoadResourceLv2 = true;
				aladdin->SetPosition(100, 20);
				aladdin->GetPosition(x, y);
			}
		}

	#pragma endregion

	#pragma region Collision
		vector<LPGAMEOBJECT> coObjects;
		if (aladdin->x < 0)
			currentCell = grid->GetCurrentCells(0, y);
		else
			currentCell = grid->GetCurrentCells(x, y);

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
		for (int i = 0; i < coObjects.size(); i++)
		{
			coObjects[i]->Update(dt + 200, &coObjects);
			coObjects[i]->Update(dt, &coObjects);			grid->UpdateObjectInCell(coObjects[i]);
		}

	#pragma endregion	

	#pragma region Remove Object
		vector<LPGAMEOBJECT> listRemoveObjects;
		for (int i = 0; i < coObjects.size(); i++)
		{
			if (dynamic_cast<Zombie *>(coObjects.at(i)))
			{
				Zombie *zombie = dynamic_cast<Zombie *>(coObjects.at(i));

				if (zombie->GetState() == GUARDIAN_STATE_DIE)
				{
					listRemoveObjects.push_back(zombie);
				}
			}
			else if (dynamic_cast<Skeleton *>(coObjects.at(i))) {
				Skeleton *skeleton = dynamic_cast<Skeleton *>(coObjects.at(i));

				if (skeleton->GetState() == SKELETON_STATE_DIE)
				{
					listRemoveObjects.push_back(skeleton);
				}
			}
			else if (dynamic_cast<Soldier *>(coObjects.at(i)))
			{
				Soldier*soldier = dynamic_cast<Soldier *>(coObjects.at(i));

				if (soldier->GetState() == SOLDIER_STATE_DIE)
				{
					listRemoveObjects.push_back(soldier);
				}
			}
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
			if (y > SCREEN_HEIGHT / 2 && y < MAX_HEIGHT_LV1 - SCREEN_HEIGHT / 2)
			{
				game->mCamera->setX(game->mCamera->getX());
				game->mCamera->setY(y - SCREEN_HEIGHT / 2);
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

		if (lv2 == true)
		{
			// Oy
			if (y > SCREEN_HEIGHT / 2 && y < MAX_HEIGHT_LV2 - SCREEN_HEIGHT / 2)
			{
				game->mCamera->setX(game->mCamera->getX());
				game->mCamera->setY(y - SCREEN_HEIGHT / 2);
			}
			else
			{
				game->mCamera->setX(game->mCamera->getX());
				game->mCamera->setY(game->mCamera->getY());
			}

			// Ox
			if (x > SCREEN_WIDTH / 2 && x < MAX_WIDTH_LV2 - SCREEN_WIDTH / 2)
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
		if (lv1 == true)
		{

			map = new	Map(/*48, 10,*/ tileset, 16, 16);
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