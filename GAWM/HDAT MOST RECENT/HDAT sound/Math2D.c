/*-------------------------------------------------------------------------- -
Project Title         :        CS 230: Project 2 Part 1
File Name             :        Math2D.c
Author                :        Shawn Masters
Creation Date         :        1/19/16
Purpose               :        Library of Collision functions.
History
- 1/19/16             :        Added header file.
- 1/19/16             :        Added body for StaticPointToStaticCircle.
- 1/19/16             :        Added body for StaticPointToStaticRect.
- 1/19/16             :        Added body for StaticCircleToStaticCircle.
- 1/19/16             :        Added body for StaticRectToStaticRect.
© Copyright 1996 - 2016, DigiPen Institute of Technology(USA).All rights reserved.
----------------------------------------------------------------------------*/
#include "Math2D.h"
#include "stdio.h"
#include "Vector2D.h"

/*
This function checks if the point P is colliding with the circle whose
center is "Center" and radius is "Radius"
*/
int StaticPointToStaticCircle(Vector2D *pP, Vector2D *pCenter, float Radius)
{
	if (Vector2DSquareDistance(pP, pCenter) > Radius * Radius)
	{
		return 0;
	}
	return 1;
}


/*
This function checks if the point Pos is colliding with the rectangle
whose center is Rect, width is "Width" and height is Height
*/
int StaticPointToStaticRect(Vector2D *pPos, Vector2D *pRect, float Width, float Height)
{
	float rectLeft = pRect->x - (Width * 0.85f);
	float rectRight = pRect->x + (Width * 0.85f);
	float rectTop = pRect->y + (Height * 0.85f);
	float rectBottom = pRect->y - (Height * 0.85f);

	if (pPos->x <= rectRight && pPos->x >= rectLeft)
	{
		if (pPos->y <= rectTop && pPos->y >= rectBottom)
		{
			return 1;
		}
	}
	return 0;
}

/*
This function checks for collision between 2 circles.
Circle0: Center is Center0, radius is "Radius0"
Circle1: Center is Center1, radius is "Radius1"
*/
int StaticCircleToStaticCircle(Vector2D *pCenter0, float Radius0, Vector2D *pCenter1, float Radius1)
{
	if (Vector2DSquareDistance(pCenter0, pCenter1) > (Radius0 + Radius1) * (Radius0 + Radius1))
	{
		return 0;
	}
	return 1;
}

/*
This functions checks if 2 rectangles are colliding
Rectangle0: Center is pRect0, width is "Width0" and height is "Height0"
Rectangle1: Center is pRect1, width is "Width1" and height is "Height1"
*/
int StaticRectToStaticRect(Vector2D *pRect0, float Width0, float Height0, Vector2D *pRect1, float Width1, float Height1)
{
	float rect0Left = pRect0->x - (Width0 * 0.85f);
	float rect0Right = pRect0->x + (Width0 * 0.85f);
	float rect0Top = pRect0->y + (Height0 * 0.85f);
	float rect0Bottom = pRect0->y - (Height0 * 0.85f);
	float rect1Left = pRect1->x - (Width1 * 0.85f);
	float rect1Right = pRect1->x + (Width1 * 0.85f);
	float rect1Top = pRect1->y + (Height1 * 0.85f);
	float rect1Bottom = pRect1->y - (Height1 * 0.85f);

	if (rect0Left > rect1Right || rect1Left > rect0Right || rect0Top < rect1Bottom || rect1Top < rect0Bottom)
	{
		return 0;
	}
	return 1;
}
