#pragma once
#include <raylib.h>
#include <../../src/mathLibrary.h>
#include <../../src/sprite.h>

#define SHOOT_SPEED 200.0f
#define SPAWN_POINTS 1
#define MAX_ENNEMIES_SPEED 150

#define SCREENWIDTH 640
#define SCREENHEIGHT 800
#define SCREENWIDTHBORDER 550
#define SCREENHEIGHTBORDER 700


typedef struct  KeyScheme
{
    KeyboardKey propulsion , rotationLeft, rotationRight, shoot,teleport1,teleport2 ;

}KeyScheme;

static KeyScheme playerScheme[2] =
{
    {KEY_R,KEY_D,KEY_G,KEY_F,KEY_T,KEY_E},
    {KEY_I,KEY_J,KEY_L,KEY_K,KEY_O,KEY_U }
} ;



typedef enum GameState
{
    GS_Menu,
    GS_PreLaunch,
    GS_Play,
    GS_Pause,
    GS_GameOver,
    GS_Victory
}GameState;


typedef enum Entities
{
    PLAYER1,
    PLAYER2,
    ENNEMIS,
    PROJECTILE
} Entities;

