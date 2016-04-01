#ifndef ENEMY_LOADER_H
#define ENEMY_LOADER_H

#include "GameState_AsteroidsDefines.h"
#include "TimerUtility.h"

#define SEEKING_MOB		1
#define OCCUPIED_SPAWN	0
// Timer for spawn points to count to after their enemy dies
#define RESPAWN_TIMER	4.5

// Mainly for special spawns
#define ENEMY_WAITING	0
#define ENEMY_DEPLOYED	1
#define DEPLOYMENT_DELAY 7.5

#define NUM_ENEMY_SHAPES 2

// Define enemy velocities
#define ENEMY_VEL_REGULAR 75.0f
#define ENEMY_VEL_SPECIAL 35.0f

// How long for standard movement pattern
#define MOVEMENT_COUNTER	7.5

// Shooting conditions
#define ENEMY_POSITION_CONDITION_OFFSET 80.0f
#define DEPLOYMENT_TIMER_OFFSET 3.5
#define STANDARD_ENEMY_WEP_DELAY 0.8

extern int active_grenade_shots;
extern AEGfxTexture *pWorld1Tex;
extern AEGfxTexture *pWorld2Tex;
extern AEGfxTexture *pWorld3Tex;
extern AEGfxTexture *pWorld4Tex;
extern Shape enemyShapes[NUM_ENEMY_SHAPES];

typedef struct EnemyInstance EnemyInstance;

enum ENEMY_TYPE
{
	// list of game object types
	ENEMY_TYPE_REGULAR = 0,
	ENEMY_TYPE_MINION,
	ENEMY_TYPE_WORLD4,
	ENEMY_TYPE_WORLD2,

	ENEMY_TYPE_NUM
};

// ID's given to spawn points that tell the enemy what to do
enum STATE_MACHINE
{
	// list of enemy logic
	HORIZONTAL_MOVEMENT_L_TO_R = 0,
	HORIZONTAL_MOVEMENT_R_TO_L,
	VERTICAL_MOVEMENT_UP_TO_DOWN,
	VERTICAL_MOVEMENT_DOWN_TO_UP,

	// special enemy logic
	SQUARE_PATTERN_START_BOTTOM_LEFT,
#define SQUARE_PATTERN_PHASES 4
	SQUARE_PATTERN_START_RIGHT,
	SPIRAL_PATTERN_L_TO_R,
	SPIRAL_PATTERN_R_TO_L,
	SPIRAL_PATTERN_UP_TO_DOWN,
	OVAL_PATTERN_HORIZONTAL,
	OVAL_PATTERN_VERTICAL,
	CIRCLE_PATTERN,

	NUM_STATE_MACHINES
};

// ID's given to spawn points that tell the enemy what to do
enum BOSS_STATE_MACHINE
{
	// list of enemy logic
	SPRINKLER_SHOTS = 0,
	HOMING_SHOTS,
	GRENADE_SHOTS,
	FINAL_BOSS,

	NUM_BOSS_STATE_MACHINES
};

enum PHASES
{
	PHASE_1 = 1,
	PHASE_2,
	PHASE_3,
	PHASE_4,
	PHASE_5,
};

// ID's given to weapons points that direct ammunition type
enum WEAPON_MACHINE
{
	// list of weapon types
	STANDARD_WEAPON,	// Shoots straight down with a delay

	NUM_WEP_MACHINES
};

enum INNER_STATE
{
	// Actions to act upon the state machine
	ON_ENTER = 0,
	ON_UPDATE,
	ON_EXIT
};

enum SPAWNPOINT_ID
{
	// Actions to act upon the state machine
	SINGLE_SPAWNER = 0,
	MULTI_SPAWNER,

	NUM_IDS
};

typedef struct
{
	unsigned int	health;		// Health of the enemy

	unsigned int	wepType;		// Weapon type of the enemy
	unsigned int	wepState;		// State of the weapon machine
	Timer*			wepDelayTimer;	// Timer for enemy shootings

	double			lifetime;	// How long we want this enemy to be on the screen -- CURRENTLY DOING NOTHING

	int				reward;		// Would be the score given to from destroying this enemy	-- CURRENTLY DOING NOTHING
}Component_EnemyData;

typedef struct
{
	Shape *mpShape;

	EnemyInstance *	mpOwner;			// This component's owner
}Component_EnemySprite;

typedef struct
{
	Vector2D		        mPosition;			// Current position
	float					mAngle;				// Current angle
	float					mScaleX;			// Current X scaling value
	float					mScaleY;			// Current Y scaling value

	Matrix2D				mTransform;			// Object transformation matrix: Each frame, calculate the object instance's transformation matrix and save it here

	EnemyInstance *	mpOwner;			// This component's owner
}Component_EnemyTransform;

// ---------------------------------------------------------------------------

typedef struct
{
	Vector2D				mVelocity;			// Current velocity

	EnemyInstance *	mpOwner;			// This component's owner
}Component_EnemyPhysics;

// ---------------------------------------------------------------------------

struct EnemyInstance
{
	unsigned long			mFlag;			// Bit mFlag, used to indicate if the object instance is active or not

	unsigned short			stateFlag;		// Flag representing the state machine of the enemy, usually reflected on the spawn point
	unsigned short			innerState;

	unsigned short			movementPhase;
	unsigned short			numPhases;

	double					stateCounter;	// 	Counter for state machine
	float					movement_angle;	//	Angle-patterns depend on this

	unsigned short			deployedFlag;	// Has this enemy been sent out yet? For special spawn point

