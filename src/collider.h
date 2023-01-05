#pragma once
#include <../../src/mathLibrary.h>
#include<stdlib.h>

void GetPlayerCollider(Vector2D* array, int length);
void GetFloatingMineCollider(Vector2D* array, float* sizeCollider, int ennemyType, Vector2D* offset);
void GetFireballMineCollider(Vector2D* array, float* sizeCollider, int ennemyType);
void GetMagneticMineCollider(Vector2D* array, float* sizeCollider, int ennemyType);
void GetMagneticFireballMineCollider(Vector2D *array, float *sizeCollider, int ennemyType);
void GetMothershipCollider(Vector2D * array,float *sizeCollider, int ennemyType);
void GetCircleCollider(Polygon * shape,int pointNumber);    
void UpdateCircleCollider(Polygon* shape, int pointNumber);