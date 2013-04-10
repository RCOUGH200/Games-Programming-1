/*
=================
Projectile.h
- Header file for class definition - SPECIFICATION
- Header file for the Sprite class
=================
*/
#ifndef _PROJECTILE_H
#define _PROJECTILE_H
#include "cSprite.h"

class Projectile : public cSprite
{
private:
	int direction;

public:
	Projectile();
	Projectile(int gotDirection);
	int GetMoveDirection();
};
#endif