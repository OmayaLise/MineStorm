#pragma once
#include <raylib.h>
#include <../../src/mathLibrary.h>

typedef struct SpriteRenderer
{
    Rectangle sprite;
    Rectangle spritePosition;
    Vector2D spritePivot;
    float rotationSprite;

} SpriteRenderer;