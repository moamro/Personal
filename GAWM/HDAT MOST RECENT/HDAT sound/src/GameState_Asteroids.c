/*-------------------------------------------------------------------------- -
Project Title         :        CS 230: Project 2 Part 2
File Name             :        GameState_Asteroids.c
Author                :        Shawn Masters
Creation Date         :        1/30/16
Purpose               :        Implementation of the play game state
History
- 1/30/16             :        Added file header.
- 1/30/16             :        Added ship movement and shooting.
- 1/30/16             :        Added asteroid creatiom, movement, and wrapping.
- 1/30/16             :        Added homing missile movement and tracking.
- 1/30/16             :        Added freeing and unloading to game loop.
© Copyright 1996 - 2016, DigiPen Institute of Technology(USA).All rights reserved.
----------------------------------------------------------------------------*/

#include "EnemyLoader.h"
#include "GameState_AsteroidsDefines.h"
#include "main.h"
#include "fmod.h"
#include "fmod_common.h"
#include "fmod_errors.h"
#include <stdlib.h>

float obj1X = 0.0f, obj1Y = 0.0f;		// Position variables for object 1
AEGfxTexture *pPlayerTex;
AEGfxTexture *pEnemyTex;
AEGfxTexture *pMinionTex;
AEGfxTexture *pLifeTex;
AEGfxTexture *pLeftSideTex;
AEGfxTexture *pRightSideTex;
AEGfxTexture *pPlayerBulletTex;
AEGfxTexture *pHealthFullTex;
AEGfxTexture *pHealthHalfTex;
AEGfxTexture *pSplashTex;
AEGfxTexture *pBoss1Tex;
AEGfxTexture *pBoss2Tex;
AEGfxTexture *pBoss3Tex;
AEGfxTexture *pBoss4Tex;
AEGfxTexture *pStartButtonTex;
AEGfxTexture *pExitButtonTex;
AEGfxTexture *pCreditsButtonTex;
AEGfxTexture *pLogoTex;
AEGfxTexture *pPlayerFlashTex;
AEGfxTexture *pBossFlash1Tex;
AEGfxTexture *pBossFlash2Tex;
AEGfxTexture *pBossFlash3Tex;
AEGfxTexture *pBossFlash4Tex;
AEGfxTexture *p11Tex;
AEGfxTexture *p12Tex;
AEGfxTexture *p13Tex;
AEGfxTexture *p21Tex;
AEGfxTexture *p22Tex;
AEGfxTexture *p23Tex;
AEGfxTexture *p31Tex;
AEGfxTexture *p32Tex;
AEGfxTexture *p33Tex;
AEGfxTexture *p41Tex;
AEGfxTexture *p42Tex;
AEGfxTexture *p43Tex;
AEGfxTexture *p1Tex;
AEGfxTexture *p2Tex;
AEGfxTexture *p3Tex;
AEGfxTexture *pCreditsTex;
AEGfxTexture *pWinTex;

TextureOffset	textureOffsets[4];
TextureOffset	bossOffsets[8];

int shipDraw = 0;
int drawDelay = 5;

unsigned int currentTextureOffset = 0;
unsigned int currentBossOffset = 0;

float animation_timer = 3.5f;
float boss_timer = 8.5f;

	// Level Selection1
unsigned int CurrentLevel = GS_ASTEROIDS_LEVEL6;
unsigned int NextLevel;
unsigned int changeLevelFlag = 0;

// List of original vertex buffers
static Shape				sgShapes[SHAPE_NUM_MAX];									// Each element in this array represents a unique shape 
static unsigned long		sgShapeNum;													// The number of defined shapes

																						// list of object instances
static GameObjectInstance		sgGameObjectInstanceList[GAME_OBJ_INST_NUM_MAX];		// Each element in this array represents a unique game object instance
static unsigned long			sgGameObjectInstanceNum;								// The number of active game object instances

static EnemyInstance*	enemyInstanceList;		// Array of enemies
static unsigned int		enemyInstanceNum = 0;	// The index that that represents the next monster to be put in a spawn point
static EnemySpawnPoint* spawnPointList;			// Array of spawn points

GameObjectInstance*		sgpShip;				// Pointer to the "Ship" game object instance
GameObjectInstance* ForwardTrail;

// number of ship available (lives 0 = game over)
int						        sgShipLives;											// The number of lives left
int								sgShipHealth;											// The amount of health left
static unsigned long			sgScore;												// Current score

//FMOD stuff
static FMOD_SYSTEM *FmodSystem;
static FMOD_SOUND *sound1, *sound2, *background, *background2;
static FMOD_CHANNEL *channel = 0;
static FMOD_CHANNEL *quietChannel = 1;
static FMOD_CHANNEL *channel2 = 2;
static FMOD_CHANNEL *channel3 = 3;
FMOD_RESULT result;
unsigned int version;

// ---------------------------------------------------------------------------
double frameTime;
double enemyTimer = 1.5;
const double enemyShootTime = 2.0;
int rockMovePatterns = 0;

Timer *fire;
Timer *splash;

float winMaxX, winMaxY, winMinX, winMinY;

int EnemiesKilled;
int BossesKilled;

GameObjectInstance *pBoss;
GameObjectInstance* pButton1;
GameObjectInstance *pButton2;
GameObjectInstance *pButton3;
GameObjectInstance *red;
GameObjectInstance *yellow;
GameObjectInstance *green;

Timer* switchscreen_timer;


// "Load" function of this state
void GameStateAsteroidsLoad(void)
{
	Shape* pShape = NULL;

	// Zero the shapes array
	memset(sgShapes, 0, sizeof(Shape) * SHAPE_NUM_MAX);
	// No shapes at this point
	sgShapeNum = 0;

	// The ship object instance hasn't been created yet, so this "sgpShip" pointer is initialized to 0
	sgpShip = 0;



	/// Create the game objects(shapes) : Ships, Bullet, Asteroid and Missile
	// How to:
	// -- Remember to create normalized shapes, which means all the vertices' coordinates should be in the [-0.5;0.5] range. Use the object instances' scale values to resize the shape.
	// -- Call “AEGfxMeshStart” to inform the graphics manager that you are about the start sending triangles.
	// -- Call “AEGfxTriAdd” to add 1 triangle.
	// -- A triangle is formed by 3 counter clockwise vertices (points).
	// -- Create all the points between (-0.5, -0.5) and (0.5, 0.5), and use the object instance's scale to change the size.
	// -- Each point can have its own color.
	// -- The color format is : ARGB, where each 2 hexadecimal digits represent the value of the Alpha, Red, Green and Blue respectively. Note that alpha blending(Transparency) is not implemented.
	// -- Each point can have its own texture coordinate (set them to 0.0f in case you’re not applying a texture).
	// -- An object (Shape) can have multiple triangles.
	// -- Call “AEGfxMeshEnd” to inform the graphics manager that you are done creating a mesh, which will return a pointer to the mesh you just created.



	// =====================
	// Create the ship shape
	// =====================

	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_SHIP;

	AEGfxMeshStart();

	AEGfxTriAdd(
		-2.5f, -2.5f, 0x00FF00FF, 0.0f, 0.5f,
		2.5f, -2.5f, 0x00FFFF00, 0.5f, 0.5f,
		-2.5f, 2.5f, 0x00F00FFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		2.5f, -2.5f, 0x00FFFFFF, 0.5f, 0.5f,
		2.5f, 2.5f, 0x00FFFFFF, 0.5f, 0.0f,
		-2.5f, 2.5f, 0x00FFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1
	pShape->mpMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pShape->mpMesh, "Failed to create mesh 1!!");

	pPlayerTex = AEGfxTextureLoad("PlayerAnimation.png");
	AE_ASSERT_MESG(pPlayerTex, "Failed to create texture1!!");

	textureOffsets[0].mX = 0.0f;			textureOffsets[0].mY = 0.0f;
	textureOffsets[1].mX = 0.5f;			textureOffsets[1].mY = 0.0f;
	textureOffsets[2].mX = 0.0f;			textureOffsets[2].mY = 0.5f;
	textureOffsets[3].mX = 0.5f;			textureOffsets[3].mY = 0.5f;

	/////////////////////////////////////////////////////////////////////
	//create the bullet shape
	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_BULLET;

	AEGfxMeshStart();

	AEGfxTriAdd(
		-2.5f, -2.5f, 0x00FF00FF, 0.0f, 1.0f,
		2.5f, -2.5f, 0x00FFFF00, 1.0f, 1.0f,
		-2.5f, 2.5f, 0x00F00FFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		2.5f, -2.5f, 0x00FFFFFF, 1.0f, 1.0f,
		2.5f, 2.5f, 0x00FFFFFF, 1.0f, 0.0f,
		-2.5f, 2.5f, 0x00FFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1
	pShape->mpMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pShape->mpMesh, "Failed to create mesh 1!!");

	pPlayerBulletTex = AEGfxTextureLoad("Bullet.png");
	AE_ASSERT_MESG(pPlayerBulletTex, "Failed to create texture1!!");


	//create enemy shape
	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_ASTEROID; //this is really an enemy

	AEGfxMeshStart();

	AEGfxTriAdd(
		-1.5f, -1.5f, 0x00FFFFFF, 0.0f, 1.0f,
		1.5f, -1.5f, 0x00FFFFFF, 1.0f, 1.0f,
		-1.5f, 1.5f, 0x00FFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		1.5f, -1.5f, 0x00FFFFFF, 1.0f, 1.0f,
		1.5f, 1.5f, 0x00FFFFFF, 1.0f, 0.0f,
		-1.5f, 1.5f, 0x00FFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1
	pShape->mpMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pShape->mpMesh, "Failed to create mesh 1!!");

	pEnemyTex = AEGfxTextureLoad("Enemy3.png");
	AE_ASSERT_MESG(pEnemyTex, "Failed to create texture1!!");


	//create homing missile shape
	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_HOMING_MISSILE;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-3.5f, 0.5f, 0xFF00CCFF, 0.0f, 0.0f,
		-3.5f, -0.5f, 0xFF00CCFF, 0.0f, 0.0f,
		0.5f, 0.0f, 0xFF00CCFF, 0.0f, 0.0f);
	pShape->mpMesh = AEGfxMeshEnd();

	//create enemy bullet shape
	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_ENEMY_BULLET;

	AEGfxMeshStart();
	AEGfxTriAdd(
		0.0f, 0.25f, 0xFF00FF00, 0.0f, 0.0f,
		0.0f, -0.25f, 0xFF00FF00, 0.0f, 0.0f,
		1.0f, 0.0f, 0xFF00FF00, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.0f, 0.15f, 0xFF00FF00, 0.0f, 0.0f,
		0.0f, -0.15f, 0xFF00FF00, 0.0f, 0.0f,
		-0.5f, 0.0f, 0xFF00FF00, 0.0f, 0.0f);
	pShape->mpMesh = AEGfxMeshEnd();

	//star1//////////////////////////////////////////////////////////
	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_STAR1;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-1.0f, -1.0f, 0xFFFFFFFF, 0.0f, 1.0f,
		1.0f, -1.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		-1.0f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		1.0f, -1.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		1.0f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f,
		-1.0f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f);
	pShape->mpMesh = AEGfxMeshEnd();

	//star2//////////////////////////////////////////////////////////
	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_STAR2;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-1.0f, -1.0f, 0xAAFFFFFF, 0.0f, 1.0f,
		1.0f, -1.0f, 0xAAFFFFFF, 1.0f, 1.0f,
		-1.0f, 1.0f, 0xAAFFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		1.0f, -1.0f, 0xAAFFFFFF, 1.0f, 1.0f,
		1.0f, 1.0f, 0xAAFFFFFF, 1.0f, 0.0f,
		-1.0f, 1.0f, 0xAAFFFFFF, 0.0f, 0.0f);
	pShape->mpMesh = AEGfxMeshEnd();

	//star3//////////////////////////////////////////////////////////
	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_STAR3;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-1.0f, -1.0f, 0x66FFFFFF, 0.0f, 1.0f,
		1.0f, -1.0f, 0x66FFFFFF, 1.0f, 1.0f,
		-1.0f, 1.0f, 0x66FFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		1.0f, -1.0f, 0x66FFFFFF, 1.0f, 1.0f,
		1.0f, 1.0f, 0x66FFFFFF, 1.0f, 0.0f,
		-1.0f, 1.0f, 0x66FFFFFF, 0.0f, 0.0f);
	pShape->mpMesh = AEGfxMeshEnd();

	//life///////////////////////////////////////////////////////////
	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_LIFE;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-2.5f, -2.5f, 0x00FF00FF, 0.0f, 1.0f,
		2.5f, -2.5f, 0x00FFFF00, 1.0f, 1.0f,
		-2.5f, 2.5f, 0x00F00FFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		2.5f, -2.5f, 0x00FFFFFF, 1.0f, 1.0f,
		2.5f, 2.5f, 0x00FFFFFF, 1.0f, 0.0f,
		-2.5f, 2.5f, 0x00FFFFFF, 0.0f, 0.0f);
	pShape->mpMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pShape->mpMesh, "Failed to create mesh 1!!");

	pLifeTex = AEGfxTextureLoad("Player.png");
	AE_ASSERT_MESG(pPlayerTex, "Failed to create texture1!!");

	//Pinktrail//////////////////////////////////////////////////////////
	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_PINK_TRAIL;

	AEGfxMeshStart();

	//1st argument: X
	//2nd argument: Y
	//3rd argument: ARGB
	AEGfxTriAdd(
		-0.5f, -0.5f, 0x66ff80ff, 0.0f, 0.0f,
		0.5f, -0.5f, 0x66ff80ff, 0.0f, 0.0f,
		-0.5f, 0.5f, 0x66ff80ff, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0x66ff80ff, 0.0f, 0.0f,
		0.5f, 0.5f, 0x66ff80ff, 0.0f, 0.0f,
		-0.5f, 0.5f, 0x66ff80ff, 0.0f, 0.0f);

	pShape->mpMesh = AEGfxMeshEnd();
	/////////////////////////////////////////////////////////////////
	//Whitetrail//////////////////////////////////////////////////////////
	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_WHITE_TRAIL;

	AEGfxMeshStart();

	//1st argument: X
	//2nd argument: Y
	//3rd argument: ARGB
	AEGfxTriAdd(
		-0.5f, -0.5f, 0x66ffffff, 0.0f, 0.0f,
		0.5f, -0.5f, 0x66ffffff, 0.0f, 0.0f,
		-0.5f, 0.5f, 0x66ffffff, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0x66ffffff, 0.0f, 0.0f,
		0.5f, 0.5f, 0x66ffffff, 0.0f, 0.0f,
		-0.5f, 0.5f, 0x66ffffff, 0.0f, 0.0f);

	pShape->mpMesh = AEGfxMeshEnd();
	/////////////////////////////////////////////////////////////////
	//Bluetrail//////////////////////////////////////////////////////////
	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_BLUE_TRAIL;

	AEGfxMeshStart();

	//1st argument: X
	//2nd argument: Y
	//3rd argument: ARGB
	AEGfxTriAdd(
		-0.5f, -0.5f, 0x664ddbff, 0.0f, 0.0f,
		0.5f, -0.5f, 0x664ddbff, 0.0f, 0.0f,
		-0.5f, 0.5f, 0x664ddbff, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0x664ddbff, 0.0f, 0.0f,
		0.5f, 0.5f, 0x664ddbff, 0.0f, 0.0f,
		-0.5f, 0.5f, 0x664ddbff, 0.0f, 0.0f);

	pShape->mpMesh = AEGfxMeshEnd();
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	//EnigmaDeath//////////////////////////////////////////////////////////
	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_ENIGMA_DEATHRED;

	AEGfxMeshStart();

	//1st argument: X
	//2nd argument: Y
	//3rd argument: ARGB
	AEGfxTriAdd(
		-0.5f, -0.5f, 0x66ff0000, 0.0f, 0.0f,
		0.5f, -0.5f, 0x66ff0000, 0.0f, 0.0f,
		-0.5f, 0.5f, 0x66ff0000, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0x66ff0000, 0.0f, 0.0f,
		0.5f, 0.5f, 0x66ff0000, 0.0f, 0.0f,
		-0.5f, 0.5f, 0x66ff0000, 0.0f, 0.0f);

	pShape->mpMesh = AEGfxMeshEnd();


	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_ENIGMA_DEATHWHITE;

	AEGfxMeshStart();

	//1st argument: X
	//2nd argument: Y
	//3rd argument: ARGB
	AEGfxTriAdd(
		-0.25f, -0.25f, 0xff8c8c8c, 0.0f, 0.0f,
		0.25f, -0.25f, 0xff8c8c8c, 0.0f, 0.0f,
		-0.25f, 0.25f, 0xff8c8c8c, 0.0f, 0.0f);

	pShape->mpMesh = AEGfxMeshEnd();

	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_ENIGMA_DEATHDEBRIS;

	AEGfxMeshStart();

	//1st argument: X
	//2nd argument: Y
	//3rd argument: ARGB
	AEGfxTriAdd(
		-0.25f, -0.25f, 0xff8c8c8c, 0.0f, 0.0f,
		0.25f, -0.25f, 0xff8c8c8c, 0.0f, 0.0f,
		-0.25f, 0.25f, 0xff8c8c8c, 0.0f, 0.0f);

	pShape->mpMesh = AEGfxMeshEnd();
	/////////////////////////////////////////////////////////////////
	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_LSIDE;

	AEGfxMeshStart();

	AEGfxTriAdd(
		-2.5f, -2.5f, 0x00FF00FF, 0.0f, 1.0f,
		2.5f, -2.5f, 0x00FFFF00, 1.0f, 1.0f,
		-2.5f, 2.5f, 0x00F00FFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		2.5f, -2.5f, 0x00FFFFFF, 1.0f, 1.0f,
		2.5f, 2.5f, 0x00FFFFFF, 1.0f, 0.0f,
		-2.5f, 2.5f, 0x00FFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1
	pShape->mpMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pShape->mpMesh, "Failed to create mesh 1!!");

	pLeftSideTex = AEGfxTextureLoad("sidebarleft.png");
	AE_ASSERT_MESG(pLeftSideTex, "Failed to create texture1!!");

	/////////////////////////////////////////////////////////////////
	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_RSIDE;

	AEGfxMeshStart();

	AEGfxTriAdd(
		-2.5f, -2.5f, 0x00FF00FF, 0.0f, 1.0f,
		2.5f, -2.5f, 0x00FFFF00, 1.0f, 1.0f,
		-2.5f, 2.5f, 0x00F00FFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		2.5f, -2.5f, 0x00FFFFFF, 1.0f, 1.0f,
		2.5f, 2.5f, 0x00FFFFFF, 1.0f, 0.0f,
		-2.5f, 2.5f, 0x00FFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1
	pShape->mpMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pShape->mpMesh, "Failed to create mesh 1!!");

	pRightSideTex = AEGfxTextureLoad("sidebarright.png");
	AE_ASSERT_MESG(pRightSideTex, "Failed to create texture1!!");

	/////////////////////////////////////////////////////////////////
	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_HEALTH_FULL;

	AEGfxMeshStart();

	AEGfxTriAdd(
		-2.5f, -2.5f, 0x00FF00FF, 0.0f, 1.0f,
		2.5f, -2.5f, 0x00FFFF00, 1.0f, 1.0f,
		-2.5f, 2.5f, 0x00F00FFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		2.5f, -2.5f, 0x00FFFFFF, 1.0f, 1.0f,
		2.5f, 2.5f, 0x00FFFFFF, 1.0f, 0.0f,
		-2.5f, 2.5f, 0x00FFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1
	pShape->mpMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pShape->mpMesh, "Failed to create mesh 1!!");

	pHealthFullTex = AEGfxTextureLoad("HealthFull.png");
	AE_ASSERT_MESG(pHealthFullTex, "Failed to create texture1!!");

	/////////////////////////////////////////////////////////////////
	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_HEALTH_HALF;

	AEGfxMeshStart();

	AEGfxTriAdd(
		-2.5f, -2.5f, 0x00FF00FF, 0.0f, 1.0f,
		2.5f, -2.5f, 0x00FFFF00, 1.0f, 1.0f,
		-2.5f, 2.5f, 0x00F00FFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		2.5f, -2.5f, 0x00FFFFFF, 1.0f, 1.0f,
		2.5f, 2.5f, 0x00FFFFFF, 1.0f, 0.0f,
		-2.5f, 2.5f, 0x00FFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1
	pShape->mpMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pShape->mpMesh, "Failed to create mesh 1!!");

	pHealthHalfTex = AEGfxTextureLoad("HealthHalf.png");
	AE_ASSERT_MESG(pHealthHalfTex, "Failed to create texture1!!");

	//create standard button shape
	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_START_BUTTON;

	AEGfxMeshStart();
	
	AEGfxTriAdd(
		-1.5f, -1.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		1.5f, -1.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-1.5f, 1.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		1.5f, -1.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		1.5f, 1.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-1.5f, 1.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	// Saving the mesh
	pShape->mpMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pShape->mpMesh, "Failed to create mesh 1!!");

	pStartButtonTex = AEGfxTextureLoad("start_button.png");
	AE_ASSERT_MESG(pStartButtonTex, "Failed to create texture1!!");


	//create splash
	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_DIGIPEN_SPLASH;

	AEGfxMeshStart();

	AEGfxTriAdd(
		-2.5f, -2.5f, 0x00FF00FF, 0.0f, 1.0f,
		2.5f, -2.5f, 0x00FFFF00, 1.0f, 1.0f,
		-2.5f, 2.5f, 0x00F00FFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		2.5f, -2.5f, 0x00FFFFFF, 1.0f, 1.0f,
		2.5f, 2.5f, 0x00FFFFFF, 1.0f, 0.0f,
		-2.5f, 2.5f, 0x00FFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1
	pShape->mpMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pShape->mpMesh, "Failed to create mesh 1!!");

	pSplashTex = AEGfxTextureLoad("splash.png");
	AE_ASSERT_MESG(pSplashTex, "Failed to create texture1!!");

	

	//THE BOSS////////////////////////////////////////////
	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_BOSS;

	AEGfxMeshStart();

	AEGfxTriAdd(
		-2.5f, -2.5f, 0x00FF00FF, 0.0f, 0.25f,
		2.5f, -2.5f, 0x00FFFF00, 0.5f, 0.25f,
		-2.5f, 2.5f, 0x00F00FFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		2.5f, -2.5f, 0x00FFFFFF, 0.5f, 0.25f,
		2.5f, 2.5f, 0x00FFFFFF, 0.5f, 0.0f,
		-2.5f, 2.5f, 0x00FFFFFF, 0.0f, 0.0f);

	bossOffsets[0].mX = 0.0f;			bossOffsets[0].mY = 0.0f;
	bossOffsets[1].mX = 0.5f;			bossOffsets[1].mY = 0.0f;
	bossOffsets[2].mX = 0.0f;			bossOffsets[2].mY = 0.25f;
	bossOffsets[3].mX = 0.5f;			bossOffsets[3].mY = 0.25f;
	bossOffsets[4].mX = 0.0f;			bossOffsets[4].mY = 0.5f;
	bossOffsets[5].mX = 0.5f;			bossOffsets[5].mY = 0.5f;
	bossOffsets[6].mX = 0.0f;			bossOffsets[6].mY = 0.75f;
	bossOffsets[7].mX = 0.5f;			bossOffsets[7].mY = 0.75f;

	// Saving the mesh (list of triangles) in pMesh1
	pShape->mpMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pShape->mpMesh, "Failed to create mesh 1!!");

	pBoss1Tex = AEGfxTextureLoad("BossAnimation.png");
	AE_ASSERT_MESG(pBoss1Tex, "Failed to create texture1!!");

	pBoss2Tex = AEGfxTextureLoad("Boss2.png");
	AE_ASSERT_MESG(pBoss2Tex, "Failed to create texture1!!");

	pBoss3Tex = AEGfxTextureLoad("Boss3.png");
	AE_ASSERT_MESG(pBoss3Tex, "Failed to create texture1!!");

	pBoss4Tex = AEGfxTextureLoad("Boss4.png");
	AE_ASSERT_MESG(pBoss4Tex, "Failed to create texture1!!");

	// Spawn Particle
	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_ENIGMA_SPAWN;

	AEGfxMeshStart();

	//1st argument: X
	//2nd argument: Y
	//3rd argument: ARGB
	AEGfxTriAdd(
		-0.5f, -0.5f, 0x666600cc, 0.0f, 0.0f,
		0.5f, -0.5f, 0x666600cc, 0.0f, 0.0f,
		-0.5f, 0.5f, 0x666600cc, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0x666600cc, 0.0f, 0.0f,
		0.5f, 0.5f, 0x666600cc, 0.0f, 0.0f,
		-0.5f, 0.5f, 0x666600cc, 0.0f, 0.0f);

	pShape->mpMesh = AEGfxMeshEnd();

	//create standard button shape
	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_EXIT_BUTTON;

	AEGfxMeshStart();

	AEGfxTriAdd(
		-1.5f, -1.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		1.5f, -1.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-1.5f, 1.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		1.5f, -1.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		1.5f, 1.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-1.5f, 1.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	// Saving the mesh
	pShape->mpMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pShape->mpMesh, "Failed to create mesh 1!!");


	pExitButtonTex = AEGfxTextureLoad("exit_button.png");
	AE_ASSERT_MESG(pExitButtonTex, "Failed to create texture1!!");

	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_CREDITS_BUTTON;

	AEGfxMeshStart();

	AEGfxTriAdd(
		-1.5f, -1.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		1.5f, -1.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-1.5f, 1.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		1.5f, -1.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		1.5f, 1.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-1.5f, 1.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	// Saving the mesh
	pShape->mpMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pShape->mpMesh, "Failed to create mesh 1!!");


	pCreditsButtonTex = AEGfxTextureLoad("credits_button.png");
	AE_ASSERT_MESG(pCreditsButtonTex, "Failed to create texture1!!");

	//////////////////////////////////////////////////////////////////

	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_LOGO;

	AEGfxMeshStart();

	AEGfxTriAdd(
		-2.5f, -2.5f, 0x00FF00FF, 0.0f, 1.0f,
		2.5f, -2.5f, 0x00FFFF00, 1.0f, 1.0f,
		-2.5f, 2.5f, 0x00F00FFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		2.5f, -2.5f, 0x00FFFFFF, 1.0f, 1.0f,
		2.5f, 2.5f, 0x00FFFFFF, 1.0f, 0.0f,
		-2.5f, 2.5f, 0x00FFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1
	pShape->mpMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pShape->mpMesh, "Failed to create mesh 1!!");

	pLogoTex = AEGfxTextureLoad("logo.png");
	AE_ASSERT_MESG(pLogoTex, "Failed to create texture1!!");

	//////////////////////////////////////////////////////////////////

	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_SHIP_FLASH;

	AEGfxMeshStart();

	AEGfxTriAdd(
		-2.5f, -2.5f, 0x00FF00FF, 0.0f, 1.0f,
		2.5f, -2.5f, 0x00FFFF00, 1.0f, 1.0f,
		-2.5f, 2.5f, 0x00F00FFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		2.5f, -2.5f, 0x00FFFFFF, 1.0f, 1.0f,
		2.5f, 2.5f, 0x00FFFFFF, 1.0f, 0.0f,
		-2.5f, 2.5f, 0x00FFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1
	pShape->mpMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pShape->mpMesh, "Failed to create mesh 1!!");

	pPlayerFlashTex = AEGfxTextureLoad("PlayerFlash.png");
	AE_ASSERT_MESG(pPlayerFlashTex, "Failed to create texture1!!");

	/////////////////////////////////////////////////////////////////////

	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_BOSS_FLASH;

	AEGfxMeshStart();

	AEGfxTriAdd(
		-2.5f, -2.5f, 0x00FF00FF, 0.0f, 1.0f,
		2.5f, -2.5f, 0x00FFFF00, 1.0f, 1.0f,
		-2.5f, 2.5f, 0x00F00FFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		2.5f, -2.5f, 0x00FFFFFF, 1.0f, 1.0f,
		2.5f, 2.5f, 0x00FFFFFF, 1.0f, 0.0f,
		-2.5f, 2.5f, 0x00FFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1
	pShape->mpMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pShape->mpMesh, "Failed to create mesh 1!!");

	pBossFlash1Tex = AEGfxTextureLoad("BossFlash1.png");
	AE_ASSERT_MESG(pBossFlash1Tex, "Failed to create texture1!!");

	pBossFlash2Tex = AEGfxTextureLoad("BossFlash2.png");
	AE_ASSERT_MESG(pBossFlash2Tex, "Failed to create texture1!!");

	pBossFlash3Tex = AEGfxTextureLoad("BossFlash3.png");
	AE_ASSERT_MESG(pBossFlash3Tex, "Failed to create texture1!!");

	pBossFlash4Tex = AEGfxTextureLoad("BossFlash4.png");
	AE_ASSERT_MESG(pBossFlash4Tex, "Failed to create texture1!!");

	//RED BOX
	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_RED_BOX;


	AEGfxMeshStart();

	//1st argument: X
	//2nd argument: Y
	//3rd argument: ARGB
	AEGfxTriAdd(
		-2.5f, -2.5f, 0x00FF00FF, 0.0f, 1.0f,
		2.5f, -2.5f, 0x00FFFF00, 1.0f, 1.0f,
		-2.5f, 2.5f, 0x00F00FFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		2.5f, -2.5f, 0x00FFFFFF, 1.0f, 1.0f,
		2.5f, 2.5f, 0x00FFFFFF, 1.0f, 0.0f,
		-2.5f, 2.5f, 0x00FFFFFF, 0.0f, 0.0f);

	pShape->mpMesh = AEGfxMeshEnd();

	p3Tex = AEGfxTextureLoad("3.png");
	AE_ASSERT_MESG(p3Tex, "Failed to create texture1!!");

	//YELLOW BOX
	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_YELLOW_BOX;


	AEGfxMeshStart();

	//1st argument: X
	//2nd argument: Y
	//3rd argument: ARGB
	AEGfxTriAdd(
		-2.5f, -2.5f, 0x00FF00FF, 0.0f, 1.0f,
		2.5f, -2.5f, 0x00FFFF00, 1.0f, 1.0f,
		-2.5f, 2.5f, 0x00F00FFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		2.5f, -2.5f, 0x00FFFFFF, 1.0f, 1.0f,
		2.5f, 2.5f, 0x00FFFFFF, 1.0f, 0.0f,
		-2.5f, 2.5f, 0x00FFFFFF, 0.0f, 0.0f);

	pShape->mpMesh = AEGfxMeshEnd();

	p2Tex = AEGfxTextureLoad("2.png");
	AE_ASSERT_MESG(p2Tex, "Failed to create texture1!!");

	//GREEN BOX
	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_GREEN_BOX;


	AEGfxMeshStart();

	//1st argument: X
	//2nd argument: Y
	//3rd argument: ARGB
	AEGfxTriAdd(
		-2.5f, -2.5f, 0x00FF00FF, 0.0f, 1.0f,
		2.5f, -2.5f, 0x00FFFF00, 1.0f, 1.0f,
		-2.5f, 2.5f, 0x00F00FFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		2.5f, -2.5f, 0x00FFFFFF, 1.0f, 1.0f,
		2.5f, 2.5f, 0x00FFFFFF, 1.0f, 0.0f,
		-2.5f, 2.5f, 0x00FFFFFF, 0.0f, 0.0f);

	pShape->mpMesh = AEGfxMeshEnd();

	p1Tex = AEGfxTextureLoad("1.png");
	AE_ASSERT_MESG(p1Tex, "Failed to create texture1!!");

	/////////////////////////////////////////////////////////////////////

	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_MAP;

	AEGfxMeshStart();

	AEGfxTriAdd(
		-2.5f, -2.5f, 0x00FF00FF, 0.0f, 1.0f,
		2.5f, -2.5f, 0x00FFFF00, 1.0f, 1.0f,
		-2.5f, 2.5f, 0x00F00FFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		2.5f, -2.5f, 0x00FFFFFF, 1.0f, 1.0f,
		2.5f, 2.5f, 0x00FFFFFF, 1.0f, 0.0f,
		-2.5f, 2.5f, 0x00FFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1
	pShape->mpMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pShape->mpMesh, "Failed to create mesh 1!!");

	p11Tex = AEGfxTextureLoad("1-1.png");
	AE_ASSERT_MESG(p11Tex, "Failed to create texture1!!");

	p12Tex = AEGfxTextureLoad("1-2.png");
	AE_ASSERT_MESG(p12Tex, "Failed to create texture1!!");

	p13Tex = AEGfxTextureLoad("1-3.png");
	AE_ASSERT_MESG(p13Tex, "Failed to create texture1!!");

	p21Tex = AEGfxTextureLoad("2-1.png");
	AE_ASSERT_MESG(p21Tex, "Failed to create texture1!!");

	p22Tex = AEGfxTextureLoad("2-2.png");
	AE_ASSERT_MESG(p22Tex, "Failed to create texture1!!");

	p23Tex = AEGfxTextureLoad("2-3.png");
	AE_ASSERT_MESG(p23Tex, "Failed to create texture1!!");

	p31Tex = AEGfxTextureLoad("3-1.png");
	AE_ASSERT_MESG(p31Tex, "Failed to create texture1!!");

	p32Tex = AEGfxTextureLoad("3-2.png");
	AE_ASSERT_MESG(p32Tex, "Failed to create texture1!!");

	p33Tex = AEGfxTextureLoad("3-3.png");
	AE_ASSERT_MESG(p33Tex, "Failed to create texture1!!");

	p41Tex = AEGfxTextureLoad("4-1.png");
	AE_ASSERT_MESG(p41Tex, "Failed to create texture1!!");

	p42Tex = AEGfxTextureLoad("4-2.png");
	AE_ASSERT_MESG(p42Tex, "Failed to create texture1!!");

	p43Tex = AEGfxTextureLoad("4-3.png");
	AE_ASSERT_MESG(p43Tex, "Failed to create texture1!!");

	//create CREDITS
	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_CREDITS;

	AEGfxMeshStart();

	AEGfxTriAdd(
		-2.5f, -2.5f, 0x00FF00FF, 0.0f, 1.0f,
		2.5f, -2.5f, 0x00FFFF00, 1.0f, 1.0f,
		-2.5f, 2.5f, 0x00F00FFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		2.5f, -2.5f, 0x00FFFFFF, 1.0f, 1.0f,
		2.5f, 2.5f, 0x00FFFFFF, 1.0f, 0.0f,
		-2.5f, 2.5f, 0x00FFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1
	pShape->mpMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pShape->mpMesh, "Failed to create mesh 1!!");

	pCreditsTex = AEGfxTextureLoad("credits.png");
	AE_ASSERT_MESG(pCreditsTex, "Failed to create texture1!!");

