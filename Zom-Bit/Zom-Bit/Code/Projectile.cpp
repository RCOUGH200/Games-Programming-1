
#include "Projectile.h"

Projectile::Projectile(): cSprite() 			// Default constructor
{
	direction = 0;
}

Projectile::Projectile(int gotDirection): cSprite()  // Constructor
{
	direction = gotDirection; //sets the movement direction of the sprite
}

int Projectile::GetMoveDirection() //gets the movement direction of the sprite
{
	return direction;
}

