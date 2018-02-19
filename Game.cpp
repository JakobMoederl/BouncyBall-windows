#include "Game.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <SOIL.h>

#define DEATH_ANIMATION_TIME 1.0f

void KillGLWindow();
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag);

using namespace std;

int i, j; //globale schleifen-zählvariable
// Standard Konstruktor
Game::Game(void)
{
	fullscreen = true;
	acceleration = 20.0f;
	acceleration_ground = 8.0f;
	acceleration_air = 3.5f;
	gravity = 20.0f;

	won = false;
	last_won = false;
	lost = false;
	pause = false;
	done = false;

	total_time = 0;
	counter = 0;
	lvl = new Level[2];
	activeLvl = 0;
	lvlAnz = 2;

	frames = 0;
	frames_last = 0;
	frames_displayed_last = 0;
}

//destruktor
Game::~Game(void)
{
}

bool Game::initFirst(){
	char lvlFile[32];
	ifstream lvlFileStream;

	total_time = 0;
	deaths = 0;
	activeLvl = 0;
	total_time = 0;
	deaths = 0;
	
	for(i = 0; i<lvlAnz; i++){
		lvl[i].deleteTextures();
	}
	sprintf(lvlFile, "lvl/%i.lvl", 1);
	lvlFileStream.open(lvlFile, ios::in);
	if(!lvlFileStream.is_open()){
		return false;
	}
	
	if(!this->lvl[0].loadLvl(lvlFileStream)){
		MessageBox(NULL,"couldnt read lvl File","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;
	}
	lvlFileStream.close();
	
	sprintf(lvlFile, "lvl/%i.lvl", 2);
	lvlFileStream.open(lvlFile, ios::in);
	if(lvlFileStream.is_open()){
		this->lvl[1].loadLvl(lvlFileStream);
	} else {
		this->lvl[1].setWidth(0);
	}
	lvlFileStream.close();

	if(lvl[1].getWidth() > lvl[0].getWidth()){
		lvl[0].getStoneBlock(2).setLayer(10);
		lvl[0].getStoneBlock(2).setDepth(4);
	}
	return true;
}

//initials the lvl specified by the given argument lvlNumber
//if lvl number is 0 it initials the game basics (maxSpeed, player radius and so on)
bool Game::init(int lvlNumber, Level& lvl)
{
	char lvlFile[32];
	ifstream lvlFileStream;

	sprintf(lvlFile, "lvl/%i.lvl", lvlNumber);
	lvlFileStream.open(lvlFile, ios::in);
	if(!lvlFileStream.is_open()){
		return false;
	}
	if(!lvl.loadLvl(lvlFileStream)){
		MessageBox(NULL,"couldnt read lvl File","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	lvlFileStream.close();
	return true;
}

//game main rutine, does everything
void Game::play(void){		//game main loop (does everything)
	extern HDC hDC;									// Private GDI Device Context
	extern MSG msg;									// Windows Message Structure

	SYSTEMTIME time_old;	//time when game calculations starten
	SYSTEMTIME time_now;	//tme when game calculations where finished

	lvlNumber = 1;
	activeLvl = 0;

	initFirst();

	GetSystemTime(&time_old);
	GetSystemTime(&time_now);

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else								// If There Are No Messages
		{
			if(!pause){
			
				//calculation the time difference since the last calculation
				GetSystemTime(&time_now);
				time_difference  =  (GLfloat)(time_now.wMilliseconds - time_old.wMilliseconds)/1000.0f;
				if(time_now.wSecond != time_old.wSecond){
					time_difference += 1;
				}
				GetSystemTime(&time_old);

				if(time_difference > 0.33f){
					time_difference = 0;
				}

				counter += time_difference;

				if(won && counter > 2.5){
					if(!last_won){
						getLvl().deleteTextures();
						if(!init(lvlNumber + lvlAnz, getLvl())){
							getLvl().setWidth(0);
						}
						activeLvl++;
						if(activeLvl >= lvlAnz){
							activeLvl = 0;
						}
						lvlNumber++;
						if(getLvl(activeLvl +1).getWidth() > getLvl().getWidth()){
							getLvl().getStoneBlock(2).setLayer(10);
							getLvl().getStoneBlock(2).setDepth(4);
						}
						frames_displayed_last = 0.0f;
						frames_last = 0.0f;
						frames = 0.0f;
						resetLvl(); 
					}
				} else if(lost){
					if(counter > DEATH_ANIMATION_TIME + 0.2f || getLvl().getPlayer().getY() - getLvl().getPlayer().getRadius() < 0){
						resetLvl();
					} else {
						getLvl().getPlayer().doAnimations(time_difference);
					}
				} else {

				//do calculating the whole game
					if(!won && !last_won){
						getLvl().addTimePlaying(time_difference);
						total_time += time_difference;
					}

					if(time_difference != 0){
						frames++;
					}

					if(counter - frames_displayed_last > 0.3f){
						frames_displayed_last += 0.3f;
						frames_last = frames;
						frames = 0;
					}

					calcKollisionen();

					doMovement();
				
					doAnimations();
				}
			}
			//draw everything
			draw();					// Draw The Scene
			SwapBuffers(hDC);		// Swap Buffers (GLfloat Buffering)

			GetSystemTime(&time_now);
			time_difference = (GLfloat)(time_now.wMilliseconds - time_old.wMilliseconds);
			if (time_now.wSecond != time_old.wSecond){
				time_difference += 1000.f;
			}
			if (time_difference < 10){
				Sleep(10 - time_difference);
			}
		}
	}
}

// Calculates if there are any kollisionen 
void Game::calcKollisionen(){
	//calculate kollisionen
	//blöcke
	for(i = 0; i < getLvl().getStoneBlockAnzahl(); i++){
		if(getLvl().getStoneBlock(i).getKollisionEdge(getPlayer())){
			getLvl().getStoneBlock(i).doKollision(getPlayer());
		}
	}
	
	for(i = 0; i < getLvl().getStoneBlockAnzahl(); i++){
		if(getLvl().getStoneBlock(i).getKollisionCorner(getPlayer())){
			getLvl().getStoneBlock(i).doKollision(getPlayer());
		}
	}

	for(i = 0; i < getLvl().getSpikeBlockAnz(); i++){
		if(getLvl().getSpikeBlock(i).getKollisionEdge(getPlayer()) || getLvl().getSpikeBlock(i).getKollisionEdge(getPlayer())){
			getLvl().getSpikeBlock(i).doKollision(getPlayer());
		}
	}
	
	for(i = 0; i < getLvl().getGoodSmileyAnzahl(); i++){
		if(getLvl().getGoodSmiley(i).getKollision(getPlayer())){
			getLvl().getGoodSmiley(i).doKollision(getPlayer());
		}
	}

	for(i = 0; i < getLvl().getEnemyAnzahl(); i++){
		if(getLvl().getEnemy(i).getKollision(getPlayer())){
			getLvl().getEnemy(i).doKollision(getPlayer());
		}
	}

	//enemys
	for(j = 0; j < getLvl().getEnemyAnzahl(); j++){
		for(i = 0; i < getLvl().getStoneBlockAnzahl(); i++){
			if(getLvl().getStoneBlock(i).getKollisionCorner(getLvl().getEnemy(j)) ||
				getLvl().getStoneBlock(i).getKollisionEdge(getLvl().getEnemy(j)))
			{
				getLvl().getStoneBlock(i).doKollision(getLvl().getEnemy(j));
			}
		}

		for(i = 0; i < getLvl().getSpikeBlockAnz(); i++){
			if(getLvl().getSpikeBlock(i).getKollisionEdge(getLvl().getEnemy(j)) ||
				getLvl().getSpikeBlock(i).getKollisionCorner(getLvl().getEnemy(j)))
			{
				getLvl().getSpikeBlock(i).doKollision(getLvl().getEnemy(j));
			}
		}
	}
}

// moves all objects and the player (also implements acceleration and jumping)
void Game::doMovement(){
	//move alle the objects
	if(move_left){
		getLvl().getPlayer().setSpeedX(getLvl().getPlayer().getSpeedX() - acceleration*time_difference);
	} else if (move_right){
		getLvl().getPlayer().setSpeedX(getLvl().getPlayer().getSpeedX() + acceleration*time_difference);
	} else if(getLvl().getPlayer().isOnFloor()){
		if(getLvl().getPlayer().getSpeedX() > 0){
			getLvl().getPlayer().setSpeedX(getLvl().getPlayer().getSpeedX() - acceleration_ground*time_difference);
		} else {
			getLvl().getPlayer().setSpeedX(getLvl().getPlayer().getSpeedX() + acceleration_ground*time_difference);
		}
		if(abs(getLvl().getPlayer().getSpeedX()) < 0.5f){
			getLvl().getPlayer().setSpeedX(0);
		}
	} else {
		if(getLvl().getPlayer().getSpeedX() > 0){
			getLvl().getPlayer().setSpeedX(getLvl().getPlayer().getSpeedX() - acceleration_air*time_difference);
		} else if(getLvl().getPlayer().getSpeedX() < 0){
			getLvl().getPlayer().setSpeedX(getLvl().getPlayer().getSpeedX() + acceleration_air*time_difference);
			}
		if(abs(getLvl().getPlayer().getSpeedX()) < 0.5f){
			getLvl().getPlayer().setSpeedX(0);
		}
	}

	if(!getLvl().getPlayer().isOnFloor()){
		getLvl().getPlayer().setSpeedY(getLvl().getPlayer().getSpeedY() - gravity*time_difference);
	} else if(jump){
		moveJump(-jump_speed);
	}
	getLvl().getPlayer().move(time_difference);
	
	for(i = 0; i < getLvl().getEnemyAnzahl(); i++){
		if(!getLvl().getEnemy(i).isOnFloor()){
			getLvl().getEnemy(i).setSpeedY(getLvl().getEnemy(i).getSpeedY() - gravity * time_difference);
		}
		getLvl().getEnemy(i).move(time_difference);
	}
	
}

//calls all animations methodes
void Game::doAnimations(){
	for(i = 0; i < getLvl().getGoodSmileyAnzahl(); i++){
		getLvl().getGoodSmiley(i).doAnimations(time_difference);
	}
	for(i = 0; i < getLvl().getEnemyAnzahl(); i++){
		getLvl().getEnemy(i).doAnimations(time_difference);
	}

	getLvl().getPlayer().doAnimations(time_difference);
}

//function should be called when you lost the lvl
//initiales the current lvl new (so you start from the beginning)
void Game::loose(void){
	if(won){ //when the player looses after he won the lvl
		return;
	}
	getLvl().getPlayer().setSpeedY(5);
	getLvl().getPlayer().setSpeedX(0);
	counter = 0.0f;
	deaths++;
	lost = true;
}

//funtion should be called when you won the lvl
//initiales the next lvl
void Game::win(void){
	counter = 0.0f;
	x_lvl_won = getLvl().getPlayer().getX();
	y_lvl_won = getLvl().getPlayer().getY();
	if(getLvl(activeLvl +1).getWidth()==0){
		last_won =true;
		lvlNumber = 1;
		activeLvl++;
		if(activeLvl >= lvlAnz){
			activeLvl = 0;
		}
		counter = 0;
	} else {
		won = true;
	}
}

//should be called when a player catches up a smiley
void Game::smileyCatched(void){
	extern Game game;
	if(getLvl().getActiveSmileys() == 0){
		game.win();
	}
}

//ends the game and closes the window
void Game::finished(void)
{
	done = true;
}

void Game::resetLvl(){

	won = false;
	lost = false;
	last_won = false;
	pause = false;
	
	getLvl().setPlayerToStart();
	getLvl().getPlayer().setSpeedX(0);
	getLvl().getPlayer().setSpeedY(0);
	getLvl().getPlayer().setOnFloor(false, 0, 0);
	getLvl().getPlayer().setLayer(10.5f);
	getLvl().getPlayer().setRotation(0);
	getLvl().getPlayer().setRadius(0.4f);
	getLvl().getPlayer().stopJumpAnimation();
	getLvl().getPlayer().stopDeathAnimation();
	getLvl().getPlayer().setActive(true);
	
	for(i = 0; i < getLvl().getGoodSmileyAnzahl(); i++){
		getLvl().getGoodSmiley(i).setActive(true);
	}
	
	for(i = 0; i < getLvl().getEnemyAnzahl(); i++){
		getLvl().getEnemy(i).reset();
	}
	getLvl().setTimePlaying(0);
	counter = 0;
	frames = 0;
	frames_displayed_last = 0;
	frames_last = 0;
	
	setWantJump(false);
	moveStop();
}

//draws the game (everything from scrolling, object, player)
void Game::draw(void){
	//reset display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	if(last_won){
		drawEndOfGame();
		return;
	}
	
	glRotatef(10.0f ,1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, -3.0f, 0.0f);

	if(won){
		glTranslatef(-x_lvl_won + (x_lvl_won - getLvl(activeLvl + 1).getStartX())*counter/2.5f, -y_lvl_won + (y_lvl_won - getLvl(activeLvl + 1).getStartY())*counter/2.5f, 4.0f*counter/2.5f);
		
	} else {
		glTranslatef(-getLvl().getPlayer().getX(), -getLvl().getPlayer().getY(), 0.0f);
	}
	
	if(getLvl(activeLvl +1).isBackgroundEnabled()){
			drawNextLvlBackground();
	}

	getLvl().getStoneBlock(0).draw();
	getLvl().getStoneBlock(1).draw();
	if(getLvl().getWidth() > getLvl(activeLvl +1).getWidth()){
		getLvl().getStoneBlock(2).draw();
		getLvl().getStoneBlock(3).draw();
	}

	if(won){
		glColor4f(1.0f, 1.0f, 1.0f, 1-counter/2.5f);
	}

	drawBackground();

	if(won){
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}

	drawNextLvl();
	
	
	if(won){
		glColor4f(1.0f, 1.0f, 1.0f, 1-counter/2.5f);
	}

	
	//Objekte
	if(getLvl(activeLvl +1).getWidth() > getLvl().getWidth()){
		getLvl().getStoneBlock(2).draw();
		getLvl().getStoneBlock(3).draw();
	}
	for(i = 4; i < getLvl().getObjectAnzahl(); i++){
		getLvl().getObject(i).draw();
	}

	//spieler
	getLvl().getPlayer().draw();

	if(pause){
		drawDarkScreen(0.3f);
	} else if(lost){
		drawDarkScreen(1.0f-(counter*1.0f/DEATH_ANIMATION_TIME));
	}

	glColor3f(1.0f, 1.0f, 1.0f);
	drawInterface();

}

//draw the text when you won the last lvl
#define x_print_endof_game 0.40f
void Game::drawEndOfGame(){
	GLfloat y = 0.0f;
	if(counter < 2.0f){
		y -= 0.4f - 0.2f*counter;
	}
	print("Smiley Wars by Jakob Moederl", -x_print_endof_game, y+0.12f);
	print("____________________________", -x_print_endof_game, y+0.12f);
	
	print("Congratulations!!!", -x_print_endof_game, y+0.06f);
	print("You finished the last level", -x_print_endof_game, y+0.03f);
	print("Your total time is: %5.2f", -x_print_endof_game, y-0.00f, total_time);
	if(deaths == 0){
		print("You never died, like a real pro", -x_print_endof_game, y-0.03f);
	} else if(deaths == 1){
		print("You only died once, not bad", -x_print_endof_game, y-0.03f);
	} else {
		print("You died %i times, thats way to often noob!", -x_print_endof_game, y-0.03f, deaths);
	}
	print("To restart the game press 'R', to quit press ESC", -x_print_endof_game, y-0.06f);
	print("I hope you enjoyed my Game,", -x_print_endof_game, y-0.1f);
	print("if so try to make your own levels",  -x_print_endof_game, y-0.13f);
	print("Thanks for playing!",  -x_print_endof_game, y-0.16f);
}

//draws the game interface (lvl number, smileys, time and so on)
void Game::drawInterface(){
	glDisable(GL_DEPTH_TEST);
	glColor3f(1.0f, 1.0f, 1.0f);
	print("Level: %i", -0.4f*(float)frame_width/(float)frame_height , 0.35f, lvlNumber);
	print("Smileys: %i/%i", -0.4f*(float)frame_width/(float)frame_height, 0.315f, (getLvl().getGoodSmileyAnzahl() - getLvl().getActiveSmileys()), getLvl().getGoodSmileyAnzahl());
	print("Time: %2.2f", -0.4f*(float)frame_width/(float)frame_height, 0.28f, getLvl().getTimePlaying());
	print("Framerate: %2.0f", -0.4f*(float)frame_width/(float)frame_height, 0.245f, 1.0f/0.3f * frames_last);

	//print("framerate: %3.1f", 15, frame_height - 80, 1.0f/time_difference);
	if(pause){
		//print("______________", -0.1f, 0.2f);
		print("Game Paused!!!", -0.15f, 0.135f);
		print("______________", -0.15f, 0.135f);
		print("Press 'P' or SPACE to resume game", -0.15f, 0.065f);
		print("Press ESC to quit",  -0.15f, 0.030f);
	} else if(won){
		//print("_________________________", frame_width/2 - 170, frame_height/2 + 48);
		print("You completed this Level!", -0.15f, 0.135f);
		print("_________________________", -0.15f, 0.135f);
		print("Your time was: %0.2f", -0.15f, 0.065f, getLvl().getTimePlaying());
		print("Next Level will start in %2.1fs", -0.15f, 0.03f, 2.5-counter);
	}
	glEnable(GL_DEPTH_TEST);
}

//draws the backgroundimage
void Game::drawBackground(){
	
	//background
	if(!getLvl().isBackgroundEnabled()){
		return;
	}
	glBindTexture(GL_TEXTURE_2D, getLvl().getBackground());
	glBegin(GL_QUADS);
		glTexCoord2f(0, getLvl().getBackgroundFaktorY()); glVertex3f(0, (float)getLvl().getHeight(), -16.0f);
		glTexCoord2f(getLvl().getBackgroundFaktorX(), getLvl().getBackgroundFaktorY()); glVertex3f((float)getLvl().getWidth(), (float)getLvl().getHeight(), -16.0f);
		glTexCoord2f(getLvl().getBackgroundFaktorX(), 0); glVertex3f((float)getLvl().getWidth(), 0, -16.0f);
		glTexCoord2f(0, 0); glVertex3f(0, 0, -16.0f);
	glEnd();

}

void Game::drawNextLvl(){
	if(getLvl(activeLvl + 1).getWidth() == 0){
		return;
	}

	glPushMatrix();
	if(getLvl(activeLvl + 1).getWidth() > getLvl().getWidth()){
		if(getLvl(activeLvl +1).getWidth() > getLvl().getWidth()){
			for(i = 1; i < 4; i++){
				getLvl(activeLvl + 1).getObject(i).draw();
			}
		}
	}
	glTranslatef(0.0f, 0.0f, -4.0f);
	for(i = 4; i<getLvl(activeLvl +1).getObjectAnzahl(); i++){
		getLvl(activeLvl+1).getObject(i).draw();
	}
	getLvl(activeLvl +1).getPlayer().draw();
	glPopMatrix();
}

void Game::drawNextLvlBackground(){
	if(getLvl(activeLvl +1).getWidth() == 0 || (getLvl(activeLvl +1).getWidth() < getLvl().getWidth() && !won)){
		return;
	}

	glTranslatef(0.0f, 0.0f, -4.0f);
	glBindTexture(GL_TEXTURE_2D, getLvl(activeLvl + 1).getBackground());
	glBegin(GL_QUADS);
		glTexCoord2f(0, getLvl(activeLvl + 1).getBackgroundFaktorY()); glVertex3f(0, (float)getLvl(activeLvl + 1).getHeight(), -16.0f);
		glTexCoord2f(getLvl(activeLvl + 1).getBackgroundFaktorX(), getLvl(activeLvl + 1).getBackgroundFaktorY()); glVertex3f((float)getLvl(activeLvl + 1).getWidth(), (float)getLvl(activeLvl + 1).getHeight(), -16.0f);
		glTexCoord2f(getLvl(activeLvl + 1).getBackgroundFaktorX(), 0); glVertex3f((float)getLvl(activeLvl + 1).getWidth(), 0, -16.0f);
		glTexCoord2f(0, 0); glVertex3f(0, 0, -16.0f);
	glEnd();
	glTranslatef(0.0f, 0.0f, 4.0f);
}

//darkens the whole screen (multiply every color with darkness)
void Game::drawDarkScreen(GLfloat darkness){
	
	glBlendFunc(GL_DST_COLOR, GL_ZERO);
	glDisable(GL_DEPTH_TEST);
	glColor3f(darkness, darkness, darkness);
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_QUADS);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
	glEnd();
	glPopMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	
}

