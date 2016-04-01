/*-------------------------------------------------------------------------- -
Project Title         :        CS 230: Project 2 Part 1
File Name             :        Matrix2D.c
Author                :        Shawn Masters
Creation Date         :        1/19/16
Purpose               :        Library of 2D matrix functions.
History
- 1/19/16             :        Added header file.
- 1/19/16             :        Added body for Matrix2DIdentity.
- 1/19/16             :        Added body for Matrix2DTranspose.
- 1/19/16             :        Added body for Matrix2DConcat.
- 1/19/16             :        Added body for Matrix2DTranslate.
- 1/19/16             :        Added body for Matrix2DScale.
- 1/19/16             :        Added body for Matrix2DRotDeg.
- 1/19/16             :        Added body for Matrix2DRotRad.
- 1/19/16             :        Added body for Matrix2DMultVec.
© Copyright 1996 - 2016, DigiPen Institute of Technology(USA).All rights reserved.
----------------------------------------------------------------------------*/
#include "Matrix2D.h"
#include <math.h>

#define PI      3.1415926535897932384626433832795


/*
This function sets the matrix Result to the identity matrix
*/
void Matrix2DIdentity(Matrix2D *pResult)
{
	pResult->m[0][0] = 1;
	pResult->m[0][1] = 0;
	pResult->m[0][2] = 0;
	pResult->m[1][0] = 0;
	pResult->m[1][1] = 1;
	pResult->m[1][2] = 0;
	pResult->m[2][0] = 0;
	pResult->m[2][1] = 0;
	pResult->m[2][2] = 1;

}

// ---------------------------------------------------------------------------

/*
This functions calculated the transpose matrix of Mtx and saves it in Result
*/
void Matrix2DTranspose(Matrix2D *pResult, Matrix2D *pMtx)
{
	pResult->m[0][0] = pMtx->m[0][0];
	pResult->m[0][1] = pMtx->m[1][0];
	pResult->m[0][2] = pMtx->m[2][0];
	pResult->m[1][0] = pMtx->m[0][1];
	pResult->m[1][1] = pMtx->m[1][1];
	pResult->m[1][2] = pMtx->m[2][1];
	pResult->m[2][0] = pMtx->m[0][2];
	pResult->m[2][1] = pMtx->m[1][2];
	pResult->m[2][2] = pMtx->m[2][2];
}

// ---------------------------------------------------------------------------

/*
This function multiplies Mtx0 with Mtx1 and saves the result in Result
Result = Mtx0*Mtx1
*/
void Matrix2DConcat(Matrix2D *pResult, Matrix2D *pMtx0, Matrix2D *pMtx1)
{
	float m00 = (pMtx0->m[0][0] * pMtx1->m[0][0]) + (pMtx0->m[0][1] * pMtx1->m[1][0]) + (pMtx0->m[0][2] * pMtx1->m[2][0]);
	float m01 = (pMtx0->m[0][0] * pMtx1->m[0][1]) + (pMtx0->m[0][1] * pMtx1->m[1][1]) + (pMtx0->m[0][2] * pMtx1->m[2][1]);
	float m02 = (pMtx0->m[0][0] * pMtx1->m[0][2]) + (pMtx0->m[0][1] * pMtx1->m[1][2]) + (pMtx0->m[0][2] * pMtx1->m[2][2]);
	float m10 = (pMtx0->m[1][0] * pMtx1->m[0][0]) + (pMtx0->m[1][1] * pMtx1->m[1][0]) + (pMtx0->m[1][2] * pMtx1->m[2][0]);
	float m11 = (pMtx0->m[1][0] * pMtx1->m[0][1]) + (pMtx0->m[1][1] * pMtx1->m[1][1]) + (pMtx0->m[1][2] * pMtx1->m[2][1]);
    float m12 = (pMtx0->m[1][0] * pMtx1->m[0][2]) + (pMtx0->m[1][1] * pMtx1->m[1][2]) + (pMtx0->m[1][2] * pMtx1->m[2][2]);
	float m20 = (pMtx0->m[2][0] * pMtx1->m[0][0]) + (pMtx0->m[2][1] * pMtx1->m[1][0]) + (pMtx0->m[2][2] * pMtx1->m[2][0]);
	float m21 = (pMtx0->m[2][0] * pMtx1->m[0][1]) + (pMtx0->m[2][1] * pMtx1->m[1][1]) + (pMtx0->m[2][2] * pMtx1->m[2][1]);
	float m22 = (pMtx0->m[2][0] * pMtx1->m[0][2]) + (pMtx0->m[2][1] * pMtx1->m[1][2]) + (pMtx0->m[2][2] * pMtx1->m[2][2]);
	pResult->m[0][0] = m00;
	pResult->m[0][1] = m01;
	pResult->m[0][2] = m02;
	pResult->m[1][0] = m10;
	pResult->m[1][1] = m11;
	pResult->m[1][2] = m12;
	pResult->m[2][0] = m20;
	pResult->m[2][1] = m21;
	pResult->m[2][2] = m22;
}

