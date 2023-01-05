#include <raylib.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include <../../src/ennemies.h>


#define SPAWN_POINTS 1
#define MAX_ENNEMIES_SPEED 150

void UpdateEnnemi(EnnemiesArray *ennemies, float deltaTime);

void UpdateEnnemiCollider(EnnemiesArray *ennemies);

void EnnemiesArrayManagement(EnnemiesArray *ennemies);

void SpawnEnnemi(SpawnStock *spawnArray, EnnemiesArray *ennemiArray, int *seed, Vector2D *playersPos[], int playerCount, int lvlIndex);

void InitiateEnnemiCollider(Ennemi *newEnnemi);

void SpawnEnnemiChild(Ennemi ennemi, EnnemiesArray *ennemiesArray, SpawnStock *spawnArray);

void SetupEnnemiCollider(Polygon *collider, int point);

int GetEmptyIndexEnnemyArray(EnnemiesArray *ennemiesArrayc);

void DrawEnnemiCollider(EnnemiesArray *ennemies, bool activeCollider);

Vector2D FindValidSpawnPoint(SpawnStock *spawnArray, int maxSpawn);

void DrawSpawnPoint(SpawnStock *spawnPoint, Texture2D tex);

void DrawEnnemies(Ennemies *ennemies, Texture2D tex);

void EnnemiesInit(int lvlIndex, Ennemies *ennemies, Texture2D tex);

void UpdateSpawnEnnemi(SpawnStock *spawnArray, EnnemiesArray *ennemiArray, float deltaTime, int *seed, Vector2D *playersPos[], int playerCount, int lvlIndex);

void UpdateEnnemiCollision(int *score, Ennemies *ennemies, EnnemiesArray *ennemiesArray, Projectiles *projectiles);

bool CheckIfStillEnnemyAlive(int ennemiesAlive, int spawnPointActive);

void DeathAttack(Ennemi *ennemi, Ennemies *ennemies, Projectiles *projectiles);

void DestroyEnnemi(int *score, Projectiles *projectiles, Ennemi *ennemi, Ennemies *ennemies);

void FreeEnnemiesArray(Ennemies *ennemies);

void EnnemiMemoryFree(Ennemies *ennemies);

Vector2D GetCloserPlayer(Vector2D ennemiPos, Vector2D *playerPos[], int indexDir);

void UpdateEnnemiStats(Ennemies *ennemies, int32_t *seed, float deltaTime, Vector2D *playersPos[], int playerCount, int lvlIndex)
{
    UpdateSpawnEnnemi(&ennemies->spawnArray, &ennemies->ennemiesArray, deltaTime, seed, playersPos, playerCount, lvlIndex);
    UpdateEnnemi(&ennemies->ennemiesArray, deltaTime);
    UpdateEnnemiCollider(&ennemies->ennemiesArray);
}

void FreeEnnemiesArray(Ennemies *ennemies)
{
    free(ennemies->spawnArray.spawnPointBox);
    free(ennemies->ennemiesArray.ennemis);
}

void EnnemiesArrayAlive(Ennemies *ennemies, int *lvlIndex, GameState *gameState, float *prelaunchCountdown, Texture2D tex)
{
    EnnemiesArrayManagement(&ennemies->ennemiesArray);

    if (!CheckIfStillEnnemyAlive(ennemies->ennemiesArray.ennemisAlive, ennemies->spawnArray.spawnPointActive))
    {
        *lvlIndex = *lvlIndex + 1;
        FreeEnnemiesArray(ennemies);
        EnnemiesInit(*lvlIndex, ennemies, tex);
        *gameState = GS_PreLaunch;
        *prelaunchCountdown = 0;
    }
}

void DrawEnnemiesAndCollider(Ennemies *ennemies, bool activeColliderShow, Texture2D tex)
{
    DrawEnnemies(ennemies, tex);
    DrawEnnemiCollider(&ennemies->ennemiesArray, activeColliderShow);
}

