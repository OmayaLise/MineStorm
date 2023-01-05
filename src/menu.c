#include <raylib.h>
#include <stdbool.h>
#include <../../src/game.h>

// Players life
void PlayersLife(Player player, Texture2D elementTex, float rectWidth, float rectHeight)
{
    elementTex.height /= 3;
    elementTex.width /= 3;
    player.playerSprite.sprite = (Rectangle){0, 0, elementTex.width / 4, elementTex.height / 2};
    for (int i = 0; i < player.life; i++)
    {
        DrawTextureRec(elementTex, player.playerSprite.sprite, (Vector2){rectWidth - (player.playerSprite.sprite.width / 1.5f + player.playerSprite.sprite.width / 2) + player.playerSprite.sprite.width / 1.5 * i, rectHeight - player.playerSprite.sprite.height / 2}, WHITE);
    }
}

// Players rectangle
void PlayersRectangle(Game game, Texture2D elementTex)
{

    // Player one life
    DrawRectangle(SCREENWIDTH / 8, SCREENHEIGHT / 9, 125, 50, GREEN);
    DrawText("Player One", SCREENWIDTH / 6 - 3, SCREENHEIGHT / 9 + 4, 15, BLACK);
    PlayersLife(game.players[0], game.elementTex, SCREENWIDTH / 8 + 125 / 2, SCREENHEIGHT / 9 + 50 / 4 + 50 / 2);

    // Player two life
    DrawRectangle(SCREENWIDTH / 1.5, SCREENHEIGHT / 9, 125, 50, RED);
    DrawText("Player Two", SCREENWIDTH / 1.42, SCREENHEIGHT / 9, 15, BLACK);
    PlayersLife(game.players[1], game.elementTex, SCREENWIDTH / 1.5 + 125 / 2, SCREENHEIGHT / 9 + 50 / 4 + 50 / 2);
}

// Player keys
void PlayersKeys()
{
    // Player one keys
    DrawText("Rotate : 'D' and 'G'", SCREENWIDTH / 8, SCREENHEIGHT / 5.5f, 15, WHITE);
    DrawText("Move : 'R'", SCREENWIDTH / 8, SCREENHEIGHT / 5.5f + 15, 15, WHITE);
    DrawText("Shoot : 'F'", SCREENWIDTH / 8, SCREENHEIGHT / 5.5f + 30, 15, WHITE);
    DrawText("Teleport : 'E' or 'I'", SCREENWIDTH / 8, SCREENHEIGHT / 5.5f + 45, 15, WHITE);

    // Player two keys
    DrawText("Rotate : 'J' and 'L'", SCREENWIDTH / 1.5, SCREENHEIGHT / 5.5f, 15, WHITE);
    DrawText("Move : 'I'", SCREENWIDTH / 1.5, SCREENHEIGHT / 5.5f + 15, 15, WHITE);
    DrawText("Shoot : 'K'", SCREENWIDTH / 1.5, SCREENHEIGHT / 5.5f + 30, 15, WHITE);
    DrawText("Teleport : 'U' or 'O'", SCREENWIDTH / 1.5, SCREENHEIGHT / 5.5f + 45, 15, WHITE);
}

// Player score
void GetPointFromKill(int *scorePlayer, EnnemiType EnnemiType, EnnemiSize EnnemiSize)
{

    if (EnnemiType == FLOATINGMINE)
    {
        *scorePlayer += 100;
        if (EnnemiSize == Medium)
            *scorePlayer += 35;
        if (EnnemiSize == Small)
            *scorePlayer += 100;
    }

    if (EnnemiType == MAGNETICMINE)
    {
        *scorePlayer += 325;
        if (EnnemiSize == Medium)
            *scorePlayer += 35;
        if (EnnemiSize == Small)
            *scorePlayer += 100;
    }

    if (EnnemiType == MAGNETICFIREBALLMINE)
    {
        *scorePlayer += 500;
        if (EnnemiSize == Medium)
            *scorePlayer += 35;
        if (EnnemiSize == Small)
            *scorePlayer += 100;
    }

    if (EnnemiType == FIREBALLMINE)
    {
        *scorePlayer += 750;
        if (EnnemiSize == Medium)
            *scorePlayer += 35;
        if (EnnemiSize == Small)
            *scorePlayer += 100;
    }

    if (EnnemiType == MOTHERSHIP)
    {
        *scorePlayer += 1000;
    }
}

