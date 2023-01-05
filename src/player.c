#include <raylib.h>
#include <stdlib.h>
#include <time.h>

#include <../../src/player.h>
#include <../../src/mathLibrary.h>
#include <../../src/collider.h>
#include <../../src/ennemies.h>
#include <../../src/menu.h>

void PlayerInit(Player *player, int index, Texture2D tex, int32_t *seed);

void DrawCollider(Player *player, bool activeCollider);

void PlayerUnload(Player *player);

int GetRandomIndexPlayer(int playerNumber, int32_t *seed);

void PlayerInputDebug(bool *activeCollider);

void PlayerInputPropulsion(PlayerSpeed *playerSpeed, Vector2D direction, float deltaTime, KeyScheme playerKeyScheme);

void PlayerInputRotation(SpriteRenderer *playerSprite, Vector2D *direction, int angularSpeed, float deltaTime, KeyScheme playerKeyScheme);

void PlayerInputShot(Player player, Projectiles *projectiles);

int FindFreeShotIndex(Projectiles *projectiles);

void PlayerInputTeleport(Player *player, int *seed);

void UpdatePlayerSpeed(PlayerSpeed *playerSpeed);

void UpdatePlayerPosition(Vector2D *position, Vector2D *velocity, float deltaTime, int life);

void UpdatePlayerSprite(Rectangle *spritePosition, Vector2D position);

void UpdatePlayerCollider(Polygon *playerCollider, AABB *boxCollider, Vector2D position, float rotation);

void UpdatePlayerInvulnerability(float *invulnerability, float deltaTime);

void PlayerHit(Player *player, int32_t *seed);

void DrawPlayer(Player *player, Texture elementTex);

void UpdatePlayerCollision(Player *player, EnnemiesArray *ennemiesArray, Projectiles *projectiles, int *seed);

void UpdatePlayerInput(Player *player, float deltaTime, Projectiles *projectiles, int32_t *seed);

void UpdatePlayerStats(Player *player, Projectiles *projectiles, float deltaTime, int32_t *seed);

void PlayerInit(Player *player, int index, Texture2D tex, int32_t *seed)
{
    // Player State
    player->life = 3;
    player->playerIndex = index;
    player->keyScheme = playerScheme[index];

    // Player Mouvement Variable

    *seed = getRandomNumber(*seed);
    int num = 45 + *seed % SCREENWIDTHBORDER;
    *seed = getRandomNumber(*seed);
    int num2 = 50 + *seed % SCREENHEIGHTBORDER;
    player->position = (Vector2D){num, num2};
    player->direction = (Vector2D){1.0f, 0.0f};
    player->speed.velocity = (Vector2D){0.0f, 0.0f};
    player->speed.speedMax = 300.0f;
    player->speed.currentSpeed = 0.0f;
    player->speed.accelerationTime = 1;
    player->speed.deccelerationTime = 1;
    player->speed.angularSpeed = 120;
    player->playerSprite.rotationSprite = 0;
    player->invulnerabilityTime = 1.0f;

    // Player sprite
    player->playerSprite.sprite = (Rectangle){0.0f, 0.0f, tex.width / 4, tex.height / 2};
    player->playerSprite.spritePosition = (Rectangle){SCREENWIDTH / 2, SCREENHEIGHT / 2, player->playerSprite.sprite.width, player->playerSprite.sprite.height};
    player->playerSprite.spritePivot = (Vector2D){player->playerSprite.sprite.width / 2, player->playerSprite.sprite.height / 2};

    // Player Collision
    player->ColliderPlayer.center.x = player->position.x;
    player->ColliderPlayer.center.y = player->position.y;
    player->ColliderPlayer.offset = (Vector2D){13, -1.5f};
    player->ColliderPlayer.size = 25;
    player->ColliderPlayer.pointCount = 3;
    player->ColliderPlayer.basePoint = malloc(3 * sizeof(Vector2D));
    player->ColliderPlayer.points = malloc(3 * sizeof(Vector2D));
    GetPlayerCollider(player->ColliderPlayer.basePoint, 3);

    for (int i = 0; i < player->ColliderPlayer.pointCount; i++)
    {
        player->ColliderPlayer.points[i] = Vector2DQuotientMultiply(player->ColliderPlayer.basePoint[i], player->ColliderPlayer.size);
        RotateVector(&player->ColliderPlayer.points[i], 90);
        player->ColliderPlayer.points[i] = AddVector2D(player->ColliderPlayer.center, player->ColliderPlayer.points[i]);
    }
    player->ColliderPlayer.isConvex = CheckPolygonIsConvex(player->ColliderPlayer);
}

