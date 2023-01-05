#include <../../src/collider.h>
#include <../../src/mathLibrary.h>
#include <stdlib.h>

static Vector2D playerCollider[] =
    {
        {-0.35f, 0.0f}, {0.0f, -1.0f}, {0.35f, 0.0f}};

static Vector2D floatingMine[] =
    {
        {-0.33f, 0.0f},
        {0.0f, -1.0f},
        {0.33f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 0.75f},
        {-1.0f, 1.0f},
};

static Vector2D fireballMine[] =
    {
        {-0.75, 0.0f}, {-1.0f, -1.0f}, {0.0f, -0.75f}, {1.00f, -1.00f}, {0.75f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.75f}, {-1.00f, 1.00f}};

static Vector2D magneticMine[] =
    {
        {-0.5, 0.0f}, {-1.0f, -1.0f}, {0.0f, -0.5f}, {1.00f, -1.00f}, {0.5f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.5f}, {-1.00f, 1.00f}};

static Vector2D magneticFireballMine[] =
    {
        {-0.63f, -0.63f}, {-0.63f, -1.0f}, {-0.5f, -0.63f}, {0.63f, -0.63f}, {1.0f, -0.63f}, {0.63f, -0.5f}, {0.63f, 0.63f}, {0.63f, 1.00f}, {0.5f, 0.63f}, {-0.63f, 0.63f}, {-1.0f, 0.63f}, {-0.63f, 0.5f}};

static Vector2D mothership[] = {
    {0.0f,-0.75f}, {0.25f,-0.5f}, {0.80f,-0.5f}, {1.0f,0.0f}, {0.80f,-0.25f}, {-0.80f,-0.25f}, {-1.0f,0.0f}, {-0.80f,-0.5f}, {-0.25f,-0.5f}};

void GetPlayerCollider(Vector2D *array, int length)
{
    for (int i = 0; i < length; i++)
    {
        array[i] = playerCollider[i];
    }
}

void GetFloatingMineCollider(Vector2D *array, float *sizeCollider, int ennemyType, Vector2D *offset)
{
    *sizeCollider = (26.5f/ (2-(0.5f* ennemyType)));
    *sizeCollider = *sizeCollider == 0 ? (25.5f/ 4.0f): *sizeCollider;
    *offset = (Vector2D){-1.0f, 1.0f};


    for (int i = 0; i < 6; i++)
    {
        array[i] = floatingMine[i];
    }
}

void GetFireballMineCollider(Vector2D *array, float *sizeCollider, int ennemyType)
{
    *sizeCollider = (25.5f / (2-(0.5f* ennemyType)));
    *sizeCollider = *sizeCollider == 0 ? (25.5f/ 4.0f): *sizeCollider;
  
    for (int i = 0; i < 8; i++)
    {
        array[i] = fireballMine[i];
    }
}

void GetMagneticMineCollider(Vector2D *array, float *sizeCollider, int ennemyType)
{
    *sizeCollider = (25.5f/ (2-(0.5f* ennemyType)));
    *sizeCollider = *sizeCollider == 0 ? (25.5f/ 4.0f): *sizeCollider;

    for (int i = 0; i < 8; i++)
    {
        array[i] = magneticMine[i];
    }
}

void GetMagneticFireballMineCollider(Vector2D *array, float *sizeCollider, int ennemyType)
{
    *sizeCollider = (35.5f/ (2-(0.5f* ennemyType)));
    *sizeCollider = *sizeCollider == 0 ? (25.5f/ 4.0f): *sizeCollider;
    for (int i = 0; i < 12; i++)
    {
        array[i] = magneticFireballMine[i];
    }
}

void GetMothershipCollider(Vector2D * array,float *sizeCollider, int ennemyType)
{ 
    *sizeCollider = (25.5f / (2-(0.5f* ennemyType)));
    *sizeCollider = *sizeCollider == 0 ? (25.5f/ 4.0f): *sizeCollider;
  
    for (int i = 0; i < 9; i++)
    {
        array[i] = magneticFireballMine[i];
    }
}

void GetCircleCollider(Polygon *shape, int pointNumber)
{
    shape->pointCount = pointNumber;

    Vector2D dirStart = {1.0f, 0.0f};
    for (int i = 0; i < shape->pointCount; i++)
    {
        RotateVector(&dirStart, 360.0f / shape->pointCount);
        dirStart =  NormalizeVector2D(dirStart);
        shape->points[i].x = shape->center.x + dirStart.x * shape->size;
        shape->points[i].y = shape->center.y + dirStart.y * shape->size;
    }
    shape->isConvex = CheckPolygonIsConvex(*shape);
    if(!shape->isConvex )
    {
        TransformPolygonInTriangle(shape);
    }
}

void UpdateCircleCollider(Polygon* shape, int pointNumber)
{
    Vector2D dirStart = {1.0f, 0.0f};
    for (int i = 0; i < shape->pointCount; i++)
    {
        RotateVector(&dirStart, 360.0f / shape->pointCount);
        shape->points[i].x = shape->center.x + (dirStart.x * shape->size);
        shape->points[i].y = shape->center.y + (dirStart.y * shape->size);
    }
}