// Get random position (Don't forget to init seed)
Vector2D GetRandomPosition(int *seed)
{
    Vector2D randomPosition = {45, 50};
    *seed = getRandomNumber(*seed);
    int num = 0;
    num = *seed % SCREENWIDTHBORDER;
    *seed = getRandomNumber(*seed);
    int num2 = 0;
    num2 = *seed % SCREENHEIGHTBORDER;
    randomPosition.x += num;
    randomPosition.y += num2;
    return randomPosition;
}

// Get valid position with random position
Vector2D GetValidPosition(int *seed, int ennemiCountMax, SpawnStock Spawny, int rayon)
{
    Vector2D randomPosition = GetRandomPosition(seed);

    for (int i = 0; i < ennemiCountMax; i++)
    {
        if (VectorCompareCircle(randomPosition, Spawny.spawnPointBox[i].position, rayon) == true)
        {
            return GetValidPosition(seed, ennemiCountMax, Spawny, rayon);
        }
    }
    return randomPosition;
}

// Get spawn points with valid positions
void GetSpawnPoint(SpawnStock *SpawnStocky, int *seed, int ennemiCountMax, int rayon)
{
    SpawnStocky->spawnPointActive = 0;

    for (int i = 0; i < ennemiCountMax; i++)
    {
        SpawnStocky->spawnPointBox[i].position = GetValidPosition(seed, SpawnStocky->spawnPointActive, *SpawnStocky, rayon);
        SpawnStocky->spawnPointBox[i].isActive = true;
        SpawnStocky->spawnPointActive++;
    }
}

void EnnemiesInit(int lvlIndex, Ennemies *ennemies, Texture2D tex)
{
    ennemies->spawnArray.ennemiCountMax = (SPAWN_POINTS + lvlIndex) * 7;
    ennemies->spawnArray.spawnPointActive = 0;
    ennemies->spawnArray.spawnPointBox = malloc(ennemies->spawnArray.ennemiCountMax * sizeof(SpawnPoint));
    for (int i = 0; i < ennemies->spawnArray.ennemiCountMax; i++)
    {
        ennemies->spawnArray.spawnPointBox[i].position = (Vector2D){0.0f, 0.0f};
        ennemies->spawnArray.spawnPointBox[i].isActive = false;
    }

    ennemies->spawnArray.spawnSprite.sprite = (Rectangle){tex.width / 4, 0, tex.width / 4, tex.height / 2};

    ennemies->spawnArray.spawnCountdown = 0;
    ennemies->spawnArray.spawnTime = 1.0f;

    // Allocation
    ennemies->ennemiesArray.ennemis = malloc(ennemies->spawnArray.ennemiCountMax * sizeof(Ennemi));
    ennemies->ennemiesArray.ennemisAlive = 0;
    ennemies->ennemiesArray.maxCountEnnemi = ennemies->spawnArray.ennemiCountMax;
    ennemies->ennemiesArray.mainEnnemiSpawm = 0;

    for (int i = 0; i < 4; i++)
    {
        ennemies->ennemiesArray.spriteEnnemi[i].sprite = (Rectangle){i * tex.width / 4, tex.height / 2, tex.width / 4, tex.height / 2};
    }
    ennemies->ennemiesArray.spriteEnnemi[4].sprite = (Rectangle){2 * tex.width / 4, 0, tex.width / 4, tex.height / 2};
}

void UpdateEnnemiCollision(int *score, Ennemies *ennemies, EnnemiesArray *ennemiesArray, Projectiles *projectiles)
{
    for (int j = 0; j < projectiles->fireBallCount; j++)
    {
        for (int i = 0; i < ennemiesArray->ennemisAlive; i++)
        {
            if (projectiles->fireballArray[j].isActive && ennemiesArray->ennemis[i].life && projectiles->fireballArray[j].shooter != ENNEMIS)
            {

                if (SATCollisionMix(projectiles->fireballArray[j].collider, ennemiesArray->ennemis[i].collider))
                {
                    FireballHit(&projectiles->fireballArray[j], projectiles);
                    DestroyEnnemi(score, projectiles, &ennemiesArray->ennemis[i], ennemies);
                }
            }
        }
    }
}

