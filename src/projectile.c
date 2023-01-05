
#include <raylib.h>
#include <../../src/projectile.h>

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

void ProjectileMemoryFree(Projectiles* projectiles);

void ProjectilesInit(Projectiles *projectiles, Texture2D tex)
{
    projectiles->maxFireballCount = START_MAX_PROJECTILE;
    projectiles->fireBallCount = 0;
    projectiles->fireballArray = malloc(projectiles->maxFireballCount * sizeof(FireBall));
    projectiles->spriteProjectiles.sprite = (Rectangle){((tex.width / 4) * 3), 0.0f, tex.width / 4, tex.height / 2};
}

void FireballHit(FireBall *fireBall, Projectiles *projectiles)
{
    fireBall->isActive = false;
    free(fireBall->collider.points);
    ProjectileArrayManagement(projectiles);
}

void ProjectileArrayManagement(Projectiles *projectiles)
{
    for (int i = projectiles->fireBallCount - 1; i >= 0; i--)
    {
        if (projectiles->fireballArray[i].isActive == true)
        {
            break;
        }
        else
        {
            if (projectiles->fireBallCount > 0)
            {
                projectiles->fireBallCount--;
            }
        }
    }
}

void SpawnFireBall(FireBall *newShoot, Vector2D position, Vector2D direction, Entities shooter)
{

    newShoot->position = position;
    newShoot->direction = direction;
    newShoot->shooter = shooter;
    newShoot->nature = PROJECTILE;
    newShoot->speed = SHOOT_SPEED;
    newShoot->isActive = true;

    newShoot->collider.center = newShoot->position;
    newShoot->collider.points = malloc(15 * sizeof(Vector2D));
    newShoot->collider.size = 6.0f;
    GetCircleCollider(&newShoot->collider, 15);
}

void UpdateFireBall(FireBall *fireBall, float deltaTime)
{
    fireBall->position = AddVector2D(fireBall->position, Vector2DQuotientMultiply(fireBall->direction, fireBall->speed * deltaTime));
    fireBall->collider.center = fireBall->position;
    UpdateCircleCollider(&fireBall->collider, fireBall->collider.pointCount);
}

void UpdateProjectiles(Projectiles *projectiles, float deltaTime)
{
    for (int i = 0; i < projectiles->fireBallCount; i++)
    {
        FireBall *shot = &projectiles->fireballArray[i];
        if (shot->isActive)
        {

            UpdateFireBall(&projectiles->fireballArray[i], deltaTime);

            if (shot->position.x > SCREENWIDTH || shot->position.x < 0)
            {
                FireballHit(&projectiles->fireballArray[i], projectiles);
                continue;
            }
            if (shot->position.y > SCREENHEIGHT || shot->position.y < 0)
            {
                FireballHit(&projectiles->fireballArray[i], projectiles);
                continue;
            }
        }
        ProjectileArrayManagement(projectiles);
    }
}


void UpdateProjectileCollision(Projectiles *projectiles, int *score)
{
    for (int i = 0; i < projectiles->fireBallCount; i++)
    {
        if (projectiles->fireballArray[i].isActive)
        {
            FireBall *shot = &projectiles->fireballArray[i];
            for (int j = 0; j < projectiles->fireBallCount; j++)
            {
                if (shot != &projectiles->fireballArray[j])
                {
                    if (projectiles->fireballArray[j].isActive)
                    {
                        if (SATCollisionTwoConvex(shot->collider, projectiles->fireballArray[j].collider))
                        {
                            if (shot->shooter != ENNEMIS && projectiles->fireballArray[j].shooter == ENNEMIS)
                            {
                                AddScore(score, 110);
                            }
                            FireballHit(shot, projectiles);
                            FireballHit(&projectiles->fireballArray[j], projectiles);
                        }
                    }
                }
            }
        }
    }
}


int FindFreeShotIndex(Projectiles *projectiles)
{
    int index = -1;
    for (index = 0; index < projectiles->fireBallCount; index++)
    {
        if (!projectiles->fireballArray[index].isActive)
        {
            return index;
        }
    }
    projectiles->fireBallCount++;
    return projectiles->fireBallCount - 1;
}


void DrawProjectiles(Projectiles *projectiles, Texture2D elementTex)
{
    for (int i = 0; i < projectiles->fireBallCount; i++)
    {
        if (projectiles->fireballArray[i].isActive)
        {
            DrawTextureRec(elementTex, projectiles->spriteProjectiles.sprite, (Vector2){projectiles->fireballArray[i].position.x - (projectiles->spriteProjectiles.sprite.width / 2) - 1, projectiles->fireballArray[i].position.y - (projectiles->spriteProjectiles.sprite.height / 2) - 1}, YELLOW);
        }
    }
}

void DrawProjectileCollider(Projectiles *projectiles, bool activeCollider)
{
    if (activeCollider)
    {

        for (int j = 0; j < projectiles->fireBallCount; j++)
        {
            if (projectiles->fireballArray[j].isActive)
            {
                DrawLine(projectiles->fireballArray[j].position.x, projectiles->fireballArray[j].position.y ,projectiles->fireballArray[j].position.x + projectiles->fireballArray[j].direction.x *25, projectiles->fireballArray[j].position.y + projectiles->fireballArray[j].direction.y *25, YELLOW);

                for (int i = 0; i < projectiles->fireballArray[j].collider.pointCount; i++)
                {
                    if (i == projectiles->fireballArray[j].collider.pointCount - 1)
                    {
                        DrawLine(projectiles->fireballArray[j].collider.points[i].x, projectiles->fireballArray[j].collider.points[i].y, projectiles->fireballArray[j].collider.points[0].x, projectiles->fireballArray[j].collider.points[0].y, GREEN);
                    }
                    else
                    {
                        DrawLine(projectiles->fireballArray[j].collider.points[i].x, projectiles->fireballArray[j].collider.points[i].y, projectiles->fireballArray[j].collider.points[i + 1].x, projectiles->fireballArray[j].collider.points[i + 1].y, GREEN);
                    }
                }
            }
        }
    }
}


bool CheckIsInScreen(Vector2D position)
{

    if (position.x > SCREENWIDTHBORDER + 45 || position.x < 45 || position.y > SCREENHEIGHTBORDER + 50 || position.y < 50)
        return false;
    else
        return true;
}

void ProjectileMemoryFree(Projectiles* projectiles)
{
    free(projectiles->fireballArray);
}