//Prints a text on the screen begining at position x, y (pixel)
GLvoid Game::print(const char *fmt, GLfloat x, GLfloat y, ...)					
{
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, y);									// Parses The String For Variables
	    vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	
	glLoadIdentity();
	glDisable(GL_BLEND);
	glTranslatef(0, 0, -1.0f);
	glRasterPos2f(x, y);
	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(font - 32);								// Sets The Base Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
	glEnable(GL_BLEND);
}

//sets the fram width (only for calculating purposes, not for actual resizing the window)
void Game::setFrameWidth(int width){
	this->frame_width = width;
}

//sets the frame height (only for calculation purposes)
void Game::setFrameHeight(int height){
	this->frame_height = height;
}

//sets the frame with and height
void Game::setFrameDimension(int width, int height){
	this->frame_width = width;
	this->frame_height = height;
}

//tells the game that the player wants to move in the left direction
void Game::moveLeft(void)
{
	this->move_left = true;
	this->move_right = false;
}

//tells the game that the player wants to move in the right direction
void Game::moveRight(void)
{
	this->move_left = false;
	this->move_right = true;
}

//tells the game that the player doesn't want to accelerate anymore
void Game::moveStop(void)
{
	this->move_left = false;
	this->move_right = false;
}

//tells the game if the player want to jump or not
void Game::setWantJump(bool jump)
{
	this->jump = jump;
}

