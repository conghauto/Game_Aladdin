//file path
#define GAME_FONT L"Resources\\prstart.ttf"

#define SIMON_WALKING_SPEED_END_GAME	0.005f 
#define SIMON_WALKING_SPEED				0.015f 
#define SIMON_CLIMBING_SPEED_X			0.01f
#define SIMON_CLIMBING_SPEED_Y			0.01f
#define SIMON_JUMP_SPEED_Y				0.05f
#define SIMON_JUMP_DEFLECT_SPEED		0.08f
#define SIMON_LEVELUP_SPEED				0.01f
#define SIMON_GRAVITY					0.00001f
#define SIMON_DIE_DEFLECT_SPEED			0.02f
#define SIMON_SIT_TO_STAND				15
#define SIMON_ON_LEFT_CHECKSTAIR		1
#define SIMON_ON_RIGHT_CHECKSTAIR		2
#define SIMON_ONSTAIR_ERR_RANGE			3

#define SIMON_DISTANCE_DASH				250
//state
#define SIMON_STATE_IDLE				0
#define SIMON_STATE_UNTOUCHABLE			100
#define SIMON_STATE_DIE					200
#define SIMON_STATE_ONCHECKSTAIR		300
#define SIMON_STATE_SIT					500
#define SIMON_STATE_WALK				600
#define SIMON_STATE_ONROPE				700
#define SIMON_STATE_ONSTAIR_IDLE		800
#define SIMON_STATE_HOLD_SHIELD			900
#define SIMON_STATE_DASHING				1000
#define SIMON_STATE_DOUBLE_JUMP			1100
#define SIMON_STATE_HURT				1200
#define SIMON_STATE_ONROPE_CLIMB		1300
#define SIMON_STATE_ONROPE_CLIMB_DOWN	1400
#define SIMON_STATE_WALK_END_GAME		1500
//action
#define SIMON_ACTION_JUMP				300
#define SIMON_ACTION_JUMP_ONROPE		1200

//action
#define SIMON_ACTION_JUMP				300
#define SIMON_ACTION_ATTACK				600
#define SIMON_ACTION_THROW				900

//animation
#define SIMON_ANI_IDLE_RIGHT			0
#define SIMON_ANI_IDLE_LEFT				1
#define SIMON_ANI_WALKING_RIGHT			2
#define SIMON_ANI_WALKING_LEFT			3
#define SIMON_ANI_ATTACK_RIGHT			4
#define SIMON_ANI_ATTACK_LEFT			5
#define SIMON_ANI_SIT_RIGHT				6
#define SIMON_ANI_SIT_LEFT				7
#define SIMON_ANI_JUMP_RIGHT			8
#define SIMON_ANI_JUMP_LEFT				9
#define SIMON_ANI_SIT_ATTACK_RIGHT		10
#define SIMON_ANI_SIT_ATTACK_LEFT		11
#define SIMON_ANI_HITUP_RLADDER			12 // hit right on stair which has up-direction from left to right
#define SIMON_ANI_HITDOWN_LLADDER		13  
#define SIMON_ANI_HITDOWN_RLADDER		14 //hit right on stair which has up-direction from right to left 
#define SIMON_ANI_HITUP_LLADDER			15 //use dik_ and nx to select ani
#define SIMON_ANI_WALKUP_RLADDER		16 // walk up on stair which has up-direction from left to right
#define SIMON_ANI_WALKDOWN_LLADDER		17 
#define SIMON_ANI_WALKUP_LLADDER		18
#define SIMON_ANI_WALKDOWN_RLADDER		19
#define SIMON_ANI_IDLE_WALKUP_RLADDER	20
#define SIMON_ANI_IDLE_WALKDOWN_LLADDER	21
#define SIMON_ANI_IDLE_WALKUP_LLADDER	22
#define SIMON_ANI_IDLE_WALKDOWN_RLADDER	23
#define SIMON_ANI_HURT_RIGHT			24
#define SIMON_ANI_HURT_LEFT				25
#define SIMON_ANI_DIE					26
#define SIMON_ANI_JUMP_ATTACK_RIGHT		27
#define SIMON_ANI_JUMP_ATTACK_LEFT		28
#define SIMON_ANI_DOUBLE_JUMP			29
#define SIMON_ANI_DASHING_RIGHT			30
#define SIMON_ANI_DASHING_LEFT			31
#define SIMON_ANI_SHIELD_RIGHT			32
#define SIMON_ANI_SHIELD_LEFT			33
#define SIMON_ANI_THROW_RIGHT			34
#define SIMON_ANI_THROW_LEFT			35
#define SIMON_ANI_SIT_THROW_RIGHT		36
#define SIMON_ANI_SIT_THROW_LEFT		37
#define SIMON_ANI_CLIMB_RIGHT			38
#define SIMON_ANI_CLIMB_LEFT			39
#define SIMON_ANI_ONROPE_IDLE_RIGHT		40
#define SIMON_ANI_ONROPE_IDLE_LEFT		41
// Whip
#define WHIP_RIGHT 						0
#define WHIP_LEFT						1
#define WHIP_RIGHT_1					2
#define WHIP_LEFT_1						3
#define WHIP_RIGHT_2					4
#define WHIP_LEFT_2						5


