#ifndef GAME_STATE_DEFINES_H
#define GAME_STATE_DEFINES_H

#include "Vector2D.h"
#include "Math2D.h"
#include "Matrix2D.h"
#include "main.h"
#include "GameState_Asteroids.h"
#include "GameStateList.h"
#include "TimerUtility.h"


typedef struct GameObjectInstance GameObjectInstance;			// Forward declaration needed, since components need to point to their owner "GameObjectInstance"

#pragma once
#define SHAPE_NUM_MAX				256					// The total number of different vertex buffer (Shape)
#define GAME_OBJ_INST_NUM_MAX		4096				// The total number of different game object instances


																// Feel free to change these values in ordet to make the game more fun
#define SHIP_INITIAL_NUM			3					// Initial number of ship lives
#define SHIP_SIZE					15.0f				// Ship size
#define SHIP_ACCEL_FORWARD			100.0f				// Ship forward acceleration (in m/s^2)
#define SHIP_ACCEL_BACKWARD			-100.0f			// Ship backward acceleration (in m/s^2)
#define SHIP_ROT_SPEED				(2.0f * PI)			// Ship rotation speed (radian/second)
#define HOMING_MISSILE_ROT_SPEED	(PI / 2.0f)			// Homing missile rotation speed (radian/second)
#define BULLET_SPEED				350.0f				// Bullet speed (m/s)
#define DEATH_INVINCE               3.0f				// The invincibility time for death
#define HEALTH_INVINCE				0.5f				// The invincibility time for getting hit
#define MAX_HEALTH					2					// The max amount of health
#define MAX_LIVES					3					// The max number of lives

																// object mFlag definition

#define FLAG_ACTIVE		0x00000001
#define FLAG_INACTIVE		0x00000000
#define SPEED 200

#define NUM_SPAWNPOINTS 13

extern GameObjectInstance* sgpShip;

extern AEGfxTexture *pEnemyTex;
extern double frameTime;

extern unsigned int CurrentLevel;
extern unsigned int NextLevel;
extern unsigned int changeLevelFlag;

extern int sgShipLives;
extern int sgShipHealth;
extern int EnemiesKilled;
extern int BossesKilled;

extern AESysInitInfo sysInitInfo;

typedef struct //used for the texture offset
{
	float mX;
	float mY;
}TextureOffset;

enum OBJECT_TYPE
{
	// list of game object types
	OBJECT_TYPE_SHIP = 0,
	OBJECT_TYPE_BULLET,
	OBJECT_TYPE_ASTEROID,
	OBJECT_TYPE_HOMING_MISSILE,
	OBJECT_TYPE_ENEMY_BULLET,
	OBJECT_TYPE_STAR1,
	OBJECT_TYPE_STAR2,
	OBJECT_TYPE_STAR3,
	OBJECT_TYPE_LIFE,
	OBJECT_TYPE_PINK_TRAIL,
	OBJECT_TYPE_WHITE_TRAIL,
	OBJECT_TYPE_BLUE_TRAIL,
	OBJECT_TYPE_ENIGMA_DEATHRED,
	OBJECT_TYPE_ENIGMA_DEATHWHITE,
	OBJECT_TYPE_ENIGMA_DEATHDEBRIS,
	OBJECT_TYPE_LSIDE,
	OBJECT_TYPE_RSIDE,
	OBJECT_TYPE_HEALTH_FULL,
	OBJECT_TYPE_HEALTH_HALF,
	OBJECT_TYPE_START_BUTTON,
	OBJECT_TYPE_DIGIPEN_SPLASH,
	OBJECT_TYPE_BOSS,
	OBJECT_TYPE_ENIGMA_SPAWN,
	OBJECT_TYPE_EXIT_BUTTON,
	OBJECT_TYPE_CREDITS_BUTTON,
	OBJECT_TYPE_LOGO,
	OBJECT_TYPE_SHIP_FLASH,
	OBJECT_TYPE_BOSS_FLASH,
	OBJECT_TYPE_RED_BOX,
	OBJECT_TYPE_YELLOW_BOX,
	OBJECT_TYPE_GREEN_BOX,
	OBJECT_TYPE_MAP,
	OBJECT_TYPE_CREDITS,
	OBJECT_TYPE_WIN,

	OBJECT_TYPE_NUM
};

enum BUTTON_IDS
{
	BUTTON_ID_START = 0,
	BUTTON_ID_EXIT,
	BUTTON_ID_CREDITS,

