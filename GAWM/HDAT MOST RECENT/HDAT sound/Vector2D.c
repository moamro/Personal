/*-------------------------------------------------------------------------- -
Project Title         :        CS 230: Project 2 Part 1
File Name             :        Vector2D.c
Author                :        Shawn Masters
Creation Date         :        1/19/16
Purpose               :        Library of functions for 2D vectors.
History
- 1/19/16             :        Added header file.
- 1/19/16             :        Added body for Vector2DZero.
- 1/19/16             :        Added body for Vector2DSet.
- 1/19/16             :        Added body for Vector2DNeg.
- 1/19/16             :        Added body for Vector2DAdd.
- 1/19/16             :        Added body for Vector2DSub.
- 1/19/16             :        Added body for Vector2DNormalize.
- 1/19/16             :        Added body for Vector2DScale.
- 1/19/16             :        Added body for Vector2DScaleAdd.
- 1/19/16             :        Added body for Vector2DScaleSub.
- 1/19/16             :        Added body for Vector2DLength.
- 1/19/16             :        Added body for Vector2DSquareLength.
- 1/19/16             :        Added body for Vector2DDistance.
- 1/19/16             :        Added body for Vector2DSquareDistance.
- 1/19/16             :        Added body for Vector2DDotProduct.
- 1/19/16             :        Added body for Vector2DFromAngleDeg.
- 1/19/16             :        Added body for Vector2DFromAngleRad.
© Copyright 1996 - 2016, DigiPen Institute of Technology(USA).All rights reserved.
----------------------------------------------------------------------------*/
#include "Vector2D.h"
#include <math.h>

#define EPSILON 0.0001
#define PI      3.1415926535897932384626433832795

// ---------------------------------------------------------------------------

void Vector2DZero(Vector2D *pResult)
{
	pResult->x = 0.0f;
	pResult->y = 0.0f;
}

// ---------------------------------------------------------------------------

void Vector2DSet(Vector2D *pResult, float x, float y)
{
	pResult->x = x;
	pResult->y = y;
}

// ---------------------------------------------------------------------------

void Vector2DNeg(Vector2D *pResult, Vector2D *pVec0)
{
	pResult->x = (pVec0->x) * -1;
	pResult->y = (pVec0->y) * -1;
}

// ---------------------------------------------------------------------------

void Vector2DAdd(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1)
{
	pResult->x = (pVec0->x) + (pVec1->x);
	pResult->y = (pVec0->y) + (pVec1->y);
}

// ---------------------------------------------------------------------------

void Vector2DSub(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1)
{
	pResult->x = (pVec0->x) - (pVec1->x);
	pResult->y = (pVec0->y) - (pVec1->y);
}

// ---------------------------------------------------------------------------

void Vector2DNormalize(Vector2D *pResult, Vector2D *pVec0)
{
	float pVec0Length;

	pVec0Length = sqrt(((pVec0->x) * (pVec0->x)) + ((pVec0->y) * (pVec0->y)));

	pResult->x = (1 / pVec0Length) * (pVec0->x);
	pResult->y = (1 / pVec0Length) * (pVec0->y);
}

// ---------------------------------------------------------------------------

void Vector2DScale(Vector2D *pResult, Vector2D *pVec0, float c)
{
	pResult->x = (pVec0->x) * c;
	pResult->y = (pVec0->y) * c;
}

// ---------------------------------------------------------------------------

void Vector2DScaleAdd(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1, float c)
{
	pResult->x = ((pVec0->x) * c) + (pVec1->x);
	pResult->y = ((pVec0->y) * c) + (pVec1->y);
}

// ---------------------------------------------------------------------------

void Vector2DScaleSub(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1, float c)
{
	pResult->x = ((pVec0->x) * c) - (pVec1->x);
	pResult->y = ((pVec0->y) * c) - (pVec1->y);
}

// ---------------------------------------------------------------------------

float Vector2DLength(Vector2D *pVec0)
{
	return sqrt(((pVec0->x) * (pVec0->x)) + ((pVec0->y) * (pVec0->y)));
}

// ---------------------------------------------------------------------------

float Vector2DSquareLength(Vector2D *pVec0)
{
	return ((pVec0->x) * (pVec0->x)) + ((pVec0->y) * (pVec0->y));
}

// ---------------------------------------------------------------------------

float Vector2DDistance(Vector2D *pVec0, Vector2D *pVec1)
{
	float xVal = ((pVec0->x) - (pVec1->x)) * ((pVec0->x) - (pVec1->x));
	float yVal = ((pVec0->y) - (pVec1->y)) * ((pVec0->y) - (pVec1->y));
	return sqrt(xVal + yVal);
}

// ---------------------------------------------------------------------------

float Vector2DSquareDistance(Vector2D *pVec0, Vector2D *pVec1)
{
	float xVal = ((pVec0->x) - (pVec1->x)) * ((pVec0->x) - (pVec1->x));
	float yVal = ((pVec0->y) - (pVec1->y)) * ((pVec0->y) - (pVec1->y));
	return (xVal + yVal);
}

// ---------------------------------------------------------------------------

float Vector2DDotProduct(Vector2D *pVec0, Vector2D *pVec1)
{
	float xVal = ((pVec0->x) * (pVec1->x));
	float yVal = ((pVec0->y) * (pVec1->y));
	return (xVal + yVal);
}

// ---------------------------------------------------------------------------

void Vector2DFromAngleDeg(Vector2D *pResult, float angle)
{
	pResult->x = cos(angle);
	pResult->y = sin(angle);
}

// ---------------------------------------------------------------------------

void Vector2DFromAngleRad(Vector2D *pResult, float angle)
{
	pResult->x = (cos(angle) * PI) / 180;
	pResult->y = (sin(angle) * PI) / 180;
}

// ---------------------------------------------------------------------------

float AngleFromVector(Vector2D* pVec)
{
	// Normalized bulletVector
	Vector2D normal;
	float angle;

	// Find the angle
	angle = acosf(pVec->x);

	// Set the normal
	Vector2DSet(&normal, 0.0f, 1.0f);

	if (Vector2DDotProduct(&pVec, &normal) < 0)
		angle = -angle;

	return angle;
}