void AddScore(int *score, int gain)
{
    *score += gain;
}

// Menu screen
GameState MenuScreen(Game *game, GameState gamestate, bool *isOpen, int *playerNumber)
{
    if (gamestate == GS_Menu)
    {
        PlayersKeys();

        DrawText("Do you want to play ?", SCREENWIDTH / 4, SCREENHEIGHT / 2.5, 30, WHITE);
        DrawText("Press 'F' :       Solo mode", SCREENWIDTH / 3.5, SCREENHEIGHT / 1.8, 20, BLUE);
        DrawText("Press 'K' :       Two players mode", SCREENWIDTH / 3.5, SCREENHEIGHT / 1.7, 20, GREEN);
        DrawText("Press 'Q' :       Exit", SCREENWIDTH / 3.5, SCREENHEIGHT / 1.6, 20, RED);

        // Launch solo mode
        if (IsKeyPressed(KEY_F))
        {
            *playerNumber = 1;
            NewGameInit(game);
            gamestate = GS_PreLaunch;
        }

        // Launch two player mode
        if (IsKeyPressed(KEY_K))
        {
            *playerNumber = 2;
            NewGameInit(game);
            gamestate = GS_PreLaunch;
        }

        // Exit game
        if (IsKeyPressed(KEY_Q))
        {
            *isOpen = false;
        }
    }
    return gamestate;
}

// Pause screen
GameState PauseScreen(Game* game)
{
    bool isPress = false;
    if (game->gameState == GS_Pause)
    {
        PlayersKeys();

        if (IsKeyPressed(KEY_SPACE) &&  !game->pressTouch)
        {
            game->gameState = GS_Play;
            game->pressTouch = true;
        }

        DrawText("Pause", SCREENWIDTH / 2.3, SCREENHEIGHT / 2.5, 30, WHITE);
        DrawText("Press 'Space' :    Resume", SCREENWIDTH / 4, SCREENHEIGHT / 1.5 + 45, 20, BLUE);
        DrawText("Press 'M' :       Back to main menu", SCREENWIDTH / 4, SCREENHEIGHT / 1.7, 20, RED);
        if (IsKeyPressed(KEY_M))
        {
            game->gameState = GS_Menu;
            ResetGameMemory(game);
        }
        if (IsKeyReleased(KEY_SPACE) && game->pressTouch)
        {
            game->pressTouch = false;
        }
    }
    if (game->gameState == GS_Play)
    {
        if (IsKeyPressed(KEY_SPACE) && ! game->pressTouch)
        {
            game->gameState = GS_Pause;
            game->pressTouch = true;
        }

        if (IsKeyReleased(KEY_SPACE) && game->pressTouch)
        {
            game->pressTouch = false;
        }
    
    }

    return game->gameState;
}

// Game Over screen
GameState GameOverScreen(Game* game)
{

    if (game->gameState != GS_GameOver)
    {
        int lifeCount = 0;
        for (int i = 0; i < game->playerCount; i++)
        {
            if (game->players[i].life > 0)
            {
                lifeCount += game->players[i].life;
                continue;
            }
        }

        if (lifeCount == 0)
        {
            game->gameState = GS_GameOver;
        }
    }
    if (game->gameState == GS_GameOver)
    {

        PlayersKeys();

        DrawText("GameOver", SCREENWIDTH / 2.5, SCREENHEIGHT / 2.5, 30, WHITE);
        DrawText(TextFormat("Score : %d", game->score), SCREENWIDTH / 3, SCREENHEIGHT / 2.25, 25, GREEN);
        DrawText("Press 'M' :       Back to menu", SCREENWIDTH / 3.9, SCREENHEIGHT / 1.8, 20, RED);
        if (IsKeyPressed(KEY_M))
        {
            game->gameState = GS_Menu;
            //ResetGameMemory(game);
        }
    }
    return game->gameState;
}

void DrawScorePoints(Game *game)
{
    DrawText(TextFormat("Score : %d",  game->score), SCREENWIDTH / 3, SCREENHEIGHT / 1 - 80, 25, WHITE);
}
