/*
=================
main.cpp
Main entry point for the Card application
=================
*/
#include <cstdlib>
#include "GameConstants.h"
#include "GameResources.h"
#include "cD3DManager.h"
#include "cD3DXSpriteMgr.h"
#include "cD3DXTexture.h"
#include "cSprite.h"
#include "Projectile.h"
#include "Character.h"
#include "cExplosion.h"
#include "cXAudio.h"
#include "cD3DXFont.h"

using namespace std;

HINSTANCE hInst; // global handle to hold the application instance
HWND wndHandle; // global variable to hold the window handle

// Get a reference to the DirectX Manager
static cD3DManager* d3dMgr = cD3DManager::getInstance();

// Get a reference to the DirectX Sprite renderer Manager 
static cD3DXSpriteMgr* d3dxSRMgr = cD3DXSpriteMgr::getInstance();

D3DXVECTOR2 balloonTrans = D3DXVECTOR2(300,300);
vector<Character*> aDummy;
vector<Character*>::iterator diter;
vector<Character*> aZombie;
vector<Character*>::iterator iter;
vector<cSprite*> healthBlock;
vector<cSprite*>::iterator hiter;
vector<Character*>::iterator index;
vector<Projectile*> aBullet;
vector<Projectile*>::iterator piter;
vector<Projectile*> aSlime;
vector<Projectile*>::iterator siter;
D3DXVECTOR3 bulletPos;
D3DXVECTOR3 slimePos;
cSprite play;
cSprite controls;
cSprite main;
RECT clientBounds;
bool gridOcp[64];

TCHAR szTempOutput[30];
int menuNumer = 0;
bool gHit = false;
int zombiesKilled = 0;
char zombiesKilledStr[50];
char roundNumStr[50];
D3DXVECTOR3 expPos;
list<cExplosion*> gExplode;
cXAudio music; //sound effects objects
cXAudio backgroundMusic; //background music object
Character thePlayer(51); //The player object and it starting grid position
int scene = 0; //Current scene
bool movePlayer = false; //Can the player move
//Whoch way the player is moving
bool goleft;
bool goright;
bool goup;
bool goDown;
int direction = 4; // Whoh of the 4 possible directions the player is facing
int playDelay = 0; //Delay between player movements
int bulletNum = -1; //The current bullet
int slimeNum = -1; // The current slime
int shotDelay; // The shotgun delay
int health = 10; //Max health  of the player
int roundNumber = 0; // The current round number
int lastRound = -1; //Holds the previous round number
int killedLastRound; //How many zombies were killed last round
int delay = 50; //The delay used for path calculations and animations
int lastScene = -1; //the last scene loaded
D3DXVECTOR3 playerPos = D3DXVECTOR3(300,600,0); //Initial pos of player
D3DXVECTOR2 playerTrans = D3DXVECTOR2(0,0); // Initital trans of player
// Initial starting position for enemy
D3DXVECTOR3 balloonPos;
//All the games textures
cD3DXTexture* balloonTextures[19];
char* balloonTxtres[] = {"Images\\zLeft.png","Images\\zRight.png","Images\\zUp.png", "Images\\zDown.png","Images\\zombiePop.png", "Images\\healthBlock.png", "Images\\pLeft.png", "Images\\pRight.png", "Images\\pUp.png", "Images\\pDown.png", "Images\\hProjectile.png", "Images\\vProjectile.png", "Images\\zomSlimeL.png", "Images\\zomSlimeR.png", "Images\\zomSlimeU.png", "Images\\zomSlimeD.png", "Images\\play.png", "Images\\controls.png", "Images\\mainMenu.png"};
/*
==================================================================
* LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
* LPARAM lParam)
* The window procedure
==================================================================
*/

