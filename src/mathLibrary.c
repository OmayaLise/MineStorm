
/* Library of math function. Library made by Bryan Bachelet && Omaya Lise */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>
#include "mathLibrary.h"

#define M_PI 3.14159265358979323846264338327950288
#define degToRad(angleInDegrees) ((angleInDegrees)*M_PI / 180.0)
#define radToDeg(angleInRadians) ((angleInRadians)*180.0 / M_PI)

// Add a vector2D with a another vector2D
Vector2D AddVector2D(Vector2D a, Vector2D b)
{
    Vector2D result = {0.0f, 0.0f};
    result.x = a.x + b.x;
    result.y = a.y + b.y;

    return result;
}

// Substract a Vector2DD with a another Vector2DD
Vector2D SubstractVector2D(Vector2D a, Vector2D b)
{
    Vector2D result = {0, 0};
    result.x = a.x - b.x;
    result.y = a.y - b.y;

    return result;
}

// Get the direction of vector with two point
Vector2D GetDirection2D(Vector2D PosA, Vector2D PosB)
{
    return SubstractVector2D(PosB, PosA);
}

// Get the magnitude of the vector
float GetVectorMagnitude(Vector2D a)
{
    float magnitude = sqrt((a.x * a.x) + (a.y * a.y));
    if (magnitude < 0)
    {
        magnitude * -1.0f;
    }
    return magnitude;
}

// Normaliser a vector 2
Vector2D NormalizeVector2D(Vector2D vectorNor)
{
    float vectorMagnitude = GetVectorMagnitude(vectorNor);
    Vector2D vectorTransform = {0.0f, 0.0f};
    if (vectorNor.x != 0.0f)
    {
        vectorTransform.x = vectorNor.x / vectorMagnitude;
    }
    if (vectorNor.x != 0.0f)
    {
        vectorTransform.y = vectorNor.y / vectorMagnitude;
    }

    return vectorTransform;
}

// Dot Product of two Vector2D
float DotProduct(Vector2D a, Vector2D b)
{
    //a = NormalizeVector2D(a);
    //b = NormalizeVector2D(b);

    return a.x * b.x + a.y * b.y;
}

// Get the determinant of a product of vector 2D
float Get2DDeterminant(Vector2D a, Vector2D b)
{
    return a.x * b.y - a.y * b.x;
}

// Get angle of degree of a vector
float GetDegreAngle(Vector2D a)
{
    Vector2D unit = {1.0f, 0.0f};
    a = NormalizeVector2D(a);
    float dot = DotProduct(a, unit);

    return radToDeg(acos(dot));
}

// Rotate a vector;
void RotateVector(Vector2D *direction, float angle)
{
    angle = degToRad(angle);
    Vector2D startVector = *direction;
    direction->x = startVector.x * cosf(angle) - (startVector.y * sinf(angle));
    direction->y = startVector.x * sinf(angle) + (startVector.y * cosf(angle));
}

// Multiply a Vector2D by a number
Vector2D Vector2DQuotientMultiply(Vector2D a, float multiplier)
{
    a.x *= multiplier;
    a.y *= multiplier;
    return a;
}

// Divide a Vector2D by a number
Vector2D Vector2DQuotientDivide(Vector2D a, float divider)
{
    a.x = a.x / divider;
    a.y = a.y / divider;
    return a;
}

// Compare two vectors
bool VectorCompare(Vector2D a, Vector2D b)
{
    if (a.x == b.x && a.y == b.y)
        return true;
}

// Compare two vectors + (circle around second vector)
bool VectorCompareCircle(Vector2D a, Vector2D b, int rayon)
{
    Vector2D dir = GetDirection2D(b, a);
    int dirMagnitude = GetVectorMagnitude(dir);
    if (dirMagnitude < rayon)
    {
        return true;
    }
    else
    {
        return false;
    }
}
// ---------- Algebric Function -----------------

// Make transition between the start value and the end value
float LerpFloat(float start, float end, float t)
{
    return ((end - start) * t) + start;
}

//Limit the value of a variable between a max and a min
float ClampFloat(float number, float min, float max)
{
    number = number > max ? max : number;
    number = number < min ? min : number;
    return number;
}

// ----- Collision -------

// Check collision between AABB and point
bool CollisionAABBPoint(AABB box1, Vector2D point)
{
    float box1MaxX = box1.x + box1.width;
    float box1MaxY = box1.y + box1.height;

    if (point.x <= box1.x || point.y <= box1.y)
    {
        return false;
    }
    if (point.x >= box1MaxX || point.y >= box1MaxY)
    {

        return false;
    }
    return true;
}

