#pragma once
#include <raylib.h>
#include <../../src/ennemies.h>
#include <../../src/player.h>
#include  <../../src/system.h>



typedef struct Game
{
    Player players[2];
    int playerCount;
    Texture2D elementTex;
    float deltaTime;
    Projectiles projectiles;
    Texture2D background;
    Texture2D foreground;
    int32_t seed;
    GameState gameState;
    Ennemies Ennemies;
    bool windowOpen, activeCollision;
    int levelindex;
    float preLauchTime;
    float preLauchCountdown;
    bool pressTouch;
    int score;
} Game;

void GameInit(Game *game);

void NewGameInit(Game * game);

void UpdateAndDraw(Game *game);

void GameUnload(Game *game);

void ResetGameMemory(Game* game);