void setUpRound(){
	if(roundNumber == 0){
		// Create player
		direction = 4;
		gridOcp[51] = true;
		thePlayer.setTranslation(D3DXVECTOR2(0.0f,0.0f));
		thePlayer.setSpritePos(playerPos);
		thePlayer.setTexture(balloonTextures[9]);
		thePlayer.Annimate();
	}
	//Delays must be reset to stop player and enemies going out of sync
	delay = 50;
	playDelay = 0;
	movePlayer = false;
	int numZombies;
	int currentPos = thePlayer.GetGridPos();
	gridOcp[currentPos] = false;
	gridOcp[51] = true;
	thePlayer.SetGridPos(51);
	killedLastRound = zombiesKilled;
	roundNumber++;
	playerPos = D3DXVECTOR3(300,600,0);;
	numZombies = 8*roundNumber;
	for(int loop = 0; loop < numZombies; loop++)
	{
		balloonPos = D3DXVECTOR3((loop%8)*100.0f,(loop/8)*100.0f,0.0f);
		aZombie.push_back(new Character(loop));
		gridOcp[loop] = true;
		aZombie[loop]->setSpritePos(balloonPos);
		aZombie[loop]->setTranslation(D3DXVECTOR2(0.0f,0.0f));
		aZombie[loop]->setTexture(balloonTextures[3]);
		aZombie[loop]->Annimate();
	}
	sprintf_s( roundNumStr, 50, "Round %d", roundNumber);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Check any available messages from the queue
	switch (message)
	{

		case WM_KEYDOWN:
			{
				//Check if the player is able to move or is currently in motion
				if(movePlayer == false){
					int b = thePlayer.GetGridPos(); // The current grid position of the player
					//**********************************************************************************************************************
					//Fir each direction the program gets the key input of the player, make sure they are not right at the edge of the screen then
					//Sets the texture of the player to the direction they moved in before checing what spcaes are free
					//if the space is free the player is set to move that way and calimes the space, freeing up their old one
					//the direction is then set for use of the projectiles
					//***********************************************************************************************************************
					if (wParam == 0x41 && playerPos.x > 0)
					{
						thePlayer.setTexture(balloonTextures[6]);
						if(gridOcp[b-1]==false && direction == 1){
							movePlayer = true;
							goleft = true;
							gridOcp[b] = false;
							gridOcp[b-1] = true;
							thePlayer.SetGridPos(b-1);
						}
						direction = 1;
						return 0;
					}
					if (wParam == 0x44 && playerPos.x < 700)
					{
						thePlayer.setTexture(balloonTextures[7]);
						if(gridOcp[b+1]==false && direction == 2){
							movePlayer = true;
							goright = true;
							gridOcp[b] = false;
							gridOcp[b+1] = true;
							thePlayer.SetGridPos(b+1);
						}
						direction = 2;
						return 0;
					}
					if (wParam == 0x57 && playerPos.y > 0)
					{
						thePlayer.setTexture(balloonTextures[8]);
						if(gridOcp[b-8]==false && direction == 3){
							movePlayer = true;
							goup = true;
							gridOcp[b] = false;
							gridOcp[b-8] = true;
							thePlayer.SetGridPos(b-8);
						}
						direction = 3;
						return 0;
					}
					if (wParam == 0x53 && playerPos.y < 700)
					{
						thePlayer.setTexture(balloonTextures[9]);
						if(gridOcp[b+8]==false && direction == 4){
							movePlayer = true;
							goDown = true;
							gridOcp[b] = false;
							gridOcp[b+8] = true;
							thePlayer.SetGridPos(b+8);
						}
						direction = 4;
						return 0;
					}
				}
				return 0;
			}
		
		case WM_LBUTTONDOWN:
			{
				//When in game, the left click shoots the players projectile
				if(scene == 2){
					//Check if the gun is still on cooldown
					if(shotDelay == 0){
						//Set the gun to go on cooldown
						shotDelay = 25;
						//Create bullet projectiles and set their direction depending on where the player is facing
						bulletNum++;
						aBullet.push_back(new Projectile(direction));
						if(direction == 1){
							bulletPos = D3DXVECTOR3(playerPos.x,playerPos.y+60,0);
						}
						if(direction == 2){
							bulletPos = D3DXVECTOR3(playerPos.x+80,playerPos.y+60,0);
						}
						if(direction == 3){
							bulletPos = D3DXVECTOR3(playerPos.x+44,playerPos.y,0);
						}
						if(direction == 4){
							bulletPos = D3DXVECTOR3(playerPos.x+44,playerPos.y+80,0);
						}
						//Set the position of the bullet then play the firing sound
						aBullet[bulletNum]->setSpritePos(bulletPos);
						music.playSound(L"Sounds\\shot.wav",false);
					}
				}
				else
				{
					//the cursor position in x and y
					POINT mouseXY;
					mouseXY.x = LOWORD(lParam);
					mouseXY.y = HIWORD(lParam);
					//Check each button depending on scene, each button then changes the current scene of the game when clicked
					if(scene == 0){
						if ((play.insideRect(play.getBoundingRect(),mouseXY)))
						{
							scene = 2;
						}
						if ((controls.insideRect(controls.getBoundingRect(),mouseXY)))
						{
							scene = 1;
						}
					}
					if(scene == 1 || scene == 3){
						if ((main.insideRect(main.getBoundingRect(),mouseXY)))
						{
							scene = 0;
						}
					}
				}

				StringCchPrintf(szTempOutput, STRSAFE_MAX_CCH, TEXT("Mouse: lLastX=%d lLastY=%d\r\n"), LOWORD(lParam), HIWORD(lParam));
				OutputDebugString(szTempOutput);
				return 0;
			}
		case WM_CLOSE:
			{
			// Exit the Game
				PostQuitMessage(0);
				 return 0;
			}

		case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			}
	}
	// Always return the message to the default window
	// procedure for further processing
	return DefWindowProc(hWnd, message, wParam, lParam);
}