//Check collision between two AABB
bool CollisionAABBtoAABB(AABB box1, AABB box2)
{
    if (box2.x + box2.width <= box1.x || box2.y + box2.height <= box1.y)
    {
        return false;
    }
    if (box2.x >= box1.x + box1.width || box2.y >= box1.y + box1.height)
    {
        return false;
    }
    return true;
}

// Check collision between a circle and a point
bool CollisionCirclePoint(Circle circle, Vector2D point)
{
    Vector2D dirPointCenter = {0, 0};
    dirPointCenter = GetDirection2D(circle.center, point);
    float dirMagnitude = GetVectorMagnitude(dirPointCenter);
    if (dirMagnitude > circle.radius)
    {
        return false;
    }

    return true;
}

// Check collision between two circle
bool CollisionCircletoCircle(Circle circle1, Circle circle2)
{
    Vector2D dirPointCenter = {0, 0};
    dirPointCenter = GetDirection2D(circle1.center, circle2.center);
    float dirMagnitude = GetVectorMagnitude(dirPointCenter);
    if (dirMagnitude > circle1.radius + circle2.radius)
    {
        return false;
    }

    return true;
}

// Loop the index value if it greater than the max point
int LoopIndexPoint(int pointCount, int index)
{
    return index >= pointCount ? index - pointCount : index;
}

// Test to find a polygon is convex
bool CheckPolygonIsConvex(Polygon shape)
{
    for (int i = 0; i < shape.pointCount; i++)
    {
        int index1 = LoopIndexPoint(shape.pointCount, i + 1);
        int index2 = LoopIndexPoint(shape.pointCount, i + 2);

        Vector2D dir1 = GetDirection2D(shape.points[i], shape.points[index2]);
        Vector2D dir2 = GetDirection2D(shape.points[i], shape.points[index1]);

        float d = Get2DDeterminant(dir1, dir2);

        if (d > 0)
            return false;
    }
    return true;
}

int FindPointUsable(int pointCount, int indexValue, int pointIgnore[])
{
    for (int j = 0; j < pointCount; j++)
    {
        if (indexValue == pointIgnore[j])
        {
            indexValue = LoopIndexPoint(pointCount, indexValue + 1); // Check if we pointe in the array value and return a possible value
            j = 0;
        }
    }
    return LoopIndexPoint(pointCount, indexValue);
}

void SetTriangle(Triangle *triangle, Vector2D point1, Vector2D point2, Vector2D point3)
{
    triangle->points[0] = point1;
    triangle->points[1] = point2;
    triangle->points[2] = point3;
}

void InitializeArrayIntAtValue(int *intArray, int length, int value)
{
    for (int i = 0; i < length; i++)
    {
        intArray[i] = value;
    }
}

// Transfom a non-convex polygon in a array of triangles
void TransformPolygonInTriangle(Polygon *polygon)
{
    Vector2D *pointPoly = polygon->points;
    int pointCountUsable = polygon->pointCount;
    int pointIgnore[polygon->pointCount];

    InitializeArrayIntAtValue(pointIgnore, polygon->pointCount, -1);

    int currentTriangle = 0;
    polygon->triangles = (Triangle *)malloc((polygon->pointCount - 2) * sizeof(Triangle));
    polygon->trianglesCount = polygon->pointCount - 2;

    for (int i = 0; i < polygon->pointCount; i++)
    {
        // If pointCounUsable <= 2, there is no triangle possible to create
        if (pointCountUsable <= 2)
        {
            break;
        }

        // Verifier que le point est utilisable
        i = FindPointUsable(polygon->pointCount, i, pointIgnore);
        int index1 = FindPointUsable(polygon->pointCount, i + 1, pointIgnore);
        int index2 = FindPointUsable(polygon->pointCount, index1 + 1, pointIgnore);

        // Recuperer les deux vecteur directionnels a tester
        Vector2D dir1 = GetDirection2D(pointPoly[i], pointPoly[index2]);
        Vector2D dir2 = GetDirection2D(pointPoly[i], pointPoly[index1]);

        float delta = Get2DDeterminant(dir1, dir2);
        if (delta > 0)
        {
            continue;
        }
        else
        {
            SetTriangle(&polygon->triangles[currentTriangle], pointPoly[i], pointPoly[index1], pointPoly[index2]);
            pointIgnore[currentTriangle] = index1;
            currentTriangle++;
            pointCountUsable--;

            if (currentTriangle > polygon->pointCount - 2 && pointCountUsable > 2)
            {
                polygon->triangles = realloc(polygon->triangles, currentTriangle * 2 * sizeof(Triangle));
                polygon->trianglesCount = currentTriangle * 2;
            }
        }
    }
}