//////////////////////////////////////////////////////////////////////////

	//create CREDITS
	pShape = sgShapes + sgShapeNum++;
	pShape->mType = OBJECT_TYPE_WIN;

	AEGfxMeshStart();

	AEGfxTriAdd(
		-2.5f, -2.5f, 0x00FF00FF, 0.0f, 1.0f,
		2.5f, -2.5f, 0x00FFFF00, 1.0f, 1.0f,
		-2.5f, 2.5f, 0x00F00FFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		2.5f, -2.5f, 0x00FFFFFF, 1.0f, 1.0f,
		2.5f, 2.5f, 0x00FFFFFF, 1.0f, 0.0f,
		-2.5f, 2.5f, 0x00FFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1
	pShape->mpMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pShape->mpMesh, "Failed to create mesh 1!!");

	pWinTex = AEGfxTextureLoad("winscreen.png");
	AE_ASSERT_MESG(pWinTex, "Failed to create texture1!!");

	// Draw enemy shapes
	InitializeEnemyShapes();

	//FMOD initialization
	FMOD_System_Create(&FmodSystem);
	FMOD_System_GetVersion(FmodSystem, &version);
	FMOD_System_Init(FmodSystem, 32, FMOD_INIT_NORMAL, NULL);

	FMOD_System_CreateSound(FmodSystem, "shot.wav", FMOD_DEFAULT, 0, &sound1);
	FMOD_System_CreateSound(FmodSystem, "pacmandeath.wav", FMOD_DEFAULT, 0, &sound2);
	FMOD_System_CreateSound(FmodSystem, "Background.wav", FMOD_DEFAULT, 0, &background);
	FMOD_System_CreateSound(FmodSystem, "World2.wav", FMOD_DEFAULT, 0, &background2);
	FMOD_Channel_SetVolume(quietChannel, 0.0);
	FMOD_Sound_SetMode(background, FMOD_LOOP_NORMAL);
	FMOD_Sound_SetMode(background2, FMOD_LOOP_NORMAL);
	FMOD_System_PlaySound(FmodSystem, background, 0, 0, channel);

	LoadEnemyFMOD();

	switchscreen_timer = ConstructTimer(1.0);

}

// ---------------------------------------------------------------------------