#define WHIP_LV0_BBOX_WIDTH				100
#define WHIP_LV0_BBOX_HEIGHT			15
#define WHIP_LV1_BBOX_WIDTH				105
#define WHIP_LV1_BBOX_HEIGHT			15
#define WHIP_LV2_BBOX_WIDTH				125
#define WHIP_LV2_BBOX_HEIGHT			15

// Big fire
#define BIGFIRE_BBOX_WIDTH				32
#define BIGFIRE_BBOX_HEIGHT				64

// Lantern 
#define LANTERN_BBOX_WIDTH				16
#define LANTERN_BBOX_HEIGHT				16


// Whip item
#define WHIPITEM_BBOX_WIDTH				32
#define WHIPITEM_BBOX_HEIGHT			32


//Money
#define MONEY_BBOX_WIDTH				30
#define MONEY_BBOX_HEIGHT				30

// Zombie
#define GUARDIAN_WALKING_SPEED			0.05f

#define GUARDIAN_BBOX_WIDTH				65
#define GUARDIAN_BBOX_HEIGHT			60
#define GUARDIAN_BBOX_HEIGHT_DIE		37

#define GUARDIAN_STATE_ATTACK			100
#define GUARDIAN_STATE_DIE				200
#define GUARDIAN_STATE_IDLE				300
#define GUARDIAN_STATE_HURT				400
#define GUARDIAN_STATE_WALK				500

#define GUARDIAN_ANI_ATTACKING_RIGHT	0
#define GUARDIAN_ANI_ATTACKING_LEFT		1
#define GUARDIAN_ANI_HURT_RIGHT			2
#define GUARDIAN_ANI_HURT_LEFT			3
#define GUARDIAN_ANI_IDLE_RIGHT			4
#define GUARDIAN_ANI_IDLE_LEFT			5
#define GUARDIAN_ANI_WALKING_RIGHT		6
#define GUARDIAN_ANI_WALKING_LEFT		7

// Jafar
#define JAFAR_WALKING_SPEED			0.05f

#define JAFAR_BBOX_WIDTH			55
#define JAFAR_BBOX_HEIGHT			60
#define JAFAR_BBOX_HEIGHT_DIE		37

#define JAFAR_STATE_ATTACK			100
#define JAFAR_STATE_DIE				200
#define JAFAR_STATE_IDLE			300
#define JAFAR_STATE_HURT			400

#define JAFAR_ANI_ATTACKING_RIGHT	2
#define JAFAR_ANI_ATTACKING_LEFT	3
#define JAFAR_ANI_HURT_RIGHT		4
#define JAFAR_ANI_HURT_LEFT			5
#define JAFAR_ANI_IDLE_RIGHT		0
#define JAFAR_ANI_IDLE_LEFT			1


// Boss Snake
#define SNAKE_WALKING_SPEED			0.05f

#define SNAKE_BBOX_WIDTH			55
#define SNAKE_BBOX_HEIGHT			60
#define SNAKE_BBOX_HEIGHT_DIE		37

#define SNAKE_STATE_ATTACK			100
#define SNAKE_STATE_DIE				200
#define SNAKE_STATE_HURT			400

#define SNAKE_ANI_ATTACKING_RIGHT	0	
#define SNAKE_ANI_ATTACKING_LEFT	1
#define SNAKE_ANI_HURT_RIGHT		2
#define SNAKE_ANI_HURT_LEFT			3

//Fire

#define FIRE_BBOX_WIDTH				30
#define FIRE_BBOX_HEIGHT			55

#define FIRE_STATE_BURNING			100
#define FIRE_STATE_DIE				200

#define FIRE_ANI_BURNING			0

//Fire Bullet

#define FIRE_BULLET_BBOX_WIDTH				90
#define FIRE_BULLET_BBOX_HEIGHT				40

#define FIRE_BULLET_STATE_ATTACK			100
#define FIRE_BULLET_STATE_DIE				200