void DrawCollider(Player *player, bool activeCollider)
{
    if (activeCollider)
    {

        for (int i = 0; i < player->ColliderPlayer.pointCount; i++)
        {
            DrawLine(player->position.x, player->position.y, player->position.x + player->direction.x * 55, player->position.y + player->direction.y * 55, YELLOW);

            if (i == player->ColliderPlayer.pointCount - 1)
            {
                DrawLine(player->ColliderPlayer.points[i].x, player->ColliderPlayer.points[i].y, player->ColliderPlayer.points[0].x, player->ColliderPlayer.points[0].y, RED);
            }
            else
            {
                DrawLine(player->ColliderPlayer.points[i].x, player->ColliderPlayer.points[i].y, player->ColliderPlayer.points[i + 1].x, player->ColliderPlayer.points[i + 1].y, BLUE);
            }
        }
    }
}

void PlayerUnload(Player *player)
{
    free(player->ColliderPlayer.basePoint);
    free(player->ColliderPlayer.points);
}

int GetRandomIndexPlayer(int playerNumber, int32_t *seed)
{
    *seed = getRandomNumber(*seed);
    return *seed % playerNumber;
}

void PlayerInputDebug(bool *activeCollider)
{
    if (IsKeyPressed(KEY_C))
    {
        *activeCollider = !*activeCollider;
    }
}

void PlayerInputPropulsion(PlayerSpeed *playerSpeed, Vector2D direction, float deltaTime, KeyScheme playerKeyScheme)
{
    if (IsKeyDown(playerKeyScheme.propulsion))
    {
        playerSpeed->currentSpeed += playerSpeed->speedMax * (deltaTime / playerSpeed->accelerationTime);

        Vector2D addVelocity = Vector2DQuotientDivide(Vector2DQuotientMultiply(direction, playerSpeed->currentSpeed), 10);
        playerSpeed->velocity = AddVector2D(playerSpeed->velocity, addVelocity);
    }
    else
    {
        playerSpeed->currentSpeed -= playerSpeed->speedMax * (deltaTime / playerSpeed->deccelerationTime);
    }
}

void PlayerInputRotation(SpriteRenderer *playerSprite, Vector2D *direction, int angularSpeed, float deltaTime, KeyScheme playerKeyScheme)
{

    if (IsKeyDown(playerKeyScheme.rotationLeft))
    {
        playerSprite->rotationSprite -= angularSpeed * deltaTime;
        RotateVector(direction, -angularSpeed * deltaTime);
    }

    if (IsKeyDown(playerKeyScheme.rotationRight))
    {
        playerSprite->rotationSprite += angularSpeed * deltaTime;
        RotateVector(direction, angularSpeed * deltaTime);
    }
    *direction = NormalizeVector2D(*direction);
}

void PlayerInputShot(Player player, Projectiles *projectiles)
{
    if (IsKeyPressed(player.keyScheme.shoot))
    {
        int index = FindFreeShotIndex(projectiles);
        SpawnFireBall(&projectiles->fireballArray[index], player.position, player.direction, player.playerIndex);

        if (projectiles->fireBallCount == projectiles->maxFireballCount)
        {
            projectiles->maxFireballCount *= 2;
            projectiles->fireballArray = realloc(projectiles->fireballArray, projectiles->maxFireballCount * sizeof(FireBall));
        }
    }
}

void PlayerInputTeleport(Player *player, int *seed)
{
    if (IsKeyPressed(player->keyScheme.teleport1) || IsKeyPressed(player->keyScheme.teleport2))
    {
        *seed = getRandomNumber(*seed);
        int num = 45 + *seed % SCREENWIDTHBORDER;
        int num2 = 50 + *seed % SCREENHEIGHTBORDER;
        player->position = (Vector2D){num, num2};
    }
}

void UpdatePlayerSpeed(PlayerSpeed *playerSpeed)
{
    playerSpeed->currentSpeed = ClampFloat(playerSpeed->currentSpeed, 0.0f, playerSpeed->speedMax);
    playerSpeed->velocity = NormalizeVector2D(playerSpeed->velocity);
    playerSpeed->velocity = Vector2DQuotientMultiply(playerSpeed->velocity, playerSpeed->currentSpeed);
}