// "Initialize" function of this state
void GameStateAsteroidsInit(void)
{
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// zero the game object instance array
	memset(sgGameObjectInstanceList, 0, sizeof(GameObjectInstance)* GAME_OBJ_INST_NUM_MAX);
	// No game object instances (sprites) at this point
	sgGameObjectInstanceNum = 0;

	// Pass pointer to ship and duration of firing delay
	sgpShip = InitializeShip();
	sgpShip->InputFlag = 1;
	sgpShip->ParticleFlag = 0; //0 auto fire off, 1 auto fire on

	GameObjectInstance *shadow = GameObjectInstanceCreate(OBJECT_TYPE_SHIP_FLASH);
	AddToObject(shadow, sgpShip->mpComponent_Transform->mPosition.x, sgpShip->mpComponent_Transform->mPosition.y, sgpShip->mpComponent_Physics->mVelocity.x,
		sgpShip->mpComponent_Physics->mVelocity.y, sgpShip->mpComponent_Transform->mScaleX, sgpShip->mpComponent_Transform->mScaleY, sgpShip->mpComponent_Transform->mAngle);

	// Initialize score conditions
	EnemiesKilled = 0;
	BossesKilled = 0;

	splash = 0;

	switch (CurrentLevel)
	{
			// Level 1 Initialize ( Ship and Timers etc )
		case GS_ASTEROIDS_LEVEL1:
		{
			sgpShip->fireTimer = 0.95;
			// Allocate the array of enemies
			enemyInstanceList = LoadEnemiesFromLevel(enemyInstanceList, 5000, CurrentLevel, &enemyInstanceNum);

			// Allocate the array of spawn points
			spawnPointList = LoadEnemySpawnsFromLevel(spawnPointList, CurrentLevel);

			// Initialize those spawn points with the beginning of the enemy array
			UpdateSpawnPointEnemies(spawnPointList, enemyInstanceList, &enemyInstanceNum);

			red = GameObjectInstanceCreate(OBJECT_TYPE_RED_BOX);
			AddToObject(red, 0, 0, 0, 0, 5, 5, 0);
			red->timer = 1.0f;
			red->mFlag = 0;

			yellow = GameObjectInstanceCreate(OBJECT_TYPE_YELLOW_BOX);
			AddToObject(yellow, 0, 0, 0, 0, 5, 5, 0);
			yellow->timer = 1.0f;
			yellow->mFlag = 0;

			green = GameObjectInstanceCreate(OBJECT_TYPE_GREEN_BOX);
			AddToObject(green, 0, 0, 0, 0, 5, 5, 0);
			green->timer = 1.0f;
			green->mFlag = 0;

			CreateHUDObjects();
			break;
		}

			// Level 2 Initialize ( Ship and Timers etc )
		case GS_ASTEROIDS_LEVEL2:
		{
			sgpShip->fireTimer = 0.9;
			enemyInstanceList = LoadEnemiesFromLevel(enemyInstanceList, 5000, CurrentLevel, &enemyInstanceNum);
			spawnPointList = LoadEnemySpawnsFromLevel(spawnPointList, CurrentLevel);
			UpdateSpawnPointEnemies(spawnPointList, enemyInstanceList, &enemyInstanceNum);

			red = GameObjectInstanceCreate(OBJECT_TYPE_RED_BOX);
			AddToObject(red, 0, 0, 0, 0, 5, 5, 0);
			red->timer = 1.0f;
			red->mFlag = 0;

			yellow = GameObjectInstanceCreate(OBJECT_TYPE_YELLOW_BOX);
			AddToObject(yellow, 0, 0, 0, 0, 5, 5, 0);
			yellow->timer = 1.0f;
			yellow->mFlag = 0;

			green = GameObjectInstanceCreate(OBJECT_TYPE_GREEN_BOX);
			AddToObject(green, 0, 0, 0, 0, 5, 5, 0);
			green->timer = 1.0f;
			green->mFlag = 0;


			CreateHUDObjects();
			break;
		}

		// Level 3 Initialize ( Ship and Timers etc )
		case GS_ASTEROIDS_LEVEL3:
		{
			sgpShip->fireTimer = 0.85;
			enemyInstanceList = LoadEnemiesFromLevel(enemyInstanceList, 5000, CurrentLevel, &enemyInstanceNum);
			spawnPointList = LoadEnemySpawnsFromLevel(spawnPointList, CurrentLevel);
			UpdateSpawnPointEnemies(spawnPointList, enemyInstanceList, &enemyInstanceNum);

			red = GameObjectInstanceCreate(OBJECT_TYPE_RED_BOX);
			AddToObject(red, 0, 0, 0, 0, 5, 5, 0);
			red->timer = 1.0f;
			red->mFlag = 0;

			yellow = GameObjectInstanceCreate(OBJECT_TYPE_YELLOW_BOX);
			AddToObject(yellow, 0, 0, 0, 0, 5, 5, 0);
			yellow->timer = 1.0f;
			yellow->mFlag = 0;

			green = GameObjectInstanceCreate(OBJECT_TYPE_GREEN_BOX);
			AddToObject(green, 0, 0, 0, 0, 5, 5, 0);
			green->timer = 1.0f;
			green->mFlag = 0;

			// Create the Boss
			pBoss = GameObjectInstanceCreate(OBJECT_TYPE_BOSS);
			Vector2DSet(&pBoss->mpComponent_Transform->mPosition, 0.0f, 330.0f);
			pBoss->timer = 0.25f;
			pBoss->bossTimer = 1.0f;
			pBoss->bossHealth = 30;
			pBoss->ParticleFlag = 0; //initial state

			GameObjectInstance *bossshadow = GameObjectInstanceCreate(OBJECT_TYPE_BOSS_FLASH);
			AddToObject(bossshadow, pBoss->mpComponent_Transform->mPosition.x, pBoss->mpComponent_Transform->mPosition.y, pBoss->mpComponent_Physics->mVelocity.x,
				pBoss->mpComponent_Physics->mVelocity.y, pBoss->mpComponent_Transform->mScaleX, pBoss->mpComponent_Transform->mScaleY, pBoss->mpComponent_Transform->mAngle);

									 // Set to start of pattern
			pBoss->stateFlag = SPRINKLER_SHOTS;
			pBoss->innerState = ON_ENTER;

			CreateHUDObjects();
			break;
		}

		// Level 2 Initialize ( Ship and Timers etc )
		case GS_ASTEROIDS_LEVEL4:
		{
			sgpShip->fireTimer = 0.8;
			FMOD_System_PlaySound(FmodSystem, background2, 0, 0, &channel2);
			enemyInstanceList = LoadEnemiesFromLevel(enemyInstanceList, 5000, CurrentLevel, &enemyInstanceNum);
			spawnPointList = LoadEnemySpawnsFromLevel(spawnPointList, CurrentLevel);
			UpdateSpawnPointEnemies(spawnPointList, enemyInstanceList, &enemyInstanceNum);

			red = GameObjectInstanceCreate(OBJECT_TYPE_RED_BOX);
			AddToObject(red, 0, 0, 0, 0, 5, 5, 0);
			red->timer = 1.0f;
			red->mFlag = 0;

			yellow = GameObjectInstanceCreate(OBJECT_TYPE_YELLOW_BOX);
			AddToObject(yellow, 0, 0, 0, 0, 5, 5, 0);
			yellow->timer = 1.0f;
			yellow->mFlag = 0;

			green = GameObjectInstanceCreate(OBJECT_TYPE_GREEN_BOX);
			AddToObject(green, 0, 0, 0, 0, 5, 5, 0);
			green->timer = 1.0f;
			green->mFlag = 0;


			CreateHUDObjects();
			break;
		}

		// Level 2 Initialize ( Ship and Timers etc )
		case GS_ASTEROIDS_LEVEL5:
		{
			sgpShip->fireTimer = 0.75;
			enemyInstanceList = LoadEnemiesFromLevel(enemyInstanceList, 5000, CurrentLevel, &enemyInstanceNum);
			spawnPointList = LoadEnemySpawnsFromLevel(spawnPointList, CurrentLevel);
			UpdateSpawnPointEnemies(spawnPointList, enemyInstanceList, &enemyInstanceNum);

			red = GameObjectInstanceCreate(OBJECT_TYPE_RED_BOX);
			AddToObject(red, 0, 0, 0, 0, 5, 5, 0);
			red->timer = 1.0f;
			red->mFlag = 0;

			yellow = GameObjectInstanceCreate(OBJECT_TYPE_YELLOW_BOX);
			AddToObject(yellow, 0, 0, 0, 0, 5, 5, 0);
			yellow->timer = 1.0f;
			yellow->mFlag = 0;

			green = GameObjectInstanceCreate(OBJECT_TYPE_GREEN_BOX);
			AddToObject(green, 0, 0, 0, 0, 5, 5, 0);
			green->timer = 1.0f;
			green->mFlag = 0;


			CreateHUDObjects();
			break;
		}

		// Level 3 Initialize ( Ship and Timers etc )
		case GS_ASTEROIDS_LEVEL6:
		{
			sgpShip->fireTimer = 0.7;
			enemyInstanceList = LoadEnemiesFromLevel(enemyInstanceList, 5000, CurrentLevel, &enemyInstanceNum);
			spawnPointList = LoadEnemySpawnsFromLevel(spawnPointList, CurrentLevel);
			UpdateSpawnPointEnemies(spawnPointList, enemyInstanceList, &enemyInstanceNum);

			red = GameObjectInstanceCreate(OBJECT_TYPE_RED_BOX);
			AddToObject(red, 0, 0, 0, 0, 5, 5, 0);
			red->timer = 1.0f;
			red->mFlag = 0;

			yellow = GameObjectInstanceCreate(OBJECT_TYPE_YELLOW_BOX);
			AddToObject(yellow, 0, 0, 0, 0, 5, 5, 0);
			yellow->timer = 1.0f;
			yellow->mFlag = 0;

			green = GameObjectInstanceCreate(OBJECT_TYPE_GREEN_BOX);
			AddToObject(green, 0, 0, 0, 0, 5, 5, 0);
			green->timer = 1.0f;
			green->mFlag = 0;

			// Create the Boss
			pBoss = GameObjectInstanceCreate(OBJECT_TYPE_BOSS);
			Vector2DSet(&pBoss->mpComponent_Transform->mPosition, 0.0f, 330.0f);
			pBoss->timer = 0.25f;
			pBoss->bossTimer = 1.0f;
			pBoss->bossHealth = 35;
			pBoss->ParticleFlag = 0; //initial state

			GameObjectInstance *bossshadow = GameObjectInstanceCreate(OBJECT_TYPE_BOSS_FLASH);
			AddToObject(bossshadow, pBoss->mpComponent_Transform->mPosition.x, pBoss->mpComponent_Transform->mPosition.y, pBoss->mpComponent_Physics->mVelocity.x,
				pBoss->mpComponent_Physics->mVelocity.y, pBoss->mpComponent_Transform->mScaleX, pBoss->mpComponent_Transform->mScaleY, pBoss->mpComponent_Transform->mAngle);

			// Set to start of pattern
			pBoss->stateFlag = SPRINKLER_SHOTS;
			pBoss->innerState = ON_ENTER;

			CreateHUDObjects();
			break;
		}

		// Level 2 Initialize ( Ship and Timers etc )
		case GS_ASTEROIDS_LEVEL7:
		{
			sgpShip->fireTimer = 0.65;
			enemyInstanceList = LoadEnemiesFromLevel(enemyInstanceList, 5000, CurrentLevel, &enemyInstanceNum);
			spawnPointList = LoadEnemySpawnsFromLevel(spawnPointList, CurrentLevel);
			UpdateSpawnPointEnemies(spawnPointList, enemyInstanceList, &enemyInstanceNum);

			red = GameObjectInstanceCreate(OBJECT_TYPE_RED_BOX);
			AddToObject(red, 0, 0, 0, 0, 5, 5, 0);
			red->timer = 1.0f;
			red->mFlag = 0;

			yellow = GameObjectInstanceCreate(OBJECT_TYPE_YELLOW_BOX);
			AddToObject(yellow, 0, 0, 0, 0, 5, 5, 0);
			yellow->timer = 1.0f;
			yellow->mFlag = 0;

			green = GameObjectInstanceCreate(OBJECT_TYPE_GREEN_BOX);
			AddToObject(green, 0, 0, 0, 0, 5, 5, 0);
			green->timer = 1.0f;
			green->mFlag = 0;


			CreateHUDObjects();
			break;
		}

		// Level 2 Initialize ( Ship and Timers etc )
		case GS_ASTEROIDS_LEVEL8:
		{
			sgpShip->fireTimer = 0.6;
			enemyInstanceList = LoadEnemiesFromLevel(enemyInstanceList, 5000, CurrentLevel, &enemyInstanceNum);
			spawnPointList = LoadEnemySpawnsFromLevel(spawnPointList, CurrentLevel);
			UpdateSpawnPointEnemies(spawnPointList, enemyInstanceList, &enemyInstanceNum);

			red = GameObjectInstanceCreate(OBJECT_TYPE_RED_BOX);
			AddToObject(red, 0, 0, 0, 0, 5, 5, 0);
			red->timer = 1.0f;
			red->mFlag = 0;

			yellow = GameObjectInstanceCreate(OBJECT_TYPE_YELLOW_BOX);
			AddToObject(yellow, 0, 0, 0, 0, 5, 5, 0);
			yellow->timer = 1.0f;
			yellow->mFlag = 0;

			green = GameObjectInstanceCreate(OBJECT_TYPE_GREEN_BOX);
			AddToObject(green, 0, 0, 0, 0, 5, 5, 0);
			green->timer = 1.0f;
			green->mFlag = 0;


			CreateHUDObjects();
			break;
		}

		// Level 3 Initialize ( Ship and Timers etc )
		case GS_ASTEROIDS_LEVEL9:
		{
			sgpShip->fireTimer = 0.55;
			enemyInstanceList = LoadEnemiesFromLevel(enemyInstanceList, 5000, CurrentLevel, &enemyInstanceNum);
			spawnPointList = LoadEnemySpawnsFromLevel(spawnPointList, CurrentLevel);
			UpdateSpawnPointEnemies(spawnPointList, enemyInstanceList, &enemyInstanceNum);

			red = GameObjectInstanceCreate(OBJECT_TYPE_RED_BOX);
			AddToObject(red, 0, 0, 0, 0, 5, 5, 0);
			red->timer = 1.0f;
			red->mFlag = 0;

			yellow = GameObjectInstanceCreate(OBJECT_TYPE_YELLOW_BOX);
			AddToObject(yellow, 0, 0, 0, 0, 5, 5, 0);
			yellow->timer = 1.0f;
			yellow->mFlag = 0;

			green = GameObjectInstanceCreate(OBJECT_TYPE_GREEN_BOX);
			AddToObject(green, 0, 0, 0, 0, 5, 5, 0);
			green->timer = 1.0f;
			green->mFlag = 0;

			// Create the Boss
			pBoss = GameObjectInstanceCreate(OBJECT_TYPE_BOSS);
			Vector2DSet(&pBoss->mpComponent_Transform->mPosition, 0.0f, 330.0f);
			pBoss->timer = 0.25f;
			pBoss->bossTimer = 1.0f;
			pBoss->bossHealth = 40;
			pBoss->ParticleFlag = 0; //initial state

			GameObjectInstance *bossshadow = GameObjectInstanceCreate(OBJECT_TYPE_BOSS_FLASH);
			AddToObject(bossshadow, pBoss->mpComponent_Transform->mPosition.x, pBoss->mpComponent_Transform->mPosition.y, pBoss->mpComponent_Physics->mVelocity.x,
				pBoss->mpComponent_Physics->mVelocity.y, pBoss->mpComponent_Transform->mScaleX, pBoss->mpComponent_Transform->mScaleY, pBoss->mpComponent_Transform->mAngle);

			// Set to start of pattern
			pBoss->stateFlag = SPRINKLER_SHOTS;
			pBoss->innerState = ON_ENTER;

			CreateHUDObjects();
			break;
		}

		// Level 2 Initialize ( Ship and Timers etc )
		case GS_ASTEROIDS_LEVEL10:
		{
			sgpShip->fireTimer = 0.5;
			enemyInstanceList = LoadEnemiesFromLevel(enemyInstanceList, 5000, CurrentLevel, &enemyInstanceNum);
			spawnPointList = LoadEnemySpawnsFromLevel(spawnPointList, CurrentLevel);
			UpdateSpawnPointEnemies(spawnPointList, enemyInstanceList, &enemyInstanceNum);

			red = GameObjectInstanceCreate(OBJECT_TYPE_RED_BOX);
			AddToObject(red, 0, 0, 0, 0, 5, 5, 0);
			red->timer = 1.0f;
			red->mFlag = 0;

			yellow = GameObjectInstanceCreate(OBJECT_TYPE_YELLOW_BOX);
			AddToObject(yellow, 0, 0, 0, 0, 5, 5, 0);
			yellow->timer = 1.0f;
			yellow->mFlag = 0;

			green = GameObjectInstanceCreate(OBJECT_TYPE_GREEN_BOX);
			AddToObject(green, 0, 0, 0, 0, 5, 5, 0);
			green->timer = 1.0f;
			green->mFlag = 0;


			CreateHUDObjects();
			break;
		}

		// Level 2 Initialize ( Ship and Timers etc )
		case GS_ASTEROIDS_LEVEL11:
		{
			sgpShip->fireTimer = 0.45;
			enemyInstanceList = LoadEnemiesFromLevel(enemyInstanceList, 5000, CurrentLevel, &enemyInstanceNum);
			spawnPointList = LoadEnemySpawnsFromLevel(spawnPointList, CurrentLevel);
			UpdateSpawnPointEnemies(spawnPointList, enemyInstanceList, &enemyInstanceNum);

			red = GameObjectInstanceCreate(OBJECT_TYPE_RED_BOX);
			AddToObject(red, 0, 0, 0, 0, 5, 5, 0);
			red->timer = 1.0f;
			red->mFlag = 0;

			yellow = GameObjectInstanceCreate(OBJECT_TYPE_YELLOW_BOX);
			AddToObject(yellow, 0, 0, 0, 0, 5, 5, 0);
			yellow->timer = 1.0f;
			yellow->mFlag = 0;

			green = GameObjectInstanceCreate(OBJECT_TYPE_GREEN_BOX);
			AddToObject(green, 0, 0, 0, 0, 5, 5, 0);
			green->timer = 1.0f;
			green->mFlag = 0;


			CreateHUDObjects();
			break;
		}

		// Level 3 Initialize ( Ship and Timers etc )
		case GS_ASTEROIDS_LEVEL12:
		{
			sgpShip->fireTimer = 0.4;
			enemyInstanceList = LoadEnemiesFromLevel(enemyInstanceList, 5000, CurrentLevel, &enemyInstanceNum);
			spawnPointList = LoadEnemySpawnsFromLevel(spawnPointList, CurrentLevel);
			UpdateSpawnPointEnemies(spawnPointList, enemyInstanceList, &enemyInstanceNum);

			red = GameObjectInstanceCreate(OBJECT_TYPE_RED_BOX);
			AddToObject(red, 0, 0, 0, 0, 5, 5, 0);
			red->timer = 1.0f;
			red->mFlag = 0;

			yellow = GameObjectInstanceCreate(OBJECT_TYPE_YELLOW_BOX);
			AddToObject(yellow, 0, 0, 0, 0, 5, 5, 0);
			yellow->timer = 1.0f;
			yellow->mFlag = 0;

			green = GameObjectInstanceCreate(OBJECT_TYPE_GREEN_BOX);
			AddToObject(green, 0, 0, 0, 0, 5, 5, 0);
			green->timer = 1.0f;
			green->mFlag = 0;

			// Create the Boss
			pBoss = GameObjectInstanceCreate(OBJECT_TYPE_BOSS);
			Vector2DSet(&pBoss->mpComponent_Transform->mPosition, 0.0f, 330.0f);
			pBoss->timer = 0.25f;
			pBoss->bossTimer = 1.0f;
			pBoss->bossHealth = 400;
			pBoss->ParticleFlag = 0; //initial state

			GameObjectInstance *bossshadow = GameObjectInstanceCreate(OBJECT_TYPE_BOSS_FLASH);
			AddToObject(bossshadow, pBoss->mpComponent_Transform->mPosition.x, pBoss->mpComponent_Transform->mPosition.y, pBoss->mpComponent_Physics->mVelocity.x,
				pBoss->mpComponent_Physics->mVelocity.y, pBoss->mpComponent_Transform->mScaleX, pBoss->mpComponent_Transform->mScaleY, pBoss->mpComponent_Transform->mAngle);

			// Set to start of pattern
			pBoss->stateFlag = FINAL_BOSS;
			pBoss->innerState = ON_ENTER;

			CreateHUDObjects();
			break;
		}

			// Start Menu Initialize ( Buttons )
		case GS_ASTEROIDS_START_SCREEN:
		{
			// Ready start button to be created
			pButton1 = 0;
			pButton2 = 0;
			pButton3 = 0;

			pButton1 = GameObjectInstanceCreate(OBJECT_TYPE_START_BUTTON);
			AddToObject(pButton1, -400, 100, 0, 0, 50, 25, 0);
			pButton1->buttonID = BUTTON_ID_START;

			pButton2 = GameObjectInstanceCreate(OBJECT_TYPE_EXIT_BUTTON);
			AddToObject(pButton2, 400, 100, 0, 0, 50, 25, 0);
			pButton2->buttonID = BUTTON_ID_EXIT;

			pButton3 = GameObjectInstanceCreate(OBJECT_TYPE_CREDITS_BUTTON);
			AddToObject(pButton3, 0, 100, 0, 0, 50, 25, 0);
			pButton3->buttonID = BUTTON_ID_CREDITS;

			GameObjectInstance *pLogo = GameObjectInstanceCreate(OBJECT_TYPE_LOGO);
			AddToObject(pLogo, 0, 375, 0, 0, 250.0f, 100.0f, 0);

			// Prevent Shooting
			SetTimerDuration(fire, 9999.0);
			break;
		}

		case GS_ASTEROIDS_SPLASH_SCREEN:
		{
			GameObjectInstanceCreate(OBJECT_TYPE_DIGIPEN_SPLASH);
			splash = ConstructTimer(5.07);
			break;
		}

		case GS_ASTEROIDS_CREDITS_SCREEN:
		{
			GameObjectInstanceCreate(OBJECT_TYPE_CREDITS);
			switchscreen_timer->current_time = 0.0;
			SetTimerDuration(fire, 9999.0);
			break;
		}

		case GS_ASTEROIDS_WIN_SCREEN:
		{
			GameObjectInstanceCreate(OBJECT_TYPE_WIN);
			switchscreen_timer->current_time = 0.0;
			SetTimerDuration(fire, 9999.0);
			break;
		}
	}

	fire->duration = sgpShip->fireTimer;
	fire->current_time = 0.0;

	CreateStars();
}

// ---------------------------------------------------------------------------

