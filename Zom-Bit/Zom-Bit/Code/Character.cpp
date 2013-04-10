
#include "Character.h"

Character::Character(): cSprite() 			// Default constructor
{
	currentDir = 4;
	mActive = true;
	mNoFrames = 3;
	mCurrentFrame = 0;
	mRate = 1.0f/25.0f;
	SetRect(&mSourceRect,0,0,100,100);
}

Character::Character(int startPos): cSprite()  // Constructor
{ 
	//make the sprite ready for annimations and store the start position and direction of the sprite
	currentDir = 4;
	currentPos = startPos;
	mActive = true;
	mNoFrames = 3;
	mCurrentFrame = 0;
	mRate = 1.0f/25.0f;
	SetRect(&mSourceRect,0,0,100,100);
}

void Character::Annimate(){ //annimate the sprite
	SetRect(&mSourceRect,mCurrentFrame*100,0,(mCurrentFrame*100)+100,100);
	cSprite::update();
	mCurrentFrame++;
	if(mCurrentFrame > mNoFrames) //when finished set to inactive so it can be cleaned up
	{
		mCurrentFrame = 0;
		mActive = false;
		OutputDebugString("mActive = False");
	}
}
int Character::GetGridPos(){ //gets the current grid psotion of the player
	return currentPos;
}
void Character::SetGridPos(int position){ //sets a new grid position of the player
	currentPos = position;
}
int Character::GetMoveDirection(){ //returns the direction the player is moving in
	return currentDir;
}
void Character::SetMoveDirection(int dir){ //sets a new movement direction
	currentDir = dir;
}
/*
=================
- Return the current segment of the sprite sheet to draw.
=================
*/
RECT Character::getSourceRect() 					// Return the current segment of the sprite sheet to draw.
{
	return mSourceRect;
}
/*
=================
- Return the current frame.
=================
*/
int Character::getCurrentFrame() 					// Return the current frame.
{
	return mCurrentFrame;
}
/*
=================
- Set the sprite to active.
=================
*/
void Character::setActive(bool sActive) 			// Set the sprite to active.
{
	mActive = sActive;
}
/*
=================
- Determine if the sprite is active.
=================
*/
bool Character::isActive() 						// Determine if the sprite is active.
{
	return mActive;
}