/*
==================================================================
* bool initWindow( HINSTANCE hInstance )
* initWindow registers the window class for the application, creates the window
==================================================================
*/
bool initWindow( HINSTANCE hInstance )
{
	WNDCLASSEX wcex;
	// Fill in the WNDCLASSEX structure. This describes how the window
	// will look to the system
	wcex.cbSize = sizeof(WNDCLASSEX); // the size of the structure
	wcex.style = CS_HREDRAW | CS_VREDRAW; // the class style
	wcex.lpfnWndProc = (WNDPROC)WndProc; // the window procedure callback
	wcex.cbClsExtra = 0; // extra bytes to allocate for this class
	wcex.cbWndExtra = 0; // extra bytes to allocate for this instance
	wcex.hInstance = hInstance; // handle to the application instance
	wcex.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_MyWindowIcon)); // icon to associate with the application
	wcex.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_GUNSIGHT));// the default cursor
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); // the background color
	wcex.lpszMenuName = NULL; // the resource name for the menu
	wcex.lpszClassName = "Zombies"; // the class name being created
	wcex.hIconSm = LoadIcon(hInstance,"ZSprite.ico"); // the handle to the small icon

	RegisterClassEx(&wcex);
	// Create the window
	wndHandle = CreateWindow("Zombies",			// the window class to use
							 "Zombie Rush",			// the title bar text
							WS_OVERLAPPEDWINDOW,	// the window style
							CW_USEDEFAULT, // the starting x coordinate
							CW_USEDEFAULT, // the starting y coordinate
							800, // the pixel width of the window
							800, // the pixel height of the window
							NULL, // the parent window; NULL for desktop
							NULL, // the menu for the application; NULL for none
							hInstance, // the handle to the application instance
							NULL); // no values passed to the window
	// Make sure that the window handle that is created is valid
	if (!wndHandle)
		return false;
	// Display the window on the screen
	ShowWindow(wndHandle, SW_SHOW);
	UpdateWindow(wndHandle);
	return true;
}

