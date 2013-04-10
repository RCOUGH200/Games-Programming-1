/*
=================
Character.h
- Header file for class definition - SPECIFICATION
- Header file for the Sprite class
=================
*/
#ifndef _CHARACTER_H
#define _CHARACTER_H
#include "cSprite.h"

class Character : public cSprite
{
private:
	bool mActive;
	int mNoFrames;
	int mCurrentFrame;
	RECT mSourceRect;
	float mRate;
	int currentPos;
	int currentDir;

public:
	Character();
	Character(int startPos);
	void update(float deltaTime);					// update the balloons position based on the translation
	void SetMoveDirection(int dir);
	int GetMoveDirection();
	void Annimate();
	int GetGridPos();
	void SetGridPos(int position);
	RECT getSourceRect();					// Return the current segment of the sprite sheet to draw.
	int getCurrentFrame();					// Return the current frame.
	void setActive(bool sActive);			// Set the sprite to active.
	bool isActive();	
};
#endif