#define FIRE_BULLET_ATTACK_RIGHT			0
#define FIRE_BULLET_ATTACK_LEFT				1

#define FIRE_BULLET_WALKING_SPEED			0.01f

//Soldier
#define SOLDIER_WALKING_SPEED			0.05f

#define SOLDIER_BBOX_WIDTH				50
#define SOLDIER_BBOX_HEIGHT				60
#define SOLDIER_BBOX_HEIGHT_DIE			37

#define SOLDIER_STATE_ATTACK			100
#define SOLDIER_STATE_DIE				200
#define SOLDIER_STATE_IDLE				300
#define SOLDIER_STATE_HURT				400
#define SOLDIER_STATE_WALK				500

#define SOLDIER_ANI_ATTACK_RIGHT		2
#define SOLDIER_ANI_ATTACK_LEFT			3
#define SOLDIER_ANI_IDLE_RIGHT			0
#define SOLDIER_ANI_IDLE_LEFT			1
#define SOLDIER_ANI_HURT_RIGHT			4
#define SOLDIER_ANI_HURT_LEFT			5
#define SOLDIER_ANI_WALK_RIGHT			6
#define SOLDIER_ANI_WALK_LEFT			7

//skeleton
#define SKELETON_WALKING_SPEED			0.005f

#define SKELETON_BBOX_WIDTH				55
#define SKELETON_BBOX_HEIGHT			60
#define SKELETON_BBOX_HEIGHT_DIE		37

#define SKELETON_STATE_WALK				100
#define SKELETON_STATE_DIE				200
#define SKELETON_STATE_IDLE				300
#define SKELETON_STATE_HURT				400

#define SKELETON_ANI_WALKING_RIGHT		2
#define SKELETON_ANI_WALKING_LEFT		3
#define SKELETON_ANI_DIE_RIGHT			4
#define SKELETON_ANI_DIE_LEFT			5
#define SKELETON_ANI_IDLE_RIGHT			0
#define SKELETON_ANI_IDLE_LEFT			1


//BossBat
#define BAT_BBOX_WIDTH				35	
#define BAT_BBOX_HEIGHT				30

#define BAT_STATE_WAIT				500
#define BAT_STATE_ATTACK			600
#define BAT_STATE_DIE				700

#define BOSSBAT_ANI_WAIT			0
#define BOSSBAT_ANI_ATTACK			1

#define BAT_ATTACK_SPEED			0.01f
//Bone
#define BONE_BBOX_WIDTH				35	
#define BONE_BBOX_HEIGHT			30

#define BONE_STATE_ATTACK			600
#define BONE_STATE_DIE				700

#define BONE_ANI_ATTACK			0

#define BONE_SPEED_ATTACK_X		0.02f
#define BONE_SPEED_ATTACK_Y		0.021f


//Stars
#define STAR_BBOX_WIDTH				35	
#define STAR_BBOX_HEIGHT			30

#define STAR_STATE_ATTACK			600
#define STAR_STATE_DIE				700

#define STAR_ANI_ATTACK			0

#define STAR_SPEED_ATTACK_X		0.02f
#define STAR_SPEED_ATTACK_Y		0.021f

// Check stair
#define CHECKSTAIR_UP_LEFT				0
#define CHECKSTAIR_UP_RIGHT				1
#define CHECKSTAIR_DOWN_LEFT			2
#define CHECKSTAIR_DOWN_RIGHT			3

#define CHECKGATE_BBOX_WIDTH			32
#define CHECKGATE_BBOX_HEIGHT			16

// Timer
#define SIMON_TIMER_ATTACK				500

#define	SIMON_LEVEL_SMALL				1
#define	SIMON_LEVEL_BIG					2

#define SIMON_STAND_BBOX_WIDTH			32
#define SIMON_STAND_BBOX_HEIGHT			45
#define SIMON_SIT_BBOX_WIDTH			32
#define SIMON_SIT_BBOX_HEIGHT			47

#define SIMON_UNTOUCHABLE_TIME			2000

#define WINDOW_CLASS_NAME L"Game Aladdin"
#define MAIN_WINDOW_TITLE L"Game Aladdin"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(255, 255, 255)
#define SCREEN_WIDTH					640
#define SCREEN_HEIGHT					370
#define MAX_HEIGHT_LV1                  1064
#define MAX_WIDTH_LV1					2144
#define MAX_HEIGHT_LV2                  440
#define MAX_WIDTH_LV2					960
#define MAX_FRAME_RATE					120

