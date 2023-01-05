#pragma once
#include <stdbool.h>

#include <../../src/projectile.h>

typedef enum EnnemiSize
{
    Small,
    Medium,
    Large,

}EnnemiSize;

typedef  enum EnnemiType
{
    FLOATINGMINE,
    MAGNETICMINE,
    MAGNETICFIREBALLMINE,
    FIREBALLMINE,
    MOTHERSHIP,
}EnnemiType;

typedef struct Ennemi
{
    Vector2D position;
    EnnemiSize sizeType;
    int size, score, speed;
    Vector2D direction;
    bool life;
    EnnemiType ennemisType;
    SpriteRenderer spriteRenderer;
    Vector2D* playerPosition[2];
    int playerCount;
    Polygon collider;
    AABB boxCollider;
    
} Ennemi;

typedef struct  EnnemiesArray
{
    Ennemi* ennemis;
    int maxCountEnnemi;
    int ennemisAlive;
    SpriteRenderer spriteEnnemi[5];
    int mainEnnemiSpawm;

}EnnemiesArray;

typedef struct SpawnPoint
{
    Vector2D position;
    bool isActive;
} SpawnPoint;

typedef struct SpawnStock
{
    SpriteRenderer spawnSprite;
    int ennemiCountMax;
    int spawnPointActive;
    SpawnPoint* spawnPointBox;
    float spawnCountdown;
    float spawnTime;

}SpawnStock;

typedef struct Ennemies
{
    SpawnStock spawnArray;
    EnnemiesArray ennemiesArray;

    
}Ennemies;

// Get random position
Vector2D GetRandomPosition(int *seed);

// Get valid position with random position
Vector2D GetValidPosition(int *seed, int ennemiCountMax, SpawnStock Spawny, int rayon);

// Get spawn points with valid positions 
void GetSpawnPoint(SpawnStock *SpawnStocky, int *seed, int ennemiCountMax, int rayon);

void EnnemiesInit(int lvlIndex, Ennemies *ennemies, Texture2D tex);

void GetSpawnPoint(SpawnStock *SpawnStocky, int *seed, int ennemiCountMax, int rayon);

void DrawSpawnPoint(SpawnStock *spawnPoint, Texture2D tex);

void UpdateEnnemiStats(Ennemies *ennemies, int32_t *seed, float deltaTime, Vector2D* playersPos[],int playerCount , int lvlIndex);

void UpdateEnnemiCollision(int*score,Ennemies* ennemies, EnnemiesArray *ennemiesArray, Projectiles *projectiles);

void EnnemiesArrayAlive(Ennemies *ennemies,int *lvlIndex, GameState* gameState,float* prelaunchCountdown,Texture2D tex);

void DrawEnnemiesAndCollider(Ennemies *ennemies,bool activeColliderShow, Texture2D tex);

void EnnemiMemoryFree(Ennemies* ennemies);