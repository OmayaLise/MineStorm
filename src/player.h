#pragma once

#include  <../../src/ennemies.h>

typedef struct PlayerSpeed
{
    Vector2D velocity;
    float currentSpeed;
    float speedMax;
    float accelerationTime;
    float deccelerationTime;

    int angularSpeed;
     
} PlayerSpeed;

typedef struct Player
{
    int playerIndex;
    Vector2D position;
    Vector2D direction;
    PlayerSpeed speed;
    SpriteRenderer playerSprite;
    Polygon ColliderPlayer;
    int life;
    float invulnerabilityTime;
    AABB boxCollider;
    KeyScheme keyScheme;

} Player;

// Init Player
void PlayerInit(Player *player, int index, Texture2D tex, int32_t *seed);

// Update Player Stats
void UpdatePlayerStats(Player* player, Projectiles* projectiles,float deltaTime,int32_t* seed);

// Update Player Collision
void UpdatePlayerCollision(Player *player, EnnemiesArray *ennemiesArray, Projectiles *projectiles, int *seed);

// Draw player
void DrawPlayer(Player *player, Texture elementTex);

// Draw Collider
void DrawCollider(Player *player, bool activeCollider);

// Unload Function
void PlayerUnload(Player *player);

// Get a random index of players
int GetRandomIndexPlayer(int playerNumber, int32_t *seed);

// Debug
void PlayerInputDebug(bool *activeCollider);