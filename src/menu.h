#pragma once

#include <../../src/game.h>

// Shows menu screen if gamestate = GS_Menu 
GameState MenuScreen(Game* game, GameState gamestate, bool* isOpen, int* playerNumber);

// Shows pause screen if space is pressed 
GameState PauseScreen(Game* game);

// Shows gameover screen if player life reaches 0
GameState GameOverScreen(Game* game);

// Shows player rectangle with life
void PlayersRectangle(Game game, Texture2D elementTex);

// Get total point from kills
void GetPointFromKill(int * scorePlayer, EnnemiType EnnemiType, EnnemiSize EnnemiSize);

void AddScore(int* score, int gain);

// Shows total point from kills
void DrawScorePoints (Game * game);