// "Update" function of this state
void GameStateAsteroidsUpdate(void)
{
	int i; //loop variable
		   //control ship movement/controls

	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjectInstance *shadow = sgGameObjectInstanceList + i;
		if ((shadow->mFlag & FLAG_ACTIVE) == 0)
			continue;
		if (shadow->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_SHIP_FLASH)
		{
			shadow->mpComponent_Transform->mPosition.x = sgpShip->mpComponent_Transform->mPosition.x;
			shadow->mpComponent_Transform->mPosition.y = sgpShip->mpComponent_Transform->mPosition.y;
		}
	}
	if (pBoss)
	{
		for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
		{
			GameObjectInstance *bossshadow = sgGameObjectInstanceList + i;
			if ((bossshadow->mFlag & FLAG_ACTIVE) == 0)
				continue;
			if (bossshadow->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_BOSS_FLASH)
			{
				bossshadow->mpComponent_Transform->mPosition.x = pBoss->mpComponent_Transform->mPosition.x;
				bossshadow->mpComponent_Transform->mPosition.y = pBoss->mpComponent_Transform->mPosition.y;
			}
		}

		if (pBoss->invinceTime > 0)
			pBoss->invinceTime -= frameTime;
	}

	animation_timer--;

	if (animation_timer <= 0)
	{
		++currentTextureOffset;			//go the the next frame in the animation

		if (currentTextureOffset >= 4)	//if we are on the final frame of the animation...
			currentTextureOffset = 0;	//...loop back to the first frame

		animation_timer = 3.5f;
	}

	boss_timer--;

	if (boss_timer <= 0)
	{
		++currentBossOffset;			//go the the next frame in the animation

		if (currentBossOffset >= 8)	//if we are on the final frame of the animation...
			currentBossOffset = 0;	//...loop back to the first frame

		boss_timer = 8.5f;
	}

	// Getting the window's world edges (These changes whenever the camera moves or zooms in/out)
	winMaxX = (AEGfxGetWinMaxX() / 2) + 100;
	winMaxY = AEGfxGetWinMaxY();
	winMinX = (AEGfxGetWinMinX() / 2) - 100;
	winMinY = AEGfxGetWinMinY();

		// Getting the frame time
	frameTime = AEFrameRateControllerGetFrameTime();

		// If the ship was created, handle priority tasks
	if (sgpShip != 0)
		Invincibility();

		// Update according to input
	HandleInput();

	UpdateTimer(switchscreen_timer);

	switch (CurrentLevel)
	{
	case GS_ASTEROIDS_LEVEL1:

		if (sgpShip->InputFlag)
		{
			// Update neccessary spawn points
			UpdateSpawnPointEnemies(spawnPointList, enemyInstanceList, &enemyInstanceNum);

			// Update enemy positions based off their velocity/behavior

			UpdateSpawnedEnemyPositions(spawnPointList, frameTime);

			// Update enemy behaviors and then collisions
			UpdateSpawnedEnemyBehaviours(spawnPointList);
			UpdateSpawnedEnemyCollisions(spawnPointList, sgGameObjectInstanceList, &enemyInstanceNum);

			// Enemy Matrices
			UpdateEnemyMatrices(spawnPointList);
		}

		if (EnemiesKilled == 25)
			ChangeLevel(GS_ASTEROIDS_LEVEL2);

		break;

	case GS_ASTEROIDS_LEVEL2:

		if (sgpShip->InputFlag)
		{
			// Update neccessary spawn points
			UpdateSpawnPointEnemies(spawnPointList, enemyInstanceList, &enemyInstanceNum);

			// Update enemy positions based off their velocity/behavior
			UpdateSpawnedEnemyPositions(spawnPointList, frameTime);

			// Update enemy behaviors and then collisions
			UpdateSpawnedEnemyBehaviours(spawnPointList);
			UpdateSpawnedEnemyCollisions(spawnPointList, sgGameObjectInstanceList, &enemyInstanceNum);

			// Enemy Matrices
			UpdateEnemyMatrices(spawnPointList);
		}
		if (EnemiesKilled == 40)
			ChangeLevel(GS_ASTEROIDS_LEVEL3);

		break;

	case GS_ASTEROIDS_LEVEL3:
		if (sgpShip->InputFlag)
		{
			if (pBoss != 0)
				BossWeaponMachine(pBoss);

			// Update neccessary spawn points
			UpdateSpawnPointEnemies(spawnPointList, enemyInstanceList, &enemyInstanceNum);

			// Update enemy positions based off their velocity/behavior
			UpdateSpawnedEnemyPositions(spawnPointList, frameTime);

			// Update enemy behaviors and then collisions
			UpdateSpawnedEnemyBehaviours(spawnPointList);
			UpdateSpawnedEnemyCollisions(spawnPointList, sgGameObjectInstanceList, &enemyInstanceNum);

			// Enemy Matrices
			UpdateEnemyMatrices(spawnPointList);
		}
		if (BossesKilled == 1)
			ChangeLevel(GS_ASTEROIDS_LEVEL4);

		break;

	case GS_ASTEROIDS_LEVEL4:

		if (sgpShip->InputFlag)
		{
			// Update neccessary spawn points
			UpdateSpawnPointEnemies(spawnPointList, enemyInstanceList, &enemyInstanceNum);

			// Update enemy positions based off their velocity/behavior
			UpdateSpawnedEnemyPositions(spawnPointList, frameTime);

			// Update enemy behaviors and then collisions
			UpdateSpawnedEnemyBehaviours(spawnPointList);
			UpdateSpawnedEnemyCollisions(spawnPointList, sgGameObjectInstanceList, &enemyInstanceNum);

			// Enemy Matrices
			UpdateEnemyMatrices(spawnPointList);
		}
		if (EnemiesKilled == 50)
			ChangeLevel(GS_ASTEROIDS_LEVEL5);

		break;

	case GS_ASTEROIDS_LEVEL5:

		if (sgpShip->InputFlag)
		{
			// Update neccessary spawn points
			UpdateSpawnPointEnemies(spawnPointList, enemyInstanceList, &enemyInstanceNum);

			// Update enemy positions based off their velocity/behavior
			UpdateSpawnedEnemyPositions(spawnPointList, frameTime);

			// Update enemy behaviors and then collisions
			UpdateSpawnedEnemyBehaviours(spawnPointList);
			UpdateSpawnedEnemyCollisions(spawnPointList, sgGameObjectInstanceList, &enemyInstanceNum);

			// Enemy Matrices
			UpdateEnemyMatrices(spawnPointList);
		}
		if (EnemiesKilled == 75)
			ChangeLevel(GS_ASTEROIDS_LEVEL6);

		break;
		
	case GS_ASTEROIDS_LEVEL6:
		if (sgpShip->InputFlag)
		{
			if (pBoss != 0)
				BossWeaponMachine(pBoss);

			// Update neccessary spawn points
			UpdateSpawnPointEnemies(spawnPointList, enemyInstanceList, &enemyInstanceNum);

			// Update enemy positions based off their velocity/behavior
			UpdateSpawnedEnemyPositions(spawnPointList, frameTime);

			// Update enemy behaviors and then collisions
			UpdateSpawnedEnemyBehaviours(spawnPointList);
			UpdateSpawnedEnemyCollisions(spawnPointList, sgGameObjectInstanceList, &enemyInstanceNum);

			// Enemy Matrices
			UpdateEnemyMatrices(spawnPointList);
		}
		if (BossesKilled == 1)
			ChangeLevel(GS_ASTEROIDS_LEVEL4);

		break;

	case GS_ASTEROIDS_LEVEL7:

		if (sgpShip->InputFlag)
		{
			// Update neccessary spawn points
			UpdateSpawnPointEnemies(spawnPointList, enemyInstanceList, &enemyInstanceNum);

			// Update enemy positions based off their velocity/behavior
			UpdateSpawnedEnemyPositions(spawnPointList, frameTime);

			// Update enemy behaviors and then collisions
			UpdateSpawnedEnemyBehaviours(spawnPointList);
			UpdateSpawnedEnemyCollisions(spawnPointList, sgGameObjectInstanceList, &enemyInstanceNum);

			// Enemy Matrices
			UpdateEnemyMatrices(spawnPointList);
		}
		if (EnemiesKilled == 75)
			ChangeLevel(GS_ASTEROIDS_LEVEL8);

		break;

	case GS_ASTEROIDS_LEVEL8:

		if (sgpShip->InputFlag)
		{
			// Update neccessary spawn points
			UpdateSpawnPointEnemies(spawnPointList, enemyInstanceList, &enemyInstanceNum);

			// Update enemy positions based off their velocity/behavior
			UpdateSpawnedEnemyPositions(spawnPointList, frameTime);

			// Update enemy behaviors and then collisions
			UpdateSpawnedEnemyBehaviours(spawnPointList);
			UpdateSpawnedEnemyCollisions(spawnPointList, sgGameObjectInstanceList, &enemyInstanceNum);

			// Enemy Matrices
			UpdateEnemyMatrices(spawnPointList);
		}
		if (EnemiesKilled == 75)
			ChangeLevel(GS_ASTEROIDS_LEVEL9);

		break;

	case GS_ASTEROIDS_LEVEL9:
		if (sgpShip->InputFlag)
		{
			if (pBoss != 0)
				BossWeaponMachine(pBoss);

			// Update neccessary spawn points
			UpdateSpawnPointEnemies(spawnPointList, enemyInstanceList, &enemyInstanceNum);

			// Update enemy positions based off their velocity/behavior
			UpdateSpawnedEnemyPositions(spawnPointList, frameTime);

			// Update enemy behaviors and then collisions
			UpdateSpawnedEnemyBehaviours(spawnPointList);
			UpdateSpawnedEnemyCollisions(spawnPointList, sgGameObjectInstanceList, &enemyInstanceNum);

			// Enemy Matrices
			UpdateEnemyMatrices(spawnPointList);
		}
		if (BossesKilled == 1)
			ChangeLevel(GS_ASTEROIDS_LEVEL10);

		break;

	case GS_ASTEROIDS_LEVEL10:

		if (sgpShip->InputFlag)
		{
			// Update neccessary spawn points
			UpdateSpawnPointEnemies(spawnPointList, enemyInstanceList, &enemyInstanceNum);

			// Update enemy positions based off their velocity/behavior
			UpdateSpawnedEnemyPositions(spawnPointList, frameTime);

			// Update enemy behaviors and then collisions
			UpdateSpawnedEnemyBehaviours(spawnPointList);
			UpdateSpawnedEnemyCollisions(spawnPointList, sgGameObjectInstanceList, &enemyInstanceNum);

			// Enemy Matrices
			UpdateEnemyMatrices(spawnPointList);
		}
		if (EnemiesKilled == 100)
			ChangeLevel(GS_ASTEROIDS_LEVEL11);

		break;

	case GS_ASTEROIDS_LEVEL11:

		if (sgpShip->InputFlag)
		{
			// Update neccessary spawn points
			UpdateSpawnPointEnemies(spawnPointList, enemyInstanceList, &enemyInstanceNum);

			// Update enemy positions based off their velocity/behavior
			UpdateSpawnedEnemyPositions(spawnPointList, frameTime);

			// Update enemy behaviors and then collisions
			UpdateSpawnedEnemyBehaviours(spawnPointList);
			UpdateSpawnedEnemyCollisions(spawnPointList, sgGameObjectInstanceList, &enemyInstanceNum);

			// Enemy Matrices
			UpdateEnemyMatrices(spawnPointList);
		}
		if (EnemiesKilled == 125)
			ChangeLevel(GS_ASTEROIDS_LEVEL12);

		break;

	case GS_ASTEROIDS_LEVEL12:
		if (sgpShip->InputFlag)
		{
			if (pBoss != 0)
				BossWeaponMachine(pBoss);

			// Update neccessary spawn points
			UpdateSpawnPointEnemies(spawnPointList, enemyInstanceList, &enemyInstanceNum);

			// Update enemy positions based off their velocity/behavior
			UpdateSpawnedEnemyPositions(spawnPointList, frameTime);

			// Update enemy behaviors and then collisions
			UpdateSpawnedEnemyBehaviours(spawnPointList);
			UpdateSpawnedEnemyCollisions(spawnPointList, sgGameObjectInstanceList, &enemyInstanceNum);

			// Enemy Matrices
			UpdateEnemyMatrices(spawnPointList);
		}
		if (BossesKilled == 1)
			ChangeLevel(GS_ASTEROIDS_WIN_SCREEN);

		break;



	case GS_ASTEROIDS_START_SCREEN:
		break;

	case GS_ASTEROIDS_SPLASH_SCREEN:
		UpdateTimer(splash);

		if (splash->doneCounting)
		{
			changeLevelFlag = 1;
			NextLevel = GS_ASTEROIDS_START_SCREEN;
		}
		break;

	case GS_ASTEROIDS_CREDITS_SCREEN:
		if (AEInputCheckTriggered(VK_SPACE) && switchscreen_timer->doneCounting == DONE_COUNTING)
		{
			ChangeLevel(GS_ASTEROIDS_START_SCREEN);
			ResetTimer(switchscreen_timer);
		}
		break;

	case GS_ASTEROIDS_WIN_SCREEN:
		if (AEInputCheckTriggered(VK_SPACE) && switchscreen_timer->doneCounting == DONE_COUNTING)
		{
			ChangeLevel(GS_ASTEROIDS_CREDITS_SCREEN);
			ResetTimer(switchscreen_timer);
		}
		break;
	}


	// Update object positions
	UpdateObjectPositions(sgGameObjectInstanceList, frameTime);

	// Specify object behaviors and then general collisions
	UpdateObjectBehaviours(sgGameObjectInstanceList);
	UpdateObjectCollisions(sgGameObjectInstanceList);

	// Calculate/Update Object Matrices
	UpdateObjectMatrices(sgGameObjectInstanceList);
}

// ---------------------------------------------------------------------------

void GameStateAsteroidsDraw(void)
{
	if (sgpShip->invinceTime > 0)
		shipDraw = 1;
	else
		shipDraw = 0;

		// Reveal the Stars
	DrawColor(OBJECT_TYPE_STAR1);
	DrawColor(OBJECT_TYPE_STAR2);
	DrawColor(OBJECT_TYPE_STAR3);

	DrawTexture(OBJECT_TYPE_BULLET, pPlayerBulletTex);

	if (shipDraw)
	{
		if (drawDelay > 0)
		{
			drawDelay--;
			// Reveal the ship trail
			DrawTrail();
			// Ship's textures
			AnimatePlayer(OBJECT_TYPE_SHIP, pPlayerTex);
		}
		if (drawDelay == 1)
			drawDelay = -5;
		if (drawDelay < 0)
		{
			drawDelay++;
			DrawTexture(OBJECT_TYPE_SHIP_FLASH, pPlayerFlashTex);
		}
		if (drawDelay == -1)
			drawDelay = 5;
	}

	else
	{
		DrawTrail();
		AnimatePlayer(OBJECT_TYPE_SHIP, pPlayerTex);
	}

	switch (CurrentLevel)
	{
		case GS_ASTEROIDS_LEVEL1:
			DrawEnemy();
			DrawColor(OBJECT_TYPE_ENIGMA_SPAWN);

				// Enemy Effects
			DrawColor(OBJECT_TYPE_ENEMY_BULLET);

			DrawColor(OBJECT_TYPE_ENIGMA_DEATHDEBRIS);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHWHITE);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHRED);

			DrawHUDTextures();

			DrawTexture(OBJECT_TYPE_RED_BOX, p3Tex);
			DrawTexture(OBJECT_TYPE_YELLOW_BOX, p2Tex);
			DrawTexture(OBJECT_TYPE_GREEN_BOX, p1Tex);

			break;

		case GS_ASTEROIDS_LEVEL2:

			DrawEnemy();
			DrawColor(OBJECT_TYPE_ENIGMA_SPAWN);

				// Enemy Effects
			DrawColor(OBJECT_TYPE_ENEMY_BULLET);
			DrawColor(OBJECT_TYPE_HOMING_MISSILE);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHDEBRIS);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHWHITE);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHRED);

			DrawHUDTextures();

			break;

		case GS_ASTEROIDS_LEVEL3:

			DrawEnemy();
			DrawColor(OBJECT_TYPE_ENIGMA_SPAWN);
			
			if (pBoss)
			{
				if (pBoss->invinceTime <= 0)
					AnimateBoss(OBJECT_TYPE_BOSS, pBoss1Tex);

				if (pBoss->invinceTime > 0)
					DrawFlash();
			}

			// Enemy Effects
			DrawColor(OBJECT_TYPE_ENEMY_BULLET);
			DrawColor(OBJECT_TYPE_HOMING_MISSILE);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHDEBRIS);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHWHITE);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHRED);

			DrawHUDTextures();

			DrawTexture(OBJECT_TYPE_RED_BOX, p3Tex);
			DrawTexture(OBJECT_TYPE_YELLOW_BOX, p2Tex);
			DrawTexture(OBJECT_TYPE_GREEN_BOX, p1Tex);

			break;

		case GS_ASTEROIDS_LEVEL4:

			DrawEnemy();
			DrawColor(OBJECT_TYPE_ENIGMA_SPAWN);

			// Enemy Effects
			DrawColor(OBJECT_TYPE_ENEMY_BULLET);
			DrawColor(OBJECT_TYPE_HOMING_MISSILE);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHDEBRIS);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHWHITE);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHRED);

			DrawHUDTextures();

			DrawTexture(OBJECT_TYPE_RED_BOX, p3Tex);
			DrawTexture(OBJECT_TYPE_YELLOW_BOX, p2Tex);
			DrawTexture(OBJECT_TYPE_GREEN_BOX, p1Tex);

			break;

		case GS_ASTEROIDS_LEVEL5:

			DrawEnemy();
			DrawColor(OBJECT_TYPE_ENIGMA_SPAWN);

			// Enemy Effects
			DrawColor(OBJECT_TYPE_ENEMY_BULLET);
			DrawColor(OBJECT_TYPE_HOMING_MISSILE);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHDEBRIS);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHWHITE);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHRED);

			DrawHUDTextures();

			DrawTexture(OBJECT_TYPE_RED_BOX, p3Tex);
			DrawTexture(OBJECT_TYPE_YELLOW_BOX, p2Tex);
			DrawTexture(OBJECT_TYPE_GREEN_BOX, p1Tex);

			break;

		case GS_ASTEROIDS_LEVEL6:

			DrawEnemy();
			DrawColor(OBJECT_TYPE_ENIGMA_SPAWN);

			if (pBoss)
			{
				if (pBoss->invinceTime <= 0)
					AnimateBoss(OBJECT_TYPE_BOSS, pBoss2Tex);

				if (pBoss->invinceTime > 0)
					DrawFlash();
			}

			// Enemy Effects
			DrawColor(OBJECT_TYPE_ENEMY_BULLET);
			DrawColor(OBJECT_TYPE_HOMING_MISSILE);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHDEBRIS);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHWHITE);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHRED);

			DrawHUDTextures();

			DrawTexture(OBJECT_TYPE_RED_BOX, p3Tex);
			DrawTexture(OBJECT_TYPE_YELLOW_BOX, p2Tex);
			DrawTexture(OBJECT_TYPE_GREEN_BOX, p1Tex);

			break;

		case GS_ASTEROIDS_LEVEL7:

			DrawEnemy();
			DrawColor(OBJECT_TYPE_ENIGMA_SPAWN);

			// Enemy Effects
			DrawColor(OBJECT_TYPE_ENEMY_BULLET);
			DrawColor(OBJECT_TYPE_HOMING_MISSILE);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHDEBRIS);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHWHITE);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHRED);

			DrawHUDTextures();

			DrawTexture(OBJECT_TYPE_RED_BOX, p3Tex);
			DrawTexture(OBJECT_TYPE_YELLOW_BOX, p2Tex);
			DrawTexture(OBJECT_TYPE_GREEN_BOX, p1Tex);

			break;

		case GS_ASTEROIDS_LEVEL8:

			DrawEnemy();
			DrawColor(OBJECT_TYPE_ENIGMA_SPAWN);

			// Enemy Effects
			DrawColor(OBJECT_TYPE_ENEMY_BULLET);
			DrawColor(OBJECT_TYPE_HOMING_MISSILE);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHDEBRIS);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHWHITE);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHRED);

			DrawHUDTextures();

			DrawTexture(OBJECT_TYPE_RED_BOX, p3Tex);
			DrawTexture(OBJECT_TYPE_YELLOW_BOX, p2Tex);
			DrawTexture(OBJECT_TYPE_GREEN_BOX, p1Tex);

			break;

		case GS_ASTEROIDS_LEVEL9:

			DrawEnemy();
			DrawColor(OBJECT_TYPE_ENIGMA_SPAWN);

			if (pBoss)
			{
				if (pBoss->invinceTime <= 0)
					AnimateBoss(OBJECT_TYPE_BOSS, pBoss3Tex);

				if (pBoss->invinceTime > 0)
					DrawFlash();
			}

			// Enemy Effects
			DrawColor(OBJECT_TYPE_ENEMY_BULLET);
			DrawColor(OBJECT_TYPE_HOMING_MISSILE);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHDEBRIS);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHWHITE);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHRED);

			DrawHUDTextures();

			DrawTexture(OBJECT_TYPE_RED_BOX, p3Tex);
			DrawTexture(OBJECT_TYPE_YELLOW_BOX, p2Tex);
			DrawTexture(OBJECT_TYPE_GREEN_BOX, p1Tex);

			break;

		case GS_ASTEROIDS_LEVEL10:

			DrawEnemy();
			DrawColor(OBJECT_TYPE_ENIGMA_SPAWN);

			// Enemy Effects
			DrawColor(OBJECT_TYPE_ENEMY_BULLET);
			DrawColor(OBJECT_TYPE_HOMING_MISSILE);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHDEBRIS);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHWHITE);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHRED);

			DrawHUDTextures();

			DrawTexture(OBJECT_TYPE_RED_BOX, p3Tex);
			DrawTexture(OBJECT_TYPE_YELLOW_BOX, p2Tex);
			DrawTexture(OBJECT_TYPE_GREEN_BOX, p1Tex);

			break;

		case GS_ASTEROIDS_LEVEL11:

			DrawEnemy();
			DrawColor(OBJECT_TYPE_ENIGMA_SPAWN);

			// Enemy Effects
			DrawColor(OBJECT_TYPE_ENEMY_BULLET);
			DrawColor(OBJECT_TYPE_HOMING_MISSILE);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHDEBRIS);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHWHITE);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHRED);

			DrawHUDTextures();

			DrawTexture(OBJECT_TYPE_RED_BOX, p3Tex);
			DrawTexture(OBJECT_TYPE_YELLOW_BOX, p2Tex);
			DrawTexture(OBJECT_TYPE_GREEN_BOX, p1Tex);

			break;

		case GS_ASTEROIDS_LEVEL12:

			DrawEnemy();
			DrawColor(OBJECT_TYPE_ENIGMA_SPAWN);

			if (pBoss)
			{
				if (pBoss->invinceTime <= 0)
					AnimateBoss(OBJECT_TYPE_BOSS, pBoss4Tex);

				if (pBoss->invinceTime > 0)
					DrawFlash();
			}

			// Enemy Effects
			DrawColor(OBJECT_TYPE_ENEMY_BULLET);
			DrawColor(OBJECT_TYPE_HOMING_MISSILE);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHDEBRIS);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHWHITE);
			DrawColor(OBJECT_TYPE_ENIGMA_DEATHRED);

			DrawHUDTextures();

			DrawTexture(OBJECT_TYPE_RED_BOX, p3Tex);
			DrawTexture(OBJECT_TYPE_YELLOW_BOX, p2Tex);
			DrawTexture(OBJECT_TYPE_GREEN_BOX, p1Tex);

			break;

			// Draw buttons
		case GS_ASTEROIDS_START_SCREEN:
			DrawTexture(OBJECT_TYPE_START_BUTTON, pStartButtonTex);
			DrawTexture(OBJECT_TYPE_EXIT_BUTTON, pExitButtonTex);
			DrawTexture(OBJECT_TYPE_CREDITS_BUTTON, pCreditsButtonTex);
			DrawTexture(OBJECT_TYPE_LOGO, pLogoTex);
			break;

			// Draw buttons
		case GS_ASTEROIDS_SPLASH_SCREEN:
			DrawTexture(OBJECT_TYPE_DIGIPEN_SPLASH, pSplashTex);
			break;

		case GS_ASTEROIDS_CREDITS_SCREEN:
			DrawTexture(OBJECT_TYPE_CREDITS, pCreditsTex);
			break;

		case GS_ASTEROIDS_WIN_SCREEN:
			DrawTexture(OBJECT_TYPE_WIN, pWinTex);
			break;
	}

}

// ---------------------------------------------------------------------------

