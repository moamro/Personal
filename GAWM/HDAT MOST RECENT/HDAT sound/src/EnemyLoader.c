#include "EnemyLoader.h"
#include "GameState_AsteroidsDefines.h"
#include <stdlib.h>
#include "fmod.h"

static Shape enemyShapes[NUM_ENEMY_SHAPES];	// Elements are defined enemy shapes, as many as eShapeNum
static unsigned int eShapeNum = 0;			// Number of enemy shapes created
AEGfxTexture *pEnemyTex;
AEGfxTexture *pMinionTex;
AEGfxTexture *pWorld2Tex;
AEGfxTexture *pWorld4Tex;
GameObjectInstance* Explosion;

//FMOD Stuff//
static FMOD_SYSTEM *FmodSystem;
static FMOD_SOUND *sound1;
static FMOD_CHANNEL *channel = 0;
FMOD_RESULT result;
unsigned int version;

void InitializeEnemyShapes()
{
	Shape* pShape = NULL;

	// Zero the shapes array
	memset(enemyShapes, 0, sizeof(Shape) * NUM_ENEMY_SHAPES);

	//create the regular enemy shape
	pShape = enemyShapes + eShapeNum++;
	pShape->mType = ENEMY_TYPE_REGULAR;

	AEGfxMeshStart();

	AEGfxTriAdd(
		-1.5f, -1.5f, 0x00FF00FF, 0.0f, 1.0f,
		1.5f, -1.5f, 0x00FFFF00, 1.0f, 1.0f,
		-1.5f, 1.5f, 0x00F00FFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		1.5f, -1.5f, 0x00FFFFFF, 1.0f, 1.0f,
		1.5f, 1.5f, 0x00FFFFFF, 1.0f, 0.0f,
		-1.5f, 1.5f, 0x00FFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1
	pShape->mpMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pShape->mpMesh, "Failed to create mesh 1!!");

	pEnemyTex = AEGfxTextureLoad("Enemy3.png");
	AE_ASSERT_MESG(pEnemyTex, "Failed to create texture1!!");

	//create the regular enemy shape
	pShape = enemyShapes + eShapeNum++;
	pShape->mType = ENEMY_TYPE_MINION;

	AEGfxMeshStart();

	AEGfxTriAdd(
		-1.5f, -1.5f, 0x00FF00FF, 0.0f, 1.0f,
		1.5f, -1.5f, 0x00FFFF00, 1.0f, 1.0f,
		-1.5f, 1.5f, 0x00F00FFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		1.5f, -1.5f, 0x00FFFFFF, 1.0f, 1.0f,
		1.5f, 1.5f, 0x00FFFFFF, 1.0f, 0.0f,
		-1.5f, 1.5f, 0x00FFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1
	pShape->mpMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pShape->mpMesh, "Failed to create mesh 1!!");

	pMinionTex = AEGfxTextureLoad("Enemy4.png");
	AE_ASSERT_MESG(pMinionTex, "Failed to create texture1!!");

	pShape = enemyShapes + eShapeNum++;
	pShape->mType = ENEMY_TYPE_WORLD4;

	AEGfxMeshStart();

	AEGfxTriAdd(
		-1.5f, -1.5f, 0x00FF00FF, 0.0f, 1.0f,
		1.5f, -1.5f, 0x00FFFF00, 1.0f, 1.0f,
		-1.5f, 1.5f, 0x00F00FFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		1.5f, -1.5f, 0x00FFFFFF, 1.0f, 1.0f,
		1.5f, 1.5f, 0x00FFFFFF, 1.0f, 0.0f,
		-1.5f, 1.5f, 0x00FFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1
	pShape->mpMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pShape->mpMesh, "Failed to create mesh 1!!");

	pWorld4Tex = AEGfxTextureLoad("Enemy1.png");
	AE_ASSERT_MESG(pWorld4Tex, "Failed to create texture1!!");

	pShape = enemyShapes + eShapeNum++;
	pShape->mType = ENEMY_TYPE_WORLD2;

	AEGfxMeshStart();

	AEGfxTriAdd(
		-1.5f, -1.5f, 0x00FF00FF, 0.0f, 1.0f,
		1.5f, -1.5f, 0x00FFFF00, 1.0f, 1.0f,
		-1.5f, 1.5f, 0x00F00FFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		1.5f, -1.5f, 0x00FFFFFF, 1.0f, 1.0f,
		1.5f, 1.5f, 0x00FFFFFF, 1.0f, 0.0f,
		-1.5f, 1.5f, 0x00FFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1
	pShape->mpMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pShape->mpMesh, "Failed to create mesh 1!!");

	pWorld2Tex = AEGfxTextureLoad("Enemy2.png");
	AE_ASSERT_MESG(pWorld2Tex, "Failed to create texture1!!");
}

void LoadEnemyFMOD(void)
{
	//FMOD initialization////////////////////////////////////////////////////
	FMOD_System_Create(&FmodSystem);
	FMOD_System_GetVersion(FmodSystem, &version);
	FMOD_System_Init(FmodSystem, 32, FMOD_INIT_NORMAL, NULL);
	FMOD_System_CreateSound(FmodSystem, "enigmaDeath.wav", FMOD_DEFAULT, 0, &sound1);
	FMOD_Sound_SetMusicChannelVolume(sound1, channel, 0.7f);
}

void FreeEnemyFMOD(void)
{
	FMOD_Sound_Release(sound1);
	FMOD_System_Close(FmodSystem);
	result = FMOD_System_Release(FmodSystem);
	int i = 0;
}

EnemyInstance* LoadEnemiesFromLevel(EnemyInstance* pEnemyArray, unsigned int size, unsigned int level, unsigned int *pMonsterCounter)
{
	// Allocate the enemyarray
	pEnemyArray = (EnemyInstance *)calloc(size, sizeof(EnemyInstance));

	unsigned int i;

	switch (level)
	{
		// Create level 1 enemies
	case GS_ASTEROIDS_LEVEL1:
		for (i = 0; i < size; i++)
		{
			EnemyInstance *pInst;

			pInst = EnemyInstanceCreate(pEnemyArray, ENEMY_TYPE_MINION, pMonsterCounter, size);
			pInst->mpComponent_Transform->mAngle = (PI / -2.0f);
			pEnemyArray[i] = *pInst;
		}
		break;

	case GS_ASTEROIDS_LEVEL2:
		for (i = 0; i < size; i++)
		{
			EnemyInstance *pInst;

			pInst = EnemyInstanceCreate(pEnemyArray, ENEMY_TYPE_MINION, pMonsterCounter, size);
			pInst->mpComponent_Transform->mAngle = (PI / -2.0f);
			pEnemyArray[i] = *pInst;
		}
		break;

	case GS_ASTEROIDS_LEVEL3:
		for (i = 0; i < size; i++)
		{
			EnemyInstance *pInst;

			pInst = EnemyInstanceCreate(pEnemyArray, ENEMY_TYPE_MINION, pMonsterCounter, size);
			pInst->mpComponent_Transform->mAngle = (PI / -2.0f);
			pEnemyArray[i] = *pInst;
		}
		break;

	case GS_ASTEROIDS_LEVEL4:
		for (i = 0; i < size; i++)
		{
			EnemyInstance *pInst;

			pInst = EnemyInstanceCreate(pEnemyArray, ENEMY_TYPE_WORLD2, pMonsterCounter, size);
			pInst->mpComponent_Transform->mAngle = (PI / -2.0f);
			pEnemyArray[i] = *pInst;
		}
		break;

	case GS_ASTEROIDS_LEVEL5:
		for (i = 0; i < size; i++)
		{
			EnemyInstance *pInst;

			pInst = EnemyInstanceCreate(pEnemyArray, ENEMY_TYPE_WORLD2, pMonsterCounter, size);
			pInst->mpComponent_Transform->mAngle = (PI / -2.0f);
			pEnemyArray[i] = *pInst;
		}
		break;

	case GS_ASTEROIDS_LEVEL6:
		for (i = 0; i < size; i++)
		{
			EnemyInstance *pInst;

			pInst = EnemyInstanceCreate(pEnemyArray, ENEMY_TYPE_WORLD2, pMonsterCounter, size);
			pInst->mpComponent_Transform->mAngle = (PI / -2.0f);
			pEnemyArray[i] = *pInst;
		}
		break;

	case GS_ASTEROIDS_LEVEL7:
		for (i = 0; i < size; i++)
		{
			EnemyInstance *pInst;

			pInst = EnemyInstanceCreate(pEnemyArray, ENEMY_TYPE_REGULAR, pMonsterCounter, size);
			pInst->mpComponent_Transform->mAngle = (PI / -2.0f);
			pEnemyArray[i] = *pInst;
		}
		break;

	case GS_ASTEROIDS_LEVEL8:
		for (i = 0; i < size; i++)
		{
			EnemyInstance *pInst;

			pInst = EnemyInstanceCreate(pEnemyArray, ENEMY_TYPE_REGULAR, pMonsterCounter, size);
			pInst->mpComponent_Transform->mAngle = (PI / -2.0f);
			pEnemyArray[i] = *pInst;
		}
		break;

	case GS_ASTEROIDS_LEVEL9:
		for (i = 0; i < size; i++)
		{
			EnemyInstance *pInst;

			pInst = EnemyInstanceCreate(pEnemyArray, ENEMY_TYPE_REGULAR, pMonsterCounter, size);
			pInst->mpComponent_Transform->mAngle = (PI / -2.0f);
			pEnemyArray[i] = *pInst;
		}
		break;

	case GS_ASTEROIDS_LEVEL10:
		for (i = 0; i < size; i++)
		{
			EnemyInstance *pInst;

			pInst = EnemyInstanceCreate(pEnemyArray, ENEMY_TYPE_WORLD4, pMonsterCounter, size);
			pInst->mpComponent_Transform->mAngle = (PI / -2.0f);
			pEnemyArray[i] = *pInst;
		}
		break;

	case GS_ASTEROIDS_LEVEL11:
		for (i = 0; i < size; i++)
		{
			EnemyInstance *pInst;

			pInst = EnemyInstanceCreate(pEnemyArray, ENEMY_TYPE_WORLD4, pMonsterCounter, size);
			pInst->mpComponent_Transform->mAngle = (PI / -2.0f);
			pEnemyArray[i] = *pInst;
		}
		break;

	case GS_ASTEROIDS_LEVEL12:
		for (i = 0; i < size; i++)
		{
			EnemyInstance *pInst;

			pInst = EnemyInstanceCreate(pEnemyArray, ENEMY_TYPE_WORLD4, pMonsterCounter, size);
			pInst->mpComponent_Transform->mAngle = (PI / -2.0f);
			pEnemyArray[i] = *pInst;
		}
		break;

	}

	*pMonsterCounter = 0;
	return pEnemyArray;
}


EnemySpawnPoint* LoadEnemySpawnsFromLevel(EnemySpawnPoint* pSpawnArray, unsigned int level)
{
	if (level == GS_ASTEROIDS_LEVEL1)
	{
		pSpawnArray = (EnemySpawnPoint *)calloc(NUM_SPAWNPOINTS, sizeof(EnemySpawnPoint));

		SetLevel1SpawnPoints(pSpawnArray);

		return pSpawnArray;
	}

	else if (level == GS_ASTEROIDS_LEVEL2)
	{
		pSpawnArray = (EnemySpawnPoint *)calloc(NUM_SPAWNPOINTS, sizeof(EnemySpawnPoint));

		SetLevel2SpawnPoints(pSpawnArray);

		return pSpawnArray;
	}

	else if (level == GS_ASTEROIDS_LEVEL3)
	{
		pSpawnArray = (EnemySpawnPoint *)calloc(NUM_SPAWNPOINTS, sizeof(EnemySpawnPoint));

		SetLevel3SpawnPoints(pSpawnArray);

		return pSpawnArray;
	}

	else if (level == GS_ASTEROIDS_LEVEL4)
	{
		pSpawnArray = (EnemySpawnPoint *)calloc(NUM_SPAWNPOINTS, sizeof(EnemySpawnPoint));

		SetLevel4SpawnPoints(pSpawnArray);

		return pSpawnArray;
	}

	else if (level == GS_ASTEROIDS_LEVEL5)
	{
		pSpawnArray = (EnemySpawnPoint *)calloc(NUM_SPAWNPOINTS, sizeof(EnemySpawnPoint));

		SetLevel5SpawnPoints(pSpawnArray);

		return pSpawnArray;
	}

	else if (level == GS_ASTEROIDS_LEVEL6)
	{
		pSpawnArray = (EnemySpawnPoint *)calloc(NUM_SPAWNPOINTS, sizeof(EnemySpawnPoint));

		SetLevel6SpawnPoints(pSpawnArray);

		return pSpawnArray;
	}

	else if (level == GS_ASTEROIDS_LEVEL7)
	{
		pSpawnArray = (EnemySpawnPoint *)calloc(NUM_SPAWNPOINTS, sizeof(EnemySpawnPoint));

		SetLevel7SpawnPoints(pSpawnArray);

		return pSpawnArray;
	}

	else if (level == GS_ASTEROIDS_LEVEL8)
	{
		pSpawnArray = (EnemySpawnPoint *)calloc(NUM_SPAWNPOINTS, sizeof(EnemySpawnPoint));

		SetLevel8SpawnPoints(pSpawnArray);

		return pSpawnArray;
	}

	else if (level == GS_ASTEROIDS_LEVEL9)
	{
		pSpawnArray = (EnemySpawnPoint *)calloc(NUM_SPAWNPOINTS, sizeof(EnemySpawnPoint));

		SetLevel9SpawnPoints(pSpawnArray);

		return pSpawnArray;
	}

	else if (level == GS_ASTEROIDS_LEVEL10)
	{
		pSpawnArray = (EnemySpawnPoint *)calloc(NUM_SPAWNPOINTS, sizeof(EnemySpawnPoint));

		SetLevel10SpawnPoints(pSpawnArray);

		return pSpawnArray;
	}

	else if (level == GS_ASTEROIDS_LEVEL11)
	{
		pSpawnArray = (EnemySpawnPoint *)calloc(NUM_SPAWNPOINTS, sizeof(EnemySpawnPoint));

		SetLevel11SpawnPoints(pSpawnArray);

		return pSpawnArray;
	}

	else if (level == GS_ASTEROIDS_LEVEL12)
	{
		pSpawnArray = (EnemySpawnPoint *)calloc(NUM_SPAWNPOINTS, sizeof(EnemySpawnPoint));

		SetLevel12SpawnPoints(pSpawnArray);

		return pSpawnArray;
	}
}


void LoadSpecialEnemySpawnpoint(EnemySpawnPoint* pSpawnPoint)
{
	// Allocate the list
	pSpawnPoint->pEnemyList = (EnemyInstance **)calloc(pSpawnPoint->numEnemies, sizeof(EnemyInstance *));
}

void UnLoadSpecialEnemySpawnpoint(EnemySpawnPoint* pSpawnPoint)
{
	// De-Allocate the list

	if (pSpawnPoint->pEnemyList != 0)
	{
		free(pSpawnPoint->pEnemyList);
		pSpawnPoint->pEnemyList = 0;
	}
}

void UpdateSpecialSpawnPointEnemyPositions(EnemySpawnPoint* pSpawnPoint)
{
	unsigned int i;

	for (i = 0; i < pSpawnPoint->numEnemies; i++)
	{
		EnemyInstance* pInst = *(pSpawnPoint->pEnemyList) + i;

		// skip non-active object
		if ((pInst->mFlag & FLAG_ACTIVE) == 0)
			continue;

		if (pInst->deployedFlag == ENEMY_WAITING)
			continue;

		pInst->mpComponent_Transform->mPosition.x += pInst->mpComponent_Physics->mVelocity.x  * (float)frameTime;
		pInst->mpComponent_Transform->mPosition.y += pInst->mpComponent_Physics->mVelocity.y * (float)frameTime;
	}
}

void UpdateSpecialSpawnPointEnemyCollisions(EnemySpawnPoint* pSpawnPoint, GameObjectInstance* pGameObjInstArray, unsigned int *pMonsterCounter)
{
	unsigned int i, j, loop;

	for (i = 0; i < pSpawnPoint->numEnemies; i++)
	{
		EnemyInstance* enemy = *(pSpawnPoint->pEnemyList) + i;

		// skip non-active object
		if ((enemy->mFlag & FLAG_ACTIVE) == 0)
			continue;

		if (enemy->deployedFlag == ENEMY_WAITING)
			continue;

		// If it's a regular enemy unit
		if (enemy->mpComponent_Sprite->mpShape->mType == ENEMY_TYPE_REGULAR || enemy->mpComponent_Sprite->mpShape->mType == ENEMY_TYPE_MINION || enemy->mpComponent_Sprite->mpShape->mType == ENEMY_TYPE_WORLD2 || enemy->mpComponent_Sprite->mpShape->mType == ENEMY_TYPE_WORLD4)
		{
			// Create collision behaviours based on object types
			for (j = 0; j < GAME_OBJ_INST_NUM_MAX; j++)
			{
				GameObjectInstance * object2 = pGameObjInstArray + j;

				// Make sure the flag is active
				if ((object2->mFlag & FLAG_ACTIVE) == 0)
					continue;

				// What happens if a bullet touches it
				if (object2->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_BULLET)
				{
					if (StaticPointToStaticRect(&object2->mpComponent_Transform->mPosition, &enemy->mpComponent_Transform->mPosition, enemy->mpComponent_Transform->mScaleX, enemy->mpComponent_Transform->mScaleY))
					{
						float angle;
						// Enemy lose health or create pixels here
						for (angle = 0.0f; angle < 360.0f; angle += 4.0f)
						{
							float randNum = ((float)rand() / (float)(RAND_MAX / 10.0f));
							Explosion = GameObjectInstanceCreate(OBJECT_TYPE_ENIGMA_DEATHRED);
							Vector2DSet(&Explosion->mpComponent_Transform->mPosition, enemy->mpComponent_Transform->mPosition.x, enemy->mpComponent_Transform->mPosition.y);
							Vector2DFromAngleDeg(&Explosion->mpComponent_Physics->mVelocity, angle + randNum);
							Vector2DScale(&Explosion->mpComponent_Physics->mVelocity, &Explosion->mpComponent_Physics->mVelocity, 50.0f);
							Explosion->timer = 1.0f;
							Explosion->ParticleFlag = 1;
						}
						for (angle = 0.0f; angle < 360.0f; angle += 4.0f)
						{
							float randNum = ((float)rand() / (float)(RAND_MAX / 10.0f));
							Explosion = GameObjectInstanceCreate(OBJECT_TYPE_ENIGMA_DEATHWHITE);
							Vector2DSet(&Explosion->mpComponent_Transform->mPosition, enemy->mpComponent_Transform->mPosition.x, enemy->mpComponent_Transform->mPosition.y);
							Vector2DFromAngleDeg(&Explosion->mpComponent_Physics->mVelocity, angle + randNum);
							Vector2DScale(&Explosion->mpComponent_Physics->mVelocity, &Explosion->mpComponent_Physics->mVelocity, 40.0f);
							Explosion->timer = 1.0f;
							Explosion->ParticleFlag = 1;
						}
						for (angle = 0.0f; angle < 360.0f; angle += 4.0f)
						{
							float randNum = ((float)rand() / (float)(RAND_MAX / 10.0f));
							Explosion = GameObjectInstanceCreate(OBJECT_TYPE_ENIGMA_DEATHDEBRIS);
							Vector2DSet(&Explosion->mpComponent_Transform->mPosition, enemy->mpComponent_Transform->mPosition.x, enemy->mpComponent_Transform->mPosition.y);
							Vector2DFromAngleDeg(&Explosion->mpComponent_Physics->mVelocity, angle + randNum);
							Vector2DScale(&Explosion->mpComponent_Physics->mVelocity, &Explosion->mpComponent_Physics->mVelocity, 80.0f + (randNum * randNum));
							Explosion->timer = 1.0f;
							Explosion->ParticleFlag = 1;
						}


						// Destroy enemy and bullet
						EnemyInstanceDestroy(enemy, pMonsterCounter);
						EnemiesKilled++;

						GameObjectInstanceDestroy(object2);
						break;
					}
				}

				// What happens if a ship touches this enemy
				if (object2->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_SHIP)
				{
					if (StaticRectToStaticRect(&object2->mpComponent_Transform->mPosition, SHIP_SIZE, SHIP_SIZE, &enemy->mpComponent_Transform->mPosition, enemy->mpComponent_Transform->mScaleX, enemy->mpComponent_Transform->mScaleY))
					{
						// Destroy Enemy
						EnemyInstanceDestroy(enemy, pMonsterCounter);

						// Reset Ship

						LoseALife();
						for (loop = 0; loop < GAME_OBJ_INST_NUM_MAX; loop++)
						{
							GameObjectInstance * object3 = pGameObjInstArray + loop;

							if ((object3->mFlag & FLAG_ACTIVE) == 0)
								continue;
							if (object3->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_LIFE)
							{
								GameObjectInstanceDestroy(object3);
								break;
							}
						}
						break;
					}
				}

			} // End of second loop
		}
	}

}