//texture
#define ID_TEX_ALLADIN					0
#define ID_TEX_LV1						10
#define ID_TEX_LV1_2					15
#define ID_TEX_ROCK						20
#define ID_TEX_SPIKE					25
#define ID_TEX_DUMBBELL				    30
#define ID_TEX_GUARDIAN					35
#define ID_TEX_GUARDIAN_RIGHT			36
#define ID_TEX_BAT						37
#define ID_TEX_TILESET					60
#define ID_TEX_TILESET_2				61
#define ID_TEX_SIT						200
#define ID_TEX_KNIFE					165
#define ID_TEX_RECT						176
#define ID_TEX_SKELETON					177
#define ID_TEX_PILLAR_1					178
#define ID_TEX_PILLAR_2					179
#define ID_TEX_PILLAR_3					180
#define ID_TEX_PILLAR_4					181
#define ID_TEX_GATE						182
#define ID_TEX_BOSS						183
#define ID_TEX_SWORD_GUARDIAN			184
#define ID_TEX_SWORD_ATTACK				185
#define ID_TEX_SWORD_WALK				186
#define ID_TEX_ALADDIN_ATTACK			187
#define ID_TEX_ALADDIN_STAND			188
#define ID_TEX_ALADDIN_SIT				189
#define ID_TEX_ALADDIN_SIT_THROW		190
// Pillar
#define ID_TEX_PILLAR1					197
#define ID_TEX_PILLAR2					198
#define ID_TEX_PILLAR3					199
#define ID_TEX_PILLAR4					200
#define ID_TEX_GATE						201

#define BRICK_BBOX_WIDTH				32
#define BRICK_BBOX_HEIGHT				3

// Item apple
#define ITEM_APPLE_BBOX_WIDTH			5
#define ITEM_APPLE_BBOX_HEIGHT			12


// GenieFace
#define ITEM_GENIEFACE_BBOX_WIDTH		35
#define ITEM_GENIEFACE_BBOX_HEIGHT		50

// GenieJar
#define ITEM_GENIEJAR_BBOX_WIDTH		23
#define ITEM_GENIEJAR_BBOX_HEIGHT		30

#define ROPE_BBOX_WIDTH					1
#define ROPE_BBOX_HEIGHT				64

#define EFFECT_TIME						200
#define LEVELUP_TIME					3000

// Item
#define ITEM_WHIPITEM					808
#define ITEM_HEART						805
#define ITEM_KNIFE						809
#define ITEM_MONEY						810
#define ITEM_APPLE						812
#define ITEM_1						    813
#define ITEM_2						    814
#define ITEM_3						    815
#define ITEM_BG_APPLE					816
#define ALADDIN_LIFE					817
#define ITEM_BG_SPEND					818
#define ITEM_BG_GOD_LIGHT				819
#define ITEM_GENIEFACE					820
#define ITEM_GENIEJAR				    821
#define ITEM_SPEND						822
#define ITEM_HEART						823
#define ITEM_BG_NEXT_GAME				824
#define ITEM_BG_GAME_OVER				825
#define ITEM_BG_INFO					826

//bullet
#define BULLET_BBOX_WIDTH				16
#define BULLET_BBOX_HEIGHT				16
#define WEAPON_ANI_BULLET				1608
#define BULLET_SPEED					0.4f
//Begin Bullet
#define BALLBEGIN					20
#define BALLBEGIN					20
#define WEAPON_ANI_BALLBEGIN		668
#define BALLBEGIN					0.1f
//ball
#define BALL_BBOX_WIDTH				20
#define BALL_BBOX_HEIGHT			20
#define WEAPON_ANI_BALL				1609
#define BALL_SPEED					0.4f
//knife
#define WEAPON_ANI_KNIFE				1607
#define WEAPON_ACTIVATE_TIME			300
#define KNIFE_BBOX_WIDTH				32
#define KNIFE_BBOX_HEIGHT				32
#define KNIFE_SPEED_X					0.05f
#define KNIFE_SPEED_Y					0.015f
// Checkpoint type
#define CHECKPOINT_LEVELUP				0
#define CHECKPOINT_BONUS				1

// Lantern 
#define LANTERN_BBOX_WIDTH				16
#define LANTERN_BBOX_HEIGHT				16

// Rock
#define ROCK_BBOX_WIDTH					32
#define ROCK_BBOX_HEIGHT				16

// Spike
#define SPIKE_BBOX_WIDTH				16
#define SPIKE_BBOX_HEIGHT				16

// Dumbbell
#define DUMBBELL_BBOX_WIDTH				16
#define DUMBBELL_BBOX_HEIGHT			40

#define CELL_SIZE						640
