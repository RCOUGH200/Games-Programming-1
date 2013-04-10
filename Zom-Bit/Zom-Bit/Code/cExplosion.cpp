/*
=================
cExplosion.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cExplosion.h"
/*
=================
- Data constructor initializes the cBalloon to the data passed to 
- the constructor from the paramater sPosition.
- Is always called, thus ensures all sprite objects are in a consistent state.
=================
*/
cExplosion::cExplosion(D3DXVECTOR3 sPosition, cD3DXTexture* aTexture) 			// Default constructor
{
	mActive = true;
	mNoFrames = 6;
	mCurrentFrame = 0;
	cSprite::setTexture(aTexture);
	cSprite::setSpritePos(sPosition);
	SetRect(&mSourceRect,0,0,cExplosion::getSTWidth(),cExplosion::getSTHeight());
}

cExplosion::cExplosion(): cSprite() 			// Default constructor
{
	mActive = true;
	mNoFrames = 6;
	mCurrentFrame = 0;
}

cExplosion::cExplosion(D3DXVECTOR3 sPosition, LPDIRECT3DDEVICE9 pd3dDevice, LPCSTR theFilename): cSprite(sPosition,pd3dDevice,theFilename)  // Constructor
{
	mActive = true;
	mNoFrames = 6;
	mCurrentFrame = 0;
	SetRect(&mSourceRect,0,0,cExplosion::getSTWidth(),cExplosion::getSTHeight());
}
/*
=================
- update the position etc of the sprite.
=================
*/
void cExplosion::Annimate() 					// update the balloons position based on the translation
{
	SetRect(&mSourceRect,mCurrentFrame*100,0,(mCurrentFrame*100)+100,100);//All sprites that annimate are 100x100
	cSprite::update();
	mCurrentFrame++;
	if(mCurrentFrame > mNoFrames)
	{
		mCurrentFrame = 0;
		mActive = false;
		OutputDebugString("mActive = False");
	}
}
/*
=================
- Return the current segment of the sprite sheet to draw.
=================
*/
RECT cExplosion::getSourceRect() 					// Return the current segment of the sprite sheet to draw.
{
	return mSourceRect;
}
/*
=================
- Return the current frame.
=================
*/
int cExplosion::getCurrentFrame() 					// Return the current frame.
{
	return mCurrentFrame;
}
/*
=================
- Set the sprite to active.
=================
*/
void cExplosion::setActive(bool sActive) 			// Set the sprite to active.
{
	mActive = sActive;
}
/*
=================
- Determine if the sprite is active.
=================
*/
bool cExplosion::isActive() 						// Determine if the sprite is active.
{
	return mActive;
}