void GameStateAsteroidsFree(void)
{
	//destroy all game objects
	int i;

	switch (CurrentLevel)
	{
		case GS_ASTEROIDS_LEVEL1:

			// Destroy all spawn points
			for (i = 0; i < NUM_SPAWNPOINTS; i++)
			{
				if ((spawnPointList + i)->spID == MULTI_SPAWNER)
				{
					UnLoadSpecialEnemySpawnpoint(spawnPointList + i);
				}

				DestroySpawnPoint(spawnPointList + i);

			}

			free(spawnPointList);

			// Destroy all enemy instances
			for (i = 0; i < 5000; i++)
			{
				EnemyInstanceDestroy(enemyInstanceList + i, &enemyInstanceNum);
			}
			free(enemyInstanceList);

			break;

		case GS_ASTEROIDS_LEVEL2:

			// Destroy all spawn points
			for (i = 0; i < NUM_SPAWNPOINTS; i++)
			{
				if ((spawnPointList + i)->spID == MULTI_SPAWNER)
				{
					UnLoadSpecialEnemySpawnpoint(spawnPointList + i);
				}

				DestroySpawnPoint(spawnPointList + i);
			}
			free(spawnPointList);

			// Destroy all enemy instances
			for (i = 0; i < 5000; i++)
			{
				EnemyInstanceDestroy(enemyInstanceList + i, &enemyInstanceNum);
			}
			free(enemyInstanceList);

			break;

		case GS_ASTEROIDS_LEVEL3:

			// Destroy all spawn points
			for (i = 0; i < NUM_SPAWNPOINTS; i++)
			{
				if ((spawnPointList + i)->spID == MULTI_SPAWNER)
				{
					UnLoadSpecialEnemySpawnpoint(spawnPointList + i);
				}

				DestroySpawnPoint(spawnPointList + i);
			}
			free(spawnPointList);

			// Destroy all enemy instances
			for (i = 0; i < 5000; i++)
			{
				EnemyInstanceDestroy(enemyInstanceList + i, &enemyInstanceNum);
			}
			free(enemyInstanceList);

			break;

		case GS_ASTEROIDS_LEVEL4:

			// Destroy all spawn points
			for (i = 0; i < NUM_SPAWNPOINTS; i++)
			{
				if ((spawnPointList + i)->spID == MULTI_SPAWNER)
				{
					UnLoadSpecialEnemySpawnpoint(spawnPointList + i);
				}

				DestroySpawnPoint(spawnPointList + i);
			}
			free(spawnPointList);

			// Destroy all enemy instances
			for (i = 0; i < 5000; i++)
			{
				EnemyInstanceDestroy(enemyInstanceList + i, &enemyInstanceNum);
			}
			free(enemyInstanceList);

			break;

		case GS_ASTEROIDS_LEVEL5:

			// Destroy all spawn points
			for (i = 0; i < NUM_SPAWNPOINTS; i++)
			{
				if ((spawnPointList + i)->spID == MULTI_SPAWNER)
				{
					UnLoadSpecialEnemySpawnpoint(spawnPointList + i);
				}

				DestroySpawnPoint(spawnPointList + i);
			}
			free(spawnPointList);

			// Destroy all enemy instances
			for (i = 0; i < 5000; i++)
			{
				EnemyInstanceDestroy(enemyInstanceList + i, &enemyInstanceNum);
			}
			free(enemyInstanceList);

			break;

		case GS_ASTEROIDS_LEVEL6:

			// Destroy all spawn points
			for (i = 0; i < NUM_SPAWNPOINTS; i++)
			{
				if ((spawnPointList + i)->spID == MULTI_SPAWNER)
				{
					UnLoadSpecialEnemySpawnpoint(spawnPointList + i);
				}

				DestroySpawnPoint(spawnPointList + i);
			}
			free(spawnPointList);

			// Destroy all enemy instances
			for (i = 0; i < 5000; i++)
			{
				EnemyInstanceDestroy(enemyInstanceList + i, &enemyInstanceNum);
			}
			free(enemyInstanceList);

			break;

		case GS_ASTEROIDS_LEVEL7:

			// Destroy all spawn points
			for (i = 0; i < NUM_SPAWNPOINTS; i++)
			{
				if ((spawnPointList + i)->spID == MULTI_SPAWNER)
				{
					UnLoadSpecialEnemySpawnpoint(spawnPointList + i);
				}

				DestroySpawnPoint(spawnPointList + i);
			}
			free(spawnPointList);

			// Destroy all enemy instances
			for (i = 0; i < 5000; i++)
			{
				EnemyInstanceDestroy(enemyInstanceList + i, &enemyInstanceNum);
			}
			free(enemyInstanceList);

			break;

		case GS_ASTEROIDS_LEVEL8:

			// Destroy all spawn points
			for (i = 0; i < NUM_SPAWNPOINTS; i++)
			{
				if ((spawnPointList + i)->spID == MULTI_SPAWNER)
				{
					UnLoadSpecialEnemySpawnpoint(spawnPointList + i);
				}

				DestroySpawnPoint(spawnPointList + i);
			}
			free(spawnPointList);

			// Destroy all enemy instances
			for (i = 0; i < 5000; i++)
			{
				EnemyInstanceDestroy(enemyInstanceList + i, &enemyInstanceNum);
			}
			free(enemyInstanceList);

			break;

		case GS_ASTEROIDS_LEVEL9:

			// Destroy all spawn points
			for (i = 0; i < NUM_SPAWNPOINTS; i++)
			{
				if ((spawnPointList + i)->spID == MULTI_SPAWNER)
				{
					UnLoadSpecialEnemySpawnpoint(spawnPointList + i);
				}

				DestroySpawnPoint(spawnPointList + i);
			}
			free(spawnPointList);

			// Destroy all enemy instances
			for (i = 0; i < 5000; i++)
			{
				EnemyInstanceDestroy(enemyInstanceList + i, &enemyInstanceNum);
			}
			free(enemyInstanceList);

			break;

		case GS_ASTEROIDS_LEVEL10:

			// Destroy all spawn points
			for (i = 0; i < NUM_SPAWNPOINTS; i++)
			{
				if ((spawnPointList + i)->spID == MULTI_SPAWNER)
				{
					UnLoadSpecialEnemySpawnpoint(spawnPointList + i);
				}

				DestroySpawnPoint(spawnPointList + i);
			}
			free(spawnPointList);

			// Destroy all enemy instances
			for (i = 0; i < 5000; i++)
			{
				EnemyInstanceDestroy(enemyInstanceList + i, &enemyInstanceNum);
			}
			free(enemyInstanceList);

			break;

		case GS_ASTEROIDS_LEVEL11:

			// Destroy all spawn points
			for (i = 0; i < NUM_SPAWNPOINTS; i++)
			{
				if ((spawnPointList + i)->spID == MULTI_SPAWNER)
				{
					UnLoadSpecialEnemySpawnpoint(spawnPointList + i);
				}

				DestroySpawnPoint(spawnPointList + i);
			}
			free(spawnPointList);

			// Destroy all enemy instances
			for (i = 0; i < 5000; i++)
			{
				EnemyInstanceDestroy(enemyInstanceList + i, &enemyInstanceNum);
			}
			free(enemyInstanceList);

			break;

		case GS_ASTEROIDS_LEVEL12:

			// Destroy all spawn points
			for (i = 0; i < NUM_SPAWNPOINTS; i++)
			{
				if ((spawnPointList + i)->spID == MULTI_SPAWNER)
				{
					UnLoadSpecialEnemySpawnpoint(spawnPointList + i);
				}

				DestroySpawnPoint(spawnPointList + i);
			}
			free(spawnPointList);

			// Destroy all enemy instances
			for (i = 0; i < 5000; i++)
			{
				EnemyInstanceDestroy(enemyInstanceList + i, &enemyInstanceNum);
			}
			free(enemyInstanceList);

			break;

		case GS_ASTEROIDS_START_SCREEN:
			break;

		case GS_ASTEROIDS_SPLASH_SCREEN:
			break;

		case GS_ASTEROIDS_CREDITS_SCREEN:
			break;

		case GS_ASTEROIDS_WIN_SCREEN:
			break;
	}

	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjectInstance * object = sgGameObjectInstanceList + i;
		if ((object->mFlag & FLAG_ACTIVE) == 1)
			GameObjectInstanceDestroy(object);
	}

}

// ---------------------------------------------------------------------------

void GameStateAsteroidsUnload(void)
{
	//destroy all shapes
	unsigned int i;

	for (i = 0; i < sgShapeNum; i++)
	{
		AEGfxMeshFree(sgShapes[i].mpMesh);
	}

	//FMOD free
	FMOD_Sound_Release(sound1);
	FMOD_Sound_Release(sound2);
	FMOD_Sound_Release(background);
	FMOD_System_Close(FmodSystem);
	result = FMOD_System_Release(FmodSystem);

	FreeEnemyFMOD();
}



// ---------------------------------------------------------------------------//
// ---------------------------------------------------------------------------//
// ---------------------------------------------------------------------------//
//							UTILITY FUNCTIONS								  //
// ---------------------------------------------------------------------------//
// ---------------------------------------------------------------------------//
// ---------------------------------------------------------------------------//




GameObjectInstance* GameObjectInstanceCreate(unsigned int ObjectType)			// From OBJECT_TYPE enum)
{
	unsigned long i;

	// loop through the object instance list to find a non-used object instance
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjectInstance* pInst = sgGameObjectInstanceList + i;
		Vector2D shipPos;
		Vector2DSet(&shipPos, 0, -200);

		// Check if current instance is not used
		if (pInst->mFlag == 0)
		{
			// It is not used => use it to create the new instance

			// Active the game object instance
			pInst->mFlag = FLAG_ACTIVE;
			pInst->mHit = 0;
			pInst->invinceTime = 0;

			pInst->mpComponent_Transform = 0;
			pInst->mpComponent_Sprite = 0;
			pInst->mpComponent_Physics = 0;
			pInst->mpComponent_Target = 0;

			// Add the components, based on the object type
			switch (ObjectType)
			{
			case OBJECT_TYPE_SHIP:
				AddComponent_Sprite(pInst, OBJECT_TYPE_SHIP);
				AddComponent_Transform(pInst, &shipPos, 0.0f, SHIP_SIZE, SHIP_SIZE);
				AddComponent_Physics(pInst, 0);
				pInst->mFlag = FLAG_ACTIVE;
				pInst->invinceTime = 0;
				break;

			case OBJECT_TYPE_BULLET:
				AddComponent_Sprite(pInst, OBJECT_TYPE_BULLET);
				AddComponent_Transform(pInst, 0, 0.0f, 10.0f, 10.0f);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_ASTEROID:
				AddComponent_Sprite(pInst, OBJECT_TYPE_ASTEROID);
				AddComponent_Transform(pInst, 0, 0.0f, 50.0f, 50.0f);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_HOMING_MISSILE:
				AddComponent_Sprite(pInst, OBJECT_TYPE_HOMING_MISSILE);
				AddComponent_Transform(pInst, 0, 0.0f, 5.0f, 5.0f);
				AddComponent_Physics(pInst, 0);
				AddComponent_Target(pInst, 0);
				break;

			case OBJECT_TYPE_ENEMY_BULLET:
				AddComponent_Sprite(pInst, OBJECT_TYPE_ENEMY_BULLET);
				AddComponent_Transform(pInst, 0, 0.0f, 15.0f, 15.0f);
				AddComponent_Physics(pInst, 0);
				AddComponent_Target(pInst, 0);
				break;

			case OBJECT_TYPE_STAR1:
				AddComponent_Sprite(pInst, OBJECT_TYPE_STAR1);
				AddComponent_Transform(pInst, 0, 0.0f, 3.0f, 3.0f);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_STAR2:
				AddComponent_Sprite(pInst, OBJECT_TYPE_STAR2);
				AddComponent_Transform(pInst, 0, 0.0f, 3.0f, 3.0f);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_STAR3:
				AddComponent_Sprite(pInst, OBJECT_TYPE_STAR3);
				AddComponent_Transform(pInst, 0, 0.0f, 3.0f, 3.0f);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_LIFE:
				AddComponent_Sprite(pInst, OBJECT_TYPE_LIFE);
				AddComponent_Transform(pInst, 0, 0.0f, SHIP_SIZE, SHIP_SIZE);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_PINK_TRAIL:
				AddComponent_Sprite(pInst, OBJECT_TYPE_PINK_TRAIL);
				AddComponent_Transform(pInst, 0, 0.0f, 5.0f, 8.0f);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_WHITE_TRAIL:
				AddComponent_Sprite(pInst, OBJECT_TYPE_WHITE_TRAIL);
				AddComponent_Transform(pInst, 0, 0.0f, 5.0f, 8.0f);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_BLUE_TRAIL:
				AddComponent_Sprite(pInst, OBJECT_TYPE_BLUE_TRAIL);
				AddComponent_Transform(pInst, 0, 0.0f, 5.0f, 8.0f);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_ENIGMA_DEATHRED:
				AddComponent_Sprite(pInst, OBJECT_TYPE_ENIGMA_DEATHRED);
				AddComponent_Transform(pInst, 0, 0.0f, 5.0f, 8.0f);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_ENIGMA_DEATHWHITE:
				AddComponent_Sprite(pInst, OBJECT_TYPE_ENIGMA_DEATHWHITE);
				AddComponent_Transform(pInst, 0, 0.0f, 5.0f, 8.0f);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_ENIGMA_DEATHDEBRIS:
				AddComponent_Sprite(pInst, OBJECT_TYPE_ENIGMA_DEATHWHITE);
				AddComponent_Transform(pInst, 0, 0.0f, 5.0f, 8.0f);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_LSIDE:
				AddComponent_Sprite(pInst, OBJECT_TYPE_LSIDE);
				AddComponent_Transform(pInst, 0, 0.0f, 1.0f, 1.0f);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_RSIDE:
				AddComponent_Sprite(pInst, OBJECT_TYPE_RSIDE);
				AddComponent_Transform(pInst, 0, 0.0f, 1.0f, 1.0f);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_HEALTH_FULL:
				AddComponent_Sprite(pInst, OBJECT_TYPE_HEALTH_FULL);
				AddComponent_Transform(pInst, 0, 0.0f, 1.0f, 1.0f);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_HEALTH_HALF:
				AddComponent_Sprite(pInst, OBJECT_TYPE_HEALTH_HALF);
				AddComponent_Transform(pInst, 0, 0.0f, 1.0f, 1.0f);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_START_BUTTON:
				AddComponent_Sprite(pInst, OBJECT_TYPE_START_BUTTON);
				AddComponent_Transform(pInst, 0, 0.0f, 10.0f, 10.0f);
				AddComponent_Physics(pInst, 0);
				pInst->buttonID = BUTTON_ID_START;
				break;

			case OBJECT_TYPE_DIGIPEN_SPLASH:
				AddComponent_Sprite(pInst, OBJECT_TYPE_DIGIPEN_SPLASH);
				AddComponent_Transform(pInst, 0, 0.0f, 320.0f, 210.0f);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_BOSS:
				AddComponent_Sprite(pInst, OBJECT_TYPE_BOSS);
				AddComponent_Transform(pInst, 0, 0.0f, 100.0f, 100.0f);
				AddComponent_Physics(pInst, 0);
				pInst->invinceTime = 0;
				break;

			case OBJECT_TYPE_ENIGMA_SPAWN:
				AddComponent_Sprite(pInst, OBJECT_TYPE_ENIGMA_SPAWN);
				AddComponent_Transform(pInst, 0, 0.0f, 5.0f, 8.0f);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_EXIT_BUTTON:
				AddComponent_Sprite(pInst, OBJECT_TYPE_EXIT_BUTTON);
				AddComponent_Transform(pInst, 0, 0.0f, 10.0f, 10.0f);
				AddComponent_Physics(pInst, 0);
				pInst->buttonID = BUTTON_ID_EXIT;
				break;

			case OBJECT_TYPE_LOGO:
				AddComponent_Sprite(pInst, OBJECT_TYPE_LOGO);
				AddComponent_Transform(pInst, 0, 0.0f, 1.0f, 1.0f);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_SHIP_FLASH:
				AddComponent_Sprite(pInst, OBJECT_TYPE_SHIP_FLASH);
				AddComponent_Transform(pInst, &shipPos, 0.0f, SHIP_SIZE, SHIP_SIZE);
				AddComponent_Physics(pInst, 0);
				pInst->mFlag = FLAG_ACTIVE;
				break;

			case OBJECT_TYPE_BOSS_FLASH:
				AddComponent_Sprite(pInst, OBJECT_TYPE_BOSS_FLASH);
				AddComponent_Transform(pInst, 0, 0.0f, 100.0f, 100.0f);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_RED_BOX:
				AddComponent_Sprite(pInst, OBJECT_TYPE_RED_BOX);
				AddComponent_Transform(pInst, 0, 0.0f, 40.0f, 40.0f);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_YELLOW_BOX:
				AddComponent_Sprite(pInst, OBJECT_TYPE_YELLOW_BOX);
				AddComponent_Transform(pInst, 0, 0.0f, 40.0f, 40.0f);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_GREEN_BOX:
				AddComponent_Sprite(pInst, OBJECT_TYPE_GREEN_BOX);
				AddComponent_Transform(pInst, 0, 0.0f, 40.0f, 40.0f);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_MAP:
				AddComponent_Sprite(pInst, OBJECT_TYPE_MAP);
				AddComponent_Transform(pInst, 0, 0.0f, 70.0f, 70.0f);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_CREDITS_BUTTON:
				AddComponent_Sprite(pInst, OBJECT_TYPE_CREDITS_BUTTON);
				AddComponent_Transform(pInst, 0, 0.0f, 70.0f, 70.0f);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_CREDITS:
				AddComponent_Sprite(pInst, OBJECT_TYPE_CREDITS);
				AddComponent_Transform(pInst, 0, 0.0f, 320.0f, 205.0f);
				AddComponent_Physics(pInst, 0);
				break;

			case OBJECT_TYPE_WIN:
				AddComponent_Sprite(pInst, OBJECT_TYPE_WIN);
				AddComponent_Transform(pInst, 0, 0.0f, 320.0f, 210.0f);
				AddComponent_Physics(pInst, 0);
				break;
			}

			++sgGameObjectInstanceNum;

			// return the newly created instance
			return pInst;
		}
	}

	// Cannot find empty slot => return 0
	return 0;
}

// ---------------------------------------------------------------------------

void GameObjectInstanceDestroy(GameObjectInstance* pInst)
{
	// if instance is destroyed before, just return
	if (pInst->mFlag == 0)
		return;

	// Zero out the mFlag
	pInst->mFlag = 0;

	RemoveComponent_Transform(pInst);
	RemoveComponent_Sprite(pInst);
	RemoveComponent_Physics(pInst);
	RemoveComponent_Target(pInst);

	--sgGameObjectInstanceNum;
}

// ---------------------------------------------------------------------------

void AddComponent_Transform(GameObjectInstance *pInst, Vector2D *pPosition, float Angle, float ScaleX, float ScaleY)
{
	if (0 != pInst)
	{
		if (0 == pInst->mpComponent_Transform)
		{
			pInst->mpComponent_Transform = (Component_Transform *)calloc(1, sizeof(Component_Transform));
		}

		Vector2D zeroVec2;
		Vector2DZero(&zeroVec2);

		pInst->mpComponent_Transform->mScaleX = ScaleX;
		pInst->mpComponent_Transform->mScaleY = ScaleY;
		pInst->mpComponent_Transform->mPosition = pPosition ? *pPosition : zeroVec2;;
		pInst->mpComponent_Transform->mAngle = Angle;
		pInst->mpComponent_Transform->mpOwner = pInst;

		if (pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_SHIP)
		{
			pInst->mpComponent_Transform->mAngle = PI / 2.0f;
		}
	}
}

// ---------------------------------------------------------------------------

void AddComponent_Sprite(GameObjectInstance *pInst, unsigned int ShapeType)
{
	if (0 != pInst)
	{
		if (0 == pInst->mpComponent_Sprite)
		{
			pInst->mpComponent_Sprite = (Component_Sprite *)calloc(1, sizeof(Component_Sprite));
		}

		pInst->mpComponent_Sprite->mpShape = sgShapes + ShapeType;
		pInst->mpComponent_Sprite->mpOwner = pInst;
	}
}

// ---------------------------------------------------------------------------

void AddComponent_Physics(GameObjectInstance *pInst, Vector2D *pVelocity)
{
	if (0 != pInst)
	{
		if (0 == pInst->mpComponent_Physics)
		{
			pInst->mpComponent_Physics = (Component_Physics *)calloc(1, sizeof(Component_Physics));
		}

		Vector2D zeroVec2;
		Vector2DZero(&zeroVec2);

		pInst->mpComponent_Physics->mVelocity = pVelocity ? *pVelocity : zeroVec2;
		pInst->mpComponent_Physics->mpOwner = pInst;
	}
}

// ---------------------------------------------------------------------------

void AddComponent_Target(GameObjectInstance *pInst, GameObjectInstance *pTarget)
{
	if (0 != pInst)
	{
		if (0 == pInst->mpComponent_Target)
		{
			pInst->mpComponent_Target = (Component_Target *)calloc(1, sizeof(Component_Target));
		}

		pInst->mpComponent_Target->follow_timer = ConstructTimer(1.0);
		pInst->mpComponent_Target->mpTarget = pTarget;
		pInst->mpComponent_Target->mpOwner = pInst;
	}
}

// ---------------------------------------------------------------------------

void RemoveComponent_Transform(GameObjectInstance *pInst)
{
	if (0 != pInst)
	{
		if (0 != pInst->mpComponent_Transform)
		{
			free(pInst->mpComponent_Transform);
			pInst->mpComponent_Transform = 0;
		}
	}
}

// ---------------------------------------------------------------------------

void RemoveComponent_Sprite(GameObjectInstance *pInst)
{
	if (0 != pInst)
	{
		if (0 != pInst->mpComponent_Sprite)
		{
			free(pInst->mpComponent_Sprite);
			pInst->mpComponent_Sprite = 0;
		}
	}
}

// ---------------------------------------------------------------------------

void RemoveComponent_Physics(GameObjectInstance *pInst)
{
	if (0 != pInst)
	{
		if (0 != pInst->mpComponent_Physics)
		{
			free(pInst->mpComponent_Physics);
			pInst->mpComponent_Physics = 0;
		}
	}
}

// ---------------------------------------------------------------------------

void RemoveComponent_Target(GameObjectInstance *pInst)
{
	if (0 != pInst)
	{
		if (0 != pInst->mpComponent_Target)
		{
	//		DestroyTimer(&pInst->mpComponent_Target->follow_timer);

			free(pInst->mpComponent_Target);
			pInst->mpComponent_Target = 0;
		}
	}
}

void AddToObject(GameObjectInstance *object, int xPos, int yPos, int xVel, int yVel, int xScale, int yScale, float angle)
{
	object->mpComponent_Transform->mPosition.x = xPos;
	object->mpComponent_Transform->mPosition.y = yPos;

	object->mpComponent_Physics->mVelocity.x = xVel;
	object->mpComponent_Physics->mVelocity.y = yVel;

	object->mpComponent_Transform->mScaleX = xScale;
	object->mpComponent_Transform->mScaleY = yScale;

	object->mpComponent_Transform->mAngle = angle;
}

void UpdateObjectMatrices(GameObjectInstance* GameObjList)
{
	for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		Matrix2D trans, rot, scale;
		GameObjectInstance* pInst = GameObjList + i;

		// skip non-active object
		if ((pInst->mFlag & FLAG_ACTIVE) == 0)
			continue;

		//set up the transformation matrix
		// Compute the scaling matrix
		Matrix2DScale(&scale, pInst->mpComponent_Transform->mScaleX, pInst->mpComponent_Transform->mScaleY);
		// Compute the rotation matrix 
		Matrix2DRotRad(&rot, pInst->mpComponent_Transform->mAngle);
		// Compute the translation matrix
		Matrix2DTranslate(&trans, pInst->mpComponent_Transform->mPosition.x, pInst->mpComponent_Transform->mPosition.y);

		// Concatenate the 3 matrix in the correct order in the object instance's transform component's "mTransform" matrix
		Matrix2DConcat(&pInst->mpComponent_Transform->mTransform, &trans, &rot);
		Matrix2DConcat(&pInst->mpComponent_Transform->mTransform, &pInst->mpComponent_Transform->mTransform, &scale);
	}
}