bool CheckIfStillEnnemyAlive(int ennemiesAlive, int spawnPointActive)
{
    if (ennemiesAlive > 0 || spawnPointActive > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void DeathAttack(Ennemi *ennemi, Ennemies *ennemies, Projectiles *projectiles)
{
    if (ennemi->ennemisType == FIREBALLMINE || ennemi->ennemisType == MAGNETICFIREBALLMINE)
    {
        Vector2D directionToPlayer = GetCloserPlayer(ennemi->position, ennemi->playerPosition, ennemi->playerCount);
        directionToPlayer = NormalizeVector2D(directionToPlayer);
        SpawnFireBall(&projectiles->fireballArray[projectiles->fireBallCount], ennemi->position, directionToPlayer, ENNEMIS);
        projectiles->fireBallCount++;
    }
}

void DestroyEnnemi(int *score, Projectiles *projectiles, Ennemi *ennemi, Ennemies *ennemies)
{
    DeathAttack(ennemi, ennemies, projectiles);
    ennemi->life = false;
    free(ennemi->collider.basePoint);
    free(ennemi->collider.points);
    free(ennemi->collider.triangles);
    SpawnEnnemiChild(*ennemi, &ennemies->ennemiesArray, &ennemies->spawnArray);
    GetPointFromKill(score, ennemi->ennemisType, ennemi->sizeType);
}

void EnnemiesArrayManagement(EnnemiesArray *ennemies)
{
    for (int i = ennemies->ennemisAlive - 1; i >= 0; i--)
    {
        if (ennemies->ennemis[i].life == true)
        {
            break;
        }
        else
        {
            if (ennemies->ennemisAlive > 0)
            {
                ennemies->ennemisAlive--;
            }
        }
    }
}

void UpdateSpawnEnnemi(SpawnStock *spawnArray, EnnemiesArray *ennemiArray, float deltaTime, int *seed, Vector2D *playersPos[], int playerCount, int lvlIndex)
{
    if (spawnArray->spawnCountdown >= spawnArray->spawnTime)
    {
        spawnArray->spawnCountdown = 0;
        if (ennemiArray->mainEnnemiSpawm < SPAWN_POINTS + lvlIndex)
        {
            SpawnEnnemi(spawnArray, ennemiArray, seed, playersPos, playerCount, lvlIndex);
            ennemiArray->mainEnnemiSpawm++;
        }
    }
    else
    {
        spawnArray->spawnCountdown += deltaTime;
    }
}

void SpawnEnnemi(SpawnStock *spawnArray, EnnemiesArray *ennemiArray, int *seed, Vector2D *playersPos[], int playerCount, int lvlIndex)
{

    Ennemi newEnnemi = {0};
    newEnnemi.position = FindValidSpawnPoint(spawnArray, SPAWN_POINTS + lvlIndex);
    newEnnemi.life = true;
    newEnnemi.sizeType = 2;
    newEnnemi.speed = 100.0f;
    *seed = getRandomNumber(*seed);
    newEnnemi.ennemisType = *seed % 4;
    newEnnemi.score = 200.0f;
    newEnnemi.playerCount = playerCount;
    for (int i = 0; i < newEnnemi.playerCount; i++)
    {
        newEnnemi.playerPosition[i] = playersPos[i];
    }
    newEnnemi.direction = GetCloserPlayer(newEnnemi.position, newEnnemi.playerPosition, newEnnemi.playerCount);
    newEnnemi.direction = NormalizeVector2D(newEnnemi.direction);
    InitiateEnnemiCollider(&newEnnemi);

    int index = GetEmptyIndexEnnemyArray(ennemiArray);
    ennemiArray->ennemis[index] = newEnnemi;
}

void InitiateEnnemiCollider(Ennemi *newEnnemi)
{
    switch (newEnnemi->ennemisType)
    {
    case 0:
        SetupEnnemiCollider(&newEnnemi->collider, 6);
        GetFloatingMineCollider(newEnnemi->collider.basePoint, &newEnnemi->collider.size, newEnnemi->sizeType, &newEnnemi->collider.offset);
        break;
    case 1:
        SetupEnnemiCollider(&newEnnemi->collider, 8);
        GetMagneticMineCollider(newEnnemi->collider.basePoint, &newEnnemi->collider.size, newEnnemi->sizeType);
        break;
    case 2:
        SetupEnnemiCollider(&newEnnemi->collider, 12);
        GetMagneticFireballMineCollider(newEnnemi->collider.basePoint, &newEnnemi->collider.size, newEnnemi->sizeType);
        break;
    case 3:
        SetupEnnemiCollider(&newEnnemi->collider, 8);
        GetFireballMineCollider(newEnnemi->collider.basePoint, &newEnnemi->collider.size, newEnnemi->sizeType);
        break;
    case 4:
        SetupEnnemiCollider(&newEnnemi->collider, 9);
        GetMothershipCollider(newEnnemi->collider.basePoint, &newEnnemi->collider.size, newEnnemi->sizeType);
        break;
    }
    newEnnemi->collider.center = AddVector2D(newEnnemi->position, newEnnemi->collider.offset);
    for (int i = 0; i < newEnnemi->collider.pointCount; i++)
    {
        newEnnemi->collider.points[i] = Vector2DQuotientMultiply(newEnnemi->collider.basePoint[i], newEnnemi->collider.size);
        newEnnemi->collider.points[i] = AddVector2D(newEnnemi->collider.center, newEnnemi->collider.points[i]);
    }
    newEnnemi->collider.isConvex = CheckPolygonIsConvex(newEnnemi->collider);
    if (!newEnnemi->collider.isConvex)
    {
        TransformPolygonInTriangle(&newEnnemi->collider);
    }
}

void SpawnEnnemiChild(Ennemi ennemi, EnnemiesArray *ennemiesArray, SpawnStock *spawnArray)
{
    Ennemi ennemiDestroy = ennemi;
    for (int i = 1; i < 3; i++)
    {

        if (ennemi.sizeType > 0)
        {
            Ennemi newEnnemi = {0};
            newEnnemi.position = FindValidSpawnPoint(spawnArray, spawnArray->spawnPointActive);
            newEnnemi.life = true;
            newEnnemi.sizeType = ennemiDestroy.sizeType - 1;
            newEnnemi.speed = MAX_ENNEMIES_SPEED / (ennemiDestroy.sizeType + 1);
            newEnnemi.ennemisType = ennemiDestroy.ennemisType;
            newEnnemi.score = 200.0f;
            newEnnemi.playerCount = ennemiDestroy.playerCount;
            for (int i = 0; i < newEnnemi.playerCount; i++)
            {
                newEnnemi.playerPosition[i] = ennemiDestroy.playerPosition[i];
            }
            newEnnemi.direction = GetCloserPlayer(newEnnemi.position, newEnnemi.playerPosition, newEnnemi.playerCount);
            newEnnemi.direction = NormalizeVector2D(newEnnemi.direction);

            InitiateEnnemiCollider(&newEnnemi);
            int index = GetEmptyIndexEnnemyArray(ennemiesArray);
            ennemiesArray->ennemis[index] = newEnnemi;
        }
    }
}

void SetupEnnemiCollider(Polygon *collider, int point)
{
    collider->pointCount = point;
    collider->basePoint = malloc(collider->pointCount * sizeof(Vector2D)); 
    collider->points = malloc(collider->pointCount * sizeof(Vector2D));
}

int GetEmptyIndexEnnemyArray(EnnemiesArray *ennemiesArray)
{
    int ennemiIndex = -1;
    for (int i = 0; i < ennemiesArray->ennemisAlive; i++)
    {
       
        if (!ennemiesArray->ennemis[i].life)
        {
            ennemiIndex = i;
            break;
        }
    }
  
    if (ennemiIndex == -1)
    {
        ennemiIndex = ennemiesArray->ennemisAlive;
        ennemiesArray->ennemisAlive++;
    }

    
    if (ennemiesArray->ennemisAlive == ennemiesArray->maxCountEnnemi)
    {
      
        ennemiesArray->maxCountEnnemi *= 2;
        ennemiesArray->ennemis = (Ennemi *)realloc(ennemiesArray->ennemis, ennemiesArray->maxCountEnnemi * sizeof(Ennemi));
        
    }

    return ennemiIndex;
}

void UpdateEnnemi(EnnemiesArray *ennemies, float deltaTime)
{
    for (int i = 0; i < ennemies->ennemisAlive; i++)
    {
        if (ennemies->ennemis[i].life)
        {
            if (ennemies->ennemis[i].ennemisType == 2 || ennemies->ennemis[i].ennemisType == 1)
            {
                ennemies->ennemis[i].direction = GetCloserPlayer(ennemies->ennemis[i].position, ennemies->ennemis[i].playerPosition, ennemies->ennemis[i].playerCount);
                ennemies->ennemis[i].direction = NormalizeVector2D(ennemies->ennemis[i].direction);
            }
            Vector2D nextPosition = Vector2DQuotientMultiply(ennemies->ennemis[i].direction, ennemies->ennemis[i].speed * deltaTime);
            ennemies->ennemis[i].position = AddVector2D(ennemies->ennemis[i].position, nextPosition);
            if (!CheckIsInScreen(ennemies->ennemis[i].position))
            {
                ennemies->ennemis[i].position.x = ennemies->ennemis[i].position.x < 55 ? ennemies->ennemis[i].position.x + SCREENWIDTHBORDER : ennemies->ennemis[i].position.x;
                ennemies->ennemis[i].position.y = ennemies->ennemis[i].position.y < 60 ? ennemies->ennemis[i].position.y + SCREENHEIGHTBORDER : ennemies->ennemis[i].position.y;
                ennemies->ennemis[i].position.x = (int)ennemies->ennemis[i].position.x % SCREENWIDTHBORDER;
                ennemies->ennemis[i].position.y = (int)ennemies->ennemis[i].position.y % SCREENHEIGHTBORDER;
            }
        }
    }
}

void UpdateEnnemiCollider(EnnemiesArray *ennemies)
{
    for (int j = 0; j < ennemies->ennemisAlive; j++)
    {
        if (ennemies->ennemis[j].life)
        {

            ennemies->ennemis[j].collider.center = ennemies->ennemis[j].position;
            for (int i = 0; i < ennemies->ennemis[j].collider.pointCount; i++)
            {
                ennemies->ennemis[j].collider.points[i] = Vector2DQuotientMultiply(ennemies->ennemis[j].collider.basePoint[i], ennemies->ennemis[j].collider.size);
                ennemies->ennemis[j].collider.points[i] = AddVector2D(ennemies->ennemis[j].collider.center, ennemies->ennemis[j].collider.points[i]);
            }
            free(ennemies->ennemis[j].collider.triangles);
            TransformPolygonInTriangle(&ennemies->ennemis[j].collider);
        }
    }
}

void DrawEnnemies(Ennemies *ennemies, Texture2D tex)
{
    for (int i = 0; i < ennemies->ennemiesArray.ennemisAlive; i++)
    {
        Ennemi currentEnnemi = ennemies->ennemiesArray.ennemis[i];
        if (currentEnnemi.life)
        {
            // Function to get the good size of the texture
            Texture2D texSized = tex;
            texSized.height /= (2 - (0.5f * currentEnnemi.sizeType));
            texSized.width /= (2 - (0.5f * currentEnnemi.sizeType));
            Rectangle currrentSprite = ennemies->ennemiesArray.spriteEnnemi[currentEnnemi.ennemisType].sprite;
            currrentSprite.y = texSized.height / 2;
            currrentSprite.x = currentEnnemi.ennemisType * texSized.width / 4;
            currrentSprite.width /= (2 - (0.5f * currentEnnemi.sizeType));
            currrentSprite.height /= (2 - (0.5f * currentEnnemi.sizeType));

            DrawTextureRec(texSized, currrentSprite, (Vector2){currentEnnemi.position.x - (currrentSprite.width + 2) / 2, currentEnnemi.position.y - (currrentSprite.height + 1) / 2}, WHITE);
        }
    }
}

void DrawEnnemiCollider(EnnemiesArray *ennemies, bool activeCollider)
{
    if (activeCollider)
    {
        for (int j = 0; j < ennemies->ennemisAlive; j++)
        {
            if (ennemies->ennemis[j].life)
            {
                DrawLine(ennemies->ennemis[j].position.x, ennemies->ennemis[j].position.y,ennemies->ennemis[j].position.x + ennemies->ennemis[j].direction.x *45, ennemies->ennemis[j].position.y +ennemies->ennemis[j].direction.y*45, YELLOW);
                for (int i = 0; i < ennemies->ennemis[j].collider.pointCount; i++)
                {
                    if (i == ennemies->ennemis[j].collider.pointCount - 1)
                    {
                        DrawLine(ennemies->ennemis[j].collider.points[i].x, ennemies->ennemis[j].collider.points[i].y, ennemies->ennemis[j].collider.points[0].x, ennemies->ennemis[j].collider.points[0].y, GREEN);
                    }
                    else
                    {
                        DrawLine(ennemies->ennemis[j].collider.points[i].x, ennemies->ennemis[j].collider.points[i].y, ennemies->ennemis[j].collider.points[i + 1].x, ennemies->ennemis[j].collider.points[i + 1].y, GREEN);
                    }
                }
            }
        }
    }
}

Vector2D FindValidSpawnPoint(SpawnStock* spawnArray, int maxSpawn)
{
    int spawnPointIndex = -1;
    Vector2D spawnPointPos = (Vector2D){-1, 0};

    for (int i = 0; i < maxSpawn; i++)
    {
        if (spawnArray->spawnPointBox[i].isActive == true)
        {
            spawnPointIndex = i;
            spawnArray->spawnPointBox[i].isActive = false;
            break;
        }
    }

    for (int i = spawnArray->spawnPointActive - 1; i >= 0; i--)
    {
        if (spawnArray->spawnPointBox[i].isActive == true)
        {
            break;
        }
        else
        {
            spawnArray->spawnPointActive--;
        }
    }

    spawnPointPos = spawnArray->spawnPointBox[spawnPointIndex].position;
    return spawnPointPos;
}

void DrawSpawnPoint(SpawnStock *spawnPoint, Texture2D tex)
{
    tex.width /= 2;
    tex.height /= 2;
    for (int i = 0; i < spawnPoint->spawnPointActive; i++)
    {
        if (spawnPoint->spawnPointBox[i].isActive)
        {
            Texture2D texSpawn = tex;
            spawnPoint->spawnSprite.sprite.x = texSpawn.width / 4;
            spawnPoint->spawnSprite.sprite.y = 0;
            spawnPoint->spawnSprite.sprite.width = texSpawn.width / 4;
            spawnPoint->spawnSprite.sprite.height = texSpawn.height / 2;

            DrawTextureRec(tex, spawnPoint->spawnSprite.sprite, (Vector2){spawnPoint->spawnPointBox[i].position.x - spawnPoint->spawnSprite.sprite.width / 2, spawnPoint->spawnPointBox[i].position.y - spawnPoint->spawnSprite.sprite.height / 2}, WHITE);
        }
    }
}

void EnnemiMemoryFree(Ennemies *ennemies)
{
    for (int i = 0; i < ennemies->ennemiesArray.ennemisAlive; i++)
    {
        if (ennemies->ennemiesArray.ennemis[i].life)
        {
            free(ennemies->ennemiesArray.ennemis[i].collider.basePoint);
            free(ennemies->ennemiesArray.ennemis[i].collider.points);
            free(ennemies->ennemiesArray.ennemis[i].collider.triangles);
        }
    }

    free(ennemies->spawnArray.spawnPointBox);
    free(ennemies->ennemiesArray.ennemis);
}

Vector2D GetCloserPlayer(Vector2D ennemiPos, Vector2D *playerPos[], int indexDir)
{
    float minDistance = 10000;
    Vector2D directionToPlayer = {0, 0};
    for (int i = 0; i < indexDir; i++)
    {
        float magnitude = GetVectorMagnitude(GetDirection2D(ennemiPos, *playerPos[i]));
        if (minDistance > magnitude)
        {
            directionToPlayer = GetDirection2D(ennemiPos, *playerPos[i]);
            minDistance = magnitude;
        }
    }
    return directionToPlayer;
}