	BUTTON_TYPE_NUM
};

typedef struct
{
	unsigned long			mType;				// Object type (Ship, bullet, etc..)
	AEGfxVertexList*		mpMesh;				// This will hold the triangles which will form the shape of the object

}Shape;

// ---------------------------------------------------------------------------

typedef struct
{
	Shape *mpShape;

	GameObjectInstance *	mpOwner;			// This component's owner
}Component_Sprite;

// ---------------------------------------------------------------------------

typedef struct
{
	Vector2D		        mPosition;			// Current position
	float					mAngle;				// Current angle
	float					mScaleX;			// Current X scaling value
	float					mScaleY;			// Current Y scaling value

	Matrix2D				mTransform;			// Object transformation matrix: Each frame, calculate the object instance's transformation matrix and save it here

	GameObjectInstance *	mpOwner;			// This component's owner
}Component_Transform;

// ---------------------------------------------------------------------------

typedef struct
{
	Vector2D				mVelocity;			// Current velocity

	GameObjectInstance *	mpOwner;			// This component's owner
}Component_Physics;

// ---------------------------------------------------------------------------

typedef struct
{
	GameObjectInstance *		mpTarget;		// Target, used by the homing missile

	GameObjectInstance *		mpOwner;		// This component's owner

	Timer*						follow_timer;	// Tracks duration of following player
}Component_Target;

// ---------------------------------------------------------------------------


//Game object instance structure
struct GameObjectInstance
{
	unsigned long				mFlag;						// Bit mFlag, used to indicate if the object instance is active or not
	unsigned long               mHit;                       // Bit mFlag, used for invincibility flag
	float                       invinceTime;                // An invincibility timer
	float                       timer;                      // A timer that can be used for a variety of purposes
	int                         ParticleFlag;
	int                         InputFlag;                  //detemines if the object is accepting user input
	double                      fireTimer;                  //fire timer

	// Boss stuff
	float                       bossTimer;
	int                         bossHealth;
	unsigned short				stateFlag;
	unsigned short				innerState;
	unsigned short				innerPhase;

	unsigned int				buttonID;					// Usage for button objects

	Component_Sprite			*mpComponent_Sprite;		// Sprite component
	Component_Transform			*mpComponent_Transform;		// Transform component
	Component_Physics			*mpComponent_Physics;		// Physics component
	Component_Target			*mpComponent_Target;		// Target component, used by the homing missile
};

void HandleInput();
void UpdateObjectCollisions(GameObjectInstance* pObjectList);
void UpdateObjectMatrices(GameObjectInstance* GameObjList);
void UpdateObjectBehaviours(GameObjectInstance* pObjList);
void UpdateObjectPositions(GameObjectInstance* pObjList, double frameTime);

void LoseALife();
void EndGame();
void Invincibility();
void IsHit();

void DrawTexture(int objType, AEGfxTexture *texture);
void DrawColor(int objType);
void DrawEnemy();
void DrawTrail();
void AnimatePlayer();
void AnimateBoss();
void DrawMap();
void DrawFlash();

// functions to create/destroy a game object instance
GameObjectInstance*			GameObjectInstanceCreate(unsigned int ObjectType);			// From OBJECT_TYPE enum
void						GameObjectInstanceDestroy(GameObjectInstance* pInst);

void AddToObject(GameObjectInstance *object, int xPos, int yPos, int xVel, int yVel, int xScale, int yScale, float angle);

// Returns pointer to the ship
GameObjectInstance* InitializeShip();

GameObjectInstance* CreateButtonObject(unsigned int buttonID);	// Create button at position pos
void CreateStars();
void CreateHUDObjects();
void DrawHUDTextures();

void ChangeLevel(int level);

// ---------------------------------------------------------------------------

// Functions to add/remove components
static void AddComponent_Transform(GameObjectInstance *pInst, Vector2D *pPosition, float Angle, float ScaleX, float ScaleY);
static void AddComponent_Sprite(GameObjectInstance *pInst, unsigned int ShapeType);
static void AddComponent_Physics(GameObjectInstance *pInst, Vector2D *pVelocity);
static void AddComponent_Target(GameObjectInstance *pInst, GameObjectInstance *pTarget);

static void RemoveComponent_Transform(GameObjectInstance *pInst);
static void RemoveComponent_Sprite(GameObjectInstance *pInst);
static void RemoveComponent_Physics(GameObjectInstance *pInst);
static void RemoveComponent_Target(GameObjectInstance *pInst);
#endif