// ---------------------------------------------------------------------------

/*
This function creates a translation matrix from x *p y and saves it in Result
*/
void Matrix2DTranslate(Matrix2D *pResult, float x, float y)
{
	pResult->m[0][0] = 1;
	pResult->m[0][1] = 0;
	pResult->m[0][2] = x;
	pResult->m[1][0] = 0;
	pResult->m[1][1] = 1;
	pResult->m[1][2] = y;
	pResult->m[2][0] = 0;
	pResult->m[2][1] = 0;
	pResult->m[2][2] = 1;
}

// ---------------------------------------------------------------------------

/*
This function creates a scaling matrix from x *p y and saves it in Result
*/
void Matrix2DScale(Matrix2D *pResult, float x, float y)
{
	pResult->m[0][0] = x;
	pResult->m[0][1] = 0;
	pResult->m[0][2] = 0;
	pResult->m[1][0] = 0;
	pResult->m[1][1] = y;
	pResult->m[1][2] = 0;
	pResult->m[2][0] = 0;
	pResult->m[2][1] = 0;
	pResult->m[2][2] = 1;
}

// ---------------------------------------------------------------------------

/*
This matrix creates a rotation matrix from "Angle" whose value is in degree.
Save the resultant matrix in Result
*/
void Matrix2DRotDeg(Matrix2D *pResult, float Angle)
{
	pResult->m[0][0] = (float) cos(Angle * (PI / 180));
	pResult->m[0][1] = -1.0f * (float) sin(Angle * (PI / 180));
	pResult->m[0][2] = 0;
	pResult->m[1][0] = (float) sin(Angle * (PI / 180));
	pResult->m[1][1] = (float) cos(Angle * (PI / 180));
	pResult->m[1][2] = 0;
	pResult->m[2][0] = 0;
	pResult->m[2][1] = 0;
	pResult->m[2][2] = 1;
}

// ---------------------------------------------------------------------------

/*
This matrix creates a rotation matrix from "Angle" whose value is in radian.
Save the resultant matrix in Result
*/
void Matrix2DRotRad(Matrix2D *pResult, float Angle)
{
	pResult->m[0][0] = (float) cos(Angle);
	pResult->m[0][1] = -1.0f * (float) sin(Angle);
	pResult->m[0][2] = 0;
	pResult->m[1][0] = (float) sin(Angle);
	pResult->m[1][1] = (float) cos(Angle);
	pResult->m[1][2] = 0;
	pResult->m[2][0] = 0;
	pResult->m[2][1] = 0;
	pResult->m[2][2] = 1;
}

// ---------------------------------------------------------------------------

/*
This function multiplies the matrix Mtx with the vector Vec and saves the result in Result
Result = Mtx * Vec
*/
void Matrix2DMultVec(Vector2D *pResult, Matrix2D *pMtx, Vector2D *pVec)
{
	float m00 = pVec->x * pMtx->m[0][0];
	float m01 = pVec->y * pMtx->m[0][1];
	float m02 = pMtx->m[0][2];
	float m10 = pVec->x * pMtx->m[1][0];
	float m11 = pVec->y * pMtx->m[1][1];
	float m12 = pMtx->m[1][2];
	pResult->x = m00 + m01; + m02;
	pResult->y = m10 + m11; + m12;
}

// ---------------------------------------------------------------------------