/*
==================================================================
// This is winmain, the main entry point for Windows applications
==================================================================
*/
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	// Initialize the window
	if ( !initWindow( hInstance ) )
		return false;
	// called after creating the window
	if ( !d3dMgr->initD3DManager(wndHandle) )
		return false;
	if ( !d3dxSRMgr->initD3DXSpriteMgr(d3dMgr->getTheD3DDevice()))
		return false;

	// Grab the frequency of the high def timer
	__int64 freq = 0;				// measured in counts per second;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	float sPC = 1.0f / (float)freq;			// number of seconds per count

	__int64 currentTime = 0;				// current time measured in counts per second;
	__int64 previousTime = 0;				// previous time measured in counts per second;
	float numFrames   = 0.0f;				// Used to hold the number of frames
	float timeElapsed = 0.0f;				// cumulative elapsed time

	GetClientRect(wndHandle,&clientBounds);

	float fpsRate = 1.0f/25.0f;
	//Load all textures used in the game
	for (int txture = 0; txture < 19; txture++)
	{
		balloonTextures[txture] = new cD3DXTexture(d3dMgr->getTheD3DDevice(), balloonTxtres[txture]);
	}
	D3DXVECTOR3 aballoonPos;
	backgroundMusic.playSound(L"Sounds\\Pokemon_Red_Blue_and_Yellow_Champion_Theme.wav",true);	//Background Music

	LPDIRECT3DSURFACE9 aSurface;				// the Direct3D surface
	LPDIRECT3DSURFACE9 theBackbuffer = NULL;  // This will hold the back buffer
	
	MSG msg;
	ZeroMemory( &msg, sizeof( msg ) );

	// Create the background surface
	aSurface = d3dMgr->getD3DSurfaceFromFile("Images\\titleScreen.png");

	// load custom font
	cD3DXFont* balloonFont = new cD3DXFont(d3dMgr->getTheD3DDevice(),hInstance, "Fonts\\8-BIT.TTF");

	RECT textPos;
	SetRect(&textPos, 20, 20, 400, 100); //Position of the round number counter
	RECT textPos2;
	SetRect(&textPos2, 20, 60, 400, 100); //Position of the zombies kileld counter

	QueryPerformanceCounter((LARGE_INTEGER*)&previousTime);
	while( msg.message!=WM_QUIT )
	{       
		// Check the message queue
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else{
			// Game code goes here
			QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
			float dt = (currentTime - previousTime)*sPC;

			// Accumulate how much time has passed.
			timeElapsed += dt;
			if(timeElapsed > fpsRate)
			{
				//Main Menu
				if(scene == 0 && lastScene!=scene){
					lastScene = scene;
						// Create the background surface
						aSurface = d3dMgr->getD3DSurfaceFromFile("Images\\titleScreen.png");
						D3DXVECTOR3 buttonPos = D3DXVECTOR3(300.0f,600.0f,0.0f);
						play.setTranslation(D3DXVECTOR2(0.0f,0.0f));
						play.setSpritePos(buttonPos);
						play.setTexture(balloonTextures[16]);
						play.otherUpdate(timeElapsed);
						controls.setTranslation(D3DXVECTOR2(0.0f,0.0f));
						buttonPos = D3DXVECTOR3(225.0f,700.0f,0.0f);
						controls.setSpritePos(buttonPos);
						controls.setTexture(balloonTextures[17]);
						controls.otherUpdate(timeElapsed);
						zombiesKilled = 0;
						killedLastRound = 0;
						roundNumber = 0;
						sprintf_s( zombiesKilledStr, 50, "");
				}
				//Controls menu
				if (scene == 1 && lastScene!=scene){
					lastScene = scene;
					// Create the background surface
					aSurface = d3dMgr->getD3DSurfaceFromFile("Images\\controlScreen.png");
					//Setup dummies that show directions, set directions and textures
					for(int i = 0; i < 4 ; i++){
						D3DXVECTOR3 dummy = D3DXVECTOR3(0.0f,0.0f,0.0f);
						dummy = D3DXVECTOR3(50.0f+(i*190),150.0f,0.0f);
						aDummy.push_back(new Character());
						aDummy[i]->setSpritePos(dummy);
						aDummy[i]->setTranslation(D3DXVECTOR2(0.0f,0.0f));
						aDummy[i]->setTexture(balloonTextures[i]);
					}
					//Create the main menu button
					D3DXVECTOR3 buttonPos = D3DXVECTOR3(200.0f,700.0f,0.0f);
					main.setTranslation(D3DXVECTOR2(0.0f,0.0f));
					main.setSpritePos(buttonPos);
					main.setTexture(balloonTextures[18]);
					main.otherUpdate(timeElapsed);
				}

				if(scene == 3 && lastScene!=scene){
					lastScene = scene;
					//Clean up scene
					sprintf_s( roundNumStr, 50, "");
					killedLastRound = 0;
					vector<Character*>::iterator iterZ = aZombie.begin();
					while (iterZ != aZombie.end())
					{
						int b = (*iterZ)->GetGridPos();
						gridOcp[b] = false;
						iterZ = aZombie.erase(iterZ);		
					}
					// Create the background surface
					aSurface = d3dMgr->getD3DSurfaceFromFile("Images\\gameOverScreen.png");
					D3DXVECTOR3 buttonPos = D3DXVECTOR3(200.0f,700.0f,0.0f);
					main.setTranslation(D3DXVECTOR2(0.0f,0.0f));
					main.setSpritePos(buttonPos);
					main.setTexture(balloonTextures[18]);
					main.otherUpdate(timeElapsed);
				}
				//Game Scene
				if(scene == 2){
					if(lastScene!=scene){
						//Set up the first round
						setUpRound();
						// Create the background surface
						aSurface = d3dMgr->getD3DSurfaceFromFile("Images\\background.png");
						//Set or reset the players health
						health = 10;
						for(int i = 0; i < 10 ; i++){
							D3DXVECTOR3 heartPos = D3DXVECTOR3(420.0f+(i*36),20.0f,0.0f);
							healthBlock.push_back(new cSprite());
							healthBlock[i]->setSpritePos(heartPos);
							healthBlock[i]->setTranslation(D3DXVECTOR2(0.0f,0.0f));
							healthBlock[i]->setTexture(balloonTextures[5]);
						}
						//As before, make sure that the prvious code only happens once
						lastScene = scene;
					}
					//Lower the delay on the players gun each frame, due to frame rate capping it creates an acurate delay 
					if(shotDelay>0){
						shotDelay--;
					}
					//Triggers next round when the correct number of zombies are killed or changes scene to game over
					if((zombiesKilled-killedLastRound)/8 == roundNumber && roundNumber!=lastRound){
						lastRound = roundNumber;
						if(lastRound!=5){
							setUpRound();
						}
						else{
							scene = 3;
						}
					}
					//Move the player and annimate in the correct direction and increase the delay timer 
					if(movePlayer == true && playDelay<25){
						playDelay++;  
						if(goleft == true){
							playerPos.x -= 4;
							if(playDelay%6 == 0){
								thePlayer.Annimate();
							}
						}
						if(goright == true){
							playerPos.x += 4;
							if(playDelay%6 == 0){
								thePlayer.Annimate();
							}
						}
						if(goup == true){
							playerPos.y -= 4;
							if(playDelay%6 == 0){
								thePlayer.Annimate();
							}
						}
						if(goDown == true){
							playerPos.y += 4;
							if(playDelay%6 == 0){
								thePlayer.Annimate();
							}
						}
					}
					//Reset everything when he is done moving
					else{
						playDelay = 0;
						movePlayer = false;
						goleft = false;
						goright = false;
						goup = false;
						goDown = false;
					}
					//Update the players position
					thePlayer.setSpritePos(playerPos);
					thePlayer.otherUpdate(timeElapsed);
					//Set the texture and translation of the bullet depending on the direction it was shot from
					for(piter = aBullet.begin(); piter != aBullet.end(); ++piter)
					{
						int thisDir = (*piter)-> GetMoveDirection();
						if(thisDir == 1){
							(*piter)->setTranslation(D3DXVECTOR2(-200.0f,0.0f));
							(*piter)->setTexture(balloonTextures[10]);
						}
						if(thisDir == 2){
							(*piter)->setTranslation(D3DXVECTOR2(200.0f,0.0f));
							(*piter)->setTexture(balloonTextures[10]);
						}
						if(thisDir == 3){
							(*piter)->setTranslation(D3DXVECTOR2(0.0f,-200.0f));
							(*piter)->setTexture(balloonTextures[11]);
						}
						if(thisDir == 4){
							(*piter)->setTranslation(D3DXVECTOR2(0.0f,200.0f));
							(*piter)->setTexture(balloonTextures[11]);
						}
					}
					for(iter = aZombie.begin(); iter != aZombie.end(); ++iter){
						//The last direction of the enemy
						int zDir = (*iter)->GetMoveDirection();
						aballoonPos = (*iter)->getSpritePos();  // get the position of the current balloon
						//Check if the delay is 50. The number 50 is used as it is the number of frames over 2 seconds
						if(delay == 50){
							//Get the enemies current position and get a copy of it incase it is altered
							int b = (*iter)->GetGridPos();
							int newPos = b;
							//Get the distance between the current enemy and the player
							float xDiff = std::abs(aballoonPos.x - playerPos.x);
							float yDiff = std::abs(aballoonPos.y - playerPos.y);
							//Check that the player is not in an adjacent block, if they are not then work out the enemy path
							if(!((xDiff == 0 && yDiff == 100)|| (yDiff == 0 && xDiff == 100))){
								//If the x distance is further, try to go that way first
								if(xDiff>yDiff){
									//check if the enemy is on the left or right
									if(aballoonPos.x > playerPos.x){
										//set the texture so that the enemy faces left
										(*iter)->setTexture(balloonTextures[0]);
										//Check is the new spot is available, if so claim it
										if(gridOcp[b-1]==false){
											zDir = 1;
											newPos = b-1;
											(*iter)->SetGridPos(newPos);
										}
										//Try going in the  direction as x path is blocked, therefor check which way in y is correct
										else{
											if(aballoonPos.y >= playerPos.y){
												//Managed to go in the y direction, cliamed the spot
												if((aballoonPos.y!=playerPos.y) && gridOcp[b-8]==false){
													(*iter)->setTexture(balloonTextures[2]);
													zDir = 3;
													newPos = b-8;
													(*iter)->SetGridPos(newPos);
												}
												//Failed to make progress either way, therefor stopped
												else{
													zDir = 0;
												}
											}
											else{
												//Check if the spot in y is free, if so claim it and move there
												if(gridOcp[b+8]==false){
													(*iter)->setTexture(balloonTextures[3]);
													zDir = 4;
													newPos = b+8;
													(*iter)->SetGridPos(newPos);
												}
												//No moves available so stand still
												else{
													zDir = 0;
												}
											}
										}
										//*********************************************************************************************************************
										//The path fidning carriex on in the same way checking all options, seems a waste of lines to repeat it so many times
										//*********************************************************************************************************************
									}
									else{
										(*iter)->setTexture(balloonTextures[1]);
										if(gridOcp[b+1]==false){
											zDir = 2;
											newPos = b+1;
											(*iter)->SetGridPos(newPos);
										}
										else{
											if(aballoonPos.y >= playerPos.y){
												if(aballoonPos.y!=playerPos.y && gridOcp[b-8]==false){
													(*iter)->setTexture(balloonTextures[2]);
													zDir = 3;
													newPos = b-8;
													(*iter)->SetGridPos(newPos);
												}
												else{
													zDir = 0;
												}
											}
											else{
												if(gridOcp[b+8]==false){
													(*iter)->setTexture(balloonTextures[3]);
													zDir = 4;
													newPos = b+8;
													(*iter)->SetGridPos(newPos);
												}
												else{
													zDir = 0;
												}
											}
										}
									}
								}
								else{
									if(aballoonPos.y > playerPos.y){
										(*iter)->setTexture(balloonTextures[2]);
										if(gridOcp[b-8]==false){
											zDir = 3;
											newPos = b-8;
											(*iter)->SetGridPos(newPos);
										}
										else{
											if(aballoonPos.x >= playerPos.x){
												if(aballoonPos.x!=playerPos.x && gridOcp[b-1]==false){
													(*iter)->setTexture(balloonTextures[0]);
													zDir = 1;
													newPos = b-1;
													(*iter)->SetGridPos(newPos);
												}
												else{
													zDir = 0;;
												}
											}
											else{
												if(gridOcp[b+1]==false){
													(*iter)->setTexture(balloonTextures[1]);
													zDir = 2;
													newPos = b+1;
													(*iter)->SetGridPos(newPos);
												}
												else{
													zDir = 0;
												}
											}
										}
									}
									else{
										(*iter)->setTexture(balloonTextures[3]);
										if(gridOcp[b+8]==false){
											zDir = 4;
											newPos = b+8;
											(*iter)->SetGridPos(newPos);
										}
										else{
											if(aballoonPos.x >= playerPos.x){
												if(aballoonPos.x!=playerPos.x && gridOcp[b-1]==false){
													(*iter)->setTexture(balloonTextures[0]);
													zDir = 1;
													newPos = b-1;
													(*iter)->SetGridPos(newPos);
												}
												else{
													zDir = 0;
												}
											}
											else{
												if(gridOcp[b+1]==false){
													(*iter)->setTexture(balloonTextures[1]);
													zDir = 2;
													newPos = b+1;
													(*iter)->SetGridPos(newPos);
												}
												else{
													zDir = 0;
												}
											}
										}
									}
								}
							}
							else{
								//if the enemy is adjacent to the player, don't move and prepare a projectile
								zDir = 0;
								slimeNum++;
								//The following code works out the direction for the projectile, it's translation and the corresponding texture
								if(yDiff == 0){
									if(aballoonPos.x > playerPos.x){
										(*iter)->setTexture(balloonTextures[0]);
										aSlime.push_back(new Projectile(1));
										slimePos = D3DXVECTOR3(aballoonPos.x+50,aballoonPos.y+60,0);
									}
									else{
										(*iter)->setTexture(balloonTextures[1]);
										aSlime.push_back(new Projectile(2));
										slimePos = D3DXVECTOR3(aballoonPos.x+50,aballoonPos.y+60,0);
									}
								}
								else{
									if(aballoonPos.y > playerPos.y){
										(*iter)->setTexture(balloonTextures[2]);
										aSlime.push_back(new Projectile(3));
										slimePos = D3DXVECTOR3(aballoonPos.x+44,aballoonPos.y+50,0);
									}
									else{
										(*iter)->setTexture(balloonTextures[3]);
										aSlime.push_back(new Projectile(4));
										slimePos = D3DXVECTOR3(aballoonPos.x+44,aballoonPos.y+50,0);
									}
								}
								//The enemy projectile noise is then played and the sprites position is set
								music.playSound(L"Sounds\\blerugh.wav",false);
								aSlime[slimeNum]->setSpritePos(slimePos);
							}
							//Reserve or free up and spaces affected by the enemy this frame
							for(int i = 0; i < 64 ; i++){
								if(i == b){	
									gridOcp[i] = false;
								}
								if(i == newPos){	
									gridOcp[i] = true;
								}
							}
						}
						//Move the enemy using positions then setting them to make sure it is 100% accurate
						if(zDir!= 0 && delay < 50){
							if(zDir ==1){
								aballoonPos.x -= 2;
							}
							if(zDir == 2){
								aballoonPos.x += 2;
							}
							if(zDir == 3){
								aballoonPos.y -= 2;
							}
							if(zDir == 4){
								aballoonPos.y += 2;
							}
							//Annimate the sprite 4 times a second
							if(delay%6 == 0){
								(*iter)->Annimate();
							}
							//Set the sprites new position
							(*iter)->setSpritePos(aballoonPos);
						}
						//Update the enemies move direction
						(*iter)->SetMoveDirection(zDir);
						//Update enemy in general
						(*iter)->otherUpdate(timeElapsed);
					}
					//Check if the delay is at maximum, if so set it to -1 (it will be increased to 0 before the end of this frame)
					if(delay == 50){
						delay = -1;
					}
					//The same process is used for enemy projectiles as player ones, setting the correct texture and translation for it's direction
					for(siter = aSlime.begin(); siter != aSlime.end(); ++siter)
					{
						int thisDir = (*siter)-> GetMoveDirection();
						if(thisDir == 1){
							(*siter)->setTranslation(D3DXVECTOR2(-100.0f,0.0f));
							(*siter)->setTexture(balloonTextures[12]);
						}
						if(thisDir == 2){
							(*siter)->setTranslation(D3DXVECTOR2(100.0f,0.0f));
							(*siter)->setTexture(balloonTextures[13]);
						}
						if(thisDir == 3){
							(*siter)->setTranslation(D3DXVECTOR2(0.0f,-100.0f));
							(*siter)->setTexture(balloonTextures[14]);
						}
						if(thisDir == 4){
							(*siter)->setTranslation(D3DXVECTOR2(0.0f,100.0f));
							(*siter)->setTexture(balloonTextures[15]);
						}
					}
					//Check for collisions between the enemies and bullets
					for(piter = aBullet.begin(); piter != aBullet.end(); ++piter)
					{
						vector<Character*>::iterator iterZ = aZombie.begin();
						while (iterZ != aZombie.end())
						{
							if ((*iterZ)->collidedWith((*iterZ)->getBoundingRect(), (*piter)->getBoundingRect()))
							{
								music.playSound(L"Sounds\\pop.wav",false); //Play the zombie death noise
								int b = (*iterZ)->GetGridPos(); //get current grid spot
								//Create death annimation
								expPos = (*iterZ)->getSpritePos(); 
								gExplode.push_back(new cExplosion(expPos,balloonTextures[4]));
								gridOcp[b] = false; //free up grid spot
								iterZ = aZombie.erase(iterZ); //delete zombie
								(*piter)->setSpritePos(D3DXVECTOR3(900.0f,900.0f,0.0f)); //Easier to move it out of the way as it will be cleaned up
								zombiesKilled++; //increase kills
								sprintf_s( zombiesKilledStr, 50, "Kills : %d", zombiesKilled); //create string to show kills
							}
							else{
								++iterZ; //iterate
							}
						}
					}
					//Check for collisions between the player and slime
					for(siter = aSlime.begin(); siter != aSlime.end(); ++siter)
					{
						if (thePlayer.collidedWith(thePlayer.getBoundingRect(), (*siter)->getBoundingRect()))
						{
							music.playSound(L"Sounds\\ow.wav",false); //Play the playe hit noise
							health--; //Lower health
							if(health > 0){
								healthBlock.pop_back(); //Remove a heart
							}
							else{
								scene = 3; //If out of health, gameover
							}
							(*siter)->setSpritePos(D3DXVECTOR3(900.0f,900.0f,0.0f)); //Easier to move it out of the way as it will be cleaned up
						}
					}
					}
					//begin the renderer and get ready to draw
					d3dMgr->beginRender();
					theBackbuffer = d3dMgr->getTheBackBuffer();
					d3dMgr->updateTheSurface(aSurface, theBackbuffer);
					d3dMgr->releaseTheBackbuffer(theBackbuffer);
					d3dxSRMgr->beginDraw();
					//Draw main menu buttons
					if(scene == 0){
						play.update();
						d3dxSRMgr->setTheTransform(play.getSpriteTransformMatrix());  
						d3dxSRMgr->drawSprite(play.getTexture(),NULL,NULL,NULL,0xFFFFFFFF);
						controls.update();
						d3dxSRMgr->setTheTransform(controls.getSpriteTransformMatrix());  
						d3dxSRMgr->drawSprite(controls.getTexture(),NULL,NULL,NULL,0xFFFFFFFF);
					}
					//Draw game over and controls screen button
					if(scene == 1 || scene == 3){
						main.update();
						d3dxSRMgr->setTheTransform(main.getSpriteTransformMatrix());  
						d3dxSRMgr->drawSprite(main.getTexture(),NULL,NULL,NULL,0xFFFFFFFF);
						diter = aDummy.begin();
						//Draw and annimate the dummies in the controls menu
						if(scene == 1){
							for(diter = aDummy.begin(); diter!= aDummy.end(); ++diter)
							{
								if(delay%6 == 0){
									(*diter)->Annimate();
								}
								(*diter)->otherUpdate(timeElapsed);
								d3dxSRMgr->setTheTransform((*diter)->getSpriteTransformMatrix());  
								d3dxSRMgr->drawSprite((*diter)->getTexture(),&((*diter)->getSourceRect()),NULL,NULL,0xFFFFFFFF);
							}
						}
					}
					//Draw the in game sprites
					if(scene == 2){
						d3dxSRMgr->setTheTransform(thePlayer.getSpriteTransformMatrix());  
						d3dxSRMgr->drawSprite(thePlayer.getTexture(),&(thePlayer.getSourceRect()),NULL,NULL,0xFFFFFFFF); //draw player
						vector<Character*>::iterator iterB = aZombie.begin();
						for(iterB = aZombie.begin(); iterB != aZombie.end(); ++iterB) //draw enemies
						{
							d3dxSRMgr->setTheTransform((*iterB)->getSpriteTransformMatrix());  
							d3dxSRMgr->drawSprite((*iterB)->getTexture(),&((*iterB)->getSourceRect()),NULL,NULL,0xFFFFFFFF);
				
						}
						hiter = healthBlock.begin();
						for(hiter = healthBlock.begin(); hiter!= healthBlock.end(); ++hiter) //draw health blocks
						{
							(*hiter)->update();
							d3dxSRMgr->setTheTransform((*hiter)->getSpriteTransformMatrix());  
							d3dxSRMgr->drawSprite((*hiter)->getTexture(),NULL,NULL,NULL,0xFFFFFFFF);
						}
						vector<Projectile*>::iterator iterc = aBullet.begin();
						while(iterc != aBullet.end()) //draw bullets
						{
							bulletPos = (*iterc)->getSpritePos();
							if(bulletPos.x > 800 || bulletPos.x < 0 || bulletPos.y > 800 || bulletPos.y < 0){
								bulletNum--; //lower the number of bullets
								iterc = aBullet.erase(iterc); //destroy if out of bounds
							}
							else{ //draw and iterate if not out of bounds
								(*iterc)->otherUpdate(timeElapsed);
								d3dxSRMgr->setTheTransform((*iterc)->getSpriteTransformMatrix());  
								d3dxSRMgr->drawSprite((*iterc)->getTexture(),NULL,NULL,NULL,0xFFFFFFFF);
								++iterc;;
							}
						}
						vector<Projectile*>::iterator spiter = aSlime.begin();
						while(spiter != aSlime.end())
						{
							slimePos = (*spiter)->getSpritePos();
							if(slimePos.x > 800 || slimePos.x < 0 || slimePos.y > 800 || slimePos.y < 0){
								slimeNum--; //lower number of slimes
								spiter = aSlime.erase(spiter); //destroy if out of bounds
							}
							else{ //draw and iterate if not out of bounds
								(*spiter)->otherUpdate(timeElapsed);
								d3dxSRMgr->setTheTransform((*spiter)->getSpriteTransformMatrix());  
								d3dxSRMgr->drawSprite((*spiter)->getTexture(),NULL,NULL,NULL,0xFFFFFFFF);
								++spiter;;
							}
						}
						list<cExplosion*>::iterator iter = gExplode.begin();
						while(iter != gExplode.end()) //draw death annimations
						{
							if((*iter)->isActive() == false)
							{
								iter = gExplode.erase(iter); //destroy when annimation is done and becomes inactive
							}
							else
							{
								if(delay%2 == 0){
									(*iter)->Annimate(); //annimate the spite
								}
								d3dxSRMgr->setTheTransform((*iter)->getSpriteTransformMatrix());  
								d3dxSRMgr->drawSprite((*iter)->getTexture(),&((*iter)->getSourceRect()),NULL,NULL,0xFFFFFFFF);
								++iter;
							}
						}
					} 
					//end drawing
					d3dxSRMgr->endDraw();
					balloonFont->printText(roundNumStr,textPos);
					balloonFont->printText(zombiesKilledStr,textPos2);
					d3dMgr->endRender();
					OutputDebugString("timeElapsed > fpsRate");
					timeElapsed = 0.0f;//reset time elapsed so that it does not run again next frame
					delay++; //increase the dealy timer
				}
				previousTime = currentTime; //set the previous time for use by the fps cap
		}
	}
	d3dxSRMgr->cleanUp();
	d3dMgr->clean();
	return (int) msg.wParam;
}