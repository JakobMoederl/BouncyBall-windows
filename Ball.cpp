#include "Ball.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "Game.h"

//standardkonstrucktor
Ball::Ball(void)
{
	onFloor = false;
	active = true;
	onFloor_begin = 0;
	onFloor_ende = 0;
	reflection_x = 0.8f;
	reflection_y = 0.6f;

	x_last = x;
	rotation = 0;
	radius = 0.3f;
	
	type = BALL_BASE;
	layer = -10.5f;
	
	verzerrung_x = 1.0f;
	verzerrung_y = 1.0f;
	active = true;
	deathAnimation = false;
	rotation_on = true;
}

//Allgemeiner Konstrucktro (sets also x and y position of the ball)
Ball::Ball(GLfloat x, GLfloat y){
	this-> x = x;
	this-> y = y;
	this ->x_last = x;

	this->rotation = 0;
	this->reflection_x = 0.8f;
	this->reflection_y = 0.6f;

	radius = 0.3f;
	
	onFloor = false;
	onFloor_begin = 0;
	onFloor_ende = 0;

	active = true;
	deathAnimation = false;

	type = BALL_BASE;

	verzerrung_x = 1.0f;
	verzerrung_y = 1.0f;

	layer = -10.5f;

	rotation_on = true;
}

//destruktor
Ball::~Ball(void)
{
}

//draw funtion, draws the ball at the right position and angel
void Ball::draw(void){
	extern Game game;
	if(!active && !deathAnimation){
		return;
	}

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,texture);
	glTranslatef(x, y, 0.0f);
	glScalef(verzerrung_x, verzerrung_y, 1.0f);
	if(rotation_on){
		glRotatef(rotation, 0.0f, 0.0f, 1.0f);
	}
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);	glVertex3f(-radius,  radius, (GLfloat)layer);				// Top Left
		glTexCoord2f(1.0f, 0.0f);	glVertex3f( radius,  radius, (GLfloat)layer);				// Top Right
		glTexCoord2f(1.0f, 1.0f);	glVertex3f( radius, -radius, (GLfloat)layer);				// Bottom Right
		glTexCoord2f(0.0f, 1.0f);	glVertex3f(-radius, -radius, (GLfloat)layer);				// Bottom Left
	glEnd();
	glPopMatrix();
}

//does the death animation (if ball is death)
void Ball::doAnimations(GLfloat time){
	extern Game game;
	if(!active && !deathAnimation){
		return;
	}
	if(deathAnimation){
		deathAnimation = false;
	}
}

//calculates if the ball object objects kollidiet with this ball object (returns true if so)
bool Ball::getKollision(Ball& object){
	if(!active || !object.isActive()){
		return false;
	}
	if(sqrtf(powf(x - object.getX(), 2) + powf(y - object.getY(), 2)) < radius + object.getRadius()){
		return true;
	}
	return false;
}

//should be called when the ball dies (or is catch up in case it is a good smiley)
void Ball::death(){
	deathAnimation = true;
	active = false;
}

//moves the ball (with rotation) and looks if it is still on a floor
void Ball::move(GLfloat time){
	setX(x + (time*speed_x));
	setY(y + (time*speed_y));
	
	if(x < onFloor_begin || x > onFloor_ende){
		onFloor = false;
	}
}

//sets the reflection factor for bouncing against a vertical wall (horizontal speed)
void Ball::setReflectionX(GLfloat reflection_x)
{
	this->reflection_x = reflection_x;
}

//sets the reflections factor for bouncing against a horizontal wall (vertical speed)
void Ball::setReflectionY(GLfloat reflection_y)
{
	this->reflection_y = reflection_y;
}

//Sets the radius of the ball
void Ball::setRadius(GLfloat radius)
{
	this->radius = radius;
}

//sets the angle of the ball
void Ball::setRotation(GLfloat rotation)
{
	this->rotation = rotation;
	this->x_last = this->x;
}

//get reflection faktor of the ball (for vertical reflection)
GLfloat Ball::getReflectionX(void)
{
	return reflection_x;
}

//returns the reflection factor of the ball (for horizontal reflection)
GLfloat Ball::getReflectionY(void)
{
	return reflection_y;
}

//gets the angle of the ball
GLfloat Ball::getRotation(){
	return rotation;
}

//gets the radius of the ball
GLfloat Ball::getRadius(void)
{
	return radius;
}

//sets if the ball is on floor or not (and also from where to where the floor is wher the ball is currently on)
void Ball::setOnFloor(bool onFloor, GLfloat begin, GLfloat ende)
{
	this->onFloor = onFloor;
	onFloor_begin = begin;
	onFloor_ende = ende;
}

//returns true if the ball is currently on floor
bool Ball::isOnFloor(void)
{
	return onFloor;
}

//sets the X position (alos calculates the angle to rotate the ball)
void Ball::setX(GLfloat x){
	x_last = this->x;
	this->x = x;

	rotation -= 360.0f*(x-x_last)/(2.0f* (float)M_PI*radius);

	if(rotation >360.0f){
		rotation -= 360.0f;
	}
}

//gets the type of the ball (for kollision)
BallDerivates Ball::getClass(){
	return type;
}

//stops the death animation (if there is any)
void Ball::stopDeathAnimation(){
	deathAnimation = false;
	verzerrung_x = 1.0f;
	verzerrung_y = 1.0f;
}

//sets if the rotation is enabled or not
void Ball::setRotationEnable(bool rotation){
	this->rotation_on = rotation;
	if(!rotation){
		this->rotation = 0;
	}
}

//gets if rotation is enabled for this ball
bool Ball::isRotationEnable(){
	return rotation_on;
}