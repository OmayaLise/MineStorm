#include <raylib.h>
#include <stdlib.h>
#include <time.h>

#include <../../src/game.h>
#include <../../src/player.h>
#include <../../src/menu.h>

void LoadAgentTexture(Texture2D *tex);
void LoadBackgroundTexture(Texture2D *background, Texture2D *foreground);

void GameInit(Game *game)
{
    time_t now = time(NULL);
    struct tm *tm_struct = localtime(&now);
    game->seed = tm_struct->tm_sec * tm_struct->tm_min * tm_struct->tm_hour;
    LoadBackgroundTexture(&game->background, &game->foreground);

    game->gameState = GS_Menu;
    game->windowOpen = true;
    game->activeCollision = false;
}

void NewGameInit(Game *game)
{
    game->levelindex = 0;
    game->preLauchTime = 1.0f;
    game->preLauchCountdown = 0.0f;
    LoadAgentTexture(&game->elementTex);
    for (int i = 0; i < game->playerCount; i++)
    {
        PlayerInit(&game->players[i], i, game->elementTex,&game->seed);
    }

    ProjectilesInit(&game->projectiles, game->elementTex);
    EnnemiesInit(game->levelindex, &game->Ennemies, game->elementTex);
}

void LoadAgentTexture(Texture2D *tex)
{
    *tex = LoadTexture("assets/minestorm_sprite_atlas_mine_storm.png");
    tex->width /= 2;
    tex->height /= 2;
}

void LoadBackgroundTexture(Texture2D *background, Texture2D *foreground)
{
    *background = LoadTexture("assets/minestorm_background.png");
    background->width *= 1;
    background->height *= 1;
    *foreground = LoadTexture("assets/minestorm_foreground.png");
    foreground->width *= 1;
    foreground->height *= 1;
}

//--------- Game Update and draw -------

void UpdateResetScene(Game *game);

void DrawDebugInfo(Player *player);

void DrawDebugGameInfo(Game *game);

void DrawGameForground(Game *game);

void DrawGameBackground(Game *game);

void UpdateAndDraw(Game *game)
{
    BeginDrawing();

    game->deltaTime = GetFrameTime();

    if (game->gameState == GS_PreLaunch)
    {
        if (game->preLauchCountdown > game->preLauchTime)
        {
            GetSpawnPoint(&game->Ennemies.spawnArray, &game->seed, game->Ennemies.spawnArray.ennemiCountMax, 10);
            game->gameState = GS_Play;
        }
        else
        {
            game->preLauchCountdown += game->deltaTime;
        }
    }

    if (game->gameState == GS_Play)
    {
        for (int i = 0; i < game->playerCount; i++)
        {
            UpdatePlayerStats(&game->players[i], &game->projectiles, game->deltaTime, &game->seed);
        }

        PlayerInputDebug(&game->activeCollision);
        UpdateProjectiles(&game->projectiles, game->deltaTime);

        int indexPlayer = GetRandomIndexPlayer(game->playerCount, &game->seed);
        if (game->playerCount == 1)
        {
            indexPlayer = 0;
        }
        Vector2D* playerPos[2] ;
        for (int i = 0; i < game->playerCount; i++)
        {
            playerPos[i] = &game->players[i].position;
        }
         
        UpdateEnnemiStats(&game->Ennemies, &game->seed, game->deltaTime, playerPos,game->playerCount, game->levelindex);

        for (int i = 0; i < game->playerCount; i++)
        {
            if (game->players[i].life > 0)
            {
                UpdatePlayerCollision(&game->players[i], &game->Ennemies.ennemiesArray, &game->projectiles, &game->seed);
            }
        }

        UpdateEnnemiCollision(&game->score, &game->Ennemies, &game->Ennemies.ennemiesArray, &game->projectiles);
        UpdateProjectileCollision(&game->projectiles, &game->score);
        EnnemiesArrayAlive(&game->Ennemies, &game->levelindex, &game->gameState, &game->preLauchCountdown, game->elementTex);
    }
    ClearBackground(BLACK);
    DrawGameBackground(game);

    if (game->gameState == GS_Play || game->gameState == GS_Pause)
    {
        for (int i = 0; i < game->playerCount; i++)
        {
            if (game->players[i].life > 0)
            {
                DrawPlayer(&game->players[i], game->elementTex);
            }
        }

        DrawProjectiles(&game->projectiles, game->elementTex);

        DrawEnnemiesAndCollider(&game->Ennemies, game->activeCollision, game->elementTex);

        DrawProjectileCollider(&game->projectiles, game->activeCollision);
        DrawSpawnPoint(&game->Ennemies.spawnArray, game->elementTex);
        for (int i = 0; i < game->playerCount; i++)
        {
            DrawCollider(&game->players[i], game->activeCollision);
        }
    }
    if (game->gameState == GS_PreLaunch)
    {
        DrawText(TextFormat("Level %d", game->levelindex + 1), SCREENWIDTH / 2 - 125, SCREENHEIGHT / 2 - 50, 50, RED);
    }

    PlayersRectangle(*game, game->elementTex);
    DrawGameForground(game);
    game->gameState = PauseScreen(game);
    game->gameState = MenuScreen(game, game->gameState, &game->windowOpen, &game->playerCount);
    if (game->gameState == GS_Play || game->gameState == GS_GameOver)
    {
        game->gameState = GameOverScreen(game);
    }
    DrawScorePoints(game);
    EndDrawing();
}

// ----- Update-> Draw function ------

void DrawGameBackground(Game *game)
{
    DrawTexture(game->background, 0, 0, WHITE);
}

void DrawDebugInfo(Player *player)
{
    DrawText(TextFormat("Dir : %f , %f", player->speed.velocity.x, player->speed.velocity.y), 50, 50.0f, 20, WHITE);
    DrawText(TextFormat("Life: %d", player->life), 50, 75.0f, 20, WHITE);
    Vector2D rotoff = (Vector2D){player->ColliderPlayer.offset.x, player->ColliderPlayer.offset.y};
    RotateVector(&rotoff, player->playerSprite.rotationSprite);
    DrawCircle(player->position.x + rotoff.x, player->position.y + rotoff.y, 5, RED);
}

void DrawDebugGameInfo(Game *game)
{
    DrawText(TextFormat("GAME State : %d ", game->gameState), 250, 150.0f, 20, WHITE);
    DrawText(TextFormat("Player Invul : %f ", game->players[0].invulnerabilityTime), 250, 170.0f, 20, WHITE);
    DrawRectangleLines((SCREENWIDTH - (SCREENWIDTHBORDER - 20)) / 2, (SCREENHEIGHT - (SCREENHEIGHTBORDER - 20)) / 2, (SCREENWIDTHBORDER - 20), (SCREENHEIGHTBORDER - 20), PURPLE);
}

void DrawGameForground(Game *game)
{
    DrawTexture(game->foreground, 0, -0, WHITE);
}

//----- Unload Function ----

void ResetGameMemory(Game *game)
{
    EnnemiMemoryFree(&game->Ennemies);
    for (int i = 0; i < game->playerCount; i++)
    {
        PlayerUnload(&game->players[i]);
    }
    ProjectileMemoryFree(&game->projectiles);
}

void GameUnload(Game *game)
{
    UnloadTexture(game->elementTex);
    UnloadTexture(game->background);
    UnloadTexture(game->foreground);
    
}