void UpdateSpecialEnemySpawnpoint(EnemySpawnPoint* pSpawnPoint, EnemyInstance* pEnemyArray, unsigned int *pMonsterIndex)
{
	unsigned short i;
	unsigned short numLiveEnemies = pSpawnPoint->numEnemies;

	if (pSpawnPoint->flag == SEEKING_MOB)
	{
		for (i = 0; i < pSpawnPoint->numEnemies; i++)
		{
			EnemyInstance* pEnemy = pEnemyArray + (*pMonsterIndex)++;
			EnemyInstance* pInst = *(pSpawnPoint->pEnemyList) + i;

			Vector2DSet(&pEnemy->mpComponent_Transform->mPosition, pSpawnPoint->position.x, pSpawnPoint->position.y);

			pInst = pEnemy;
			pInst->stateFlag = pSpawnPoint->enemyLogicID;
			pInst->innerState = ON_ENTER;
			pInst->deployedFlag = ENEMY_WAITING;

			pSpawnPoint->pEnemyList[i] = pInst;
		}

		// Occupy the spawn
		pSpawnPoint->flag = OCCUPIED_SPAWN;
	}

	// Loop through the enemies and count live entities
	for (i = 0; i < pSpawnPoint->numEnemies; i++)
	{
		EnemyInstance* pEnemy = *(pSpawnPoint->pEnemyList) + i;

		// Count the enemyi non-active
		if ((pEnemy->mFlag & FLAG_ACTIVE) == 0)
		{
			// Decrement the amount of live enemies
			numLiveEnemies--;
			continue;
		}
	}

	// If all enemies died, flag the spawn point
	if (numLiveEnemies == 0)
		pSpawnPoint->flag = SEEKING_MOB;
	else
		pSpawnPoint->flag = OCCUPIED_SPAWN;
}

void UpdateSpawnedEnemyPositions(EnemySpawnPoint* pSpawnArray, double frameTime)
{
	unsigned int i;

	for (i = 0; i < NUM_SPAWNPOINTS; i++)
	{
		EnemyInstance*	pInst;

		// Skip unused spawn point
		if ((pSpawnArray[i].active & FLAG_ACTIVE) == 0)
			continue;

		// skip empty spawn point
		if (pSpawnArray[i].flag == SEEKING_MOB)
			continue;

		// Need a special function for special spawnpoit
		if (pSpawnArray[i].spID == MULTI_SPAWNER)
		{
			UpdateSpecialSpawnPointEnemyPositions(&pSpawnArray[i]);
			continue;
		}

		pInst = pSpawnArray[i].pEnemy;

		// skip non-active object
		if ((pInst->mFlag & FLAG_ACTIVE) == 0)
			continue;

		//Vector2DNormalize(&pInst->mpComponent_Physics->mVelocity, &pInst->mpComponent_Physics->mVelocity);
		pInst->mpComponent_Transform->mPosition.x += pInst->mpComponent_Physics->mVelocity.x  * (float)frameTime;
		pInst->mpComponent_Transform->mPosition.y += pInst->mpComponent_Physics->mVelocity.y * (float)frameTime;
	}
}

void UpdateSpawnedEnemyCollisions(EnemySpawnPoint* pSpawnArray, GameObjectInstance* pGameObjInstArray, unsigned int *pMonsterCounter)
{
	unsigned int i, j;
	int loop;
	EnemyInstance* enemy;

	// Loop through all spawn points
	for (i = 0; i < NUM_SPAWNPOINTS; i++)
	{
		// Skip unused spawn point
		if ((pSpawnArray[i].active & FLAG_ACTIVE) == 0)
			continue;

		// Need a special function for special spawnpoint
		if (pSpawnArray[i].spID == MULTI_SPAWNER)
		{
			UpdateSpecialSpawnPointEnemyCollisions(&pSpawnArray[i], pGameObjInstArray, pMonsterCounter);
			continue;
		}

		if (pSpawnArray[i].flag == SEEKING_MOB)
			continue;

		enemy = pSpawnArray[i].pEnemy;

		if ((enemy->mFlag & FLAG_ACTIVE) == 0)
			continue;

		// If it's a regular enemy unit
		if (enemy->mpComponent_Sprite->mpShape->mType == ENEMY_TYPE_REGULAR || enemy->mpComponent_Sprite->mpShape->mType == ENEMY_TYPE_MINION || enemy->mpComponent_Sprite->mpShape->mType == ENEMY_TYPE_WORLD2 || enemy->mpComponent_Sprite->mpShape->mType == ENEMY_TYPE_WORLD4)
		{
			// Create collision behaviours based on object types
			for (j = 0; j < GAME_OBJ_INST_NUM_MAX; j++)
			{
				GameObjectInstance * object2 = pGameObjInstArray + j;

				// Make sure the flag is active
				if ((object2->mFlag & FLAG_ACTIVE) == 0)
					continue;

				// What happens if a bullet touches it
				if (object2->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_BULLET)
				{
					if (StaticPointToStaticRect(&object2->mpComponent_Transform->mPosition, &enemy->mpComponent_Transform->mPosition, enemy->mpComponent_Transform->mScaleX, enemy->mpComponent_Transform->mScaleY))
					{
						float angle;

						// Death sound for enemy
						FMOD_System_PlaySound(FmodSystem, sound1, 0, 0, channel);

						// Enemy lose health or create pixels here
						for (angle = 0.0f; angle < 360.0f; angle += 4.0f)
						{
							float randNum = ((float)rand() / (float)(RAND_MAX / 10.0f));
							Explosion = GameObjectInstanceCreate(OBJECT_TYPE_ENIGMA_DEATHRED);
							Vector2DSet(&Explosion->mpComponent_Transform->mPosition, enemy->mpComponent_Transform->mPosition.x, enemy->mpComponent_Transform->mPosition.y);
							Vector2DFromAngleDeg(&Explosion->mpComponent_Physics->mVelocity, angle + randNum);
							Vector2DScale(&Explosion->mpComponent_Physics->mVelocity, &Explosion->mpComponent_Physics->mVelocity, 50.0f);
							Explosion->timer = 1.0f;
							Explosion->ParticleFlag = 1;
						}
						for (angle = 0.0f; angle < 360.0f; angle += 4.0f)
						{
							float randNum = ((float)rand() / (float)(RAND_MAX / 10.0f));
							Explosion = GameObjectInstanceCreate(OBJECT_TYPE_ENIGMA_DEATHWHITE);
							Vector2DSet(&Explosion->mpComponent_Transform->mPosition, enemy->mpComponent_Transform->mPosition.x, enemy->mpComponent_Transform->mPosition.y);
							Vector2DFromAngleDeg(&Explosion->mpComponent_Physics->mVelocity, angle + randNum);
							Vector2DScale(&Explosion->mpComponent_Physics->mVelocity, &Explosion->mpComponent_Physics->mVelocity, 40.0f);
							Explosion->timer = 1.0f;
							Explosion->ParticleFlag = 1;
						}
						for (angle = 0.0f; angle < 360.0f; angle += 4.0f)
						{
							float randNum = ((float)rand() / (float)(RAND_MAX / 10.0f));
							Explosion = GameObjectInstanceCreate(OBJECT_TYPE_ENIGMA_DEATHDEBRIS);
							Vector2DSet(&Explosion->mpComponent_Transform->mPosition, enemy->mpComponent_Transform->mPosition.x, enemy->mpComponent_Transform->mPosition.y);
							Vector2DFromAngleDeg(&Explosion->mpComponent_Physics->mVelocity, angle + randNum);
							Vector2DScale(&Explosion->mpComponent_Physics->mVelocity, &Explosion->mpComponent_Physics->mVelocity, 80.0f + (randNum * randNum));
							Explosion->timer = 1.0f;
							Explosion->ParticleFlag = 1;
						}


						// Destroy enemy and bullet
						EnemyInstanceDestroy(enemy, pMonsterCounter);

						EnemiesKilled++;

						pSpawnArray[i].flag = SEEKING_MOB;

						GameObjectInstanceDestroy(object2);
						break;
					}
				}

				// What happens if a ship touches this enemy
				if (object2->mpComponent_Sprite->mpShape->mType == OBJECT_TYPE_SHIP)
				{
					if (StaticRectToStaticRect(&object2->mpComponent_Transform->mPosition, SHIP_SIZE, SHIP_SIZE, &enemy->mpComponent_Transform->mPosition, enemy->mpComponent_Transform->mScaleX, enemy->mpComponent_Transform->mScaleY))
					{
						// Destroy Enemy
						EnemyInstanceDestroy(enemy, pMonsterCounter);
						pSpawnArray[i].flag = SEEKING_MOB;

						// Reset Ship

						IsHit();
						break;
					}
				}

			} // End of second loop
		} // End of regular enemy type

	}
}