void GetAxis(AxisArray *axisArray, Vector2D *pointsShape, int length)
{
    axisArray->axisCount = 0;

    for (int i = 0; i < length; i++)
    {
        Vector2D dir = GetDirection2D(pointsShape[i], pointsShape[LoopIndexPoint(length, i + 1)]);
        RotateVector(&dir, 90);
        dir = NormalizeVector2D(dir);
        axisArray->axis[i] = dir;
        axisArray->axisCount++;
    }
   
}

Projection ProjectPointOnAxis(Vector2D points[], int pointlenght, Vector2D axis)
{
    Projection projection = {0, 0};
    projection.min = DotProduct(points[0], axis);
    projection.max = projection.min;
    for (int i = 1; i < pointlenght; i++)
    {
        float dot = DotProduct(points[i], axis);

        if (dot < projection.min)
            projection.min = dot;

        if (dot > projection.max)
            projection.max = dot;
    }
    return projection;
}

bool CheckProjectionsOverlap(Projection p1, Projection p2)
{
        if (p2.max > p1.min && p2.min < p1.max)
        return true;
    else
        return false;
}

bool SATConvexCollision(Vector2D *pointsShape1, int pointlength1, Vector2D *pointsShape2, int pointlength2)
{
    // Get axis
    AxisArray axis1;
    AxisArray axis2;

    axis1.axis = malloc(pointlength1 * sizeof(Vector2D));
    axis2.axis = malloc(pointlength2 * sizeof(Vector2D));

    GetAxis(&axis1, pointsShape1, pointlength1);
    GetAxis(&axis2, pointsShape2, pointlength2);

    for (int i = 0; i < axis1.axisCount; i++)
    {
        Vector2D axisProject = axis1.axis[i];
        //Projeter des points
        Projection p1 = ProjectPointOnAxis(pointsShape1, pointlength1, axisProject);
        Projection p2 = ProjectPointOnAxis(pointsShape2, pointlength2, axisProject);

        if (!CheckProjectionsOverlap(p1, p2))
        {
            free(axis1.axis);
            free(axis2.axis);
            return false;
        }
    }

    for (int i = 0; i < axis2.axisCount; i++)
    {
        Vector2D axisProject = axis2.axis[i];
        //Projeter des points
        Projection p1 = ProjectPointOnAxis(pointsShape1, pointlength1, axisProject);
        Projection p2 = ProjectPointOnAxis(pointsShape2, pointlength2, axisProject);

        if (!CheckProjectionsOverlap(p1, p2))
        {
            free(axis1.axis);
            free(axis2.axis);
            return false;
        }
    }

    //Free axis at the end
    free(axis1.axis);
    free(axis2.axis);
    return true;
}

bool SATCollisionTwoConvex(Polygon shape1, Polygon shape2)
{
    bool isCollision = SATConvexCollision(shape1.points, shape1.pointCount, shape2.points, shape2.pointCount);
    if (isCollision)
        return isCollision;
}

bool SATCollisionMix(Polygon convex, Polygon nonConvex)
{

    /// assert(convex.isConvex && "First arg is not convex. Please verify our arguments");
    //// assert(!nonConvex.isConvex && "Second arg is convex. Please verify our arguments");
    for (int i = 0; i < nonConvex.trianglesCount; i++)
    {
        bool isCollision = SATConvexCollision(convex.points, convex.pointCount, nonConvex.triangles[i].points, 3);
        if (isCollision)
            return isCollision;
    }
    return false;
}

bool SATCollisionTwoNonConvex(Polygon shape1, Polygon shape2)
{

    for (int i = 0; i < shape1.trianglesCount; i++)
    {
        for (int j = 0; j < shape2.trianglesCount; j++)
        {
            bool isCollision = SATConvexCollision(shape1.triangles[i].points, 3, shape2.triangles[j].points, 3);
            if (isCollision)
                return isCollision;
        }
    }

    for (int i = 0; i < shape2.trianglesCount; i++)
    {
        for (int j = 0; j < shape1.trianglesCount; j++)
        {
            bool isCollision = SATConvexCollision(shape2.triangles[i].points, 3, shape1.triangles[j].points, 3);
            if (isCollision)
                return isCollision;
        }
    }

    return false;
}