void UpdateObjectCollisions(GameObjectInstance* pObjectList)
{
	int loop1, loop2, loop3 = 0;

	for (loop1 = 0; loop1 < GAME_OBJ_INST_NUM_MAX; loop1++)
	{
		GameObjectInstance * object = pObjectList + loop1;

		if ((object->mFlag & FLAG_ACTIVE) == 0)
			continue;

		if (object->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_START_BUTTON || object->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_EXIT_BUTTON || object->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_CREDITS_BUTTON)
		{
			for (loop2 = 0; loop2 < GAME_OBJ_INST_NUM_MAX; loop2++)
			{
				GameObjectInstance *object2 = pObjectList + loop2;

				if ((object2->mFlag & FLAG_ACTIVE) == 0)
					continue;

				if (object2->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_BULLET)
				{
					if (StaticPointToStaticRect(&object2->mpComponent_Transform->mPosition, &object->mpComponent_Transform->mPosition, object->mpComponent_Transform->mScaleX, object->mpComponent_Transform->mScaleY))
					{
						switch (object->buttonID)
						{
						case BUTTON_ID_START:

							NextLevel = GS_ASTEROIDS_LEVEL1;
							changeLevelFlag = 1;
							break;

						case BUTTON_ID_EXIT:
							CurrentLevel = GS_QUIT;
							gGameStateNext = GS_QUIT;
							break;

						case BUTTON_ID_CREDITS:
							NextLevel = GS_ASTEROIDS_CREDITS_SCREEN;
							changeLevelFlag = 1;
							break;
						}
					}
				}

			}
		}

			// What is our Asteroid Colliding with
		if (object->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_ASTEROID)
		{
			for (loop2 = 0; loop2 < GAME_OBJ_INST_NUM_MAX; loop2++)
			{
				GameObjectInstance * object2 = pObjectList + loop2;

				if ((object2->mFlag & FLAG_ACTIVE) == 0)
					continue;

				if (object2->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_BULLET)
				{
					if (StaticPointToStaticRect(&object2->mpComponent_Transform->mPosition, &object->mpComponent_Transform->mPosition, object->mpComponent_Transform->mScaleX, object->mpComponent_Transform->mScaleY))
					{
						GameObjectInstanceDestroy(object);
						GameObjectInstanceDestroy(object2);
						break;
					}
				}
				if (object2->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_SHIP)
				{
					if (StaticRectToStaticRect(&object2->mpComponent_Transform->mPosition, SHIP_SIZE, SHIP_SIZE, &object->mpComponent_Transform->mPosition, object->mpComponent_Transform->mScaleX, object->mpComponent_Transform->mScaleY))
					{
						GameObjectInstanceDestroy(object);

						IsHit();

						break;

					}

				}
			}
		}

		else if (object->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_ENEMY_BULLET && ((sgpShip->mHit & FLAG_ACTIVE) == 0))
		{

			if (StaticRectToStaticRect(&sgpShip->mpComponent_Transform->mPosition, SHIP_SIZE, SHIP_SIZE, &object->mpComponent_Transform->mPosition, object->mpComponent_Transform->mScaleX, object->mpComponent_Transform->mScaleY))
			{
				GameObjectInstanceDestroy(object);

				IsHit();

				break;
			}
		}

		else if (object->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_BOSS)
		{
			for (loop2 = 0; loop2 < GAME_OBJ_INST_NUM_MAX; loop2++)
			{
				GameObjectInstance * object2 = pObjectList + loop2;

				if ((object2->mFlag & FLAG_ACTIVE) == 0)
					continue;

				if (object2->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_BULLET && pBoss->invinceTime <= 0)
				{
					if (StaticPointToStaticRect(&object2->mpComponent_Transform->mPosition, &object->mpComponent_Transform->mPosition, object->mpComponent_Transform->mScaleX * 1.5f, object->mpComponent_Transform->mScaleY *1.5f))
					{
						object->bossHealth--;
						pBoss->invinceTime = 0.1;
						GameObjectInstanceDestroy(object2);

						if (object->bossHealth <= 0)
						{
							GameObjectInstanceDestroy(object);

							// Increment the boss-kill counter
							BossesKilled++;

							pBoss = 0;
						}
						break;
					}
				}
			}
		}


	}
}

void UpdateObjectBehaviours(GameObjectInstance* pObjList)
{
	float winMaxX, winMaxY, winMinX, winMinY;
	double frameTime;

	frameTime = AEFrameRateControllerGetFrameTime();

	winMaxX = (AEGfxGetWinMaxX() / 2) + 100;
	winMaxY = AEGfxGetWinMaxY();
	winMinX = (AEGfxGetWinMinX() / 2) - 100;
	winMinY = AEGfxGetWinMinY();

		// Only look at this if it's a boss level
	if (pBoss != 0)
	{
		pBoss->timer -= frameTime;
		pBoss->bossTimer -= frameTime;
		pBoss->mpComponent_Physics->mVelocity.y = 0.0f;

		if (pBoss->bossTimer <= 0 && pBoss->ParticleFlag == 0) //intitial movement
		{
			pBoss->mpComponent_Physics->mVelocity.x = -90.0f;
			pBoss->bossTimer = 4.0f;
			pBoss->ParticleFlag = -1; //delay state
		}
		if (pBoss->bossTimer <= 0 && pBoss->ParticleFlag == -1) //delay
		{
			pBoss->mpComponent_Physics->mVelocity.x = 0.0f;
			pBoss->bossTimer = 2.0f;
			pBoss->ParticleFlag = 2; //moving right
		}
		if (pBoss->bossTimer <= 0 && pBoss->ParticleFlag == 2) //moving right
		{
			pBoss->mpComponent_Physics->mVelocity.x = 90.0f;
			pBoss->bossTimer = 8.0f;
			pBoss->ParticleFlag = 3; //moving left
		}
		if (pBoss->bossTimer <= 0 && pBoss->ParticleFlag == 3) //moving left
		{
			pBoss->mpComponent_Physics->mVelocity.x = -90.0f;
			pBoss->bossTimer = 4.0f;
			pBoss->ParticleFlag = 0; //initial movement
		}


		if (pBoss->timer <= 0.0f)
		{
	/*		Vector2D bulletVelocity;
			GameObjectInstance* pBullet;

			// Shoot this bullet downward
			Vector2DSet(&bulletVelocity, 0.0f, -1.0f * BULLET_SPEED);

			// Create the bullet
			pBullet = BulletCreate(pBoss->mpComponent_Transform->mPosition, bulletVelocity, pBoss->mpComponent_Transform->mAngle, OBJECT_TYPE_ENEMY_BULLET);
			pBullet->mpComponent_Transform->mPosition.y = pBoss->mpComponent_Transform->mPosition.y - 130.0f;
			pBullet->mpComponent_Transform->mAngle = (PI / -2.0f);*/
			pBoss->timer = 0.25f;
		}
	}

		if (sgpShip->InputFlag)
		{
			UpdateTimer(fire);
			if (fire->doneCounting && AEInputCheckCurr(VK_SPACE) && CurrentLevel != GS_ASTEROIDS_START_SCREEN)
			{
				ResetTimer(fire);
				GameObjectInstance * bulletLeft = GameObjectInstanceCreate(OBJECT_TYPE_BULLET);
				GameObjectInstance * bulletMid = GameObjectInstanceCreate(OBJECT_TYPE_BULLET);
				GameObjectInstance * bulletRight = GameObjectInstanceCreate(OBJECT_TYPE_BULLET);

				bulletLeft->mpComponent_Physics->mVelocity.x = (cosf(sgpShip->mpComponent_Transform->mAngle) + (PI / 15.0f)) * BULLET_SPEED;
				bulletLeft->mpComponent_Physics->mVelocity.y = sinf(sgpShip->mpComponent_Transform->mAngle) * BULLET_SPEED;

				bulletLeft->mpComponent_Transform->mPosition.x = sgpShip->mpComponent_Transform->mPosition.x + 5.0f;
				bulletLeft->mpComponent_Transform->mPosition.y = sgpShip->mpComponent_Transform->mPosition.y + 15.0f;

				bulletLeft->mpComponent_Transform->mAngle = sgpShip->mpComponent_Transform->mAngle - (PI / 15.0f);

				bulletMid->mpComponent_Physics->mVelocity.x = cosf(sgpShip->mpComponent_Transform->mAngle) * BULLET_SPEED;
				bulletMid->mpComponent_Physics->mVelocity.y = sinf(sgpShip->mpComponent_Transform->mAngle) * BULLET_SPEED;

				bulletMid->mpComponent_Transform->mPosition.x = sgpShip->mpComponent_Transform->mPosition.x;
				bulletMid->mpComponent_Transform->mPosition.y = sgpShip->mpComponent_Transform->mPosition.y + 15.0f;

				bulletMid->mpComponent_Transform->mAngle = sgpShip->mpComponent_Transform->mAngle;

				bulletRight->mpComponent_Physics->mVelocity.x = (cosf(sgpShip->mpComponent_Transform->mAngle) - (PI / 15.0f)) * BULLET_SPEED;
				bulletRight->mpComponent_Physics->mVelocity.y = sinf(sgpShip->mpComponent_Transform->mAngle) * BULLET_SPEED;

				bulletRight->mpComponent_Transform->mPosition.x = sgpShip->mpComponent_Transform->mPosition.x - 5.0f;
				bulletRight->mpComponent_Transform->mPosition.y = sgpShip->mpComponent_Transform->mPosition.y + 15.0f;

				bulletRight->mpComponent_Transform->mAngle = sgpShip->mpComponent_Transform->mAngle + (PI / 15.0f);

				//Firing sound
				FMOD_System_PlaySound(FmodSystem, sound1, 0, 0, &quietChannel);
			}
		}

	for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjectInstance* pInst = pObjList + i;

		// skip non-active object
		if ((pInst->mFlag & FLAG_ACTIVE) == 0)
			continue;

		// check if the object is a ship
		if (pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_SHIP)
		{
			// warp the ship from one end of the screen to the other
			pInst->mpComponent_Transform->mPosition.x = AEWrap(pInst->mpComponent_Transform->mPosition.x, winMinX - SHIP_SIZE, winMaxX + SHIP_SIZE);
			pInst->mpComponent_Transform->mPosition.y = AEWrap(pInst->mpComponent_Transform->mPosition.y, winMinY - SHIP_SIZE, winMaxY + SHIP_SIZE);
		}

		// Bullet behavior
		if (pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_BULLET)
		{
			if (pInst->mpComponent_Transform->mPosition.x > winMaxX || pInst->mpComponent_Transform->mPosition.x < winMinX)
			{
				GameObjectInstanceDestroy(pInst);
				continue;
			}
			if (pInst->mpComponent_Transform->mPosition.y > winMaxY || pInst->mpComponent_Transform->mPosition.y < winMinY)
			{
				GameObjectInstanceDestroy(pInst);
				continue;
			}
			continue;
		}

		// Asteroid behavior
		if (pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_ASTEROID)
		{
			pInst->mpComponent_Transform->mPosition.x = AEWrap(pInst->mpComponent_Transform->mPosition.x, winMinX - pInst->mpComponent_Transform->mScaleX, winMaxX + pInst->mpComponent_Transform->mScaleX);
			pInst->mpComponent_Transform->mPosition.y = AEWrap(pInst->mpComponent_Transform->mPosition.y, winMinY - pInst->mpComponent_Transform->mScaleY, winMaxY + pInst->mpComponent_Transform->mScaleY);
		}

		// Star behavior
		if (pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_STAR1)
		{
			pInst->mpComponent_Transform->mPosition.x = AEWrap(pInst->mpComponent_Transform->mPosition.x, winMinX - pInst->mpComponent_Transform->mScaleX, winMaxX + pInst->mpComponent_Transform->mScaleX);
			pInst->mpComponent_Transform->mPosition.y = AEWrap(pInst->mpComponent_Transform->mPosition.y, winMinY - pInst->mpComponent_Transform->mScaleY, winMaxY + pInst->mpComponent_Transform->mScaleY);
		}

		if (pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_STAR2)
		{
			pInst->mpComponent_Transform->mPosition.x = AEWrap(pInst->mpComponent_Transform->mPosition.x, winMinX - pInst->mpComponent_Transform->mScaleX, winMaxX + pInst->mpComponent_Transform->mScaleX);
			pInst->mpComponent_Transform->mPosition.y = AEWrap(pInst->mpComponent_Transform->mPosition.y, winMinY - pInst->mpComponent_Transform->mScaleY, winMaxY + pInst->mpComponent_Transform->mScaleY);
		}

		if (pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_STAR3)
		{
			pInst->mpComponent_Transform->mPosition.x = AEWrap(pInst->mpComponent_Transform->mPosition.x, winMinX - pInst->mpComponent_Transform->mScaleX, winMaxX + pInst->mpComponent_Transform->mScaleX);
			pInst->mpComponent_Transform->mPosition.y = AEWrap(pInst->mpComponent_Transform->mPosition.y, winMinY - pInst->mpComponent_Transform->mScaleY, winMaxY + pInst->mpComponent_Transform->mScaleY);
		}

		// Homing missile behavior (Not every game object instance will have this component!)
		if (pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_HOMING_MISSILE)
		{
			pInst->mpComponent_Transform->mPosition.x = AEWrap(pInst->mpComponent_Transform->mPosition.x, winMinX - 5.0f, winMaxX + 5.0f);
			pInst->mpComponent_Transform->mPosition.y = AEWrap(pInst->mpComponent_Transform->mPosition.y, winMinY - 5.0f, winMaxY + 5.0f);

			int loopVar;

			// Update the timer for following the player
			UpdateTimer(pInst->mpComponent_Target->follow_timer);

			for (loopVar = 0; loopVar < GAME_OBJ_INST_NUM_MAX; loopVar++)
			{
				GameObjectInstance * target = pObjList + loopVar;

				if ((target->mFlag & FLAG_ACTIVE) == 0)
					continue;
				if (target->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_SHIP)
				{
					pInst->mpComponent_Target->mpTarget = target;
					break;
				}
			}
			if (pInst->mpComponent_Target->mpTarget != NULL && pInst->mpComponent_Target->mpTarget->mpComponent_Transform != NULL)
			{
				Vector2D targetVector;
				Vector2D normal;
				Vector2DSet(&targetVector, pInst->mpComponent_Target->mpTarget->mpComponent_Transform->mPosition.x - pInst->mpComponent_Transform->mPosition.x,
					pInst->mpComponent_Target->mpTarget->mpComponent_Transform->mPosition.y - pInst->mpComponent_Transform->mPosition.y);
				Vector2DSet(&normal, pInst->mpComponent_Physics->mVelocity.y * -1, pInst->mpComponent_Physics->mVelocity.x);

				if (Vector2DDotProduct(&targetVector, &normal) >= 0)
				{
					pInst->mpComponent_Transform->mAngle += HOMING_MISSILE_ROT_SPEED * (float)frameTime;
				}
				else
				{
					pInst->mpComponent_Transform->mAngle -= HOMING_MISSILE_ROT_SPEED * (float)frameTime;
				}
				Vector2DSet(&pInst->mpComponent_Physics->mVelocity, cosf(pInst->mpComponent_Transform->mAngle) * BULLET_SPEED / 1.5f, sinf(pInst->mpComponent_Transform->mAngle) * BULLET_SPEED / 1.5f);
			}

			// If we've followed the player for an adequate enough time
			if (pInst->mpComponent_Target->follow_timer->doneCounting == DONE_COUNTING)
			{
				ScatterShotExplosion(pInst->mpComponent_Transform->mPosition, 10, OBJECT_TYPE_ENEMY_BULLET);
				GameObjectInstanceDestroy(pInst);

				// Detonate into a scatter here
				active_grenade_shots--;
				break;
			}
		}

		//trail behavior
		if (pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_PINK_TRAIL)
		{
			pInst->timer -= frameTime;

			if (pInst->timer <= 0)
			{
				pInst->mFlag = 0;
			}
		}
		if (pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_WHITE_TRAIL)
		{
			pInst->timer -= frameTime;

			if (pInst->timer <= 0)
			{
				pInst->mFlag = 0;
			}
		}

		if (pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_BLUE_TRAIL)
		{
			pInst->timer -= frameTime;

			if (pInst->timer <= 0)
			{
				pInst->mFlag = 0;
			}
		}

		if (pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_ENIGMA_DEATHRED)
		{
			pInst->timer -= frameTime;

			if (pInst->timer <= 0 && pInst->ParticleFlag == 1)
			{
				Vector2DScale(&pInst->mpComponent_Physics->mVelocity, &pInst->mpComponent_Physics->mVelocity, -5.0f);
				pInst->timer = 0.20f;
				pInst->ParticleFlag = 0;
			}

			else if (pInst->timer <= 0 && pInst->ParticleFlag == 0)
			{
				pInst->mFlag = 0;
			}
		}

		if (pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_ENIGMA_DEATHWHITE)
		{
			pInst->timer -= frameTime;

			if (pInst->timer <= 0 && pInst->ParticleFlag == 1)
			{
				Vector2DScale(&pInst->mpComponent_Physics->mVelocity, &pInst->mpComponent_Physics->mVelocity, -5.0f);
				pInst->timer = 0.20f;
				pInst->ParticleFlag = 0;
			}

			else if (pInst->timer <= 0 && pInst->ParticleFlag == 0)
			{
				pInst->mFlag = 0;
			}
		}

		if (pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_ENIGMA_DEATHDEBRIS)
		{
			pInst->timer -= frameTime;

			if (pInst->timer <= 0 && pInst->ParticleFlag == 1)
			{
				Vector2DScale(&pInst->mpComponent_Physics->mVelocity, &pInst->mpComponent_Physics->mVelocity, -5.0f);
				pInst->timer = 0.20f;
				pInst->ParticleFlag = 0;
			}

			else if (pInst->timer <= 0 && pInst->ParticleFlag == 0)
			{
				pInst->mFlag = 0;
			}
		}

		if (pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_ENIGMA_SPAWN)
		{
			pInst->timer -= frameTime;

			if (pInst->timer <= 0 && pInst->ParticleFlag == 1)
			{
				Vector2DScale(&pInst->mpComponent_Physics->mVelocity, &pInst->mpComponent_Physics->mVelocity, -5.0f);
				pInst->timer = 0.20f;
				pInst->ParticleFlag = 0;
			}

			else if (pInst->timer <= 0 && pInst->ParticleFlag != 1)
			{
				pInst->mFlag = 0;
			}
		}

		if (pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_RED_BOX)
		{
			red->timer -= frameTime;
			if (red->timer <= 0.0f)
			{
				red->mFlag = 0;
				red->timer = 1.0f;
				yellow = GameObjectInstanceCreate(OBJECT_TYPE_YELLOW_BOX);
				yellow->timer = 1.0f;
			}
		}
		if (pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_YELLOW_BOX)
		{
			yellow->timer -= frameTime;
			if (yellow->timer <= 0.0f)
			{
				yellow->mFlag = 0;
				yellow->timer = 1.0f;
				green = GameObjectInstanceCreate(OBJECT_TYPE_GREEN_BOX);
				green->timer = 1.0f;
			}
		}
		if (pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_GREEN_BOX)
		{
			green->timer -= frameTime;
			if (green->timer <= 0.0f)
			{
				green->timer = 1.0f;
				green->mFlag = 0;
				red->timer = 1.0f;
			}
		}


	}
}

void UpdateObjectPositions(GameObjectInstance* pObjList, double frameTime)
{
	for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjectInstance* pInst = pObjList + i;

		// skip non-active object
		if ((pInst->mFlag & FLAG_ACTIVE) == 0)
			continue;

		if (pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_SHIP)
		{
			//Vector2D velocity;
			//Vector2DSet(&velocity, pInst->mpComponent_Physics->mVelocity.x, pInst->mpComponent_Physics->mVelocity.y);

			pInst->mpComponent_Transform->mPosition.x += pInst->mpComponent_Physics->mVelocity.x  * (float)frameTime * SPEED;
			pInst->mpComponent_Transform->mPosition.y += pInst->mpComponent_Physics->mVelocity.y  * (float)frameTime * SPEED;
			continue;
		}

		//Vector2DNormalize(&pInst->mpComponent_Physics->mVelocity, &pInst->mpComponent_Physics->mVelocity);
		pInst->mpComponent_Transform->mPosition.x += pInst->mpComponent_Physics->mVelocity.x  * (float)frameTime;
		pInst->mpComponent_Transform->mPosition.y += pInst->mpComponent_Physics->mVelocity.y * (float)frameTime;
	}
}


	// Should put a switch statement here