void UpdateEnemyMatrices(EnemySpawnPoint* pSpawnArray)
{
	unsigned int i, j;

	for (i = 0; i < NUM_SPAWNPOINTS; i++)
	{
		Matrix2D		trans, rot, scale;
		EnemyInstance*	pInst;

		// Skip unused spawn point
		if ((pSpawnArray[i].active & FLAG_ACTIVE) == 0)
			continue;

		// skip empty spawn point
		if (pSpawnArray[i].flag == SEEKING_MOB)
			continue;

		// Need a special function for special spawnpoint
		if (pSpawnArray[i].spID == MULTI_SPAWNER)
		{
			for (j = 0; j < pSpawnArray[i].numEnemies; j++)
			{
				pInst = *(pSpawnArray[i].pEnemyList) + j;

				// skip non-active object
				if ((pInst->mFlag & FLAG_ACTIVE) == 0)
					continue;

				if (pInst->deployedFlag == ENEMY_WAITING)
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

			continue;
		}

		pInst = pSpawnArray[i].pEnemy;

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

void UpdateSpawnPointEnemies(EnemySpawnPoint* pSpawnArray, EnemyInstance* pEnemyArray, unsigned int *pMonsterIndex)
{
	unsigned int i;

	for (i = 0; i < NUM_SPAWNPOINTS; i++)
	{
		// Skip unused spawn point
		if ((pSpawnArray[i].active & FLAG_ACTIVE) == 0)
			continue;

		// Isolate special spawnpoints
		if (pSpawnArray[i].spID == MULTI_SPAWNER)
		{
			UpdateSpecialEnemySpawnpoint(&pSpawnArray[i], pEnemyArray, pMonsterIndex);
			continue;
		}

		// Don't bother updating the spawn point if it's not looking for an enemy
		if (pSpawnArray[i].flag != SEEKING_MOB)
			continue;

		// Update the empty spawn point's timers
		UpdateTimer(pSpawnArray[i].SpawnTimer);

		// Only put in a new enemy if the spawn timer has counted fully
		if (pSpawnArray[i].SpawnTimer->doneCounting == DONE_COUNTING)
		{

			EnemyInstance* pEnemyInst = pEnemyArray + (*pMonsterIndex)++;
			float angle;

			// Put the mob in position
			Vector2DSet(&pEnemyInst->mpComponent_Transform->mPosition, pSpawnArray[i].position.x, pSpawnArray[i].position.y);

			// Give the behavior to the enemy depending on spawn point
			pEnemyInst->stateFlag = pSpawnArray[i].enemyLogicID;

			// Occupy the spawn point
			pSpawnArray[i].flag = OCCUPIED_SPAWN;

			// Reset the timer
			ResetTimer(pSpawnArray[i].SpawnTimer);

			// Assign the enemy pointer
			pSpawnArray[i].pEnemy = pEnemyInst;

			for (angle = 0.0f; angle < 360.0f; angle += 4.0f)
			{
				float randNum = ((float)rand() / (float)(RAND_MAX / 10.0f));
				Explosion = GameObjectInstanceCreate(OBJECT_TYPE_ENIGMA_SPAWN);
				Vector2DSet(&Explosion->mpComponent_Transform->mPosition, pEnemyInst->mpComponent_Transform->mPosition.x, pEnemyInst->mpComponent_Transform->mPosition.y);
				Vector2DFromAngleDeg(&Explosion->mpComponent_Physics->mVelocity, angle + randNum);
				Vector2DScale(&Explosion->mpComponent_Physics->mVelocity, &Explosion->mpComponent_Physics->mVelocity, 40.0f);
				Explosion->timer = 1.0f;
				Explosion->ParticleFlag = 2;
			}

		}
	}
}

EnemySpawnPoint SetSpawnPoint(EnemySpawnPoint pSpawnPoint, Vector2D position, unsigned int flag, unsigned short logic_flag)
{
	pSpawnPoint.flag = flag;
	pSpawnPoint.enemyLogicID = logic_flag;
	Vector2DSet(&(pSpawnPoint.position), position.x, position.y);

	return pSpawnPoint;
}

void DestroySpawnPoint(EnemySpawnPoint* pSpawnPoint)
{
	RemoveComponent_Timer(pSpawnPoint->SpawnTimer);
}

void UpdateSpawnedEnemyBehaviours(EnemySpawnPoint* pSpawnArray)
{
	unsigned int i, j;

	for (i = 0; i < NUM_SPAWNPOINTS; i++)
	{
		EnemyInstance*	pInst;

		// Skip unused spawn point
		if ((pSpawnArray[i].active & FLAG_ACTIVE) == 0)
			continue;

		// skip empty spawn point
		if (pSpawnArray[i].flag == SEEKING_MOB)
			continue;

		// Instruct special spawn
		if (pSpawnArray[i].spID == MULTI_SPAWNER)
		{
			// Update the timer for deployment delay
			UpdateTimer(pSpawnArray[i].SpawnTimer);

			for (j = 0; j < pSpawnArray[i].numEnemies; j++)
			{
				pInst = *(pSpawnArray[i].pEnemyList) + j;

				// skip non-active object
				if ((pInst->mFlag & FLAG_ACTIVE) == 0)
					continue;

				// If timer is done, flag this enemy for deployment and reset the counter
				if (pSpawnArray[i].SpawnTimer->doneCounting == DONE_COUNTING && pInst->deployedFlag == ENEMY_WAITING)
				{
					float angle;

					pInst->deployedFlag = ENEMY_DEPLOYED;
					ResetTimer(pSpawnArray[i].SpawnTimer);

					// Spawn particle
					for (angle = 0.0f; angle < 360.0f; angle += 4.0f)
					{
						float randNum = ((float)rand() / (float)(RAND_MAX / 10.0f));
						Explosion = GameObjectInstanceCreate(OBJECT_TYPE_ENIGMA_SPAWN);
						Vector2DSet(&Explosion->mpComponent_Transform->mPosition, pInst->mpComponent_Transform->mPosition.x, pInst->mpComponent_Transform->mPosition.y);
						Vector2DFromAngleDeg(&Explosion->mpComponent_Physics->mVelocity, angle + randNum);
						Vector2DScale(&Explosion->mpComponent_Physics->mVelocity, &Explosion->mpComponent_Physics->mVelocity, 40.0f);
						Explosion->timer = 1.0f;
						Explosion->ParticleFlag = 2;
					}
				}

				// If it's deployed, give it actions
				if (pInst->deployedFlag == ENEMY_DEPLOYED)
				{
					EnemyStateMachine(pInst);
					EnemyWeaponMachine(pInst);
				}
			}

			continue;
		}

		pInst = pSpawnArray[i].pEnemy;

		// skip non-active object
		if ((pInst->mFlag & FLAG_ACTIVE) == 0)
			continue;

		EnemyStateMachine(pInst);
		EnemyWeaponMachine(pInst);
	}


}


void EnemyStateMachine(EnemyInstance *pInst)
{
	switch (pInst->stateFlag)
	{

	case HORIZONTAL_MOVEMENT_L_TO_R:
		switch (pInst->innerState)
		{
		case ON_ENTER:
			pInst->mpComponent_Physics->mVelocity.x = ENEMY_VEL_REGULAR;
			pInst->innerState = ON_UPDATE;
			pInst->stateCounter = MOVEMENT_COUNTER;
			break;

		case ON_UPDATE:
			pInst->stateCounter -= frameTime;

			// Make sure we're within bounds of grid as well
			if (pInst->stateCounter <= 0.0 || pInst->mpComponent_Transform->mPosition.x >= 370.0f)
			{
				pInst->innerState = ON_EXIT;
			}
			break;

		case ON_EXIT:
			pInst->stateFlag = HORIZONTAL_MOVEMENT_R_TO_L;
			pInst->innerState = ON_ENTER;
			break;
		}
		break;

	case HORIZONTAL_MOVEMENT_R_TO_L:
		switch (pInst->innerState)
		{
		case ON_ENTER:
			pInst->mpComponent_Physics->mVelocity.x = -ENEMY_VEL_REGULAR;
			pInst->innerState = ON_UPDATE;
			pInst->stateCounter = MOVEMENT_COUNTER;
			break;

		case ON_UPDATE:
			pInst->stateCounter -= frameTime;

			if (pInst->stateCounter <= 0.0 || pInst->mpComponent_Transform->mPosition.x <= -370.0f)
			{
				pInst->innerState = ON_EXIT;
			}
			break;

		case ON_EXIT:
			pInst->stateFlag = HORIZONTAL_MOVEMENT_L_TO_R;
			pInst->innerState = ON_ENTER;
			break;
		}
		break;

	case VERTICAL_MOVEMENT_UP_TO_DOWN:
		switch (pInst->innerState)
		{
		case ON_ENTER:
			pInst->mpComponent_Physics->mVelocity.y = -ENEMY_VEL_REGULAR;
			pInst->innerState = ON_UPDATE;
			pInst->stateCounter = MOVEMENT_COUNTER;
			break;

		case ON_UPDATE:
			pInst->stateCounter -= frameTime;

			if (pInst->stateCounter <= 0.0 || pInst->mpComponent_Transform->mPosition.y <= 0.0f)
			{
				pInst->innerState = ON_EXIT;
			}
			break;

		case ON_EXIT:
			pInst->stateFlag = VERTICAL_MOVEMENT_DOWN_TO_UP;
			pInst->innerState = ON_ENTER;
			break;
		}
		break;

	case VERTICAL_MOVEMENT_DOWN_TO_UP:
		switch (pInst->innerState)
		{
		case ON_ENTER:
			pInst->mpComponent_Physics->mVelocity.y = ENEMY_VEL_REGULAR / 2;
			pInst->innerState = ON_UPDATE;
			pInst->stateCounter = MOVEMENT_COUNTER;
			break;

		case ON_UPDATE:
			pInst->stateCounter -= frameTime;

			// If we've been in this state long enough or if we need to turn back due to boundaries
			if (pInst->stateCounter <= 0.0 || pInst->mpComponent_Transform->mPosition.y >= 300.0f)
			{
				pInst->innerState = ON_EXIT;
			}
			break;

		case ON_EXIT:
			pInst->stateFlag = VERTICAL_MOVEMENT_UP_TO_DOWN;
			pInst->innerState = ON_ENTER;
			break;
		}
		break;

	case SQUARE_PATTERN_START_BOTTOM_LEFT:
		switch (pInst->innerState)
		{
		case ON_ENTER:
			pInst->innerState = ON_UPDATE;
			pInst->stateCounter = MOVEMENT_COUNTER + DEPLOYMENT_TIMER_OFFSET;

			pInst->movementPhase = PHASE_1;
			break;

		case ON_UPDATE:
			pInst->stateCounter -= frameTime;

			// Switch between phases
			switch (pInst->movementPhase)
			{
				// Phase 1 will be going to the right
			case PHASE_1:
			{
				pInst->mpComponent_Physics->mVelocity.x = ENEMY_VEL_REGULAR;
				pInst->mpComponent_Physics->mVelocity.y = 0.0f;

				// If we've spent the desired time in this phase
				if (pInst->stateCounter <= 0.0)
				{
					// Reset the timer and move on to next phase
					pInst->stateCounter = MOVEMENT_COUNTER;

					pInst->movementPhase = PHASE_2;
				}

				break;
			}

			// Going up
			case PHASE_2:
			{
				pInst->mpComponent_Physics->mVelocity.x = 0.0f;
				pInst->mpComponent_Physics->mVelocity.y = ENEMY_VEL_REGULAR;

				// If we've spent the desired time in this phase
				if (pInst->stateCounter <= 0.0)
				{
					// Reset the timer and move on to next phase
					pInst->stateCounter = MOVEMENT_COUNTER;

					pInst->movementPhase = PHASE_3;
				}

				break;
			}

			// Going left
			case PHASE_3:
			{
				pInst->mpComponent_Physics->mVelocity.x = -ENEMY_VEL_REGULAR;
				pInst->mpComponent_Physics->mVelocity.y = 0.0f;

				// If we've spent the desired time in this phase
				if (pInst->stateCounter <= 0.0)
				{
					// Reset the timer and move on to next phase
					pInst->stateCounter = MOVEMENT_COUNTER;

					pInst->movementPhase = PHASE_4;
				}

				break;
			}

			case PHASE_4:
			{
				pInst->mpComponent_Physics->mVelocity.x = 0.0f;
				pInst->mpComponent_Physics->mVelocity.y = -ENEMY_VEL_REGULAR;

				// If we've spent the desired time in this phase
				if (pInst->stateCounter <= 0.0)
				{
					// Reset the timer and move on to next phase
					pInst->stateCounter = MOVEMENT_COUNTER;

					pInst->movementPhase = PHASE_1;
				}

				break;
			}
			}


			if (pInst->stateCounter <= 0.0)
			{
				// If we're going too far right -> move UP
				if (pInst->mpComponent_Transform->mPosition.x >= 300.0f && pInst->mpComponent_Transform->mPosition.y <= 150.0f)
				{
					// Since square pattern and we were moving to the right
					pInst->mpComponent_Physics->mVelocity.x = 0.0f;
					pInst->mpComponent_Physics->mVelocity.y = ENEMY_VEL_REGULAR;
				}

				// If we're going too far up -> move LEFT
				if (pInst->mpComponent_Transform->mPosition.x > 200.0f && pInst->mpComponent_Transform->mPosition.y >= 200.0f)
				{
					pInst->mpComponent_Physics->mVelocity.x = -ENEMY_VEL_REGULAR;
					pInst->mpComponent_Physics->mVelocity.y = 0.0f;
				}

				// If we're going too far left -> move DOWN
				if (pInst->mpComponent_Transform->mPosition.x <= -300.0f && pInst->mpComponent_Transform->mPosition.y > 150.0f)
				{
					pInst->mpComponent_Physics->mVelocity.x = 0.0f;
					pInst->mpComponent_Physics->mVelocity.y = -ENEMY_VEL_REGULAR;
				}

				// If we're going too far down -> move RIGHT
				if (pInst->mpComponent_Transform->mPosition.x < -200.0f && pInst->mpComponent_Transform->mPosition.y <= 200.0f)
				{
					pInst->mpComponent_Physics->mVelocity.x = ENEMY_VEL_REGULAR;
					pInst->mpComponent_Physics->mVelocity.y = 0.0f;
				}
			}

			break;

		case ON_EXIT:
			break;
		}

	case SPIRAL_PATTERN_L_TO_R:
		switch (pInst->innerState)
		{
		case ON_ENTER:
			pInst->innerState = ON_UPDATE;
			pInst->stateCounter = MOVEMENT_COUNTER + DEPLOYMENT_TIMER_OFFSET;

			pInst->movement_angle = 0;
			pInst->movementPhase = PHASE_1;
			break;

		case ON_UPDATE:
			pInst->stateCounter -= frameTime;

			// Switch between phases
			switch (pInst->movementPhase)
			{
				// Phase 1 will be going to the right
			case PHASE_1:
			{
				pInst->movement_angle += PI * frameTime;

				pInst->mpComponent_Physics->mVelocity.x = ENEMY_VEL_REGULAR;
				pInst->mpComponent_Physics->mVelocity.y = sinf(pInst->movement_angle) * ENEMY_VEL_REGULAR * 1.5;

				// If we've spent the desired time in this phase
				if (pInst->stateCounter <= 0.0)
				{
					// Reset the timer and move on to next phase
					pInst->stateCounter = MOVEMENT_COUNTER;
					//	pInst->movement_angle = 0.0f;

					pInst->movementPhase = PHASE_2;
				}

				break;
			}

			// Going left
			case PHASE_2:
			{
				pInst->movement_angle += PI * frameTime;

				pInst->mpComponent_Physics->mVelocity.x = -ENEMY_VEL_REGULAR;
				pInst->mpComponent_Physics->mVelocity.y = sinf(pInst->movement_angle) * ENEMY_VEL_REGULAR * 1.5;

				// If we've spent the desired time in this phase
				if (pInst->stateCounter <= 0.0)
				{
					// Reset the timer and move on to next phase
					pInst->stateCounter = MOVEMENT_COUNTER;
					//		pInst->movement_angle = 0.0f;

					pInst->movementPhase = PHASE_1;
				}

				break;
			}
			}	// End of inner phase

			break;

		case ON_EXIT:
			break;
		}
		break;


	case SPIRAL_PATTERN_R_TO_L:
		switch (pInst->innerState)
		{
		case ON_ENTER:
			pInst->innerState = ON_UPDATE;
			pInst->stateCounter = MOVEMENT_COUNTER + DEPLOYMENT_TIMER_OFFSET;

			pInst->movement_angle = 0;
			pInst->movementPhase = PHASE_1;
			break;

		case ON_UPDATE:
			pInst->stateCounter -= frameTime;

			// Switch between phases
			switch (pInst->movementPhase)
			{
				// Phase 1 will be going to the right
			case PHASE_1:
			{
				pInst->movement_angle += PI * frameTime;

				pInst->mpComponent_Physics->mVelocity.x = -ENEMY_VEL_REGULAR;
				pInst->mpComponent_Physics->mVelocity.y = sinf(pInst->movement_angle) * ENEMY_VEL_REGULAR * 1.5;

				// If we've spent the desired time in this phase
				if (pInst->stateCounter <= 0.0)
				{
					// Reset the timer and move on to next phase
					pInst->stateCounter = MOVEMENT_COUNTER;
					//	pInst->movement_angle = 0.0f;

					pInst->movementPhase = PHASE_2;
				}

				break;
			}

			// Going left
			case PHASE_2:
			{
				pInst->movement_angle += PI * frameTime;

				pInst->mpComponent_Physics->mVelocity.x = ENEMY_VEL_REGULAR;
				pInst->mpComponent_Physics->mVelocity.y = sinf(pInst->movement_angle) * ENEMY_VEL_REGULAR * 1.5;

				// If we've spent the desired time in this phase
				if (pInst->stateCounter <= 0.0)
				{
					// Reset the timer and move on to next phase
					pInst->stateCounter = MOVEMENT_COUNTER;
					//		pInst->movement_angle = 0.0f;

					pInst->movementPhase = PHASE_1;
				}

				break;
			}

			}	// End of inner phase

			break;

		case ON_EXIT:
			break;
		}
		break;

	case SPIRAL_PATTERN_UP_TO_DOWN:
		switch (pInst->innerState)
		{
		case ON_ENTER:
			pInst->innerState = ON_UPDATE;
			pInst->stateCounter = 4.0;

			pInst->movement_angle = 0;
			pInst->movementPhase = PHASE_1;
			break;

		case ON_UPDATE:
			pInst->stateCounter -= frameTime;

			// Switch between phases
			switch (pInst->movementPhase)
			{
				// Phase 1 will be going to the right
			case PHASE_1:
			{
				pInst->movement_angle += PI * frameTime;

				pInst->mpComponent_Physics->mVelocity.x = sinf(pInst->movement_angle) * ENEMY_VEL_REGULAR * 2.5f;
				pInst->mpComponent_Physics->mVelocity.y = -ENEMY_VEL_REGULAR;

				// If we've spent the desired time in this phase
				if (pInst->stateCounter <= 0.0)
				{
					// Reset the timer and move on to next phase
					pInst->stateCounter = 4.0;
					//	pInst->movement_angle = 0.0f;

					pInst->movementPhase = PHASE_2;
				}

				break;
			}

			// Going left
			case PHASE_2:
			{
				pInst->movement_angle += PI * frameTime;

				pInst->mpComponent_Physics->mVelocity.x = -sinf(pInst->movement_angle) * ENEMY_VEL_REGULAR * 2.5f;
				pInst->mpComponent_Physics->mVelocity.y = ENEMY_VEL_REGULAR;

				// If we've spent the desired time in this phase
				if (pInst->stateCounter <= 0.0)
				{
					// Reset the timer and move on to next phase
					pInst->stateCounter = 4.0;
					//		pInst->movement_angle = 0.0f;

					pInst->movementPhase = PHASE_1;
				}

				break;
			}
			}	// End of inner phase

			break;

		case ON_EXIT:
			break;
		}
		break;


	case CIRCLE_PATTERN:
		switch (pInst->innerState)
		{
		case ON_ENTER:
			pInst->innerState = ON_UPDATE;
			pInst->stateCounter = MOVEMENT_COUNTER;

			pInst->movement_angle = 0;
			pInst->movementPhase = PHASE_1;
			break;

		case ON_UPDATE:
			pInst->movement_angle += PI * frameTime;

			pInst->mpComponent_Physics->mVelocity.x = cosf(pInst->movement_angle) * ENEMY_VEL_REGULAR * 2.0f;
			pInst->mpComponent_Physics->mVelocity.y = sinf(pInst->movement_angle) * ENEMY_VEL_REGULAR * 2.0f;

			break;

		case ON_EXIT:
			break;
		}
		break;

	case OVAL_PATTERN_HORIZONTAL:
		switch (pInst->innerState)
		{
		case ON_ENTER:
			pInst->innerState = ON_UPDATE;
			pInst->stateCounter = MOVEMENT_COUNTER;

			pInst->movement_angle = 0;
			pInst->movementPhase = PHASE_1;
			break;

		case ON_UPDATE:
			pInst->movement_angle += PI * frameTime;

			pInst->mpComponent_Physics->mVelocity.x = cosf(pInst->movement_angle) * ENEMY_VEL_REGULAR * 5.0f;
			pInst->mpComponent_Physics->mVelocity.y = sinf(pInst->movement_angle) * ENEMY_VEL_REGULAR;

			break;

		case ON_EXIT:
			break;
		}
		break;

	case OVAL_PATTERN_VERTICAL:
		switch (pInst->innerState)
		{
		case ON_ENTER:
			pInst->innerState = ON_UPDATE;
			pInst->stateCounter = MOVEMENT_COUNTER;

			pInst->movement_angle = 0;
			pInst->movementPhase = PHASE_1;
			break;

		case ON_UPDATE:
			pInst->movement_angle += PI * frameTime;

			pInst->mpComponent_Physics->mVelocity.x = cosf(pInst->movement_angle) * ENEMY_VEL_REGULAR;
			pInst->mpComponent_Physics->mVelocity.y = sinf(pInst->movement_angle) * ENEMY_VEL_REGULAR * 5.0f;

			break;

		case ON_EXIT:
			break;
		}
		break;

	}
}

GameObjectInstance* BulletCreate(Vector2D position, Vector2D velocity, float angle, unsigned int bulletType)
{
	GameObjectInstance* pBullet = GameObjectInstanceCreate(bulletType);

	pBullet->mpComponent_Physics->mVelocity.x = velocity.x;
	pBullet->mpComponent_Physics->mVelocity.y = velocity.y;

	pBullet->mpComponent_Transform->mPosition.x = position.x;
	pBullet->mpComponent_Transform->mPosition.y = position.y;

	pBullet->mpComponent_Transform->mAngle = angle;

	return pBullet;
}


void ScatterShotExplosion(Vector2D position, unsigned int num_shots, unsigned int bulletType)
{
	// Direction of the bullet
	Vector2D dirVector;

	// Angle between each shot
	float angle_step_size = (2.0f * PI) / num_shots;

	// Boundaries for the angle
	float cur_angle = 0.0f;
	float angle_end = (2.0f * PI) - angle_step_size;

	for (cur_angle; cur_angle <= angle_end; cur_angle += angle_step_size)
	{
		// Set the direction of the velocity vector
		Vector2DSet(&dirVector, cosf(cur_angle), sinf(cur_angle));
		Vector2DScale(&dirVector, &dirVector, BULLET_SPEED);
		BulletCreate(position, dirVector, cur_angle, bulletType);
	}
}

int active_standard_enemy_bullets;
void EnemyWeaponMachine(EnemyInstance *pInst)
{
	// What's this enemie's weapon type
	switch (pInst->mpComponent_EnemyData->wepType)
	{
		// If it's the STANDARD wep type, act accordingly
	case STANDARD_WEAPON:
	{
		// What's the current state of this weapon?
		switch (pInst->mpComponent_EnemyData->wepState)
		{
			// Let's execute this case only if the enemy is somewhere near player at all --NOT ADDED YET
		case ON_ENTER:
		{
			// If the enemy does not align with the player on the the x-axis, don't shoot 
			if (pInst->mpComponent_Transform->mPosition.x >= sgpShip->mpComponent_Transform->mPosition.x - ENEMY_POSITION_CONDITION_OFFSET
				&& pInst->mpComponent_Transform->mPosition.x <= sgpShip->mpComponent_Transform->mPosition.x + ENEMY_POSITION_CONDITION_OFFSET)
			{
				Vector2D bulletVelocity;
				GameObjectInstance* pBullet;

				// Shoot this bullet downward
				Vector2DSet(&bulletVelocity, 0.0f, -1.0f * BULLET_SPEED);

				// Create the bullet
				pBullet = BulletCreate(pInst->mpComponent_Transform->mPosition, bulletVelocity, pInst->mpComponent_Transform->mAngle, OBJECT_TYPE_ENEMY_BULLET);

				// Reset the timer
				ResetTimer(pInst->mpComponent_EnemyData->wepDelayTimer);

				// Commence updating
				pInst->mpComponent_EnemyData->wepState = ON_UPDATE;
				break;
			}
		}

		// Handle delay or anything that should happen while the enemy is waiting to shoot again
		case ON_UPDATE:
		{
			// Update the timer
			UpdateTimer(pInst->mpComponent_EnemyData->wepDelayTimer);

			// Allow to shoot again after the timer is done ticking
			if (pInst->mpComponent_EnemyData->wepDelayTimer->doneCounting == DONE_COUNTING)
			{
				// Go back to the beginning
				pInst->mpComponent_EnemyData->wepState = ON_ENTER;
			}

			break;
		}

		// No functionality needed for on exit since we shoot on a timer
		case ON_EXIT:
			break;
		}
		break;
	}

	}
}


float boss_wep_angle = 0;
Timer* boss_wep_timer;
Timer* burst_fire_delay;
Timer* FinalPhase3;
int	bullets_fired;
int active_grenade_shots;
void BossWeaponMachine(GameObjectInstance *pInst)
{

	// What's the weapon phase?
	switch (pInst->stateFlag)
	{
		// Sprinkler Pattern
	case SPRINKLER_SHOTS:
	{
		switch (pInst->innerState)
		{
		case ON_ENTER:
		{
			// Not much to initialize
			pInst->innerState = ON_UPDATE;
			boss_wep_angle = PI / 4.0f;

			pInst->innerPhase = PHASE_1;
			boss_wep_timer = ConstructTimer(0.5);
			bullets_fired = 0;
			break;
		}

		// Handle delay or anything that should happen while the enemy is waiting to shoot again
		case ON_UPDATE:
		{
			float angle_offset = PI / 4.0f;

			float start_angle = 0 + angle_offset;
			float end_angle = PI - angle_offset;

			float angle_step_size = PI / 12.0f;

			switch (pInst->innerPhase)
			{
				// Phase 1 of sprinklers is shooting from smaller angle to the bigger angle
			case PHASE_1:
			{
				UpdateTimer(boss_wep_timer);

				if (boss_wep_timer->doneCounting == DONE_COUNTING)
				{
					Vector2D bulletVelocity;
					Vector2D bulletPosition;

					GameObjectInstance* pBullet;

					// Create the bullet vector and negate the y direction
					Vector2DSet(&bulletVelocity, cosf(boss_wep_angle) * BULLET_SPEED * 1.35f, sinf(boss_wep_angle) * BULLET_SPEED * 1.35f);
					bulletVelocity.y *= -1.0f;

					// Create the position
					Vector2DSet(&bulletPosition, pInst->mpComponent_Transform->mPosition.x, pInst->mpComponent_Transform->mPosition.y);

					// Create the bullet
					pBullet = BulletCreate(bulletPosition, bulletVelocity, -boss_wep_angle, OBJECT_TYPE_ENEMY_BULLET);

					// Increment the angle
					boss_wep_angle += angle_step_size;

					// If the angle exceeds the end angle, do phase 2 pattern
					if (boss_wep_angle >= end_angle)
					{
						pInst->innerPhase = PHASE_2;
					}

					ResetTimer(boss_wep_timer);
				}

				break;
			}	// End of Phase 1

				// Phase 2 is shooting from bigger angle to the smaller angle
			case PHASE_2:
			{
				UpdateTimer(boss_wep_timer);

				if (boss_wep_timer->doneCounting == DONE_COUNTING)
				{
					Vector2D bulletVelocity;
					Vector2D bulletPosition;

					GameObjectInstance* pBullet;

					// Create the bullet vector and negate the y direction
					Vector2DSet(&bulletVelocity, cosf(boss_wep_angle) * BULLET_SPEED * 1.35f, sinf(boss_wep_angle) * BULLET_SPEED * 1.35f);
					bulletVelocity.y *= -1.0f;

					// Create the position
					Vector2DSet(&bulletPosition, pInst->mpComponent_Transform->mPosition.x, pInst->mpComponent_Transform->mPosition.y);

					// Create the bullet
					pBullet = BulletCreate(bulletPosition, bulletVelocity, -boss_wep_angle, OBJECT_TYPE_ENEMY_BULLET);

					// Increment the angle
					boss_wep_angle -= angle_step_size;

					// If the angle exceeds the start angle, do phase 1 pattern
					if (boss_wep_angle <= start_angle)
					{
						pInst->innerPhase = PHASE_1;
					}

					ResetTimer(boss_wep_timer);
				}

				break;
			}	// End of Phase 2

			}

			// Sprinkler Shots will be the first 1/3rd of health
			if (pInst->bossHealth < 20)
			{
				pInst->innerState = ON_EXIT;
			}

			break;
		}

		//	Go to next boss weapon
		case ON_EXIT:
		{
			pInst->stateFlag = HOMING_SHOTS;
			pInst->innerState = ON_ENTER;
			break;
		}
		}

		break;
	}// End of second Switch


	 // Triple parallel bullets home towards the players position
	case HOMING_SHOTS:
	{
		switch (pInst->innerState)
		{
		case ON_ENTER:
		{

			// Not much to initialize
			pInst->innerState = ON_UPDATE;

			pInst->innerPhase = PHASE_1;

			boss_wep_timer->duration = 0.1;

			burst_fire_delay = ConstructTimer(.7);
			bullets_fired = 0;
			break;
		}

		case ON_UPDATE:
		{

			switch (pInst->innerPhase)
			{
				// Phase 1 of sprinklers is shooting from smaller angle to the bigger angle
			case PHASE_1:
			{
				UpdateTimer(burst_fire_delay);

				// When this is on, shoot for a while
				if (burst_fire_delay->doneCounting == DONE_COUNTING)
				{

					UpdateTimer(boss_wep_timer);

					if (boss_wep_timer->doneCounting == DONE_COUNTING)
					{
						Vector2D bulletVelocity;
						Vector2D bulletPosition;

						GameObjectInstance* pBullet;
						float angle;

						// Get the proper directional vector
						bulletVelocity.x = sgpShip->mpComponent_Transform->mPosition.x - pInst->mpComponent_Transform->mPosition.x;
						bulletVelocity.y = sgpShip->mpComponent_Transform->mPosition.y - pInst->mpComponent_Transform->mPosition.y;

						// Normalize the vector and then scale it by the bullet speed
						Vector2DNormalize(&bulletVelocity, &bulletVelocity);

						angle = AngleFromVector(&bulletVelocity);

						// Scale the velocity
						Vector2DScale(&bulletVelocity, &bulletVelocity, BULLET_SPEED * .90f);

						// Create the position
						Vector2DSet(&bulletPosition, pInst->mpComponent_Transform->mPosition.x, pInst->mpComponent_Transform->mPosition.y);

						// Create the bullet
						pBullet = BulletCreate(bulletPosition, bulletVelocity, -angle, OBJECT_TYPE_ENEMY_BULLET);

						// Increment the bullets shot to keep track of our burst fire
						bullets_fired++;

						// Track burst configs
						if (bullets_fired >= 4)
						{
							bullets_fired = 0;
							ResetTimer(burst_fire_delay);
						}

						ResetTimer(boss_wep_timer);
					}
				}

				break;
			}	// End of Phase 1

			}	// End of phase switching


				// This phase happens for a small amount of time
			if (pInst->bossHealth < 10)
			{
				pInst->innerState = ON_EXIT;
			}

			break;
		}

		case ON_EXIT:
		{
			pInst->stateFlag = GRENADE_SHOTS;
			pInst->innerState = ON_ENTER;
			break;
		}
		}

		break;

	}// End of second Switch


	 // Grenade that detonates near the player and creates a temporary harmful region  
	case GRENADE_SHOTS:
	{
		switch (pInst->innerState)
		{
		case ON_ENTER:
		{
			// Not much to initialize
			pInst->innerState = ON_UPDATE;

			pInst->innerPhase = PHASE_1;

			boss_wep_timer->duration = 0.35;

			active_grenade_shots = 0;
			bullets_fired = 0;
			break;
		}

		case ON_UPDATE:
		{

			switch (pInst->innerPhase)
			{
				// Grenade shot phase shoots a couple missiles
			case PHASE_1:
			{
				// Don't shoot
				if (active_grenade_shots < 2)
				{
					UpdateTimer(boss_wep_timer);

					if (boss_wep_timer->doneCounting == DONE_COUNTING)
					{
						Vector2D bulletVelocity;
						Vector2D bulletPosition;

						GameObjectInstance* pBullet;
						float angle;

						// Get the proper directional vector
						bulletVelocity.x = sgpShip->mpComponent_Transform->mPosition.x - pInst->mpComponent_Transform->mPosition.x;
						bulletVelocity.y = sgpShip->mpComponent_Transform->mPosition.y - pInst->mpComponent_Transform->mPosition.y;

						// Normalize the vector and then scale it by the bullet speed
						Vector2DNormalize(&bulletVelocity, &bulletVelocity);

						angle = AngleFromVector(&bulletVelocity);

						// Scale the velocity
						Vector2DScale(&bulletVelocity, &bulletVelocity, BULLET_SPEED * 1.0f);

						// Create the position
						Vector2DSet(&bulletPosition, pInst->mpComponent_Transform->mPosition.x, pInst->mpComponent_Transform->mPosition.y);

						// Create the missile
						pBullet = BulletCreate(bulletPosition, bulletVelocity, -angle, OBJECT_TYPE_HOMING_MISSILE);

						// Follow the target for some amount of seconds before exploding
						pBullet->mpComponent_Target->follow_timer->duration = 2.0;

						// Increment the active missiles
						active_grenade_shots++;

						ResetTimer(boss_wep_timer);
					}
				}

				break;
			}
			}
		}

		case ON_EXIT:
			break;
		}

		break;

	}// End of second Switch

	case FINAL_BOSS:
	{
		switch (pInst->innerState)
		{
		case ON_ENTER:
		{

			// Not much to initialize
			pInst->innerState = ON_UPDATE;

			pInst->innerPhase = PHASE_1;

			boss_wep_timer = ConstructTimer(.1);

			FinalPhase3 = ConstructTimer(.5);
			burst_fire_delay = ConstructTimer(.2);
			bullets_fired = 0;
			break;
		}

		case ON_UPDATE:
		{
			if (pInst->bossHealth <= 300 && pInst->bossHealth > 200)
			{
				pInst->innerPhase = PHASE_2;
			}
			if (pInst->bossHealth <= 200 && pInst->bossHealth > 100)
			{
				pInst->innerPhase = PHASE_3;
			}
			if (pInst->bossHealth <= 100)
			{
				pInst->innerPhase = PHASE_4;
			}
			switch (pInst->innerPhase)
			{
				// Phase 1 of sprinklers is shooting from smaller angle to the bigger angle
			case PHASE_1:
			{
				UpdateTimer(burst_fire_delay);

				// When this is on, shoot for a while
				if (burst_fire_delay->doneCounting == DONE_COUNTING)
				{

					UpdateTimer(boss_wep_timer);

					if (boss_wep_timer->doneCounting == DONE_COUNTING)
					{
						Vector2D bulletVelocity;
						Vector2D bulletPosition;

						GameObjectInstance* pBullet;
						float angle;

						// Get the proper directional vector
						bulletVelocity.x = sgpShip->mpComponent_Transform->mPosition.x - pInst->mpComponent_Transform->mPosition.x;
						bulletVelocity.y = sgpShip->mpComponent_Transform->mPosition.y - pInst->mpComponent_Transform->mPosition.y;

						// Normalize the vector and then scale it by the bullet speed
						Vector2DNormalize(&bulletVelocity, &bulletVelocity);

						angle = AngleFromVector(&bulletVelocity);

						// Scale the velocity
						Vector2DScale(&bulletVelocity, &bulletVelocity, BULLET_SPEED * .90f);

						// Create the position
						Vector2DSet(&bulletPosition, pInst->mpComponent_Transform->mPosition.x - 10.0f, pInst->mpComponent_Transform->mPosition.y);

						// Create the bullet
						pBullet = BulletCreate(bulletPosition, bulletVelocity, -angle, OBJECT_TYPE_ENEMY_BULLET);

						// Increment the bullets shot to keep track of our burst fire
						bullets_fired++;

						// Track burst configs
						if (bullets_fired >= 4)
						{
							bullets_fired = 0;
							ResetTimer(burst_fire_delay);
						}

						// Get the proper directional vector
						bulletVelocity.x = sgpShip->mpComponent_Transform->mPosition.x - pInst->mpComponent_Transform->mPosition.x;
						bulletVelocity.y = sgpShip->mpComponent_Transform->mPosition.y - pInst->mpComponent_Transform->mPosition.y;

						// Normalize the vector and then scale it by the bullet speed
						Vector2DNormalize(&bulletVelocity, &bulletVelocity);

						angle = AngleFromVector(&bulletVelocity);

						// Scale the velocity
						Vector2DScale(&bulletVelocity, &bulletVelocity, BULLET_SPEED * .90f);

						// Create the position
						Vector2DSet(&bulletPosition, pInst->mpComponent_Transform->mPosition.x + 10.0f, pInst->mpComponent_Transform->mPosition.y);

						// Create the bullet
						pBullet = BulletCreate(bulletPosition, bulletVelocity, -angle, OBJECT_TYPE_ENEMY_BULLET);

						// Increment the bullets shot to keep track of our burst fire
						bullets_fired++;

						// Track burst configs
						if (bullets_fired >= 4)
						{
							bullets_fired = 0;
							ResetTimer(burst_fire_delay);
						}

						// Get the proper directional vector
						bulletVelocity.x = sgpShip->mpComponent_Transform->mPosition.x - pInst->mpComponent_Transform->mPosition.x;
						bulletVelocity.y = sgpShip->mpComponent_Transform->mPosition.y - pInst->mpComponent_Transform->mPosition.y;

						// Normalize the vector and then scale it by the bullet speed
						Vector2DNormalize(&bulletVelocity, &bulletVelocity);

						angle = AngleFromVector(&bulletVelocity);

						// Scale the velocity
						Vector2DScale(&bulletVelocity, &bulletVelocity, BULLET_SPEED * .90f);

						// Create the position
						Vector2DSet(&bulletPosition, pInst->mpComponent_Transform->mPosition.x - 30.0f, pInst->mpComponent_Transform->mPosition.y);

						// Create the bullet
						pBullet = BulletCreate(bulletPosition, bulletVelocity, -angle, OBJECT_TYPE_ENEMY_BULLET);

						// Increment the bullets shot to keep track of our burst fire
						bullets_fired++;

						// Track burst configs
						if (bullets_fired >= 4)
						{
							bullets_fired = 0;
							ResetTimer(burst_fire_delay);
						}

						// Get the proper directional vector
						bulletVelocity.x = sgpShip->mpComponent_Transform->mPosition.x - pInst->mpComponent_Transform->mPosition.x;
						bulletVelocity.y = sgpShip->mpComponent_Transform->mPosition.y - pInst->mpComponent_Transform->mPosition.y;

						// Normalize the vector and then scale it by the bullet speed
						Vector2DNormalize(&bulletVelocity, &bulletVelocity);

						angle = AngleFromVector(&bulletVelocity);

						// Scale the velocity
						Vector2DScale(&bulletVelocity, &bulletVelocity, BULLET_SPEED * .90f);

						// Create the position
						Vector2DSet(&bulletPosition, pInst->mpComponent_Transform->mPosition.x + 30.0f, pInst->mpComponent_Transform->mPosition.y);

						// Create the bullet
						pBullet = BulletCreate(bulletPosition, bulletVelocity, -angle, OBJECT_TYPE_ENEMY_BULLET);

						// Increment the bullets shot to keep track of our burst fire
						bullets_fired++;

						// Track burst configs
						if (bullets_fired >= 4)
						{
							bullets_fired = 0;
							ResetTimer(burst_fire_delay);
						}

						ResetTimer(boss_wep_timer);
					}
				}

				break;
			}	// End of Phase 1

			case PHASE_2:
			{
				// Don't shoot
				if (active_grenade_shots < 10)
				{
					UpdateTimer(boss_wep_timer);

					if (boss_wep_timer->doneCounting == DONE_COUNTING)
					{
						Vector2D bulletVelocity;
						Vector2D bulletPosition;

						GameObjectInstance* pBullet;
						float angle;

						// Get the proper directional vector
						bulletVelocity.x = sgpShip->mpComponent_Transform->mPosition.x - pInst->mpComponent_Transform->mPosition.x;
						bulletVelocity.y = sgpShip->mpComponent_Transform->mPosition.y - pInst->mpComponent_Transform->mPosition.y;

						// Normalize the vector and then scale it by the bullet speed
						Vector2DNormalize(&bulletVelocity, &bulletVelocity);

						angle = AngleFromVector(&bulletVelocity);

						// Scale the velocity
						Vector2DScale(&bulletVelocity, &bulletVelocity, BULLET_SPEED * 1.0f);

						// Create the position
						Vector2DSet(&bulletPosition, pInst->mpComponent_Transform->mPosition.x - 40.0f, pInst->mpComponent_Transform->mPosition.y);

						// Create the missile
						pBullet = BulletCreate(bulletPosition, bulletVelocity, -angle, OBJECT_TYPE_HOMING_MISSILE);

						// Follow the target for some amount of seconds before exploding
						pBullet->mpComponent_Target->follow_timer->duration = 2.0;

						// Increment the active missiles
						active_grenade_shots++;

						bulletVelocity.x = sgpShip->mpComponent_Transform->mPosition.x - pInst->mpComponent_Transform->mPosition.x;
						bulletVelocity.y = sgpShip->mpComponent_Transform->mPosition.y - pInst->mpComponent_Transform->mPosition.y;

						// Normalize the vector and then scale it by the bullet speed
						Vector2DNormalize(&bulletVelocity, &bulletVelocity);

						angle = AngleFromVector(&bulletVelocity);

						// Scale the velocity
						Vector2DScale(&bulletVelocity, &bulletVelocity, BULLET_SPEED * 1.0f);

						// Create the position
						Vector2DSet(&bulletPosition, pInst->mpComponent_Transform->mPosition.x + 40.0f, pInst->mpComponent_Transform->mPosition.y);

						// Create the missile
						pBullet = BulletCreate(bulletPosition, bulletVelocity, -angle, OBJECT_TYPE_HOMING_MISSILE);

						// Follow the target for some amount of seconds before exploding
						pBullet->mpComponent_Target->follow_timer->duration = 2.0;

						// Increment the active missiles
						active_grenade_shots++;

						ResetTimer(boss_wep_timer);
					}
				}

				break;
			}

			case PHASE_3:
			{
				UpdateTimer(FinalPhase3);

				// When this is on, shoot for a while
				if (FinalPhase3->doneCounting == DONE_COUNTING)
				{

					UpdateTimer(boss_wep_timer);

					if (boss_wep_timer->doneCounting == DONE_COUNTING)
					{
						Vector2D bulletVelocity;
						Vector2D bulletPosition;

						GameObjectInstance* pBullet;
						float angle;

						if (bullets_fired >= 9)
						{
							bullets_fired = 0;
							ResetTimer(FinalPhase3);
						}
						//middle bullet/////////////////////////////////////////////////////////////////////
						// Get the proper directional vector
						Vector2DSet(&bulletVelocity, 0.0f, -1.0f);

						// Normalize the vector and then scale it by the bullet speed
						Vector2DNormalize(&bulletVelocity, &bulletVelocity);

						angle = AngleFromVector(&bulletVelocity);

						// Scale the velocity
						Vector2DScale(&bulletVelocity, &bulletVelocity, BULLET_SPEED * .90f);

						// Create the position
						Vector2DSet(&bulletPosition, pInst->mpComponent_Transform->mPosition.x, pInst->mpComponent_Transform->mPosition.y - 20.0f);

						// Create the bullet
						pBullet = BulletCreate(bulletPosition, bulletVelocity, -angle, OBJECT_TYPE_ENEMY_BULLET);

						// Increment the bullets shot to keep track of our burst fire
						bullets_fired++;

						//second bullet//////////////////////////////////////////////////////////////////////
						// Get the proper directional vector
						Vector2DSet(&bulletVelocity, 0.0f, -1.0f);

						// Normalize the vector and then scale it by the bullet speed
						Vector2DNormalize(&bulletVelocity, &bulletVelocity);

						angle = AngleFromVector(&bulletVelocity);

						// Scale the velocity
						Vector2DScale(&bulletVelocity, &bulletVelocity, BULLET_SPEED * .90f);

						// Create the position
						Vector2DSet(&bulletPosition, pInst->mpComponent_Transform->mPosition.x + 25.0f, pInst->mpComponent_Transform->mPosition.y - 15.0f);

						// Create the bullet
						pBullet = BulletCreate(bulletPosition, bulletVelocity, -angle, OBJECT_TYPE_ENEMY_BULLET);

						// Increment the bullets shot to keep track of our burst fire
						bullets_fired++;

						//third bullet//////////////////////////////////////////////////////////////////////
						// Get the proper directional vector
						Vector2DSet(&bulletVelocity, 0.0f, -1.0f);

						// Normalize the vector and then scale it by the bullet speed
						Vector2DNormalize(&bulletVelocity, &bulletVelocity);

						angle = AngleFromVector(&bulletVelocity);

						// Scale the velocity
						Vector2DScale(&bulletVelocity, &bulletVelocity, BULLET_SPEED * .90f);

						// Create the position
						Vector2DSet(&bulletPosition, pInst->mpComponent_Transform->mPosition.x - 25.0f, pInst->mpComponent_Transform->mPosition.y - 15.0f);

						// Create the bullet
						pBullet = BulletCreate(bulletPosition, bulletVelocity, -angle, OBJECT_TYPE_ENEMY_BULLET);

						// Increment the bullets shot to keep track of our burst fire
						bullets_fired++;

						//fourth bullet//////////////////////////////////////////////////////////////////////
						// Get the proper directional vector
						Vector2DSet(&bulletVelocity, 0.0f, -1.0f);

						// Normalize the vector and then scale it by the bullet speed
						Vector2DNormalize(&bulletVelocity, &bulletVelocity);

						angle = AngleFromVector(&bulletVelocity);

						// Scale the velocity
						Vector2DScale(&bulletVelocity, &bulletVelocity, BULLET_SPEED * .90f);

						// Create the position
						Vector2DSet(&bulletPosition, pInst->mpComponent_Transform->mPosition.x - 50.0f, pInst->mpComponent_Transform->mPosition.y - 10.0f);

						// Create the bullet
						pBullet = BulletCreate(bulletPosition, bulletVelocity, -angle, OBJECT_TYPE_ENEMY_BULLET);

						// Increment the bullets shot to keep track of our burst fire
						bullets_fired++;

						//fith bullet//////////////////////////////////////////////////////////////////////
						// Get the proper directional vector
						Vector2DSet(&bulletVelocity, 0.0f, -1.0f);

						// Normalize the vector and then scale it by the bullet speed
						Vector2DNormalize(&bulletVelocity, &bulletVelocity);

						angle = AngleFromVector(&bulletVelocity);

						// Scale the velocity
						Vector2DScale(&bulletVelocity, &bulletVelocity, BULLET_SPEED * .90f);

						// Create the position
						Vector2DSet(&bulletPosition, pInst->mpComponent_Transform->mPosition.x + 50.0f, pInst->mpComponent_Transform->mPosition.y - 10.0f);

						// Create the bullet
						pBullet = BulletCreate(bulletPosition, bulletVelocity, -angle, OBJECT_TYPE_ENEMY_BULLET);

						// Increment the bullets shot to keep track of our burst fire
						bullets_fired++;

						//sixth bullet//////////////////////////////////////////////////////////////////////
						// Get the proper directional vector
						Vector2DSet(&bulletVelocity, 0.0f, -1.0f);

						// Normalize the vector and then scale it by the bullet speed
						Vector2DNormalize(&bulletVelocity, &bulletVelocity);

						angle = AngleFromVector(&bulletVelocity);

						// Scale the velocity
						Vector2DScale(&bulletVelocity, &bulletVelocity, BULLET_SPEED * .90f);

						// Create the position
						Vector2DSet(&bulletPosition, pInst->mpComponent_Transform->mPosition.x - 75.0f, pInst->mpComponent_Transform->mPosition.y - 5.0f);

						// Create the bullet
						pBullet = BulletCreate(bulletPosition, bulletVelocity, -angle, OBJECT_TYPE_ENEMY_BULLET);

						// Increment the bullets shot to keep track of our burst fire
						bullets_fired++;

						//seventh bullet//////////////////////////////////////////////////////////////////////
						// Get the proper directional vector
						Vector2DSet(&bulletVelocity, 0.0f, -1.0f);

						// Normalize the vector and then scale it by the bullet speed
						Vector2DNormalize(&bulletVelocity, &bulletVelocity);

						angle = AngleFromVector(&bulletVelocity);

						// Scale the velocity
						Vector2DScale(&bulletVelocity, &bulletVelocity, BULLET_SPEED * .90f);

						// Create the position
						Vector2DSet(&bulletPosition, pInst->mpComponent_Transform->mPosition.x + 75.0f, pInst->mpComponent_Transform->mPosition.y - 5.0f);

						// Create the bullet
						pBullet = BulletCreate(bulletPosition, bulletVelocity, -angle, OBJECT_TYPE_ENEMY_BULLET);

						// Increment the bullets shot to keep track of our burst fire
						bullets_fired++;

						//eighth bullet//////////////////////////////////////////////////////////////////////
						// Get the proper directional vector
						Vector2DSet(&bulletVelocity, 0.0f, -1.0f);

						// Normalize the vector and then scale it by the bullet speed
						Vector2DNormalize(&bulletVelocity, &bulletVelocity);

						angle = AngleFromVector(&bulletVelocity);

						// Scale the velocity
						Vector2DScale(&bulletVelocity, &bulletVelocity, BULLET_SPEED * .90f);

						// Create the position
						Vector2DSet(&bulletPosition, pInst->mpComponent_Transform->mPosition.x - 100.0f, pInst->mpComponent_Transform->mPosition.y);

						// Create the bullet
						pBullet = BulletCreate(bulletPosition, bulletVelocity, -angle, OBJECT_TYPE_ENEMY_BULLET);

						// Increment the bullets shot to keep track of our burst fire
						bullets_fired++;

						//ninth bullet//////////////////////////////////////////////////////////////////////
						// Get the proper directional vector
						Vector2DSet(&bulletVelocity, 0.0f, -1.0f);

						// Normalize the vector and then scale it by the bullet speed
						Vector2DNormalize(&bulletVelocity, &bulletVelocity);

						angle = AngleFromVector(&bulletVelocity);

						// Scale the velocity
						Vector2DScale(&bulletVelocity, &bulletVelocity, BULLET_SPEED * .90f);

						// Create the position
						Vector2DSet(&bulletPosition, pInst->mpComponent_Transform->mPosition.x + 100.0f, pInst->mpComponent_Transform->mPosition.y);

						// Create the bullet
						pBullet = BulletCreate(bulletPosition, bulletVelocity, -angle, OBJECT_TYPE_ENEMY_BULLET);

						// Increment the bullets shot to keep track of our burst fire
						bullets_fired++;

						ResetTimer(boss_wep_timer);

					}
				}

				break;
			}	// End of Phase 3

			case PHASE_4:
			{
				UpdateTimer(FinalPhase3);

				// When this is on, shoot for a while
				if (FinalPhase3->doneCounting == DONE_COUNTING)
				{

					UpdateTimer(boss_wep_timer);

					if (boss_wep_timer->doneCounting == DONE_COUNTING)
					{
						Vector2D bulletVelocity;
						Vector2D bulletPosition;

						GameObjectInstance* pBullet;
						float angle;

						// Get the proper directional vector
						bulletVelocity.x = sgpShip->mpComponent_Transform->mPosition.x - pInst->mpComponent_Transform->mPosition.x;
						bulletVelocity.y = sgpShip->mpComponent_Transform->mPosition.y - pInst->mpComponent_Transform->mPosition.y;

						// Normalize the vector and then scale it by the bullet speed
						Vector2DNormalize(&bulletVelocity, &bulletVelocity);

						angle = AngleFromVector(&bulletVelocity);

						// Scale the velocity
						Vector2DScale(&bulletVelocity, &bulletVelocity, BULLET_SPEED * .90f);

						// Create the position
						Vector2DSet(&bulletPosition, pInst->mpComponent_Transform->mPosition.x - 10.0f, pInst->mpComponent_Transform->mPosition.y);

						// Create the bullet
						pBullet = BulletCreate(bulletPosition, bulletVelocity, -angle, OBJECT_TYPE_ENEMY_BULLET);

						// Increment the bullets shot to keep track of our burst fire
						bullets_fired++;

						// Track burst configs
						if (bullets_fired >= 1)
						{
							bullets_fired = 0;
							ResetTimer(FinalPhase3);
						}

						if (active_grenade_shots < 4)
						{
							UpdateTimer(boss_wep_timer);

							if (boss_wep_timer->doneCounting == DONE_COUNTING)
							{
								Vector2D bulletVelocity;
								Vector2D bulletPosition;

								GameObjectInstance* pBullet;
								float angle;

								// Get the proper directional vector
								bulletVelocity.x = sgpShip->mpComponent_Transform->mPosition.x - pInst->mpComponent_Transform->mPosition.x;
								bulletVelocity.y = sgpShip->mpComponent_Transform->mPosition.y - pInst->mpComponent_Transform->mPosition.y;

								// Normalize the vector and then scale it by the bullet speed
								Vector2DNormalize(&bulletVelocity, &bulletVelocity);

								angle = AngleFromVector(&bulletVelocity);

								// Scale the velocity
								Vector2DScale(&bulletVelocity, &bulletVelocity, BULLET_SPEED * 1.0f);

								// Create the position
								Vector2DSet(&bulletPosition, pInst->mpComponent_Transform->mPosition.x - 40.0f, pInst->mpComponent_Transform->mPosition.y);

								// Create the missile
								pBullet = BulletCreate(bulletPosition, bulletVelocity, -angle, OBJECT_TYPE_HOMING_MISSILE);

								// Follow the target for some amount of seconds before exploding
								pBullet->mpComponent_Target->follow_timer->duration = 2.0;

								// Increment the active missiles
								active_grenade_shots++;

								bulletVelocity.x = sgpShip->mpComponent_Transform->mPosition.x - pInst->mpComponent_Transform->mPosition.x;
								bulletVelocity.y = sgpShip->mpComponent_Transform->mPosition.y - pInst->mpComponent_Transform->mPosition.y;

								// Normalize the vector and then scale it by the bullet speed
								Vector2DNormalize(&bulletVelocity, &bulletVelocity);

								angle = AngleFromVector(&bulletVelocity);

								// Scale the velocity
								Vector2DScale(&bulletVelocity, &bulletVelocity, BULLET_SPEED * 1.0f);

								// Create the position
								Vector2DSet(&bulletPosition, pInst->mpComponent_Transform->mPosition.x + 40.0f, pInst->mpComponent_Transform->mPosition.y);

								// Create the missile
								pBullet = BulletCreate(bulletPosition, bulletVelocity, -angle, OBJECT_TYPE_HOMING_MISSILE);

								// Follow the target for some amount of seconds before exploding
								pBullet->mpComponent_Target->follow_timer->duration = 2.0;

								// Increment the active missiles
								active_grenade_shots++;
								ResetTimer(boss_wep_timer);
							}
						}
					}

					break;
				}
			}	// End of Phase 4

			}	// End of phase switching


				// This phase happens for a small amount of time
			if (pInst->bossHealth < 10)
			{
				pInst->innerState = ON_EXIT;
			}

			break;
		}

		case ON_EXIT:
		{
			pInst->innerState = ON_ENTER;
			break;
		}
		}

		break;

	}// End of second Switch

	}// End of first switch


}

EnemyInstance* EnemyInstanceCreate(EnemyInstance* pEnemyArray, unsigned int EnemyType, unsigned int *pMonsterCounter, unsigned int numMonsters)
{
	unsigned int i;

	// loop through the object instance list to find a non-used object instance
	for (i = 0; i < numMonsters; i++)
	{
		EnemyInstance* pInst = pEnemyArray + i;

		// Check if current instance is not used
		if (pInst->mFlag == 0)
		{
			// It is not used => use it to create the new instance

			// Active the game object instance if allocated
			pInst->mFlag = FLAG_ACTIVE;

			pInst->mpComponent_Transform = 0;
			pInst->mpComponent_Sprite = 0;
			pInst->mpComponent_Physics = 0;
			pInst->mpComponent_EnemyData = 0;

			// Add the components, based on the object type
			switch (EnemyType)
			{
			case ENEMY_TYPE_REGULAR:
				AddComponent_EnemySprite(pInst, enemyShapes, ENEMY_TYPE_REGULAR);
				AddComponent_EnemyTransform(pInst, 0, 0.0f, 20.0f, 20.0f);
				AddComponent_EnemyPhysics(pInst, 0);
				AddComponent_EnemyData(pInst, 3, 0, 0, 0);
				break;

			case ENEMY_TYPE_MINION:
				AddComponent_EnemySprite(pInst, enemyShapes, ENEMY_TYPE_MINION);
				AddComponent_EnemyTransform(pInst, 0, 0.0f, 20.0f, 20.0f);
				AddComponent_EnemyPhysics(pInst, 0);
				AddComponent_EnemyData(pInst, 3, 0, 0, 0);
				break;

			case ENEMY_TYPE_WORLD2:
				AddComponent_EnemySprite(pInst, enemyShapes, ENEMY_TYPE_WORLD2);
				AddComponent_EnemyTransform(pInst, 0, 0.0f, 20.0f, 20.0f);
				AddComponent_EnemyPhysics(pInst, 0);
				AddComponent_EnemyData(pInst, 3, 0, 0, 0);
				break;

			case ENEMY_TYPE_WORLD4:
				AddComponent_EnemySprite(pInst, enemyShapes, ENEMY_TYPE_WORLD4);
				AddComponent_EnemyTransform(pInst, 0, 0.0f, 20.0f, 20.0f);
				AddComponent_EnemyPhysics(pInst, 0);
				AddComponent_EnemyData(pInst, 3, 0, 0, 0);
				break;
			}

			//		++(*pMonsterCounter);

			// return the newly created instance
			return pInst;
		}
	}

	// Cannot find empty slot => return 0
	return 0;
}

// ---------------------------------------------------------------------------

void EnemyInstanceDestroy(EnemyInstance* pInst, unsigned int *pMonsterCounter)
{
	// if instance is destroyed before, just return
	if (pInst->mFlag == 0)
		return;

	// Zero out the mFlag
	pInst->mFlag = 0;

	RemoveComponent_EnemyTransform(pInst);
	RemoveComponent_EnemySprite(pInst);
	RemoveComponent_EnemyPhysics(pInst);
	RemoveComponent_EnemyData(pInst);

	*pMonsterCounter--;
}

void AddComponent_EnemyData(EnemyInstance *pInst, unsigned int health, unsigned int wepType, double lifetime, int reward)
{
	if (0 != pInst)
	{
		if (0 == pInst->mpComponent_EnemyData)
		{
			pInst->mpComponent_EnemyData = (Component_EnemyData *)calloc(1, sizeof(Component_EnemyData));
		}

		pInst->mpComponent_EnemyData->health = health;

		// Initialize wep data , this stuff should be arguments but it's not urgent
		pInst->mpComponent_EnemyData->wepType = wepType;
		pInst->mpComponent_EnemyData->wepState = ON_ENTER;
		pInst->mpComponent_EnemyData->wepDelayTimer = ConstructTimer(STANDARD_ENEMY_WEP_DELAY);

		pInst->mpComponent_EnemyData->lifetime = lifetime;
		pInst->mpComponent_EnemyData->reward = reward;
	}
}

void AddComponent_EnemyTransform(EnemyInstance *pInst, Vector2D *pPosition, float Angle, float ScaleX, float ScaleY)
{
	if (0 != pInst)
	{
		if (0 == pInst->mpComponent_Transform)
		{
			pInst->mpComponent_Transform = (Component_EnemyTransform *)calloc(1, sizeof(Component_EnemyTransform));
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

void AddComponent_EnemySprite(EnemyInstance *pInst, Shape* sgShapes, unsigned int ShapeType)
{
	if (0 != pInst)
	{
		if (0 == pInst->mpComponent_Sprite)
		{
			pInst->mpComponent_Sprite = (Component_EnemySprite *)calloc(1, sizeof(Component_EnemySprite));
		}

		pInst->mpComponent_Sprite->mpShape = sgShapes + ShapeType;
		pInst->mpComponent_Sprite->mpOwner = pInst;
	}
}

// ---------------------------------------------------------------------------

void AddComponent_EnemyPhysics(EnemyInstance *pInst, Vector2D *pVelocity)
{
	if (0 != pInst)
	{
		if (0 == pInst->mpComponent_Physics)
		{
			pInst->mpComponent_Physics = (Component_EnemyPhysics *)calloc(1, sizeof(Component_EnemyPhysics));
		}

		Vector2D zeroVec2;
		Vector2DZero(&zeroVec2);

		pInst->mpComponent_Physics->mVelocity = pVelocity ? *pVelocity : zeroVec2;
		pInst->mpComponent_Physics->mpOwner = pInst;
	}
}

// ---------------------------------------------------------------------------

void RemoveComponent_EnemyTransform(EnemyInstance *pInst)
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

void RemoveComponent_EnemySprite(EnemyInstance *pInst)
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

void RemoveComponent_EnemyPhysics(EnemyInstance *pInst)
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

void RemoveComponent_EnemyData(EnemyInstance *pInst)
{
	if (0 != pInst)
	{
		if (0 != pInst->mpComponent_EnemyData)
		{
			DestroyTimer(pInst->mpComponent_EnemyData->wepDelayTimer);
			free(pInst->mpComponent_EnemyData);
			pInst->mpComponent_EnemyData = 0;
		}
	}
}


///////////////////////////////////////////////////////////////
//						LEVEL SETTINGS						 //
///////////////////////////////////////////////////////////////


void SetLevel1SpawnPoints(EnemySpawnPoint* pSpawnArray)
{
	Vector2D position;

	//
	Vector2DSet(&position, -150.0f, 200.0f);
	pSpawnArray[0] = SetSpawnPoint(pSpawnArray[0], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[0].SpawnTimer = CreateComponent_Timer(pSpawnArray[0].SpawnTimer, DEPLOYMENT_DELAY - 2.0);
	pSpawnArray[0].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;
	pSpawnArray[0].active = FLAG_ACTIVE;

	Vector2DSet(&position, 150.0f, 200.0f);
	pSpawnArray[2] = SetSpawnPoint(pSpawnArray[2], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_R_TO_L);
	pSpawnArray[2].SpawnTimer = CreateComponent_Timer(pSpawnArray[2].SpawnTimer, DEPLOYMENT_DELAY - 2.0);
	pSpawnArray[2].SpawnTimer->current_time = pSpawnArray[2].SpawnTimer->duration;
	pSpawnArray[2].active = FLAG_ACTIVE;

	Vector2DSet(&position, -200.0f, 300.0f);
	pSpawnArray[3] = SetSpawnPoint(pSpawnArray[3], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[3].SpawnTimer = CreateComponent_Timer(pSpawnArray[3].SpawnTimer, DEPLOYMENT_DELAY - 2.0);
	pSpawnArray[3].SpawnTimer->current_time = pSpawnArray[3].SpawnTimer->duration;
	pSpawnArray[3].active = FLAG_ACTIVE;

	Vector2DSet(&position, 200.0f, 300.0f);
	pSpawnArray[10] = SetSpawnPoint(pSpawnArray[10], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_R_TO_L);
	pSpawnArray[10].SpawnTimer = CreateComponent_Timer(pSpawnArray[10].SpawnTimer, DEPLOYMENT_DELAY - 2.0);
	pSpawnArray[10].SpawnTimer->current_time = pSpawnArray[10].SpawnTimer->duration;
	pSpawnArray[10].active = FLAG_ACTIVE;
	//

	//
	Vector2DSet(&position, -400.0f, 300.0f);
	pSpawnArray[1] = SetSpawnPoint(pSpawnArray[1], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[1].SpawnTimer = CreateComponent_Timer(pSpawnArray[1].SpawnTimer, RESPAWN_TIMER - 2.0f);
	pSpawnArray[1].SpawnTimer->current_time = pSpawnArray[1].SpawnTimer->duration;
	pSpawnArray[1].active = FLAG_ACTIVE;

	Vector2DSet(&position, -350.0f, 200.0f);
	pSpawnArray[4] = SetSpawnPoint(pSpawnArray[4], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[4].SpawnTimer = CreateComponent_Timer(pSpawnArray[4].SpawnTimer, RESPAWN_TIMER - 2.0f);
	pSpawnArray[4].SpawnTimer->current_time = pSpawnArray[4].SpawnTimer->duration;
	pSpawnArray[4].active = FLAG_ACTIVE;

	Vector2DSet(&position, -300.0f, 100.0f);
	pSpawnArray[5] = SetSpawnPoint(pSpawnArray[5], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[5].SpawnTimer = CreateComponent_Timer(pSpawnArray[5].SpawnTimer, RESPAWN_TIMER - 2.0f);
	pSpawnArray[5].SpawnTimer->current_time = pSpawnArray[5].SpawnTimer->duration;
	pSpawnArray[5].active = FLAG_ACTIVE;
	//

	//
	Vector2DSet(&position, 400.0f, 300.0f);
	pSpawnArray[6] = SetSpawnPoint(pSpawnArray[6], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_R_TO_L);
	pSpawnArray[6].SpawnTimer = CreateComponent_Timer(pSpawnArray[6].SpawnTimer, RESPAWN_TIMER - 2.0f);
	pSpawnArray[6].SpawnTimer->current_time = pSpawnArray[6].SpawnTimer->duration;
	pSpawnArray[6].active = FLAG_ACTIVE;

	Vector2DSet(&position, 350.0f, 200.0f);
	pSpawnArray[7] = SetSpawnPoint(pSpawnArray[7], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_R_TO_L);
	pSpawnArray[7].SpawnTimer = CreateComponent_Timer(pSpawnArray[7].SpawnTimer, RESPAWN_TIMER - 2.0f);
	pSpawnArray[7].SpawnTimer->current_time = pSpawnArray[7].SpawnTimer->duration;
	pSpawnArray[7].active = FLAG_ACTIVE;

	Vector2DSet(&position, 300.0f, 100.0f);
	pSpawnArray[8] = SetSpawnPoint(pSpawnArray[8], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_R_TO_L);
	pSpawnArray[8].SpawnTimer = CreateComponent_Timer(pSpawnArray[8].SpawnTimer, RESPAWN_TIMER - 2.0f);
	pSpawnArray[8].SpawnTimer->current_time = pSpawnArray[8].SpawnTimer->duration;
	pSpawnArray[8].active = FLAG_ACTIVE;
	//

	//
	Vector2DSet(&position, -120.0f, 400.0f);
	pSpawnArray[9] = SetSpawnPoint(pSpawnArray[9], position, SEEKING_MOB, OVAL_PATTERN_HORIZONTAL);
	pSpawnArray[9].SpawnTimer = CreateComponent_Timer(pSpawnArray[9].SpawnTimer, RESPAWN_TIMER - 2.0f);
	pSpawnArray[9].SpawnTimer->current_time = pSpawnArray[9].SpawnTimer->duration;
	pSpawnArray[9].active = FLAG_ACTIVE;


	Vector2DSet(&position, 120.0f, 400.0f);
	pSpawnArray[11] = SetSpawnPoint(pSpawnArray[11], position, SEEKING_MOB, OVAL_PATTERN_HORIZONTAL);
	pSpawnArray[11].SpawnTimer = CreateComponent_Timer(pSpawnArray[11].SpawnTimer, RESPAWN_TIMER - 2.0f);
	pSpawnArray[11].SpawnTimer->current_time = pSpawnArray[11].SpawnTimer->duration;
	pSpawnArray[11].active = FLAG_ACTIVE;
	//

	// The special 1
	Vector2DSet(&position, 0.0f, 100.0f);
	pSpawnArray[12] = SetSpawnPoint(pSpawnArray[12], position, SEEKING_MOB, OVAL_PATTERN_VERTICAL);
	pSpawnArray[12].SpawnTimer = CreateComponent_Timer(pSpawnArray[12].SpawnTimer, DEPLOYMENT_DELAY);
	pSpawnArray[12].SpawnTimer->current_time = pSpawnArray[12].SpawnTimer->duration;
	pSpawnArray[12].active = FLAG_ACTIVE;
}

void SetLevel2SpawnPoints(EnemySpawnPoint* pSpawnArray)
{
	Vector2D position;

	Vector2DSet(&position, -600.0f, 0.0f);
	pSpawnArray[0] = SetSpawnPoint(pSpawnArray[0], position, SEEKING_MOB, SPIRAL_PATTERN_L_TO_R);
	pSpawnArray[0].SpawnTimer = CreateComponent_Timer(pSpawnArray[0].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[0].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;
	pSpawnArray[0].spID = MULTI_SPAWNER;
	pSpawnArray[0].numEnemies = 3;
	pSpawnArray[0].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[0]);

	Vector2DSet(&position, -600.0f, 100.0f);
	pSpawnArray[1] = SetSpawnPoint(pSpawnArray[1], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[1].SpawnTimer = CreateComponent_Timer(pSpawnArray[1].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[1].SpawnTimer->current_time = pSpawnArray[1].SpawnTimer->duration;
	pSpawnArray[1].active = FLAG_ACTIVE;

	Vector2DSet(&position, -600.0f, 200.0f);
	pSpawnArray[2] = SetSpawnPoint(pSpawnArray[2], position, SEEKING_MOB, SPIRAL_PATTERN_L_TO_R);
	pSpawnArray[2].SpawnTimer = CreateComponent_Timer(pSpawnArray[2].SpawnTimer, DEPLOYMENT_DELAY);
	pSpawnArray[2].SpawnTimer->current_time = pSpawnArray[2].SpawnTimer->duration;
	pSpawnArray[2].spID = MULTI_SPAWNER;
	pSpawnArray[2].numEnemies = 3;
	pSpawnArray[2].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[2]);

	Vector2DSet(&position, -600.0f, 300.0f);
	pSpawnArray[3] = SetSpawnPoint(pSpawnArray[3], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[3].SpawnTimer = CreateComponent_Timer(pSpawnArray[3].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[3].SpawnTimer->current_time = pSpawnArray[3].SpawnTimer->duration;
	pSpawnArray[3].active = FLAG_ACTIVE;

	Vector2DSet(&position, -300.0f, 500.0f);
	pSpawnArray[4] = SetSpawnPoint(pSpawnArray[4], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[4].SpawnTimer = CreateComponent_Timer(pSpawnArray[4].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[4].SpawnTimer->current_time = pSpawnArray[4].SpawnTimer->duration;
	pSpawnArray[4].active = FLAG_ACTIVE;

	Vector2DSet(&position, -100.0f, 500.0f);
	pSpawnArray[5] = SetSpawnPoint(pSpawnArray[5], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[5].SpawnTimer = CreateComponent_Timer(pSpawnArray[5].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[5].SpawnTimer->current_time = pSpawnArray[5].SpawnTimer->duration;
	pSpawnArray[5].active = FLAG_ACTIVE;

	Vector2DSet(&position, 100.0f, 500.0f);
	pSpawnArray[6] = SetSpawnPoint(pSpawnArray[6], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[6].SpawnTimer = CreateComponent_Timer(pSpawnArray[6].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[6].SpawnTimer->current_time = pSpawnArray[6].SpawnTimer->duration;
	pSpawnArray[6].active = FLAG_ACTIVE;

	Vector2DSet(&position, 300.0f, 500.0f);
	pSpawnArray[7] = SetSpawnPoint(pSpawnArray[7], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[7].SpawnTimer = CreateComponent_Timer(pSpawnArray[7].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[7].SpawnTimer->current_time = pSpawnArray[7].SpawnTimer->duration;
	pSpawnArray[7].active = FLAG_ACTIVE;

	Vector2DSet(&position, 600.0f, 350.0f);
	pSpawnArray[8] = SetSpawnPoint(pSpawnArray[8], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_R_TO_L);
	pSpawnArray[8].SpawnTimer = CreateComponent_Timer(pSpawnArray[8].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[8].SpawnTimer->current_time = pSpawnArray[8].SpawnTimer->duration;
	pSpawnArray[8].active = FLAG_ACTIVE;

	Vector2DSet(&position, 600.0f, 200.0f);
	pSpawnArray[9] = SetSpawnPoint(pSpawnArray[9], position, SEEKING_MOB, SPIRAL_PATTERN_R_TO_L);
	pSpawnArray[9].SpawnTimer = CreateComponent_Timer(pSpawnArray[9].SpawnTimer, DEPLOYMENT_DELAY);
	pSpawnArray[9].SpawnTimer->current_time = pSpawnArray[9].SpawnTimer->duration;
	pSpawnArray[9].spID = MULTI_SPAWNER;
	pSpawnArray[9].numEnemies = 3;
	pSpawnArray[9].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[9]);

	Vector2DSet(&position, 600.0f, 150.0f);
	pSpawnArray[10] = SetSpawnPoint(pSpawnArray[10], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_R_TO_L);
	pSpawnArray[10].SpawnTimer = CreateComponent_Timer(pSpawnArray[10].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[10].SpawnTimer->current_time = pSpawnArray[10].SpawnTimer->duration;
	pSpawnArray[10].active = FLAG_INACTIVE;

	Vector2DSet(&position, 600.0f, 100.0f);
	pSpawnArray[11] = SetSpawnPoint(pSpawnArray[11], position, SEEKING_MOB, SPIRAL_PATTERN_R_TO_L);
	pSpawnArray[11].SpawnTimer = CreateComponent_Timer(pSpawnArray[11].SpawnTimer, DEPLOYMENT_DELAY);
	pSpawnArray[11].SpawnTimer->current_time = pSpawnArray[11].SpawnTimer->duration;
	pSpawnArray[11].spID = MULTI_SPAWNER;
	pSpawnArray[11].numEnemies = 6;
	pSpawnArray[11].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[11]);

	// The special 1
	Vector2DSet(&position, 600.0f, 0.0f);
	pSpawnArray[12] = SetSpawnPoint(pSpawnArray[12], position, SEEKING_MOB, SPIRAL_PATTERN_R_TO_L);
	pSpawnArray[12].SpawnTimer = CreateComponent_Timer(pSpawnArray[12].SpawnTimer, DEPLOYMENT_DELAY);
	pSpawnArray[12].SpawnTimer->current_time = pSpawnArray[12].SpawnTimer->duration;
	pSpawnArray[12].spID = MULTI_SPAWNER;
	pSpawnArray[12].numEnemies = 10;
	pSpawnArray[12].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[12]);
}

// World 1 Boss
void SetLevel3SpawnPoints(EnemySpawnPoint* pSpawnArray)
{
	Vector2D position;

	Vector2DSet(&position, -600.0f, 0.0f);
	pSpawnArray[0] = SetSpawnPoint(pSpawnArray[0], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[0].SpawnTimer = CreateComponent_Timer(pSpawnArray[0].SpawnTimer, DEPLOYMENT_DELAY / 2.0);
	pSpawnArray[0].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;
	pSpawnArray[0].spID = MULTI_SPAWNER;
	pSpawnArray[0].numEnemies = 6;
	pSpawnArray[0].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[0]);

	Vector2DSet(&position, -600.0f, 200.0f);
	pSpawnArray[2] = SetSpawnPoint(pSpawnArray[2], position, SEEKING_MOB, SPIRAL_PATTERN_L_TO_R);
	pSpawnArray[2].SpawnTimer = CreateComponent_Timer(pSpawnArray[2].SpawnTimer, DEPLOYMENT_DELAY / 2.0);
	pSpawnArray[2].SpawnTimer->current_time = pSpawnArray[2].SpawnTimer->duration;
	pSpawnArray[2].spID = MULTI_SPAWNER;
	pSpawnArray[2].numEnemies = 3;
	pSpawnArray[2].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[2]);

	Vector2DSet(&position, -600.0f, 100.0f);
	pSpawnArray[3] = SetSpawnPoint(pSpawnArray[3], position, SEEKING_MOB, SPIRAL_PATTERN_L_TO_R);
	pSpawnArray[3].SpawnTimer = CreateComponent_Timer(pSpawnArray[3].SpawnTimer, DEPLOYMENT_DELAY / 2.0);
	pSpawnArray[3].SpawnTimer->current_time = pSpawnArray[3].SpawnTimer->duration - (0.25 * DEPLOYMENT_DELAY);
	pSpawnArray[3].spID = MULTI_SPAWNER;
	pSpawnArray[3].numEnemies = 3;
	pSpawnArray[3].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[3]);

	Vector2DSet(&position, -600.0f, 100.0f);
	pSpawnArray[1] = SetSpawnPoint(pSpawnArray[1], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[1].SpawnTimer = CreateComponent_Timer(pSpawnArray[1].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[1].SpawnTimer->current_time = pSpawnArray[1].SpawnTimer->duration;
	pSpawnArray[1].active = FLAG_INACTIVE;

	Vector2DSet(&position, -300.0f, 500.0f);
	pSpawnArray[4] = SetSpawnPoint(pSpawnArray[4], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[4].SpawnTimer = CreateComponent_Timer(pSpawnArray[4].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[4].SpawnTimer->current_time = pSpawnArray[4].SpawnTimer->duration;
	pSpawnArray[4].active = FLAG_INACTIVE;

	Vector2DSet(&position, -100.0f, 500.0f);
	pSpawnArray[5] = SetSpawnPoint(pSpawnArray[5], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[5].SpawnTimer = CreateComponent_Timer(pSpawnArray[5].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[5].SpawnTimer->current_time = pSpawnArray[5].SpawnTimer->duration / 2.0;
	pSpawnArray[5].active = FLAG_INACTIVE;

	// Circles in corner
	Vector2DSet(&position, -400.0f, 400.0f);
	pSpawnArray[6] = SetSpawnPoint(pSpawnArray[6], position, SEEKING_MOB, CIRCLE_PATTERN);
	pSpawnArray[6].SpawnTimer = CreateComponent_Timer(pSpawnArray[6].SpawnTimer, 1.0);
	pSpawnArray[6].SpawnTimer->current_time = pSpawnArray[6].SpawnTimer->duration;
	pSpawnArray[6].spID = MULTI_SPAWNER;
	pSpawnArray[6].numEnemies = 4;
	pSpawnArray[6].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[6]);

	Vector2DSet(&position, 400.0f, 400.0f);
	pSpawnArray[7] = SetSpawnPoint(pSpawnArray[7], position, SEEKING_MOB, CIRCLE_PATTERN);
	pSpawnArray[7].SpawnTimer = CreateComponent_Timer(pSpawnArray[7].SpawnTimer, 1.0);
	pSpawnArray[7].SpawnTimer->current_time = pSpawnArray[7].SpawnTimer->duration;
	pSpawnArray[7].spID = MULTI_SPAWNER;
	pSpawnArray[7].numEnemies = 4;
	pSpawnArray[7].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[7]);
	//

	// Circles
	Vector2DSet(&position, -100.0f, 100.0f);
	pSpawnArray[8] = SetSpawnPoint(pSpawnArray[8], position, SEEKING_MOB, CIRCLE_PATTERN);
	pSpawnArray[8].SpawnTimer = CreateComponent_Timer(pSpawnArray[8].SpawnTimer, 1.0);
	pSpawnArray[8].SpawnTimer->current_time = pSpawnArray[8].SpawnTimer->duration;
	pSpawnArray[8].spID = MULTI_SPAWNER;
	pSpawnArray[8].numEnemies = 4;
	pSpawnArray[8].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[8]);

	Vector2DSet(&position, 100.0f, 100.0f);
	pSpawnArray[9] = SetSpawnPoint(pSpawnArray[9], position, SEEKING_MOB, CIRCLE_PATTERN);
	pSpawnArray[9].SpawnTimer = CreateComponent_Timer(pSpawnArray[9].SpawnTimer, 1.0);
	pSpawnArray[9].SpawnTimer->current_time = pSpawnArray[9].SpawnTimer->duration;
	pSpawnArray[9].spID = MULTI_SPAWNER;
	pSpawnArray[9].numEnemies = 4;
	pSpawnArray[9].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[9]);
	//


	//Right side
	Vector2DSet(&position, 600.0f, 100.0f);
	pSpawnArray[11] = SetSpawnPoint(pSpawnArray[11], position, SEEKING_MOB, SPIRAL_PATTERN_R_TO_L);
	pSpawnArray[11].SpawnTimer = CreateComponent_Timer(pSpawnArray[11].SpawnTimer, DEPLOYMENT_DELAY / 2.0);
	pSpawnArray[11].SpawnTimer->current_time = pSpawnArray[11].SpawnTimer->duration - (0.25 * DEPLOYMENT_DELAY);
	pSpawnArray[11].spID = MULTI_SPAWNER;
	pSpawnArray[11].numEnemies = 3;
	pSpawnArray[11].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[11]);

	Vector2DSet(&position, 600.0f, 200.0f);
	pSpawnArray[10] = SetSpawnPoint(pSpawnArray[10], position, SEEKING_MOB, SPIRAL_PATTERN_R_TO_L);
	pSpawnArray[10].SpawnTimer = CreateComponent_Timer(pSpawnArray[10].SpawnTimer, DEPLOYMENT_DELAY / 2.0);
	pSpawnArray[10].SpawnTimer->current_time = pSpawnArray[10].SpawnTimer->duration;
	pSpawnArray[10].spID = MULTI_SPAWNER;
	pSpawnArray[10].numEnemies = 3;
	pSpawnArray[10].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[10]);

	// The special 1
	Vector2DSet(&position, 600.0f, 0.0f);
	pSpawnArray[12] = SetSpawnPoint(pSpawnArray[12], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_R_TO_L);
	pSpawnArray[12].SpawnTimer = CreateComponent_Timer(pSpawnArray[12].SpawnTimer, DEPLOYMENT_DELAY / 2.0);
	pSpawnArray[12].SpawnTimer->current_time = pSpawnArray[12].SpawnTimer->duration;
	pSpawnArray[12].spID = MULTI_SPAWNER;
	pSpawnArray[12].numEnemies = 6;
	pSpawnArray[12].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[12]);
}

void SetLevel4SpawnPoints(EnemySpawnPoint* pSpawnArray)
{
	Vector2D position;

	Vector2DSet(&position, -200.0f, 50.0f);
	pSpawnArray[0] = SetSpawnPoint(pSpawnArray[0], position, SEEKING_MOB, SPIRAL_PATTERN_L_TO_R);
	pSpawnArray[0].SpawnTimer = CreateComponent_Timer(pSpawnArray[0].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[0].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;
	pSpawnArray[0].spID = MULTI_SPAWNER;
	pSpawnArray[0].numEnemies = 3;
	pSpawnArray[0].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[0]);

	Vector2DSet(&position, -600.0f, 150.0f);
	pSpawnArray[1] = SetSpawnPoint(pSpawnArray[1], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[1].SpawnTimer = CreateComponent_Timer(pSpawnArray[1].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[1].SpawnTimer->current_time = pSpawnArray[1].SpawnTimer->duration;
	pSpawnArray[1].active = FLAG_ACTIVE;

	Vector2DSet(&position, -600.0f, 250.0f);
	pSpawnArray[2] = SetSpawnPoint(pSpawnArray[2], position, SEEKING_MOB, SPIRAL_PATTERN_L_TO_R);
	pSpawnArray[2].SpawnTimer = CreateComponent_Timer(pSpawnArray[2].SpawnTimer, DEPLOYMENT_DELAY);
	pSpawnArray[2].SpawnTimer->current_time = pSpawnArray[2].SpawnTimer->duration;
	pSpawnArray[2].spID = MULTI_SPAWNER;
	pSpawnArray[2].numEnemies = 3;
	pSpawnArray[2].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[2]);

	Vector2DSet(&position, -600.0f, 300.0f);
	pSpawnArray[3] = SetSpawnPoint(pSpawnArray[3], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[3].SpawnTimer = CreateComponent_Timer(pSpawnArray[3].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[3].SpawnTimer->current_time = pSpawnArray[3].SpawnTimer->duration;
	pSpawnArray[3].active = FLAG_ACTIVE;

	Vector2DSet(&position, -300.0f, 500.0f);
	pSpawnArray[4] = SetSpawnPoint(pSpawnArray[4], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[4].SpawnTimer = CreateComponent_Timer(pSpawnArray[4].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[4].SpawnTimer->current_time = pSpawnArray[4].SpawnTimer->duration;
	pSpawnArray[4].active = FLAG_ACTIVE;

	Vector2DSet(&position, -100.0f, 500.0f);
	pSpawnArray[5] = SetSpawnPoint(pSpawnArray[5], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[5].SpawnTimer = CreateComponent_Timer(pSpawnArray[5].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[5].SpawnTimer->current_time = pSpawnArray[5].SpawnTimer->duration;
	pSpawnArray[5].active = FLAG_ACTIVE;

	Vector2DSet(&position, 100.0f, 500.0f);
	pSpawnArray[6] = SetSpawnPoint(pSpawnArray[6], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[6].SpawnTimer = CreateComponent_Timer(pSpawnArray[6].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[6].SpawnTimer->current_time = pSpawnArray[6].SpawnTimer->duration;
	pSpawnArray[6].active = FLAG_ACTIVE;

	Vector2DSet(&position, 300.0f, 500.0f);
	pSpawnArray[7] = SetSpawnPoint(pSpawnArray[7], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[7].SpawnTimer = CreateComponent_Timer(pSpawnArray[7].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[7].SpawnTimer->current_time = pSpawnArray[7].SpawnTimer->duration;
	pSpawnArray[7].active = FLAG_ACTIVE;

	Vector2DSet(&position, 600.0f, 350.0f);
	pSpawnArray[8] = SetSpawnPoint(pSpawnArray[8], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_R_TO_L);
	pSpawnArray[8].SpawnTimer = CreateComponent_Timer(pSpawnArray[8].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[8].SpawnTimer->current_time = pSpawnArray[8].SpawnTimer->duration;
	pSpawnArray[8].active = FLAG_ACTIVE;

	Vector2DSet(&position, 600.0f, 200.0f);
	pSpawnArray[9] = SetSpawnPoint(pSpawnArray[9], position, SEEKING_MOB, SPIRAL_PATTERN_L_TO_R);
	pSpawnArray[9].SpawnTimer = CreateComponent_Timer(pSpawnArray[9].SpawnTimer, DEPLOYMENT_DELAY);
	pSpawnArray[9].SpawnTimer->current_time = pSpawnArray[9].SpawnTimer->duration;
	pSpawnArray[9].spID = MULTI_SPAWNER;
	pSpawnArray[9].numEnemies = 3;
	pSpawnArray[9].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[9]);

	Vector2DSet(&position, 600.0f, 150.0f);
	pSpawnArray[10] = SetSpawnPoint(pSpawnArray[10], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_R_TO_L);
	pSpawnArray[10].SpawnTimer = CreateComponent_Timer(pSpawnArray[10].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[10].SpawnTimer->current_time = pSpawnArray[10].SpawnTimer->duration;
	pSpawnArray[10].active = FLAG_ACTIVE;

	Vector2DSet(&position, 600.0f, 100.0f);
	pSpawnArray[11] = SetSpawnPoint(pSpawnArray[11], position, SEEKING_MOB, SPIRAL_PATTERN_L_TO_R);
	pSpawnArray[11].SpawnTimer = CreateComponent_Timer(pSpawnArray[11].SpawnTimer, DEPLOYMENT_DELAY);
	pSpawnArray[11].SpawnTimer->current_time = pSpawnArray[11].SpawnTimer->duration;
	pSpawnArray[11].spID = MULTI_SPAWNER;
	pSpawnArray[11].numEnemies = 6;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[11]);

	// The special 1
	Vector2DSet(&position, -600.0f, 0.0f);
	pSpawnArray[12] = SetSpawnPoint(pSpawnArray[12], position, SEEKING_MOB, SPIRAL_PATTERN_L_TO_R);
	pSpawnArray[12].SpawnTimer = CreateComponent_Timer(pSpawnArray[12].SpawnTimer, DEPLOYMENT_DELAY);
	pSpawnArray[12].SpawnTimer->current_time = pSpawnArray[12].SpawnTimer->duration;
	pSpawnArray[12].spID = MULTI_SPAWNER;
	pSpawnArray[12].numEnemies = 10;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[12]);
}

void SetLevel5SpawnPoints(EnemySpawnPoint* pSpawnArray)
{
	Vector2D position;

	Vector2DSet(&position, -200.0f, 50.0f);
	pSpawnArray[0] = SetSpawnPoint(pSpawnArray[0], position, SEEKING_MOB, SPIRAL_PATTERN_L_TO_R);
	pSpawnArray[0].SpawnTimer = CreateComponent_Timer(pSpawnArray[0].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[0].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;
	pSpawnArray[0].spID = MULTI_SPAWNER;
	pSpawnArray[0].numEnemies = 3;
	pSpawnArray[0].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[0]);

	Vector2DSet(&position, -600.0f, 150.0f);
	pSpawnArray[1] = SetSpawnPoint(pSpawnArray[1], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[1].SpawnTimer = CreateComponent_Timer(pSpawnArray[1].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[1].SpawnTimer->current_time = pSpawnArray[1].SpawnTimer->duration;
	pSpawnArray[1].active = FLAG_ACTIVE;

	Vector2DSet(&position, -600.0f, 250.0f);
	pSpawnArray[2] = SetSpawnPoint(pSpawnArray[2], position, SEEKING_MOB, SPIRAL_PATTERN_L_TO_R);
	pSpawnArray[2].SpawnTimer = CreateComponent_Timer(pSpawnArray[2].SpawnTimer, DEPLOYMENT_DELAY);
	pSpawnArray[2].SpawnTimer->current_time = pSpawnArray[2].SpawnTimer->duration;
	pSpawnArray[2].spID = MULTI_SPAWNER;
	pSpawnArray[2].numEnemies = 3;
	pSpawnArray[2].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[2]);

	Vector2DSet(&position, -600.0f, 300.0f);
	pSpawnArray[3] = SetSpawnPoint(pSpawnArray[3], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[3].SpawnTimer = CreateComponent_Timer(pSpawnArray[3].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[3].SpawnTimer->current_time = pSpawnArray[3].SpawnTimer->duration;
	pSpawnArray[3].active = FLAG_ACTIVE;

	Vector2DSet(&position, -300.0f, 500.0f);
	pSpawnArray[4] = SetSpawnPoint(pSpawnArray[4], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[4].SpawnTimer = CreateComponent_Timer(pSpawnArray[4].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[4].SpawnTimer->current_time = pSpawnArray[4].SpawnTimer->duration;
	pSpawnArray[4].active = FLAG_ACTIVE;

	Vector2DSet(&position, -100.0f, 500.0f);
	pSpawnArray[5] = SetSpawnPoint(pSpawnArray[5], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[5].SpawnTimer = CreateComponent_Timer(pSpawnArray[5].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[5].SpawnTimer->current_time = pSpawnArray[5].SpawnTimer->duration;
	pSpawnArray[5].active = FLAG_ACTIVE;

	Vector2DSet(&position, 100.0f, 500.0f);
	pSpawnArray[6] = SetSpawnPoint(pSpawnArray[6], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[6].SpawnTimer = CreateComponent_Timer(pSpawnArray[6].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[6].SpawnTimer->current_time = pSpawnArray[6].SpawnTimer->duration;
	pSpawnArray[6].active = FLAG_ACTIVE;

	Vector2DSet(&position, 300.0f, 500.0f);
	pSpawnArray[7] = SetSpawnPoint(pSpawnArray[7], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[7].SpawnTimer = CreateComponent_Timer(pSpawnArray[7].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[7].SpawnTimer->current_time = pSpawnArray[7].SpawnTimer->duration;
	pSpawnArray[7].active = FLAG_ACTIVE;

	Vector2DSet(&position, 600.0f, 350.0f);
	pSpawnArray[8] = SetSpawnPoint(pSpawnArray[8], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_R_TO_L);
	pSpawnArray[8].SpawnTimer = CreateComponent_Timer(pSpawnArray[8].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[8].SpawnTimer->current_time = pSpawnArray[8].SpawnTimer->duration;
	pSpawnArray[8].active = FLAG_ACTIVE;

	Vector2DSet(&position, 600.0f, 200.0f);
	pSpawnArray[9] = SetSpawnPoint(pSpawnArray[9], position, SEEKING_MOB, SPIRAL_PATTERN_L_TO_R);
	pSpawnArray[9].SpawnTimer = CreateComponent_Timer(pSpawnArray[9].SpawnTimer, DEPLOYMENT_DELAY);
	pSpawnArray[9].SpawnTimer->current_time = pSpawnArray[9].SpawnTimer->duration;
	pSpawnArray[9].spID = MULTI_SPAWNER;
	pSpawnArray[9].numEnemies = 3;
	pSpawnArray[9].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[9]);

	Vector2DSet(&position, 600.0f, 150.0f);
	pSpawnArray[10] = SetSpawnPoint(pSpawnArray[10], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_R_TO_L);
	pSpawnArray[10].SpawnTimer = CreateComponent_Timer(pSpawnArray[10].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[10].SpawnTimer->current_time = pSpawnArray[10].SpawnTimer->duration;
	pSpawnArray[10].active = FLAG_ACTIVE;

	Vector2DSet(&position, 600.0f, 100.0f);
	pSpawnArray[11] = SetSpawnPoint(pSpawnArray[11], position, SEEKING_MOB, SPIRAL_PATTERN_L_TO_R);
	pSpawnArray[11].SpawnTimer = CreateComponent_Timer(pSpawnArray[11].SpawnTimer, DEPLOYMENT_DELAY);
	pSpawnArray[11].SpawnTimer->current_time = pSpawnArray[11].SpawnTimer->duration;
	pSpawnArray[11].spID = MULTI_SPAWNER;
	pSpawnArray[11].numEnemies = 6;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[11]);

	// The special 1
	Vector2DSet(&position, -600.0f, 0.0f);
	pSpawnArray[12] = SetSpawnPoint(pSpawnArray[12], position, SEEKING_MOB, SPIRAL_PATTERN_L_TO_R);
	pSpawnArray[12].SpawnTimer = CreateComponent_Timer(pSpawnArray[12].SpawnTimer, DEPLOYMENT_DELAY);
	pSpawnArray[12].SpawnTimer->current_time = pSpawnArray[12].SpawnTimer->duration;
	pSpawnArray[12].spID = MULTI_SPAWNER;
	pSpawnArray[12].numEnemies = 10;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[12]);
}

// World 2 Boss
void SetLevel6SpawnPoints(EnemySpawnPoint* pSpawnArray)
{
	Vector2D position;

	Vector2DSet(&position, 0.0f, 400.0f);
	pSpawnArray[0] = SetSpawnPoint(pSpawnArray[0], position, SEEKING_MOB, OVAL_PATTERN_HORIZONTAL);
	pSpawnArray[0].SpawnTimer = CreateComponent_Timer(pSpawnArray[0].SpawnTimer, 1.0);
	pSpawnArray[0].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;
	pSpawnArray[0].spID = MULTI_SPAWNER;
	pSpawnArray[0].numEnemies = 4;
	pSpawnArray[0].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[0]);

	// Top 4
	Vector2DSet(&position, 200.0f, 400.0f);
	pSpawnArray[2] = SetSpawnPoint(pSpawnArray[2], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[2].SpawnTimer = CreateComponent_Timer(pSpawnArray[2].SpawnTimer, 1.5);
	pSpawnArray[2].SpawnTimer->current_time = pSpawnArray[2].SpawnTimer->duration;
	pSpawnArray[2].spID = MULTI_SPAWNER;
	pSpawnArray[2].numEnemies = 5;
	pSpawnArray[2].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[2]);

	Vector2DSet(&position, -200.0f, 400.0f);
	pSpawnArray[3] = SetSpawnPoint(pSpawnArray[3], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[3].SpawnTimer = CreateComponent_Timer(pSpawnArray[3].SpawnTimer, 1.5);
	pSpawnArray[3].SpawnTimer->current_time = pSpawnArray[3].SpawnTimer->duration;
	pSpawnArray[3].spID = MULTI_SPAWNER;
	pSpawnArray[3].numEnemies = 5;
	pSpawnArray[3].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[3]);

	Vector2DSet(&position, -400.0f, 400.0f);
	pSpawnArray[1] = SetSpawnPoint(pSpawnArray[1], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[1].SpawnTimer = CreateComponent_Timer(pSpawnArray[1].SpawnTimer, 1.5);
	pSpawnArray[1].SpawnTimer->current_time = pSpawnArray[1].SpawnTimer->duration;
	pSpawnArray[1].spID = MULTI_SPAWNER;
	pSpawnArray[1].numEnemies = 5;
	pSpawnArray[1].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[1]);

	Vector2DSet(&position, 400.0f, 400.0f);
	pSpawnArray[4] = SetSpawnPoint(pSpawnArray[4], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[4].SpawnTimer = CreateComponent_Timer(pSpawnArray[4].SpawnTimer, 1.5);
	pSpawnArray[4].SpawnTimer->current_time = pSpawnArray[4].SpawnTimer->duration;
	pSpawnArray[4].spID = MULTI_SPAWNER;
	pSpawnArray[4].numEnemies = 5;
	pSpawnArray[4].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[4]);
	//

	// Spirals
	Vector2DSet(&position, -400.0f, 150.0f);
	pSpawnArray[5] = SetSpawnPoint(pSpawnArray[5], position, SEEKING_MOB, OVAL_PATTERN_VERTICAL);
	pSpawnArray[5].SpawnTimer = CreateComponent_Timer(pSpawnArray[5].SpawnTimer, 1.0);
	pSpawnArray[5].SpawnTimer->current_time = pSpawnArray[5].SpawnTimer->duration;
	pSpawnArray[5].spID = MULTI_SPAWNER;
	pSpawnArray[5].numEnemies = 5;
	pSpawnArray[5].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[5]);

	Vector2DSet(&position, -500.0f, 350.0f);
	pSpawnArray[6] = SetSpawnPoint(pSpawnArray[6], position, SEEKING_MOB, OVAL_PATTERN_VERTICAL);
	pSpawnArray[6].SpawnTimer = CreateComponent_Timer(pSpawnArray[6].SpawnTimer, 1.0);
	pSpawnArray[6].SpawnTimer->current_time = pSpawnArray[6].SpawnTimer->duration;
	pSpawnArray[6].spID = MULTI_SPAWNER;
	pSpawnArray[6].numEnemies = 5;
	pSpawnArray[6].active = FLAG_INACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[6]);

	Vector2DSet(&position, 400.0f, 150.0f);
	pSpawnArray[7] = SetSpawnPoint(pSpawnArray[7], position, SEEKING_MOB, OVAL_PATTERN_VERTICAL);
	pSpawnArray[7].SpawnTimer = CreateComponent_Timer(pSpawnArray[7].SpawnTimer, 1.0);
	pSpawnArray[7].SpawnTimer->current_time = pSpawnArray[7].SpawnTimer->duration;
	pSpawnArray[7].spID = MULTI_SPAWNER;
	pSpawnArray[7].numEnemies = 5;
	pSpawnArray[7].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[7]);
	//

	// Circles
	Vector2DSet(&position, -100.0f, 100.0f);
	pSpawnArray[8] = SetSpawnPoint(pSpawnArray[8], position, SEEKING_MOB, CIRCLE_PATTERN);
	pSpawnArray[8].SpawnTimer = CreateComponent_Timer(pSpawnArray[8].SpawnTimer, 1.0);
	pSpawnArray[8].SpawnTimer->current_time = pSpawnArray[8].SpawnTimer->duration;
	pSpawnArray[8].spID = MULTI_SPAWNER;
	pSpawnArray[8].numEnemies = 4;
	pSpawnArray[8].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[8]);

	Vector2DSet(&position, 100.0f, 100.0f);
	pSpawnArray[9] = SetSpawnPoint(pSpawnArray[9], position, SEEKING_MOB, CIRCLE_PATTERN);
	pSpawnArray[9].SpawnTimer = CreateComponent_Timer(pSpawnArray[9].SpawnTimer, 1.0);
	pSpawnArray[9].SpawnTimer->current_time = pSpawnArray[9].SpawnTimer->duration;
	pSpawnArray[9].spID = MULTI_SPAWNER;
	pSpawnArray[9].numEnemies = 4;
	pSpawnArray[9].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[9]);

	Vector2DSet(&position, 200.0f, 100.0f);
	pSpawnArray[11] = SetSpawnPoint(pSpawnArray[11], position, SEEKING_MOB, CIRCLE_PATTERN);
	pSpawnArray[11].SpawnTimer = CreateComponent_Timer(pSpawnArray[11].SpawnTimer, 1.0);
	pSpawnArray[11].SpawnTimer->current_time = pSpawnArray[11].SpawnTimer->duration;
	pSpawnArray[11].spID = MULTI_SPAWNER;
	pSpawnArray[11].numEnemies = 4;
	pSpawnArray[11].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[11]);


	Vector2DSet(&position, -200.0f, 100.0f);
	pSpawnArray[10] = SetSpawnPoint(pSpawnArray[10], position, SEEKING_MOB, CIRCLE_PATTERN);
	pSpawnArray[10].SpawnTimer = CreateComponent_Timer(pSpawnArray[10].SpawnTimer, 1.0);
	pSpawnArray[10].SpawnTimer->current_time = pSpawnArray[10].SpawnTimer->duration;
	pSpawnArray[10].spID = MULTI_SPAWNER;
	pSpawnArray[10].numEnemies = 4;
	pSpawnArray[10].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[10]);
	//
}

void SetLevel7SpawnPoints(EnemySpawnPoint* pSpawnArray)
{
	Vector2D position;

	Vector2DSet(&position, -500.0f, 0.0f);
	pSpawnArray[0] = SetSpawnPoint(pSpawnArray[0], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[0].SpawnTimer = CreateComponent_Timer(pSpawnArray[0].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[0].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, -500.0f, 100.0f);
	pSpawnArray[1] = SetSpawnPoint(pSpawnArray[1], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[1].SpawnTimer = CreateComponent_Timer(pSpawnArray[1].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[1].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, -500.0f, 200.0f);
	pSpawnArray[2] = SetSpawnPoint(pSpawnArray[2], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[2].SpawnTimer = CreateComponent_Timer(pSpawnArray[2].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[2].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, -500.0f, 300.0f);
	pSpawnArray[3] = SetSpawnPoint(pSpawnArray[3], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[3].SpawnTimer = CreateComponent_Timer(pSpawnArray[3].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[3].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, -300.0f, 400.0f);
	pSpawnArray[4] = SetSpawnPoint(pSpawnArray[4], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[4].SpawnTimer = CreateComponent_Timer(pSpawnArray[4].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[4].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, -100.0f, 400.0f);
	pSpawnArray[5] = SetSpawnPoint(pSpawnArray[5], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[5].SpawnTimer = CreateComponent_Timer(pSpawnArray[5].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[5].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, 100.0f, 400.0f);
	pSpawnArray[6] = SetSpawnPoint(pSpawnArray[6], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[6].SpawnTimer = CreateComponent_Timer(pSpawnArray[6].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[6].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, 300.0f, 400.0f);
	pSpawnArray[7] = SetSpawnPoint(pSpawnArray[7], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[7].SpawnTimer = CreateComponent_Timer(pSpawnArray[7].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[7].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, 500.0f, 350.0f);
	pSpawnArray[8] = SetSpawnPoint(pSpawnArray[8], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_R_TO_L);
	pSpawnArray[8].SpawnTimer = CreateComponent_Timer(pSpawnArray[8].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[8].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, 500.0f, 250.0f);
	pSpawnArray[9] = SetSpawnPoint(pSpawnArray[9], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_R_TO_L);
	pSpawnArray[9].SpawnTimer = CreateComponent_Timer(pSpawnArray[9].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[9].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, 500.0f, 150.0f);
	pSpawnArray[10] = SetSpawnPoint(pSpawnArray[10], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_R_TO_L);
	pSpawnArray[10].SpawnTimer = CreateComponent_Timer(pSpawnArray[10].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[10].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, -600.0f, 100.0f);
	pSpawnArray[11] = SetSpawnPoint(pSpawnArray[11], position, SEEKING_MOB, SPIRAL_PATTERN_L_TO_R);
	pSpawnArray[11].SpawnTimer = CreateComponent_Timer(pSpawnArray[11].SpawnTimer, DEPLOYMENT_DELAY - 5.0);
	pSpawnArray[11].SpawnTimer->current_time = pSpawnArray[11].SpawnTimer->duration;
	pSpawnArray[11].spID = MULTI_SPAWNER;
	pSpawnArray[11].numEnemies = 5;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[11]);

	// The special 1
	Vector2DSet(&position, -600.0f, 0.0f);
	pSpawnArray[12] = SetSpawnPoint(pSpawnArray[12], position, SEEKING_MOB, SPIRAL_PATTERN_L_TO_R);
	pSpawnArray[12].SpawnTimer = CreateComponent_Timer(pSpawnArray[12].SpawnTimer, DEPLOYMENT_DELAY - 5.0);
	pSpawnArray[12].SpawnTimer->current_time = pSpawnArray[12].SpawnTimer->duration;
	pSpawnArray[12].spID = MULTI_SPAWNER;
	pSpawnArray[12].numEnemies = 10;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[12]);
}

void SetLevel8SpawnPoints(EnemySpawnPoint* pSpawnArray)
{
	Vector2D position;

	Vector2DSet(&position, -500.0f, 0.0f);
	pSpawnArray[0] = SetSpawnPoint(pSpawnArray[0], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[0].SpawnTimer = CreateComponent_Timer(pSpawnArray[0].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[0].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, -500.0f, 100.0f);
	pSpawnArray[1] = SetSpawnPoint(pSpawnArray[1], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[1].SpawnTimer = CreateComponent_Timer(pSpawnArray[1].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[1].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, -500.0f, 200.0f);
	pSpawnArray[2] = SetSpawnPoint(pSpawnArray[2], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[2].SpawnTimer = CreateComponent_Timer(pSpawnArray[2].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[2].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, -500.0f, 300.0f);
	pSpawnArray[3] = SetSpawnPoint(pSpawnArray[3], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[3].SpawnTimer = CreateComponent_Timer(pSpawnArray[3].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[3].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, -300.0f, 400.0f);
	pSpawnArray[4] = SetSpawnPoint(pSpawnArray[4], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[4].SpawnTimer = CreateComponent_Timer(pSpawnArray[4].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[4].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, -100.0f, 400.0f);
	pSpawnArray[5] = SetSpawnPoint(pSpawnArray[5], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[5].SpawnTimer = CreateComponent_Timer(pSpawnArray[5].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[5].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, 100.0f, 400.0f);
	pSpawnArray[6] = SetSpawnPoint(pSpawnArray[6], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[6].SpawnTimer = CreateComponent_Timer(pSpawnArray[6].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[6].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, 300.0f, 400.0f);
	pSpawnArray[7] = SetSpawnPoint(pSpawnArray[7], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[7].SpawnTimer = CreateComponent_Timer(pSpawnArray[7].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[7].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, 500.0f, 350.0f);
	pSpawnArray[8] = SetSpawnPoint(pSpawnArray[8], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_R_TO_L);
	pSpawnArray[8].SpawnTimer = CreateComponent_Timer(pSpawnArray[8].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[8].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, 500.0f, 250.0f);
	pSpawnArray[9] = SetSpawnPoint(pSpawnArray[9], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_R_TO_L);
	pSpawnArray[9].SpawnTimer = CreateComponent_Timer(pSpawnArray[9].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[9].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, 500.0f, 150.0f);
	pSpawnArray[10] = SetSpawnPoint(pSpawnArray[10], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_R_TO_L);
	pSpawnArray[10].SpawnTimer = CreateComponent_Timer(pSpawnArray[10].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[10].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, -600.0f, 100.0f);
	pSpawnArray[11] = SetSpawnPoint(pSpawnArray[11], position, SEEKING_MOB, SPIRAL_PATTERN_L_TO_R);
	pSpawnArray[11].SpawnTimer = CreateComponent_Timer(pSpawnArray[11].SpawnTimer, DEPLOYMENT_DELAY - 5.0);
	pSpawnArray[11].SpawnTimer->current_time = pSpawnArray[11].SpawnTimer->duration;
	pSpawnArray[11].spID = MULTI_SPAWNER;
	pSpawnArray[11].numEnemies = 5;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[11]);

	// The special 1
	Vector2DSet(&position, -600.0f, 0.0f);
	pSpawnArray[12] = SetSpawnPoint(pSpawnArray[12], position, SEEKING_MOB, SPIRAL_PATTERN_L_TO_R);
	pSpawnArray[12].SpawnTimer = CreateComponent_Timer(pSpawnArray[12].SpawnTimer, DEPLOYMENT_DELAY - 5.0);
	pSpawnArray[12].SpawnTimer->current_time = pSpawnArray[12].SpawnTimer->duration;
	pSpawnArray[12].spID = MULTI_SPAWNER;
	pSpawnArray[12].numEnemies = 10;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[12]);
}

// World 3 Boss
void SetLevel9SpawnPoints(EnemySpawnPoint* pSpawnArray)
{
	Vector2D position;

	Vector2DSet(&position, -600.0f, 0.0f);
	pSpawnArray[0] = SetSpawnPoint(pSpawnArray[0], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[0].SpawnTimer = CreateComponent_Timer(pSpawnArray[0].SpawnTimer, DEPLOYMENT_DELAY / 2.0);
	pSpawnArray[0].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;
	pSpawnArray[0].spID = MULTI_SPAWNER;
	pSpawnArray[0].numEnemies = 6;
	pSpawnArray[0].active = FLAG_INACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[0]);

	// Top 4
	Vector2DSet(&position, 200.0f, 400.0f);
	pSpawnArray[2] = SetSpawnPoint(pSpawnArray[2], position, SEEKING_MOB, OVAL_PATTERN_HORIZONTAL);
	pSpawnArray[2].SpawnTimer = CreateComponent_Timer(pSpawnArray[2].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[2].SpawnTimer->current_time = pSpawnArray[2].SpawnTimer->duration;
	pSpawnArray[2].active = FLAG_ACTIVE;

	Vector2DSet(&position, -200.0f, 400.0f);
	pSpawnArray[3] = SetSpawnPoint(pSpawnArray[3], position, SEEKING_MOB, OVAL_PATTERN_HORIZONTAL);
	pSpawnArray[3].SpawnTimer = CreateComponent_Timer(pSpawnArray[3].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[3].SpawnTimer->current_time = pSpawnArray[3].SpawnTimer->duration;
	pSpawnArray[3].active = FLAG_ACTIVE;

	Vector2DSet(&position, -400.0f, 400.0f);
	pSpawnArray[1] = SetSpawnPoint(pSpawnArray[1], position, SEEKING_MOB, OVAL_PATTERN_HORIZONTAL);
	pSpawnArray[1].SpawnTimer = CreateComponent_Timer(pSpawnArray[1].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[1].SpawnTimer->current_time = pSpawnArray[1].SpawnTimer->duration;
	pSpawnArray[1].active = FLAG_ACTIVE;

	Vector2DSet(&position, 400.0f, 400.0f);
	pSpawnArray[4] = SetSpawnPoint(pSpawnArray[4], position, SEEKING_MOB, OVAL_PATTERN_HORIZONTAL);
	pSpawnArray[4].SpawnTimer = CreateComponent_Timer(pSpawnArray[4].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[4].SpawnTimer->current_time = pSpawnArray[4].SpawnTimer->duration;
	pSpawnArray[4].active = FLAG_ACTIVE;
	//

	// Spirals
	Vector2DSet(&position, -200.0f, 350.0f);
	pSpawnArray[5] = SetSpawnPoint(pSpawnArray[5], position, SEEKING_MOB, SPIRAL_PATTERN_UP_TO_DOWN);
	pSpawnArray[5].SpawnTimer = CreateComponent_Timer(pSpawnArray[5].SpawnTimer, 1.0);
	pSpawnArray[5].SpawnTimer->current_time = pSpawnArray[5].SpawnTimer->duration;
	pSpawnArray[5].spID = MULTI_SPAWNER;
	pSpawnArray[5].numEnemies = 5;
	pSpawnArray[5].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[5]);

	Vector2DSet(&position, -500.0f, 350.0f);
	pSpawnArray[6] = SetSpawnPoint(pSpawnArray[6], position, SEEKING_MOB, SPIRAL_PATTERN_UP_TO_DOWN);
	pSpawnArray[6].SpawnTimer = CreateComponent_Timer(pSpawnArray[6].SpawnTimer, 1.0);
	pSpawnArray[6].SpawnTimer->current_time = pSpawnArray[6].SpawnTimer->duration;
	pSpawnArray[6].spID = MULTI_SPAWNER;
	pSpawnArray[6].numEnemies = 5;
	pSpawnArray[6].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[6]);

	Vector2DSet(&position, 200.0f, 350.0f);
	pSpawnArray[7] = SetSpawnPoint(pSpawnArray[7], position, SEEKING_MOB, SPIRAL_PATTERN_UP_TO_DOWN);
	pSpawnArray[7].SpawnTimer = CreateComponent_Timer(pSpawnArray[7].SpawnTimer, 1.0);
	pSpawnArray[7].SpawnTimer->current_time = pSpawnArray[7].SpawnTimer->duration;
	pSpawnArray[7].spID = MULTI_SPAWNER;
	pSpawnArray[7].numEnemies = 5;
	pSpawnArray[7].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[7]);
	//

	// Spirals
	Vector2DSet(&position, 0.0f, 275.0f);
	pSpawnArray[8] = SetSpawnPoint(pSpawnArray[8], position, SEEKING_MOB, SPIRAL_PATTERN_UP_TO_DOWN);
	pSpawnArray[8].SpawnTimer = CreateComponent_Timer(pSpawnArray[8].SpawnTimer, 1.0);
	pSpawnArray[8].SpawnTimer->current_time = pSpawnArray[8].SpawnTimer->duration;
	pSpawnArray[8].spID = MULTI_SPAWNER;
	pSpawnArray[8].numEnemies = 5;
	pSpawnArray[8].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[8]);

	Vector2DSet(&position, -400.0f, 350.0f);
	pSpawnArray[9] = SetSpawnPoint(pSpawnArray[9], position, SEEKING_MOB, SPIRAL_PATTERN_UP_TO_DOWN);
	pSpawnArray[9].SpawnTimer = CreateComponent_Timer(pSpawnArray[9].SpawnTimer, 1.0);
	pSpawnArray[9].SpawnTimer->current_time = pSpawnArray[9].SpawnTimer->duration;
	pSpawnArray[9].spID = MULTI_SPAWNER;
	pSpawnArray[9].numEnemies = 5;
	pSpawnArray[9].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[9]);

	Vector2DSet(&position, 400.0f, 350.0f);
	pSpawnArray[11] = SetSpawnPoint(pSpawnArray[11], position, SEEKING_MOB, SPIRAL_PATTERN_UP_TO_DOWN);
	pSpawnArray[11].SpawnTimer = CreateComponent_Timer(pSpawnArray[11].SpawnTimer, 1.0);
	pSpawnArray[11].SpawnTimer->current_time = pSpawnArray[11].SpawnTimer->duration;
	pSpawnArray[11].spID = MULTI_SPAWNER;
	pSpawnArray[11].numEnemies = 5;
	pSpawnArray[11].active = FLAG_ACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[11]);
	//

	Vector2DSet(&position, 400.0f, 200.0f);
	pSpawnArray[10] = SetSpawnPoint(pSpawnArray[10], position, SEEKING_MOB, SPIRAL_PATTERN_R_TO_L);
	pSpawnArray[10].SpawnTimer = CreateComponent_Timer(pSpawnArray[10].SpawnTimer, DEPLOYMENT_DELAY / 2.0);
	pSpawnArray[10].SpawnTimer->current_time = pSpawnArray[10].SpawnTimer->duration;
	pSpawnArray[10].spID = MULTI_SPAWNER;
	pSpawnArray[10].numEnemies = 3;
	pSpawnArray[10].active = FLAG_INACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[10]);
}

void SetLevel10SpawnPoints(EnemySpawnPoint* pSpawnArray)
{
	Vector2D position;

	Vector2DSet(&position, -500.0f, 0.0f);
	pSpawnArray[0] = SetSpawnPoint(pSpawnArray[0], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[0].SpawnTimer = CreateComponent_Timer(pSpawnArray[0].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[0].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, -500.0f, 100.0f);
	pSpawnArray[1] = SetSpawnPoint(pSpawnArray[1], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[1].SpawnTimer = CreateComponent_Timer(pSpawnArray[1].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[1].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, -500.0f, 200.0f);
	pSpawnArray[2] = SetSpawnPoint(pSpawnArray[2], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[2].SpawnTimer = CreateComponent_Timer(pSpawnArray[2].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[2].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, -500.0f, 300.0f);
	pSpawnArray[3] = SetSpawnPoint(pSpawnArray[3], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[3].SpawnTimer = CreateComponent_Timer(pSpawnArray[3].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[3].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, -300.0f, 400.0f);
	pSpawnArray[4] = SetSpawnPoint(pSpawnArray[4], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[4].SpawnTimer = CreateComponent_Timer(pSpawnArray[4].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[4].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, -100.0f, 400.0f);
	pSpawnArray[5] = SetSpawnPoint(pSpawnArray[5], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[5].SpawnTimer = CreateComponent_Timer(pSpawnArray[5].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[5].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, 100.0f, 400.0f);
	pSpawnArray[6] = SetSpawnPoint(pSpawnArray[6], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[6].SpawnTimer = CreateComponent_Timer(pSpawnArray[6].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[6].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, 300.0f, 400.0f);
	pSpawnArray[7] = SetSpawnPoint(pSpawnArray[7], position, SEEKING_MOB, VERTICAL_MOVEMENT_UP_TO_DOWN);
	pSpawnArray[7].SpawnTimer = CreateComponent_Timer(pSpawnArray[7].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[7].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, 500.0f, 350.0f);
	pSpawnArray[8] = SetSpawnPoint(pSpawnArray[8], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_R_TO_L);
	pSpawnArray[8].SpawnTimer = CreateComponent_Timer(pSpawnArray[8].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[8].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, 500.0f, 250.0f);
	pSpawnArray[9] = SetSpawnPoint(pSpawnArray[9], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_R_TO_L);
	pSpawnArray[9].SpawnTimer = CreateComponent_Timer(pSpawnArray[9].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[9].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, 500.0f, 150.0f);
	pSpawnArray[10] = SetSpawnPoint(pSpawnArray[10], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_R_TO_L);
	pSpawnArray[10].SpawnTimer = CreateComponent_Timer(pSpawnArray[10].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[10].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;

	Vector2DSet(&position, -600.0f, 100.0f);
	pSpawnArray[11] = SetSpawnPoint(pSpawnArray[11], position, SEEKING_MOB, SPIRAL_PATTERN_L_TO_R);
	pSpawnArray[11].SpawnTimer = CreateComponent_Timer(pSpawnArray[11].SpawnTimer, DEPLOYMENT_DELAY - 5.0);
	pSpawnArray[11].SpawnTimer->current_time = pSpawnArray[11].SpawnTimer->duration;
	pSpawnArray[11].spID = MULTI_SPAWNER;
	pSpawnArray[11].numEnemies = 5;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[11]);

	// The special 1
	Vector2DSet(&position, -600.0f, 0.0f);
	pSpawnArray[12] = SetSpawnPoint(pSpawnArray[12], position, SEEKING_MOB, SPIRAL_PATTERN_L_TO_R);
	pSpawnArray[12].SpawnTimer = CreateComponent_Timer(pSpawnArray[12].SpawnTimer, DEPLOYMENT_DELAY - 5.0);
	pSpawnArray[12].SpawnTimer->current_time = pSpawnArray[12].SpawnTimer->duration;
	pSpawnArray[12].spID = MULTI_SPAWNER;
	pSpawnArray[12].numEnemies = 10;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[12]);
}

void SetLevel11SpawnPoints(EnemySpawnPoint* pSpawnArray)
{
	Vector2D position;

	//
	Vector2DSet(&position, -150.0f, 200.0f);
	pSpawnArray[0] = SetSpawnPoint(pSpawnArray[0], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[0].SpawnTimer = CreateComponent_Timer(pSpawnArray[0].SpawnTimer, DEPLOYMENT_DELAY - 2.0);
	pSpawnArray[0].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;
	pSpawnArray[0].active = FLAG_ACTIVE;

	Vector2DSet(&position, 150.0f, 200.0f);
	pSpawnArray[2] = SetSpawnPoint(pSpawnArray[2], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_R_TO_L);
	pSpawnArray[2].SpawnTimer = CreateComponent_Timer(pSpawnArray[2].SpawnTimer, DEPLOYMENT_DELAY - 2.0);
	pSpawnArray[2].SpawnTimer->current_time = pSpawnArray[2].SpawnTimer->duration;
	pSpawnArray[2].active = FLAG_ACTIVE;

	Vector2DSet(&position, -200.0f, 300.0f);
	pSpawnArray[3] = SetSpawnPoint(pSpawnArray[3], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[3].SpawnTimer = CreateComponent_Timer(pSpawnArray[3].SpawnTimer, DEPLOYMENT_DELAY - 2.0);
	pSpawnArray[3].SpawnTimer->current_time = pSpawnArray[3].SpawnTimer->duration;
	pSpawnArray[3].active = FLAG_ACTIVE;

	Vector2DSet(&position, 200.0f, 300.0f);
	pSpawnArray[10] = SetSpawnPoint(pSpawnArray[10], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_R_TO_L);
	pSpawnArray[10].SpawnTimer = CreateComponent_Timer(pSpawnArray[10].SpawnTimer, DEPLOYMENT_DELAY - 2.0);
	pSpawnArray[10].SpawnTimer->current_time = pSpawnArray[10].SpawnTimer->duration;
	pSpawnArray[10].active = FLAG_ACTIVE;
	//

	//
	Vector2DSet(&position, -300.0f, 300.0f);
	pSpawnArray[1] = SetSpawnPoint(pSpawnArray[1], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[1].SpawnTimer = CreateComponent_Timer(pSpawnArray[1].SpawnTimer, RESPAWN_TIMER - 2.0f);
	pSpawnArray[1].SpawnTimer->current_time = pSpawnArray[1].SpawnTimer->duration;
	pSpawnArray[1].active = FLAG_ACTIVE;

	Vector2DSet(&position, -250.0f, 200.0f);
	pSpawnArray[4] = SetSpawnPoint(pSpawnArray[4], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[4].SpawnTimer = CreateComponent_Timer(pSpawnArray[4].SpawnTimer, RESPAWN_TIMER - 2.0f);
	pSpawnArray[4].SpawnTimer->current_time = pSpawnArray[4].SpawnTimer->duration;
	pSpawnArray[4].active = FLAG_ACTIVE;

	Vector2DSet(&position, -200.0f, 100.0f);
	pSpawnArray[5] = SetSpawnPoint(pSpawnArray[5], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[5].SpawnTimer = CreateComponent_Timer(pSpawnArray[5].SpawnTimer, RESPAWN_TIMER - 2.0f);
	pSpawnArray[5].SpawnTimer->current_time = pSpawnArray[5].SpawnTimer->duration;
	pSpawnArray[5].active = FLAG_ACTIVE;
	//

	//
	Vector2DSet(&position, 300.0f, 300.0f);
	pSpawnArray[6] = SetSpawnPoint(pSpawnArray[6], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_R_TO_L);
	pSpawnArray[6].SpawnTimer = CreateComponent_Timer(pSpawnArray[6].SpawnTimer, RESPAWN_TIMER - 2.0f);
	pSpawnArray[6].SpawnTimer->current_time = pSpawnArray[6].SpawnTimer->duration;
	pSpawnArray[6].active = FLAG_ACTIVE;

	Vector2DSet(&position, 250.0f, 200.0f);
	pSpawnArray[7] = SetSpawnPoint(pSpawnArray[7], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_R_TO_L);
	pSpawnArray[7].SpawnTimer = CreateComponent_Timer(pSpawnArray[7].SpawnTimer, RESPAWN_TIMER - 2.0f);
	pSpawnArray[7].SpawnTimer->current_time = pSpawnArray[7].SpawnTimer->duration;
	pSpawnArray[7].active = FLAG_ACTIVE;

	Vector2DSet(&position, 200.0f, 100.0f);
	pSpawnArray[8] = SetSpawnPoint(pSpawnArray[8], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_R_TO_L);
	pSpawnArray[8].SpawnTimer = CreateComponent_Timer(pSpawnArray[8].SpawnTimer, RESPAWN_TIMER - 2.0f);
	pSpawnArray[8].SpawnTimer->current_time = pSpawnArray[8].SpawnTimer->duration;
	pSpawnArray[8].active = FLAG_ACTIVE;
	//

	//
	Vector2DSet(&position, -120.0f, 400.0f);
	pSpawnArray[9] = SetSpawnPoint(pSpawnArray[9], position, SEEKING_MOB, OVAL_PATTERN_HORIZONTAL);
	pSpawnArray[9].SpawnTimer = CreateComponent_Timer(pSpawnArray[9].SpawnTimer, RESPAWN_TIMER - 2.0f);
	pSpawnArray[9].SpawnTimer->current_time = pSpawnArray[9].SpawnTimer->duration;
	pSpawnArray[9].active = FLAG_ACTIVE;


	Vector2DSet(&position, 120.0f, 400.0f);
	pSpawnArray[11] = SetSpawnPoint(pSpawnArray[11], position, SEEKING_MOB, OVAL_PATTERN_HORIZONTAL);
	pSpawnArray[11].SpawnTimer = CreateComponent_Timer(pSpawnArray[11].SpawnTimer, RESPAWN_TIMER - 2.0f);
	pSpawnArray[11].SpawnTimer->current_time = pSpawnArray[11].SpawnTimer->duration;
	pSpawnArray[11].active = FLAG_ACTIVE;
	//

	// The special 1
	Vector2DSet(&position, 0.0f, 100.0f);
	pSpawnArray[12] = SetSpawnPoint(pSpawnArray[12], position, SEEKING_MOB, OVAL_PATTERN_VERTICAL);
	pSpawnArray[12].SpawnTimer = CreateComponent_Timer(pSpawnArray[12].SpawnTimer, DEPLOYMENT_DELAY);
	pSpawnArray[12].SpawnTimer->current_time = pSpawnArray[12].SpawnTimer->duration;
	pSpawnArray[12].active = FLAG_ACTIVE;
}

// World 4 Boss
void SetLevel12SpawnPoints(EnemySpawnPoint* pSpawnArray)
{
	Vector2D position;

	Vector2DSet(&position, -600.0f, 0.0f);
	pSpawnArray[0] = SetSpawnPoint(pSpawnArray[0], position, SEEKING_MOB, HORIZONTAL_MOVEMENT_L_TO_R);
	pSpawnArray[0].SpawnTimer = CreateComponent_Timer(pSpawnArray[0].SpawnTimer, DEPLOYMENT_DELAY / 2.0);
	pSpawnArray[0].SpawnTimer->current_time = pSpawnArray[0].SpawnTimer->duration;
	pSpawnArray[0].spID = MULTI_SPAWNER;
	pSpawnArray[0].numEnemies = 6;
	pSpawnArray[0].active = FLAG_INACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[0]);

	Vector2DSet(&position, -600.0f, 200.0f);
	pSpawnArray[2] = SetSpawnPoint(pSpawnArray[2], position, SEEKING_MOB, SPIRAL_PATTERN_L_TO_R);
	pSpawnArray[2].SpawnTimer = CreateComponent_Timer(pSpawnArray[2].SpawnTimer, DEPLOYMENT_DELAY / 2.0);
	pSpawnArray[2].SpawnTimer->current_time = pSpawnArray[2].SpawnTimer->duration;
	pSpawnArray[2].spID = MULTI_SPAWNER;
	pSpawnArray[2].numEnemies = 3;
	pSpawnArray[2].active = FLAG_INACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[2]);

	Vector2DSet(&position, -600.0f, 100.0f);
	pSpawnArray[3] = SetSpawnPoint(pSpawnArray[3], position, SEEKING_MOB, SPIRAL_PATTERN_L_TO_R);
	pSpawnArray[3].SpawnTimer = CreateComponent_Timer(pSpawnArray[3].SpawnTimer, DEPLOYMENT_DELAY / 2.0);
	pSpawnArray[3].SpawnTimer->current_time = pSpawnArray[3].SpawnTimer->duration - (0.25 * DEPLOYMENT_DELAY);
	pSpawnArray[3].spID = MULTI_SPAWNER;
	pSpawnArray[3].numEnemies = 3;
	pSpawnArray[3].active = FLAG_INACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[3]);

	// Top 2
	Vector2DSet(&position, -400.0f, 400.0f);
	pSpawnArray[1] = SetSpawnPoint(pSpawnArray[1], position, SEEKING_MOB, OVAL_PATTERN_HORIZONTAL);
	pSpawnArray[1].SpawnTimer = CreateComponent_Timer(pSpawnArray[1].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[1].SpawnTimer->current_time = pSpawnArray[1].SpawnTimer->duration;
	pSpawnArray[1].active = FLAG_INACTIVE;

	Vector2DSet(&position, -400.0f, 400.0f);
	pSpawnArray[4] = SetSpawnPoint(pSpawnArray[4], position, SEEKING_MOB, OVAL_PATTERN_HORIZONTAL);
	pSpawnArray[4].SpawnTimer = CreateComponent_Timer(pSpawnArray[4].SpawnTimer, RESPAWN_TIMER);
	pSpawnArray[4].SpawnTimer->current_time = pSpawnArray[4].SpawnTimer->duration;
	pSpawnArray[4].active = FLAG_INACTIVE;
	//

	// Spirals
	Vector2DSet(&position, -200.0f, 350.0f);
	pSpawnArray[5] = SetSpawnPoint(pSpawnArray[5], position, SEEKING_MOB, SPIRAL_PATTERN_UP_TO_DOWN);
	pSpawnArray[5].SpawnTimer = CreateComponent_Timer(pSpawnArray[5].SpawnTimer, 1.0);
	pSpawnArray[5].SpawnTimer->current_time = pSpawnArray[5].SpawnTimer->duration;
	pSpawnArray[5].spID = MULTI_SPAWNER;
	pSpawnArray[5].numEnemies = 5;
	pSpawnArray[5].active = FLAG_INACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[5]);

	Vector2DSet(&position, -100.0f, 350.0f);
	pSpawnArray[6] = SetSpawnPoint(pSpawnArray[6], position, SEEKING_MOB, SPIRAL_PATTERN_UP_TO_DOWN);
	pSpawnArray[6].SpawnTimer = CreateComponent_Timer(pSpawnArray[6].SpawnTimer, 1.0);
	pSpawnArray[6].SpawnTimer->current_time = pSpawnArray[6].SpawnTimer->duration;
	pSpawnArray[6].spID = MULTI_SPAWNER;
	pSpawnArray[6].numEnemies = 5;
	pSpawnArray[6].active = FLAG_INACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[6]);

	Vector2DSet(&position, 200.0f, 350.0f);
	pSpawnArray[7] = SetSpawnPoint(pSpawnArray[7], position, SEEKING_MOB, SPIRAL_PATTERN_UP_TO_DOWN);
	pSpawnArray[7].SpawnTimer = CreateComponent_Timer(pSpawnArray[7].SpawnTimer, 1.0);
	pSpawnArray[7].SpawnTimer->current_time = pSpawnArray[7].SpawnTimer->duration;
	pSpawnArray[7].spID = MULTI_SPAWNER;
	pSpawnArray[7].numEnemies = 5;
	pSpawnArray[7].active = FLAG_INACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[7]);
	//

	// Spirals
	Vector2DSet(&position, 0.0f, 275.0f);
	pSpawnArray[8] = SetSpawnPoint(pSpawnArray[8], position, SEEKING_MOB, SPIRAL_PATTERN_UP_TO_DOWN);
	pSpawnArray[8].SpawnTimer = CreateComponent_Timer(pSpawnArray[8].SpawnTimer, 1.0);
	pSpawnArray[8].SpawnTimer->current_time = pSpawnArray[8].SpawnTimer->duration;
	pSpawnArray[8].spID = MULTI_SPAWNER;
	pSpawnArray[8].numEnemies = 5;
	pSpawnArray[8].active = FLAG_INACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[8]);

	Vector2DSet(&position, -400.0f, 350.0f);
	pSpawnArray[9] = SetSpawnPoint(pSpawnArray[9], position, SEEKING_MOB, SPIRAL_PATTERN_UP_TO_DOWN);
	pSpawnArray[9].SpawnTimer = CreateComponent_Timer(pSpawnArray[9].SpawnTimer, 1.0);
	pSpawnArray[9].SpawnTimer->current_time = pSpawnArray[9].SpawnTimer->duration;
	pSpawnArray[9].spID = MULTI_SPAWNER;
	pSpawnArray[9].numEnemies = 5;
	pSpawnArray[9].active = FLAG_INACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[9]);

	Vector2DSet(&position, 400.0f, 350.0f);
	pSpawnArray[11] = SetSpawnPoint(pSpawnArray[11], position, SEEKING_MOB, SPIRAL_PATTERN_UP_TO_DOWN);
	pSpawnArray[11].SpawnTimer = CreateComponent_Timer(pSpawnArray[11].SpawnTimer, 1.0);
	pSpawnArray[11].SpawnTimer->current_time = pSpawnArray[11].SpawnTimer->duration;
	pSpawnArray[11].spID = MULTI_SPAWNER;
	pSpawnArray[11].numEnemies = 5;
	pSpawnArray[11].active = FLAG_INACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[11]);
	//

	Vector2DSet(&position, 400.0f, 200.0f);
	pSpawnArray[10] = SetSpawnPoint(pSpawnArray[10], position, SEEKING_MOB, SPIRAL_PATTERN_R_TO_L);
	pSpawnArray[10].SpawnTimer = CreateComponent_Timer(pSpawnArray[10].SpawnTimer, DEPLOYMENT_DELAY / 2.0);
	pSpawnArray[10].SpawnTimer->current_time = pSpawnArray[10].SpawnTimer->duration;
	pSpawnArray[10].spID = MULTI_SPAWNER;
	pSpawnArray[10].numEnemies = 3;
	pSpawnArray[10].active = FLAG_INACTIVE;
	LoadSpecialEnemySpawnpoint(&pSpawnArray[10]);

}