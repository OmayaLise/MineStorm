#pragma once
#include <raylib.h>
#include <../../src/mathLibrary.h>
#include <../../src/system.h>
#include <../../src/collider.h>

#define START_MAX_PROJECTILE 15

typedef struct FireBall
{
    Vector2D position;
    Vector2D direction;
    float speed;
    Entities shooter;
    Entities nature;
    Polygon collider;
    bool isActive;
    AABB boxCollider;

}FireBall;

typedef struct  Projectiles
{
    FireBall* fireballArray;
    int fireBallCount, maxFireballCount;
    SpriteRenderer spriteProjectiles;
    
}Projectiles;


void ProjectilesInit(Projectiles *projectiles, Texture2D tex);

void ProjectileArrayManagement(Projectiles *projectiles);

void FireballHit(FireBall *fireBall, Projectiles *projectiles);

void SpawnFireBall(FireBall *newShoot, Vector2D position, Vector2D direction, Entities shooter);

void UpdateFireBall(FireBall *fireBall, float deltaTime);

void UpdateProjectiles(Projectiles *projectiles, float deltaTime);

void UpdateProjectileCollision(Projectiles *projectiles, int *score);

int FindFreeShotIndex(Projectiles *projectiles);

void DestroyProjectile(FireBall *projectile);

void DrawProjectiles(Projectiles *projectiles, Texture2D elementTex);

void DrawProjectileCollider(Projectiles *projectiles, bool activeCollider);

bool CheckIsInScreen(Vector2D position);

void ProjectileMemoryFree(Projectiles* projectiles);