void HandleInput()
{
	int i; //loop variable
		   //control ship movement/controls
	if (sgpShip->InputFlag)
	{
		if (AEInputCheckCurr(VK_UP) && sgpShip->mpComponent_Transform->mPosition.y < -100.0f)
		{
			sgpShip->mpComponent_Physics->mVelocity.y = 1.0f / 2.0f;

			for (i = 0; i < 1; i++)
			{
				ForwardTrail = GameObjectInstanceCreate(OBJECT_TYPE_WHITE_TRAIL);
				ForwardTrail->mpComponent_Transform->mPosition.x = sgpShip->mpComponent_Transform->mPosition.x + 1;
				ForwardTrail->mpComponent_Transform->mPosition.y = sgpShip->mpComponent_Transform->mPosition.y - 20;
				ForwardTrail->mpComponent_Physics->mVelocity.x = 0;
				ForwardTrail->mpComponent_Physics->mVelocity.y = -20;
				ForwardTrail->timer = 0.5f;
			}
			for (i = 0; i < 1; i++)
			{
				ForwardTrail = GameObjectInstanceCreate(OBJECT_TYPE_BLUE_TRAIL);
				ForwardTrail->mpComponent_Transform->mPosition.x = sgpShip->mpComponent_Transform->mPosition.x + 4;
				ForwardTrail->mpComponent_Transform->mPosition.y = sgpShip->mpComponent_Transform->mPosition.y - 20;
				ForwardTrail->mpComponent_Physics->mVelocity.x = 0;
				ForwardTrail->mpComponent_Physics->mVelocity.y = -20;
				ForwardTrail->timer = 0.5f;
			}
			for (i = 0; i < 1; i++)
			{
				ForwardTrail = GameObjectInstanceCreate(OBJECT_TYPE_BLUE_TRAIL);
				ForwardTrail->mpComponent_Transform->mPosition.x = sgpShip->mpComponent_Transform->mPosition.x - 3;
				ForwardTrail->mpComponent_Transform->mPosition.y = sgpShip->mpComponent_Transform->mPosition.y - 20;
				ForwardTrail->mpComponent_Physics->mVelocity.x = 0;
				ForwardTrail->mpComponent_Physics->mVelocity.y = -20;
				ForwardTrail->timer = 0.5f;
			}
			for (i = 0; i < 1; i++)
			{
				ForwardTrail = GameObjectInstanceCreate(OBJECT_TYPE_PINK_TRAIL);
				ForwardTrail->mpComponent_Transform->mPosition.x = sgpShip->mpComponent_Transform->mPosition.x + 8;
				ForwardTrail->mpComponent_Transform->mPosition.y = sgpShip->mpComponent_Transform->mPosition.y - 20;
				ForwardTrail->mpComponent_Physics->mVelocity.x = 0;
				ForwardTrail->mpComponent_Physics->mVelocity.y = -20;
				ForwardTrail->timer = 0.5f;
			}
			for (i = 0; i < 1; i++)
			{
				ForwardTrail = GameObjectInstanceCreate(OBJECT_TYPE_PINK_TRAIL);
				ForwardTrail->mpComponent_Transform->mPosition.x = sgpShip->mpComponent_Transform->mPosition.x - 7;
				ForwardTrail->mpComponent_Transform->mPosition.y = sgpShip->mpComponent_Transform->mPosition.y - 20;
				ForwardTrail->mpComponent_Physics->mVelocity.x = 0;
				ForwardTrail->mpComponent_Physics->mVelocity.y = -20;
				ForwardTrail->timer = 0.5f;
			}
		}
		else if (AEInputCheckCurr(VK_DOWN) && sgpShip->mpComponent_Transform->mPosition.y > -540.0f)
		{
			//sgpShip->mpComponent_Physics->mVelocity.y = (-50 * frameTime + sgpShip->mpComponent_Physics->mVelocity.y);
			sgpShip->mpComponent_Physics->mVelocity.y = -1.0f - 1.0f / 3.0f;
		}
		else
		{
			sgpShip->mpComponent_Physics->mVelocity.y = 0.0f;
		}

		if (AEInputCheckCurr(VK_LEFT) && sgpShip->mpComponent_Transform->mPosition.x > -558.0f)
		{
			sgpShip->mpComponent_Physics->mVelocity.x = -0.75f * 1.5f;

			for (i = 0; i < 1; i++)
			{
				ForwardTrail = GameObjectInstanceCreate(OBJECT_TYPE_WHITE_TRAIL);
				ForwardTrail->mpComponent_Transform->mPosition.x = sgpShip->mpComponent_Transform->mPosition.x + 15;
				ForwardTrail->mpComponent_Transform->mPosition.y = sgpShip->mpComponent_Transform->mPosition.y;
				ForwardTrail->mpComponent_Physics->mVelocity.x = 20;
				ForwardTrail->mpComponent_Physics->mVelocity.y = 0;
				ForwardTrail->timer = 0.5f;
			}
			for (i = 0; i < 1; i++)
			{
				ForwardTrail = GameObjectInstanceCreate(OBJECT_TYPE_BLUE_TRAIL);
				ForwardTrail->mpComponent_Transform->mPosition.x = sgpShip->mpComponent_Transform->mPosition.x + 15;
				ForwardTrail->mpComponent_Transform->mPosition.y = sgpShip->mpComponent_Transform->mPosition.y - 4;
				ForwardTrail->mpComponent_Physics->mVelocity.x = 20;
				ForwardTrail->mpComponent_Physics->mVelocity.y = 0;
				ForwardTrail->timer = 0.5f;
			}
			for (i = 0; i < 1; i++)
			{
				ForwardTrail = GameObjectInstanceCreate(OBJECT_TYPE_BLUE_TRAIL);
				ForwardTrail->mpComponent_Transform->mPosition.x = sgpShip->mpComponent_Transform->mPosition.x + 15;
				ForwardTrail->mpComponent_Transform->mPosition.y = sgpShip->mpComponent_Transform->mPosition.y + 2;
				ForwardTrail->mpComponent_Physics->mVelocity.x = 20;
				ForwardTrail->mpComponent_Physics->mVelocity.y = 0;
				ForwardTrail->timer = 0.5f;
			}
			for (i = 0; i < 1; i++)
			{
				ForwardTrail = GameObjectInstanceCreate(OBJECT_TYPE_PINK_TRAIL);
				ForwardTrail->mpComponent_Transform->mPosition.x = sgpShip->mpComponent_Transform->mPosition.x + 15;
				ForwardTrail->mpComponent_Transform->mPosition.y = sgpShip->mpComponent_Transform->mPosition.y + 5;
				ForwardTrail->mpComponent_Physics->mVelocity.x = 20;
				ForwardTrail->mpComponent_Physics->mVelocity.y = 0;
				ForwardTrail->timer = 0.5f;
			}
			for (i = 0; i < 1; i++)
			{
				ForwardTrail = GameObjectInstanceCreate(OBJECT_TYPE_PINK_TRAIL);
				ForwardTrail->mpComponent_Transform->mPosition.x = sgpShip->mpComponent_Transform->mPosition.x + 15;
				ForwardTrail->mpComponent_Transform->mPosition.y = sgpShip->mpComponent_Transform->mPosition.y - 9;
				ForwardTrail->mpComponent_Physics->mVelocity.x = 20;
				ForwardTrail->mpComponent_Physics->mVelocity.y = 0;
				ForwardTrail->timer = 0.5f;
			}
		}
		else if (AEInputCheckCurr(VK_RIGHT) && sgpShip->mpComponent_Transform->mPosition.x < 558.0f)
		{
			sgpShip->mpComponent_Physics->mVelocity.x = 0.75f * 1.5f;

			for (i = 0; i < 1; i++)
			{
				ForwardTrail = GameObjectInstanceCreate(OBJECT_TYPE_WHITE_TRAIL);
				ForwardTrail->mpComponent_Transform->mPosition.x = sgpShip->mpComponent_Transform->mPosition.x - 15;
				ForwardTrail->mpComponent_Transform->mPosition.y = sgpShip->mpComponent_Transform->mPosition.y - 4;
				ForwardTrail->mpComponent_Physics->mVelocity.x = -20;
				ForwardTrail->mpComponent_Physics->mVelocity.y = 0;
				ForwardTrail->timer = 0.5f;
			}
			for (i = 0; i < 1; i++)
			{
				ForwardTrail = GameObjectInstanceCreate(OBJECT_TYPE_BLUE_TRAIL);
				ForwardTrail->mpComponent_Transform->mPosition.x = sgpShip->mpComponent_Transform->mPosition.x - 15;
				ForwardTrail->mpComponent_Transform->mPosition.y = sgpShip->mpComponent_Transform->mPosition.y;
				ForwardTrail->mpComponent_Physics->mVelocity.x = -20;
				ForwardTrail->mpComponent_Physics->mVelocity.y = 0;
				ForwardTrail->timer = 0.5f;
			}
			for (i = 0; i < 1; i++)
			{
				ForwardTrail = GameObjectInstanceCreate(OBJECT_TYPE_BLUE_TRAIL);
				ForwardTrail->mpComponent_Transform->mPosition.x = sgpShip->mpComponent_Transform->mPosition.x - 15;
				ForwardTrail->mpComponent_Transform->mPosition.y = sgpShip->mpComponent_Transform->mPosition.y - 6;
				ForwardTrail->mpComponent_Physics->mVelocity.x = -20;
				ForwardTrail->mpComponent_Physics->mVelocity.y = 0;
				ForwardTrail->timer = 0.5f;
			}
			for (i = 0; i < 1; i++)
			{
				ForwardTrail = GameObjectInstanceCreate(OBJECT_TYPE_PINK_TRAIL);
				ForwardTrail->mpComponent_Transform->mPosition.x = sgpShip->mpComponent_Transform->mPosition.x - 15;
				ForwardTrail->mpComponent_Transform->mPosition.y = sgpShip->mpComponent_Transform->mPosition.y - 9;
				ForwardTrail->mpComponent_Physics->mVelocity.x = -20;
				ForwardTrail->mpComponent_Physics->mVelocity.y = 0;
				ForwardTrail->timer = 0.5f;
			}
			for (i = 0; i < 1; i++)
			{
				ForwardTrail = GameObjectInstanceCreate(OBJECT_TYPE_PINK_TRAIL);
				ForwardTrail->mpComponent_Transform->mPosition.x = sgpShip->mpComponent_Transform->mPosition.x - 15;
				ForwardTrail->mpComponent_Transform->mPosition.y = sgpShip->mpComponent_Transform->mPosition.y + 5;
				ForwardTrail->mpComponent_Physics->mVelocity.x = -20;
				ForwardTrail->mpComponent_Physics->mVelocity.y = 0;
				ForwardTrail->timer = 0.5f;
			}
		}
		else
		{
			sgpShip->mpComponent_Physics->mVelocity.x = 0.0f;
		}

		if (CurrentLevel == GS_ASTEROIDS_START_SCREEN)
		{
			if (switchscreen_timer->doneCounting == DONE_COUNTING)
			{
				if (AEInputCheckTriggered(VK_SPACE))
				{
					Vector2D bullet;
					Vector2DSet(&bullet, 0.0f, BULLET_SPEED);
					BulletCreate(sgpShip->mpComponent_Transform->mPosition, bullet, sgpShip->mpComponent_Transform->mAngle, OBJECT_TYPE_BULLET);
				}
			}
		}
	}

	/*shoot homing missile
	if (AEInputCheckTriggered('M'))
	{
		GameObjectInstance * missile = GameObjectInstanceCreate(OBJECT_TYPE_HOMING_MISSILE);

		missile->mpComponent_Transform->mPosition.x = sgpShip->mpComponent_Transform->mPosition.x;
		missile->mpComponent_Transform->mPosition.y = sgpShip->mpComponent_Transform->mPosition.y + 30.0f;

		missile->mpComponent_Transform->mAngle = sgpShip->mpComponent_Transform->mAngle;

		Vector2DSet(&missile->mpComponent_Physics->mVelocity, cosf(missile->mpComponent_Transform->mAngle) * BULLET_SPEED, sinf(missile->mpComponent_Transform->mAngle) * BULLET_SPEED);
	}*/
}

void LoseALife()
{
	int i;
	sgpShip->mpComponent_Transform->mPosition.x = 0.0f;
	sgpShip->mpComponent_Transform->mPosition.y = -200.0f;
	sgpShip->mpComponent_Physics->mVelocity.x = 0.0f;
	sgpShip->mpComponent_Physics->mVelocity.y = 0.0f;

	sgShipLives--;

	//Death sound
	FMOD_System_PlaySound(FmodSystem, sound2, 0, 0, &channel);

	EndGame();

	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjectInstance * object3 = sgGameObjectInstanceList + i;

		if ((object3->mFlag & FLAG_ACTIVE) == 0)
			continue;
		if (object3->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_LIFE)
		{
			GameObjectInstanceDestroy(object3);
			break;
		}
	}

	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjectInstance *halfHealth = sgGameObjectInstanceList + i;

		if ((halfHealth->mFlag & FLAG_ACTIVE) == 0)
			continue;

		if (halfHealth->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_HEALTH_HALF)
		{
			GameObjectInstance *fullHealth = GameObjectInstanceCreate(OBJECT_TYPE_HEALTH_FULL);
			AddToObject(fullHealth, -680, 225, 0, 0, 25, 25, 0);

			GameObjectInstanceDestroy(halfHealth);


		}
	}
}

void EndGame()
{
	if (sgShipLives < 1)
	{
		ChangeLevel(CurrentLevel);
	}
}

void Invincibility()
{
	if (sgpShip->mHit & FLAG_ACTIVE)
	{
		sgpShip->invinceTime -= frameTime;
	}
	if (sgpShip->invinceTime <= 0)
	{
		sgpShip->mHit = 0;
		sgpShip->InputFlag = 1;
	}
}

void IsHit()
{
	int i;
	sgpShip->mHit = FLAG_ACTIVE;
	sgpShip->invinceTime = HEALTH_INVINCE;
	sgShipHealth--;

	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjectInstance *fullHealth = sgGameObjectInstanceList + i;

		if ((fullHealth->mFlag & FLAG_ACTIVE) == 0)
			continue;

		if (fullHealth->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_HEALTH_FULL)
		{
			GameObjectInstance *halfHealth = GameObjectInstanceCreate(OBJECT_TYPE_HEALTH_HALF);
			AddToObject(halfHealth, -680, 225, 0, 0, 25, 25, 0);

			GameObjectInstanceDestroy(fullHealth);


		}
	}

	if (sgShipHealth <= 0)
	{
		sgpShip->invinceTime = DEATH_INVINCE;
		sgpShip->InputFlag = 0;
		red = GameObjectInstanceCreate(OBJECT_TYPE_RED_BOX);
		red->timer = 1.0f;
		LoseALife();
		sgShipHealth = MAX_HEALTH;
	}
}

void DrawTexture(int objType, AEGfxTexture *texture)
{
	int i;
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjectInstance* pInst = sgGameObjectInstanceList + i;

		// skip non-active object
		if ((pInst->mFlag & FLAG_ACTIVE) == 0)
			continue;

		else if (pInst->mpComponent_Sprite->mpShape->mType == objType)
		{
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxTextureSet(texture, 0.0f, 0.0f);
			AEGfxSetTransform(pInst->mpComponent_Transform->mTransform.m);
			AEGfxMeshDraw(pInst->mpComponent_Sprite->mpShape->mpMesh, AE_GFX_MDM_TRIANGLES);
		}

	}
}



void DrawColor(int objType)
{
	int i;
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjectInstance* pInst = sgGameObjectInstanceList + i;

		// skip non-active object
		if ((pInst->mFlag & FLAG_ACTIVE) == 0)
			continue;

		if (pInst->mpComponent_Sprite->mpShape->mType == objType)
		{
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
			AEGfxTextureSet(NULL, 0, 0);
			AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
			AEGfxSetTransform(pInst->mpComponent_Transform->mTransform.m);
			AEGfxMeshDraw(pInst->mpComponent_Sprite->mpShape->mpMesh, AE_GFX_MDM_TRIANGLES);
		}

	}
}

void DrawEnemy()
{

	int i, j;

	for (i = 0; i < NUM_SPAWNPOINTS; i++)
	{
		EnemyInstance*	pInst;

		// Skip unused spawn point
		if ((spawnPointList[i].active & FLAG_ACTIVE) == 0)
			continue;

		// skip empty spawn point
		if (spawnPointList[i].flag == SEEKING_MOB)
			continue;

		// Instruct special spawn
		if (spawnPointList[i].spID == MULTI_SPAWNER)
		{
			// Update the timer for deployment delay
			UpdateTimer(spawnPointList[i].SpawnTimer);

			for (j = 0; j < spawnPointList[i].numEnemies; j++)
			{
				pInst = *(spawnPointList[i].pEnemyList) + j;

				// skip non-active object
				if ((pInst->mFlag & FLAG_ACTIVE) == 0)
					continue;

				if (pInst->mpComponent_Sprite->mpShape->mType == ENEMY_TYPE_REGULAR)
				{
					AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
					AEGfxTextureSet(pEnemyTex, 0.0f, 0.0f);
					AEGfxSetTransform(pInst->mpComponent_Transform->mTransform.m);
					AEGfxMeshDraw(pInst->mpComponent_Sprite->mpShape->mpMesh, AE_GFX_MDM_TRIANGLES);
				}

				if (pInst->mpComponent_Sprite->mpShape->mType == ENEMY_TYPE_MINION)
				{
					AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
					AEGfxTextureSet(pMinionTex, 0.0f, 0.0f);
					AEGfxSetTransform(pInst->mpComponent_Transform->mTransform.m);
					AEGfxMeshDraw(pInst->mpComponent_Sprite->mpShape->mpMesh, AE_GFX_MDM_TRIANGLES);
				}

				if (pInst->mpComponent_Sprite->mpShape->mType == ENEMY_TYPE_WORLD2)
				{
					AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
					AEGfxTextureSet(pMinionTex, 0.0f, 0.0f);
					AEGfxSetTransform(pInst->mpComponent_Transform->mTransform.m);
					AEGfxMeshDraw(pInst->mpComponent_Sprite->mpShape->mpMesh, AE_GFX_MDM_TRIANGLES);
				}

				if (pInst->mpComponent_Sprite->mpShape->mType == ENEMY_TYPE_WORLD4)
				{
					AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
					AEGfxTextureSet(pMinionTex, 0.0f, 0.0f);
					AEGfxSetTransform(pInst->mpComponent_Transform->mTransform.m);
					AEGfxMeshDraw(pInst->mpComponent_Sprite->mpShape->mpMesh, AE_GFX_MDM_TRIANGLES);
				}

				else
				{
					AEGfxSetRenderMode(AE_GFX_RM_COLOR);
					AEGfxTextureSet(NULL, 0, 0);
					AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
					AEGfxSetTransform(pInst->mpComponent_Transform->mTransform.m);
					AEGfxMeshDraw(pInst->mpComponent_Sprite->mpShape->mpMesh, AE_GFX_MDM_TRIANGLES);
				}
			}

			continue;
		}

		pInst = spawnPointList[i].pEnemy;

		// skip non-active object
		if ((pInst->mFlag & FLAG_ACTIVE) == 0)
			continue;

		else if (pInst->mpComponent_Sprite->mpShape->mType == ENEMY_TYPE_REGULAR)
		{
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxTextureSet(pEnemyTex, 0.0f, 0.0f);
			AEGfxSetTransform(pInst->mpComponent_Transform->mTransform.m);
			AEGfxMeshDraw(pInst->mpComponent_Sprite->mpShape->mpMesh, AE_GFX_MDM_TRIANGLES);
		}

		else if (pInst->mpComponent_Sprite->mpShape->mType == ENEMY_TYPE_MINION)
		{
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxTextureSet(pMinionTex, 0.0f, 0.0f);
			AEGfxSetTransform(pInst->mpComponent_Transform->mTransform.m);
			AEGfxMeshDraw(pInst->mpComponent_Sprite->mpShape->mpMesh, AE_GFX_MDM_TRIANGLES);
		}

		else if (pInst->mpComponent_Sprite->mpShape->mType == ENEMY_TYPE_WORLD2)
		{
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxTextureSet(pMinionTex, 0.0f, 0.0f);
			AEGfxSetTransform(pInst->mpComponent_Transform->mTransform.m);
			AEGfxMeshDraw(pInst->mpComponent_Sprite->mpShape->mpMesh, AE_GFX_MDM_TRIANGLES);
		}

		else if (pInst->mpComponent_Sprite->mpShape->mType == ENEMY_TYPE_WORLD4)
		{
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxTextureSet(pMinionTex, 0.0f, 0.0f);
			AEGfxSetTransform(pInst->mpComponent_Transform->mTransform.m);
			AEGfxMeshDraw(pInst->mpComponent_Sprite->mpShape->mpMesh, AE_GFX_MDM_TRIANGLES);
		}

		else
		{
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
			AEGfxTextureSet(NULL, 0, 0);
			AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
			AEGfxSetTransform(pInst->mpComponent_Transform->mTransform.m);
			AEGfxMeshDraw(pInst->mpComponent_Sprite->mpShape->mpMesh, AE_GFX_MDM_TRIANGLES);
		}
	}
}

void DrawTrail()
{
	int i;
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjectInstance* pInst = sgGameObjectInstanceList + i;

		// skip non-active object
		if ((pInst->mFlag & FLAG_ACTIVE) == 0)
			continue;

		else if (pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_BLUE_TRAIL
			|| pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_PINK_TRAIL
			|| pInst->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_WHITE_TRAIL)
		{
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
			AEGfxTextureSet(NULL, 0, 0);
			AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
			AEGfxSetTransform(pInst->mpComponent_Transform->mTransform.m);
			AEGfxMeshDraw(pInst->mpComponent_Sprite->mpShape->mpMesh, AE_GFX_MDM_TRIANGLES);
		}
	}
}

void DrawMap()
{
	switch (CurrentLevel)
	{
	case GS_ASTEROIDS_LEVEL1:
		DrawTexture(OBJECT_TYPE_MAP, p11Tex);
		break;

	case GS_ASTEROIDS_LEVEL2:
		DrawTexture(OBJECT_TYPE_MAP, p12Tex);
		break;

	case GS_ASTEROIDS_LEVEL3:
		DrawTexture(OBJECT_TYPE_MAP, p13Tex);
		break;

	case GS_ASTEROIDS_LEVEL4:
		DrawTexture(OBJECT_TYPE_MAP, p21Tex);
		break;

	case GS_ASTEROIDS_LEVEL5:
		DrawTexture(OBJECT_TYPE_MAP, p22Tex);
		break;

	case GS_ASTEROIDS_LEVEL6:
		DrawTexture(OBJECT_TYPE_MAP, p23Tex);
		break;

	case GS_ASTEROIDS_LEVEL7:
		DrawTexture(OBJECT_TYPE_MAP, p31Tex);
		break;

	case GS_ASTEROIDS_LEVEL8:
		DrawTexture(OBJECT_TYPE_MAP, p32Tex);
		break;

	case GS_ASTEROIDS_LEVEL9:
		DrawTexture(OBJECT_TYPE_MAP, p33Tex);
		break;

	case GS_ASTEROIDS_LEVEL10:
		DrawTexture(OBJECT_TYPE_MAP, p41Tex);
		break;

	case GS_ASTEROIDS_LEVEL11:
		DrawTexture(OBJECT_TYPE_MAP, p42Tex);
		break;

	case GS_ASTEROIDS_LEVEL12:
		DrawTexture(OBJECT_TYPE_MAP, p43Tex);
		break;
	}
}

void AnimatePlayer(int objType, AEGfxTexture *texture)
{
	int i;
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjectInstance* pInst = sgGameObjectInstanceList + i;

		// skip non-active object
		if ((pInst->mFlag & FLAG_ACTIVE) == 0)
			continue;

		else if (pInst->mpComponent_Sprite->mpShape->mType == objType)
		{
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxTextureSet(texture, textureOffsets[currentTextureOffset].mX, textureOffsets[currentTextureOffset].mY);
			AEGfxSetTransform(pInst->mpComponent_Transform->mTransform.m);
			AEGfxMeshDraw(pInst->mpComponent_Sprite->mpShape->mpMesh, AE_GFX_MDM_TRIANGLES);
		}

	}
}