void UpdatePlayerPosition(Vector2D *position, Vector2D *velocity, float deltaTime, int life)
{
    if (life>0)
    {

        position->x += velocity->x * deltaTime;
        position->y += velocity->y * deltaTime;
        if (!CheckIsInScreen(*position))
        {
            position->x = position->x < 55 ? position->x + SCREENWIDTHBORDER : position->x;
            position->y = position->y < 60 ? position->y + SCREENHEIGHTBORDER : position->y;
            position->x = (int)position->x % SCREENWIDTHBORDER;
            position->y = (int)position->y % SCREENHEIGHTBORDER;
        }
    }
    else
    {
        position->x += -10000;
        position->y += -10000;
    }
}

void UpdatePlayerSprite(Rectangle *spritePosition, Vector2D position)
{
    spritePosition->x = position.x;
    spritePosition->y = position.y;
}

void UpdatePlayerCollider(Polygon *playerCollider, AABB *boxCollider, Vector2D position, float rotation)
{

    Vector2D rotoff = (Vector2D){playerCollider->offset.x, playerCollider->offset.y};
    RotateVector(&rotoff, rotation);
    playerCollider->center.x = position.x + rotoff.x;
    playerCollider->center.y = position.y + rotoff.y;

    for (int i = 0; i < playerCollider->pointCount; i++)
    {
        playerCollider->points[i] = Vector2DQuotientMultiply(playerCollider->basePoint[i], playerCollider->size);
        RotateVector(&playerCollider->points[i], 90 + rotation);
        playerCollider->points[i] = AddVector2D(playerCollider->center, playerCollider->points[i]);
    }
}

void UpdatePlayerInvulnerability(float *invulnerability, float deltaTime)
{
    if (*invulnerability > 0)
    {
        *invulnerability -= deltaTime;
    }
}

void PlayerHit(Player *player, int32_t *seed)
{
    player->life--;
    *seed = getRandomNumber(*seed);
    player->position.x = *seed % SCREENWIDTHBORDER;
    *seed = getRandomNumber(*seed);
    player->position.y = *seed % SCREENHEIGHTBORDER;
    player->invulnerabilityTime = 1.0f;
}

void DrawPlayer(Player *player, Texture elementTex)
{
    DrawTexturePro(elementTex, player->playerSprite.sprite, player->playerSprite.spritePosition, (Vector2){player->playerSprite.spritePivot.x, player->playerSprite.spritePivot.y}, player->playerSprite.rotationSprite + 90, player->playerIndex == 1 ? RED : GREEN);
}

void UpdatePlayerCollision(Player *player, EnnemiesArray *ennemiesArray, Projectiles *projectiles, int *seed)
{

    for (int i = 0; i < projectiles->fireBallCount; i++)
    {
        if (projectiles->fireballArray[i].isActive && projectiles->fireballArray[i].shooter != player->playerIndex)
        {
            if (SATCollisionTwoConvex(player->ColliderPlayer, projectiles->fireballArray[i].collider))
            {
                FireballHit(&projectiles->fireballArray[i], projectiles);
                if (player->invulnerabilityTime < 0)
                {
                    PlayerHit(player, seed);
                    return;
                }
            }
        }
    }

    for (int i = 0; i < ennemiesArray->ennemisAlive; i++)
    {
        if (ennemiesArray->ennemis[i].life)
        {
            if (SATCollisionMix(player->ColliderPlayer, ennemiesArray->ennemis[i].collider))
            {
                if (player->invulnerabilityTime < 0)
                {
                    PlayerHit(player, seed);
                    return;
                }
            }
        }
    }
}

void UpdatePlayerInput(Player *player, float deltaTime, Projectiles *projectiles, int32_t *seed)
{
    PlayerInputPropulsion(&player->speed, player->direction, deltaTime, player->keyScheme);
    PlayerInputRotation(&player->playerSprite, &player->direction, player->speed.angularSpeed, deltaTime, player->keyScheme);
    PlayerInputShot(*player, projectiles);
    PlayerInputTeleport(player, seed);
}

void UpdatePlayerStats(Player *player, Projectiles *projectiles, float deltaTime, int32_t *seed)
{
    if (player->life > 0)
    {

        UpdatePlayerInput(player, deltaTime, projectiles, seed);
        UpdatePlayerSpeed(&player->speed);
    }

    UpdatePlayerPosition(&player->position, &player->speed.velocity, deltaTime,player->life);
    UpdatePlayerSprite(&player->playerSprite.spritePosition, player->position);
    UpdatePlayerCollider(&player->ColliderPlayer, &player->boxCollider, player->position, player->playerSprite.rotationSprite);
    UpdatePlayerInvulnerability(&player->invulnerabilityTime, deltaTime);
}