	Component_EnemySprite		*mpComponent_Sprite;		// Sprite component
	Component_EnemyTransform	*mpComponent_Transform;		// Transform component
	Component_EnemyPhysics		*mpComponent_Physics;		// Physics component
	Component_EnemyData			*mpComponent_EnemyData;		// Enemy Vars component
};

typedef struct
{
	unsigned short		spID;			// Purpose of the spawnpoint
	unsigned short		active;			// Flag for active spawn point

	Vector2D			position;		// Position of the spawn point

	unsigned short		enemyLogicID;	// ID of the logic of the enemy to deploy
	unsigned int		flag;			// Flag for seeking or occupied spawn point

	EnemyInstance*		pEnemy;			// Pointer to the enemy in this struct
	EnemyInstance**		pEnemyList;		// Pointer to the linked list of enemies
	unsigned short		numEnemies;		// Number of enemies in the special list

	Timer* SpawnTimer;					// Timer flagging for next enemy to spawn here
}EnemySpawnPoint;

EnemyInstance* LoadEnemiesFromLevel(EnemyInstance* pEnemyArray, unsigned int size, unsigned int level, unsigned int *pMonsterCounter);
EnemySpawnPoint* LoadEnemySpawnsFromLevel(EnemySpawnPoint* pSpawnArray, unsigned int level);

void SetLevel1SpawnPoints(EnemySpawnPoint* pSpawnArray);
void SetLevel2SpawnPoints(EnemySpawnPoint* pSpawnArray);
void SetLevel3SpawnPoints(EnemySpawnPoint* pSpawnArray);

void SetLevel4SpawnPoints(EnemySpawnPoint* pSpawnArray);
void SetLevel5SpawnPoints(EnemySpawnPoint* pSpawnArray);
void SetLevel6SpawnPoints(EnemySpawnPoint* pSpawnArray);

void SetLevel7SpawnPoints(EnemySpawnPoint* pSpawnArray);
void SetLevel8SpawnPoints(EnemySpawnPoint* pSpawnArray);
void SetLevel9SpawnPoints(EnemySpawnPoint* pSpawnArray);

void SetLevel10SpawnPoints(EnemySpawnPoint* pSpawnArray);
void SetLevel11SpawnPoints(EnemySpawnPoint* pSpawnArray);
void SetLevel12SpawnPoints(EnemySpawnPoint* pSpawnArray);

//FMOD
void LoadEnemyFMOD(void);
void FreeEnemyFMOD(void);

/// Special spawn functions
void LoadSpecialEnemySpawnpoint(EnemySpawnPoint* pSpawnPoint);
void UnLoadSpecialEnemySpawnpoint(EnemySpawnPoint* pSpawnPoint);
void UpdateSpecialSpawnPointEnemyPositions(EnemySpawnPoint* pSpawnPoint);
void UpdateSpecialSpawnPointEnemyCollisions(EnemySpawnPoint* pSpawnPoint, GameObjectInstance* pGameObjInstArray, unsigned int *pMonsterCounter);
void UpdateSpecialEnemySpawnpoint(EnemySpawnPoint* pSpawnPoint, EnemyInstance* pEnemyArray, unsigned int *pMonsterIndex);
///

void UpdateSpawnPointEnemies(EnemySpawnPoint* pSpawnArray, EnemyInstance* pEnemyArray, unsigned int *pMonsterIndex);
static EnemySpawnPoint SetSpawnPoint(EnemySpawnPoint pSpawnPoint, Vector2D position, unsigned int flag, unsigned short logic_flag);
void DestroySpawnPoint(EnemySpawnPoint* pSpawnPoint);

void InitializeEnemyShapes();
void UpdateSpawnedEnemyPositions(EnemySpawnPoint* pSpawnArray, double frameTime);
void UpdateSpawnedEnemyCollisions(EnemySpawnPoint* pSpawnArray, GameObjectInstance* pGameObjInstArray, unsigned int *pMonsterCounter);
void UpdateSpawnedEnemyBehaviours(EnemySpawnPoint* pSpawnArray);
void UpdateEnemyMatrices(EnemySpawnPoint* pSpawnArray);

void EnemyStateMachine(EnemyInstance *pInst);
void EnemyWeaponMachine(EnemyInstance *pInst);
void BossWeaponMachine(GameObjectInstance *pInst);

GameObjectInstance* BulletCreate(Vector2D position, Vector2D velocity, float angle, unsigned int bulletType);
void ScatterShotExplosion(Vector2D position, unsigned int num_shots, unsigned int bulletType);

// functions to create/destroy a enemy object instance
EnemyInstance*	EnemyInstanceCreate(EnemyInstance* pEnemyArray, unsigned int EnemyType, unsigned int *pMonsterCounter, unsigned int numMonsters);		// From ENEMY_TYPE enum
void			EnemyInstanceDestroy(EnemyInstance* pInst, unsigned int *pMonsterCounter);

static void AddComponent_EnemyTransform(EnemyInstance *pInst, Vector2D *pPosition, float Angle, float ScaleX, float ScaleY);
static void AddComponent_EnemySprite(EnemyInstance *pInst, Shape* sgShapes, unsigned int ShapeType);
static void AddComponent_EnemyPhysics(EnemyInstance *pInst, Vector2D *pVelocity);
static void AddComponent_EnemyData(EnemyInstance *pInst, unsigned int health, unsigned int wepType, double lifetime, int reward);

static void RemoveComponent_EnemyTransform(EnemyInstance *pInst);
static void RemoveComponent_EnemySprite(EnemyInstance *pInst);
static void RemoveComponent_EnemyPhysics(EnemyInstance *pInst);
static void RemoveComponent_EnemyData(EnemyInstance *pInst);

#endif