//makes a jump (from a plain surface, not from an edge)
void Game::moveJump(GLfloat jump_speed)
{
	getLvl().getPlayer().setSpeedY(-jump_speed);
	getLvl().getPlayer().setOnFloor(false, 0, 0);
	getLvl().getPlayer().startJumpAnimation();
}

//returns if the player wants to move left, not if the player is actual moving left
bool Game::isMovingLeft(void)
{
	return move_left;
}

//returns if the player wants to move right, not if the player is actual moving right
bool Game::isMovingRight(void)
{
	return move_right;
}

//returns if the player wants to jump
bool Game::wantJump(void)
{
	return jump;
}

//sets if the game is paused or not
void Game::setPause(bool pause){
	if(!last_won){
		this->pause = pause;
	}
}

//returns true if the game is paused
bool Game::isPause(void){
	return pause;
}

//sets the fullscreenflag
void Game::setFullscreen(bool fullscreen)
{
	this->fullscreen = fullscreen;
}

//reutrns the fullscreenflag
bool Game::isFullscreen(void)
{
	return fullscreen;
}

//toggles betwen fullscreen and window mode
void Game::toggleFullscreen(void)
{
	KillGLWindow();						// Kill Our Current Window
	fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
	// Recreate Our OpenGL Window
	if (!CreateGLWindow("Smiley Wars (c) by Jakob Möderl",640,480,16,fullscreen))
	{
		return ;						// Quit If Window Was Not Created
	}
	getLvl().deleteTextures();
	getLvl(activeLvl + 1).deleteTextures();
	init(lvlNumber, getLvl(activeLvl));
	init(lvlNumber + 1, getLvl(activeLvl + 1));
}

//returns a reference to the player object
Player& Game::getPlayer(){
	return getLvl().getPlayer();
}

//returns the number of the actual lvl
int Game::getActualLvl(){
	return lvlNumber;
}

//returns the number of the active lvl
int Game::getActiveLvl(){
	return activeLvl;
}

//returns a reference to the lvl active object
Level& Game::getLvl(){
	return lvl[activeLvl];
}

//returns a reference to the lvl object
Level& Game::getLvl(int lvlNumber){
	lvlNumber = lvlNumber%lvlAnz;
	return lvl[lvlNumber];
}

//sets the font 
void Game::setFont(GLuint font){
	this->font = font;
}

//gets the used font
GLuint Game::getFont(){
	return font;
}

//returns true if the lvl is won
bool Game::isWon(){
	return won;
}

void Game::setJumpSpeed(GLfloat jump_speed){
	this->jump_speed = jump_speed;
}

GLfloat Game::getJumpSpeed(){
	return jump_speed;
}