void AnimateBoss(int objType, AEGfxTexture *texture)
{
	int i;
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjectInstance* pInst = sgGameObjectInstanceList + i;

		// skip non-active object
		if ((pInst->mFlag & FLAG_ACTIVE) == 0)
			continue;

		else if (pInst->mpComponent_Sprite->mpShape->mType == objType)
		{
			switch (CurrentLevel)
			{

			case GS_ASTEROIDS_LEVEL3:
				AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
				AEGfxTextureSet(pBoss1Tex, textureOffsets[currentBossOffset].mX, textureOffsets[currentBossOffset].mY);
				AEGfxSetTransform(pInst->mpComponent_Transform->mTransform.m);
				AEGfxMeshDraw(pInst->mpComponent_Sprite->mpShape->mpMesh, AE_GFX_MDM_TRIANGLES);
				break;

			case GS_ASTEROIDS_LEVEL6:
				DrawTexture(OBJECT_TYPE_BOSS_FLASH, pBoss2Tex);
				break;

			case GS_ASTEROIDS_LEVEL9:
				DrawTexture(OBJECT_TYPE_BOSS_FLASH, pBoss3Tex);
				break;

			case GS_ASTEROIDS_LEVEL12:
				DrawTexture(OBJECT_TYPE_BOSS_FLASH, pBoss4Tex);
				break;
			}
		}

	}
}

void DrawFlash()
{
	switch (CurrentLevel)
	{
	case GS_ASTEROIDS_LEVEL3:
		DrawTexture(OBJECT_TYPE_BOSS_FLASH, pBossFlash1Tex);
		break;

	case GS_ASTEROIDS_LEVEL6:
		DrawTexture(OBJECT_TYPE_BOSS_FLASH, pBossFlash2Tex);
		break;

	case GS_ASTEROIDS_LEVEL9:
		DrawTexture(OBJECT_TYPE_BOSS_FLASH, pBossFlash3Tex);
		break;

	case GS_ASTEROIDS_LEVEL12:
		DrawTexture(OBJECT_TYPE_BOSS_FLASH, pBossFlash4Tex);
		break;
	}
}

void CreateStars()
{
	//LAYER 1 STARS
	GameObjectInstance * l1star1 = GameObjectInstanceCreate(OBJECT_TYPE_STAR1);
	GameObjectInstance * l1star2 = GameObjectInstanceCreate(OBJECT_TYPE_STAR2);
	GameObjectInstance * l1star3 = GameObjectInstanceCreate(OBJECT_TYPE_STAR3);
	GameObjectInstance * l1star4 = GameObjectInstanceCreate(OBJECT_TYPE_STAR1);
	GameObjectInstance * l1star5 = GameObjectInstanceCreate(OBJECT_TYPE_STAR2);
	GameObjectInstance * l1star6 = GameObjectInstanceCreate(OBJECT_TYPE_STAR3);
	GameObjectInstance * l1star7 = GameObjectInstanceCreate(OBJECT_TYPE_STAR1);
	GameObjectInstance * l1star8 = GameObjectInstanceCreate(OBJECT_TYPE_STAR2);
	GameObjectInstance * l1star9 = GameObjectInstanceCreate(OBJECT_TYPE_STAR3);
	GameObjectInstance * l1star10 = GameObjectInstanceCreate(OBJECT_TYPE_STAR1);
	GameObjectInstance * l1star11 = GameObjectInstanceCreate(OBJECT_TYPE_STAR2);
	GameObjectInstance * l1star12 = GameObjectInstanceCreate(OBJECT_TYPE_STAR3);
	GameObjectInstance * l1star13 = GameObjectInstanceCreate(OBJECT_TYPE_STAR1);
	GameObjectInstance * l1star14 = GameObjectInstanceCreate(OBJECT_TYPE_STAR2);
	GameObjectInstance * l1star15 = GameObjectInstanceCreate(OBJECT_TYPE_STAR3);
	GameObjectInstance * l1star16 = GameObjectInstanceCreate(OBJECT_TYPE_STAR1);
	GameObjectInstance * l1star17 = GameObjectInstanceCreate(OBJECT_TYPE_STAR2);
	GameObjectInstance * l1star18 = GameObjectInstanceCreate(OBJECT_TYPE_STAR3);
	GameObjectInstance * l1star19 = GameObjectInstanceCreate(OBJECT_TYPE_STAR1);
	GameObjectInstance * l1star20 = GameObjectInstanceCreate(OBJECT_TYPE_STAR2);

	//LAYER 2 STARS
	GameObjectInstance * l2star1 = GameObjectInstanceCreate(OBJECT_TYPE_STAR1);
	GameObjectInstance * l2star2 = GameObjectInstanceCreate(OBJECT_TYPE_STAR2);
	GameObjectInstance * l2star3 = GameObjectInstanceCreate(OBJECT_TYPE_STAR3);
	GameObjectInstance * l2star4 = GameObjectInstanceCreate(OBJECT_TYPE_STAR1);
	GameObjectInstance * l2star5 = GameObjectInstanceCreate(OBJECT_TYPE_STAR2);
	GameObjectInstance * l2star6 = GameObjectInstanceCreate(OBJECT_TYPE_STAR3);
	GameObjectInstance * l2star7 = GameObjectInstanceCreate(OBJECT_TYPE_STAR1);
	GameObjectInstance * l2star8 = GameObjectInstanceCreate(OBJECT_TYPE_STAR2);
	GameObjectInstance * l2star9 = GameObjectInstanceCreate(OBJECT_TYPE_STAR3);
	GameObjectInstance * l2star10 = GameObjectInstanceCreate(OBJECT_TYPE_STAR1);
	GameObjectInstance * l2star11 = GameObjectInstanceCreate(OBJECT_TYPE_STAR2);
	GameObjectInstance * l2star12 = GameObjectInstanceCreate(OBJECT_TYPE_STAR3);
	GameObjectInstance * l2star13 = GameObjectInstanceCreate(OBJECT_TYPE_STAR1);
	GameObjectInstance * l2star14 = GameObjectInstanceCreate(OBJECT_TYPE_STAR2);
	GameObjectInstance * l2star15 = GameObjectInstanceCreate(OBJECT_TYPE_STAR3);
	GameObjectInstance * l2star16 = GameObjectInstanceCreate(OBJECT_TYPE_STAR1);
	GameObjectInstance * l2star17 = GameObjectInstanceCreate(OBJECT_TYPE_STAR2);
	GameObjectInstance * l2star18 = GameObjectInstanceCreate(OBJECT_TYPE_STAR3);
	GameObjectInstance * l2star19 = GameObjectInstanceCreate(OBJECT_TYPE_STAR1);
	GameObjectInstance * l2star20 = GameObjectInstanceCreate(OBJECT_TYPE_STAR2);

	//LAYER 3 STARS
	GameObjectInstance * l3star1 = GameObjectInstanceCreate(OBJECT_TYPE_STAR1);
	GameObjectInstance * l3star2 = GameObjectInstanceCreate(OBJECT_TYPE_STAR2);
	GameObjectInstance * l3star3 = GameObjectInstanceCreate(OBJECT_TYPE_STAR3);
	GameObjectInstance * l3star4 = GameObjectInstanceCreate(OBJECT_TYPE_STAR1);
	GameObjectInstance * l3star5 = GameObjectInstanceCreate(OBJECT_TYPE_STAR2);
	GameObjectInstance * l3star6 = GameObjectInstanceCreate(OBJECT_TYPE_STAR3);
	GameObjectInstance * l3star7 = GameObjectInstanceCreate(OBJECT_TYPE_STAR1);
	GameObjectInstance * l3star8 = GameObjectInstanceCreate(OBJECT_TYPE_STAR2);
	GameObjectInstance * l3star9 = GameObjectInstanceCreate(OBJECT_TYPE_STAR3);
	GameObjectInstance * l3star10 = GameObjectInstanceCreate(OBJECT_TYPE_STAR1);
	GameObjectInstance * l3star11 = GameObjectInstanceCreate(OBJECT_TYPE_STAR2);
	GameObjectInstance * l3star12 = GameObjectInstanceCreate(OBJECT_TYPE_STAR3);
	GameObjectInstance * l3star13 = GameObjectInstanceCreate(OBJECT_TYPE_STAR1);
	GameObjectInstance * l3star14 = GameObjectInstanceCreate(OBJECT_TYPE_STAR2);
	GameObjectInstance * l3star15 = GameObjectInstanceCreate(OBJECT_TYPE_STAR3);
	GameObjectInstance * l3star16 = GameObjectInstanceCreate(OBJECT_TYPE_STAR1);
	GameObjectInstance * l3star17 = GameObjectInstanceCreate(OBJECT_TYPE_STAR2);
	GameObjectInstance * l3star18 = GameObjectInstanceCreate(OBJECT_TYPE_STAR3);
	GameObjectInstance * l3star19 = GameObjectInstanceCreate(OBJECT_TYPE_STAR1);
	GameObjectInstance * l3star20 = GameObjectInstanceCreate(OBJECT_TYPE_STAR2);

	///////////////////////////////////////////////////////////


	//LAYER 1 STAR IMPLEMENTATION//////////////////////////////
	///////////////////////////////////////////////////////////

	AddToObject(l1star1, 300, 200, 0, -200.0f, 2, 2, 0.0f);
	AddToObject(l1star2, -300, 100, 0, -200.0f, 2, 2, 0.0f);
	AddToObject(l1star3, -100, 200, 0, -200.0f, 2, 2, 0.0f);
	AddToObject(l1star4, 200, -200, 0, -200.0f, 2, 2, 0.0f);
	AddToObject(l1star5, 400, -200, 0, -200.0f, 2, 2, 0.0f);
	AddToObject(l1star6, -200, -200, 0, -200.0f, 2, 2, 0.0f);
	AddToObject(l1star7, -100, 200, 0, -200.0f, 2, 2, 0.0f);
	AddToObject(l1star8, 150, -400, 0, -200.0f, 2, 2, 0.0f);
	AddToObject(l1star9, 50, -50, 0, -200.0f, 2, 2, 0.0f);
	AddToObject(l1star10, 100, -50, 0, -200.0f, 2, 2, 0.0f);
	AddToObject(l1star11, 600, -50, 0, -200.0f, 2, 2, 0.0f);
	AddToObject(l1star12, -900, 400, 0, -200.0f, 2, 2, 0.0f);
	AddToObject(l1star13, 800, 100, 0, -200.0f, 2, 2, 0.0f);
	AddToObject(l1star14, 600, 70, 0, -200.0f, 2, 2, 0.0f);
	AddToObject(l1star15, -700, -300, 0, -200.0f, 2, 2, 0.0f);
	AddToObject(l1star16, 800, -300, 0, -200.0f, 2, 2, 0.0f);
	AddToObject(l1star17, 1000, 200, 0, -200.0f, 2, 2, 0.0f);
	AddToObject(l1star18, -500, 300, 0, -200.0f, 2, 2, 0.0f);
	AddToObject(l1star19, -850, -300, 0, -200.0f, 2, 2, 0.0f);
	AddToObject(l1star20, -600, 200, 0, -200.0f, 2, 2, 0.0f);

	//LAYER 2 STAR IMPLEMENTATION////////////////////////////
	/////////////////////////////////////////////////////////

	AddToObject(l2star1, 125, 120, 0, -250.0f, 2.5, 2.5 , 0.0f);
	AddToObject(l2star2, -210, 50, 0, -250.0f, 2.5, 2.5 , 0.0f);
	AddToObject(l2star3, -300, 400, 0, -250.0f, 2.5, 2.5 , 0.0f);
	AddToObject(l2star4, 100, -200, 0, -250.0f, 2.5, 2.5 , 0.0f);
	AddToObject(l2star5, 425, -275, 0, -250.0f, 2.5, 2.5 , 0.0f);
	AddToObject(l2star6, -230, -100, 0, -250.0f, 2.5, 2.5 , 0.0f);
	AddToObject(l2star7, -100, 150, 0, -250.0f, 2.5, 2.5 , 0.0f);
	AddToObject(l2star8, 100, -400, 0, -250.0f, 2.5, 2.5 , 0.0f);
	AddToObject(l2star9, 350, -50, 0, -250.0f, 2.5, 2.5 , 0.0f);
	AddToObject(l2star10, 250, 50, 0, -250.0f, 2.5, 2.5 , 0.0f);
	AddToObject(l2star11, 500, 420, 0, -250.0f, 2.5, 2.5 , 0.0f);
	AddToObject(l2star12, -625, 120, 0, -250.0f, 2.5, 2.5 , 0.0f);
	AddToObject(l2star13, 900, 300, 0, -250.0f, 2.5, 2.5 , 0.0f);
	AddToObject(l2star14, -800, 520, 0, -250.0f, 2.5, 2.5 , 0.0f);
	AddToObject(l2star15, -750, 325, 0, -250.0f, 2.5, 2.5 , 0.0f);
	AddToObject(l2star16, 825, 300, 0, -250.0f, 2.5, 2.5 , 0.0f);
	AddToObject(l2star17, -1000, 120, 0, -250.0f, 2.5, 2.5 , 0.0f);
	AddToObject(l2star18, 600, 0, 0, -250.0f, 2.5, 2.5 , 0.0f);
	AddToObject(l2star19, 1000, 0, 0, -250.0f, 2.5, 2.5 , 0.0f);
	AddToObject(l2star20, -600, 320, 0, -250.0f, 2.5, 2.5 , 0.0f);

	//LAYER 3 STAR IMPLEMENTATION////////////////////////////
	/////////////////////////////////////////////////////////

	AddToObject(l3star1, 25, 420, 0, -300.0f, 2.8, 2.8, 0.0f);
	AddToObject(l3star2, 325, 220, 0, -300.0f, 2.8, 2.8, 0.0f);
	AddToObject(l3star3, -190, 50, 0, -300.0f, 2.8, 2.8, 0.0f);
	AddToObject(l3star4, 400, 400, 0, -300.0f, 2.8, 2.8, 0.0f);
	AddToObject(l3star5, -200, 120, 0, -300.0f, 2.8, 2.8, 0.0f);
	AddToObject(l3star6, -150, -150, 0, -300.0f, 2.8, 2.8, 0.0f);
	AddToObject(l3star7, 125, 120, 0, -300.0f, 2.8, 2.8, 0.0f);
	AddToObject(l3star8, 250, 0, 0, -300.0f, 2.8, 2.8, 0.0f);
	AddToObject(l3star9, 300, 150, 0, -300.0f, 2.8, 2.8, 0.0f);
	AddToObject(l3star10, 0, -120, 0, -300.0f, 2.8, 2.8, 0.0f);
	AddToObject(l3star11, 700, 420, 0, -300.0f, 2.8, 2.8, 0.0f);
	AddToObject(l3star12, -700, 120, 0, -300.0f, 2.8, 2.8, 0.0f);
	AddToObject(l3star13, 425, 0, 0, -300.0f, 2.8, 2.8, 0.0f);
	AddToObject(l3star14, -900, -300, 0, -300.0f, 2.8, 2.8, 0.0f);
	AddToObject(l3star15, -500, -20, 0, -300.0f, 2.8, 2.8, 0.0f);
	AddToObject(l3star16, 900, 500, 0, -300.0f, 2.8, 2.8, 0.0f);
	AddToObject(l3star17, 600, -300, 0, -300.0f, 2.8, 2.8, 0.0f);
	AddToObject(l3star18, -750, 120, 0, -300.0f, 2.8, 2.8, 0.0f);
	AddToObject(l3star19, 825, 500, 0, -300.0f, 2.8, 2.8, 0.0f);
	AddToObject(l3star20, 500, 320, 0, -300.0f, 2.8, 2.8, 0.0f);
}

void CreateHUDObjects()
{
	GameObjectInstance * LeftSide = GameObjectInstanceCreate(OBJECT_TYPE_LSIDE);
	GameObjectInstance * RightSide = GameObjectInstanceCreate(OBJECT_TYPE_RSIDE);

	GameObjectInstance * life1 = GameObjectInstanceCreate(OBJECT_TYPE_LIFE);
	GameObjectInstance * life2 = GameObjectInstanceCreate(OBJECT_TYPE_LIFE);
	GameObjectInstance * life3 = GameObjectInstanceCreate(OBJECT_TYPE_LIFE);

	GameObjectInstance * healthFull = GameObjectInstanceCreate(OBJECT_TYPE_HEALTH_FULL);

	GameObjectInstance * map = GameObjectInstanceCreate(OBJECT_TYPE_MAP);

	AddToObject(life3, -880, 400, 0, 0, SHIP_SIZE, SHIP_SIZE, PI / 2.0f);
	AddToObject(life2, -780, 400, 0, 0, SHIP_SIZE, SHIP_SIZE, PI / 2.0f);
	AddToObject(life1, -680, 400, 0, 0, SHIP_SIZE, SHIP_SIZE, PI / 2.0f);

	AddToObject(LeftSide, -770, 0, 0, 0, 204, 76, PI / 2.0f);

	AddToObject(RightSide, 770, 0, 0, 0, 204, 76, PI / -2.0f);

	AddToObject(healthFull, -680, 225, 0, 0, 25, 25, 0);

	AddToObject(map, 770, 350, 0, 0, 50, 50, 0);
}

void DrawHUDTextures()
{
	DrawTexture(OBJECT_TYPE_LSIDE, pLeftSideTex);
	DrawTexture(OBJECT_TYPE_RSIDE, pRightSideTex);
	DrawTexture(OBJECT_TYPE_LIFE, pLifeTex);
	DrawTexture(OBJECT_TYPE_HEALTH_FULL, pHealthFullTex);
	DrawTexture(OBJECT_TYPE_HEALTH_HALF, pHealthHalfTex);
	DrawMap();
}



GameObjectInstance* InitializeShip()
{
	GameObjectInstance* pShip = GameObjectInstanceCreate(OBJECT_TYPE_SHIP);

	sgScore = 0;
	sgShipLives = MAX_LIVES;
	sgShipHealth = MAX_HEALTH;
	
	fire = ConstructTimer(1.0);

	return pShip;
}

GameObjectInstance* CreateButtonObject(unsigned int buttonID)
{
		// Create the button
	GameObjectInstance* pButton = GameObjectInstanceCreate(OBJECT_TYPE_START_BUTTON);

		// Modify it
	pButton->buttonID = buttonID;
	Vector2DSet(&pButton->mpComponent_Transform->mPosition, 0.0f, 0.0f);
	pButton->mpComponent_Transform->mScaleX = 50.0f;
	pButton->mpComponent_Transform->mScaleY = 20.0f;

	return pButton;
}



void ChangeLevel(int level)
{
	switch (level)
	{
		// Level 1
	case GS_ASTEROIDS_LEVEL1:
	{
		NextLevel = GS_ASTEROIDS_LEVEL1;
		gGameStateNext = GS_RESTART;
		changeLevelFlag = 1;
		break;
	}

	case GS_ASTEROIDS_LEVEL2:
	{
		NextLevel = GS_ASTEROIDS_LEVEL2;
		gGameStateNext = GS_RESTART;
		changeLevelFlag = 1;
		break;
	}


	case GS_ASTEROIDS_LEVEL3:
	{
		NextLevel = GS_ASTEROIDS_LEVEL3;
		gGameStateNext = GS_RESTART;
		changeLevelFlag = 1;
		break;
	}

	case GS_ASTEROIDS_LEVEL4:
	{
		NextLevel = GS_ASTEROIDS_LEVEL4;
		gGameStateNext = GS_RESTART;
		changeLevelFlag = 1;
		break;
	}

	case GS_ASTEROIDS_LEVEL5:
	{
		NextLevel = GS_ASTEROIDS_LEVEL5;
		gGameStateNext = GS_RESTART;
		changeLevelFlag = 1;
		break;
	}

	case GS_ASTEROIDS_LEVEL6:
	{
		NextLevel = GS_ASTEROIDS_LEVEL6;
		gGameStateNext = GS_RESTART;
		changeLevelFlag = 1;
		break;
	}

	case GS_ASTEROIDS_LEVEL7:
	{
		NextLevel = GS_ASTEROIDS_LEVEL7;
		gGameStateNext = GS_RESTART;
		changeLevelFlag = 1;
		break;
	}

	case GS_ASTEROIDS_LEVEL8:
	{
		NextLevel = GS_ASTEROIDS_LEVEL8;
		gGameStateNext = GS_RESTART;
		changeLevelFlag = 1;
		break;
	}

	case GS_ASTEROIDS_LEVEL9:
	{
		NextLevel = GS_ASTEROIDS_LEVEL9;
		gGameStateNext = GS_RESTART;
		changeLevelFlag = 1;
		break;
	}

	case GS_ASTEROIDS_LEVEL10:
	{
		NextLevel = GS_ASTEROIDS_LEVEL10;
		gGameStateNext = GS_RESTART;
		changeLevelFlag = 1;
		break;
	}

	case GS_ASTEROIDS_LEVEL11:
	{
		NextLevel = GS_ASTEROIDS_LEVEL11;
		gGameStateNext = GS_RESTART;
		changeLevelFlag = 1;
		break;
	}

	case GS_ASTEROIDS_LEVEL12:
	{
		NextLevel = GS_ASTEROIDS_LEVEL12;
		gGameStateNext = GS_RESTART;
		changeLevelFlag = 1;
		break;
	}

	case GS_ASTEROIDS_START_SCREEN:
	{
		NextLevel = GS_ASTEROIDS_START_SCREEN;
		gGameStateNext = GS_RESTART;
		changeLevelFlag = 1;
		break;
	}

	case GS_ASTEROIDS_SPLASH_SCREEN:
	{
		NextLevel = GS_ASTEROIDS_SPLASH_SCREEN;
		gGameStateNext = GS_RESTART;
		changeLevelFlag = 1;
		break;
	}

	case GS_ASTEROIDS_CREDITS_SCREEN:
	{
		NextLevel = GS_ASTEROIDS_CREDITS_SCREEN;
		gGameStateNext = GS_RESTART;
		changeLevelFlag = 1;
		break;
	}

	case GS_ASTEROIDS_WIN_SCREEN:
	{
		NextLevel = GS_ASTEROIDS_WIN_SCREEN;
		gGameStateNext = GS_RESTART;
		changeLevelFlag = 1;
		break;
	}

	case GS_QUIT:
	{
		NextLevel = GS_QUIT;
		gGameStateNext = GS_QUIT;
		changeLevelFlag = 1;
		break;
	}

	}

}