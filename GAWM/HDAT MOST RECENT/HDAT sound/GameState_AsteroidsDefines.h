#ifndef GAME_STATE_DEFINES_H
#define GAME_STATE_DEFINES_H

#include "Vector2D.h"
#include "Math2D.h"
#include "Matrix2D.h"
#include "main.h"
#include "GameState_Asteroids.h"


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

																// object mFlag definition

#define FLAG_ACTIVE		0x00000001
#define SPEED 200

#define NUM_SPAWNPOINTS 12

extern GameObjectInstance* sgpShip;

extern AEGfxTexture *pEnemyTex;
extern double frameTime;

extern int sgShipLives;

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

	OBJECT_TYPE_NUM
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
}Component_Target;

// ---------------------------------------------------------------------------


//Game object instance structure
struct GameObjectInstance
{
	unsigned long				mFlag;						// Bit mFlag, used to indicate if the object instance is active or not
	float                       timer;                      // A timer that can be used for a variety of purposes

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

// functions to create/destroy a game object instance
GameObjectInstance*			GameObjectInstanceCreate(unsigned int ObjectType);			// From OBJECT_TYPE enum
void						GameObjectInstanceDestroy(GameObjectInstance* pInst);

void AddToObject(GameObjectInstance *object, int xPos, int yPos, int xVel, int yVel, int xScale, int yScale, float angle);

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


/*enemy moving and shooting - temp
enemyTimer += frameTime;

if (enemyTimer >= enemyShootTime)
{
int i = 0;
rockMovePatterns++;
if (rockMovePatterns > 3)
{
rockMovePatterns = 0; //reset the movement pattern
}

for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
{
GameObjectInstance * enemy = sgGameObjectInstanceList + i;

if ((enemy->mFlag & FLAG_ACTIVE) == 0)
continue;
if (enemy->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_ASTEROID)
{
if (rockMovePatterns == 0)
{
enemy->mpComponent_Physics->mVelocity.x = 30;
enemy->mpComponent_Physics->mVelocity.y = 0;
}
else if (rockMovePatterns == 1)
{
enemy->mpComponent_Physics->mVelocity.y = 30;
enemy->mpComponent_Physics->mVelocity.x = 0;
}

else if (rockMovePatterns == 2)
{
enemy->mpComponent_Physics->mVelocity.x = -30;
enemy->mpComponent_Physics->mVelocity.y = 0;
}
else if (rockMovePatterns == 3)
{
enemy->mpComponent_Physics->mVelocity.y = -30;
enemy->mpComponent_Physics->mVelocity.x = 0;
}
GameObjectInstance * bullet = GameObjectInstanceCreate(OBJECT_TYPE_ENEMY_BULLET);

bullet->mpComponent_Physics->mVelocity.x = cosf(enemy->mpComponent_Transform->mAngle) * BULLET_SPEED;
bullet->mpComponent_Physics->mVelocity.y = sinf(enemy->mpComponent_Transform->mAngle) * BULLET_SPEED;

bullet->mpComponent_Transform->mPosition.x = enemy->mpComponent_Transform->mPosition.x;
bullet->mpComponent_Transform->mPosition.y = (enemy->mpComponent_Transform->mPosition.y - 20);

bullet->mpComponent_Transform->mAngle = enemy->mpComponent_Transform->mAngle;
}
}
enemyTimer = 0.0;
}
*/
