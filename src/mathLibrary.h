#pragma once
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
// --------------- Struct --------------

typedef struct Vector2D
{
    float x, y;

} Vector2D;

typedef struct Vector3D
{
    float x, y, z;

} Vector3D;

typedef struct AABB
{
    float x, y, width, height;
} AABB;

typedef struct Circle
{
    Vector2D center;
    float radius;

} Circle;

typedef struct Triangle
{
    Vector2D points[3];

} Triangle;

typedef struct Polygon
{
    Vector2D center;
    Vector2D offset;
    Vector2D sizePoly;
    float size;
    int pointCount;
    Vector2D *points;
    Vector2D* basePoint;
    bool isConvex;
    int trianglesCount;
    Triangle *triangles;

} Polygon;

typedef struct AxisArray
{
    Vector2D *axis;
    int axisCount;
} AxisArray;

typedef struct Projection
{
    float min, max;
} Projection;



// Get a random number
static int32_t getRandomNumber(int32_t state)
{
    uint64_t x = (uint64_t)state;
    int m = pow(2,31);
    int a = 1103515245;
    int c = 12345;
    uint64_t result = (a * x + c) % m ; // La formule

    return result;
}

// --------------- Vector 2D Functions --------------

// Add a Vector2DD with a another Vector2DD
Vector2D AddVector2D(Vector2D a, Vector2D b);

// Substract a Vector2DD with a another Vector2DD
Vector2D SubstractVector2D(Vector2D a, Vector2D b);

// Get the direction of vector with two point
Vector2D GetDirection2D( Vector2D PosA, Vector2D PosB);

// Normaliser a vector 2
Vector2D NormalizeVector2D(Vector2D vectorNor);

// Dot Product of two Vector2D
float DotProduct(Vector2D a, Vector2D b);

//  Get distance of a vector
float GetVectorMagnitude(Vector2D a);

// Get the determinant of a product of vector 2D
float Get2DDeterminant(Vector2D a, Vector2D b);

// Multiply a Vector2D by a number
Vector2D Vector2DQuotientMultiply(Vector2D a, float multiplier);

// Divide a Vector2D by a number
Vector2D Vector2DQuotientDivide(Vector2D a, float divider);

// Compare two vectors
bool VectorCompare(Vector2D a, Vector2D b);

bool VectorCompareCircle(Vector2D a, Vector2D b, int rayon);

// --------- Math algebric function ------------------

// Make transition between the start value and the end value
float LerpFloat(float start, float end, float t);

//Limit the value of a variable between a max and a min
float ClampFloat(float number, float min,float max);

// ----------- Rotation Vector 2D ---------

// Get the angle of vector in degree
float GetDegreAngle(Vector2D a);

// Rotate a vector
void RotateVector(Vector2D *direction, float angle);

// --------- Polygon 2D function -----------

// Test to find a polygon is convex 
bool CheckPolygonIsConvex(Polygon shape);

// Transfom a non-convex polygon in a array of triangles
void TransformPolygonInTriangle(Polygon *polygon);

// --------------- Vector 3D Functions --------------

// Multiplication of Vector3D
Vector3D CrossProduct(Vector3D a, Vector3D b);

// -----------  Collision Function ------------

// Check collision between AABB and point
bool CollisionAABBPoint(AABB aabb, Vector2D P);

//Check collision between two AABB
bool CollisionAABBtoAABB(AABB box1, AABB box2);

// Check collision between a circle and a point
bool CollisionCirclePoint(Circle circle, Vector2D point);

// Check collision between two circle
bool CollisionCircletoCircle(Circle circle1, Circle circle2);

// Check collision between AABB and circle
bool CollisionCircleToAABB(Circle circle, AABB box);

// SAT with two convex polygons
bool SATCollisionTwoConvex(Polygon shape1, Polygon shape2);

// SAT with convex polygon and non-convex polygon
bool SATCollisionMix(Polygon convex, Polygon nonConvex);

// SAT with two non-convex polygons
bool SATCollisionTwoNonConvex(Polygon shape1, Polygon shape2);