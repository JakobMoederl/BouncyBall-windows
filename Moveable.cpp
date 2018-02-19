#include "Moveable.h"

Moveable::Moveable(void)
{
	speed_x = 0;
	speed_y = 0;
	speed_max_x = 10;
	speed_max_y = 10;
}


Moveable::~Moveable(void)
{
}

//moves the object depending on the time since the last move
void Moveable::move(GLfloat time){
	x += (time*speed_x);
	y += (time*speed_y);
}

//Sets the x speed, if the x speed is greater then the max speed value it is set to max speed value
void Moveable::setSpeedX(GLfloat speedX)
{
	if(speedX > speed_max_x){
		speed_x = speed_max_x;
	} else if(speedX < -speed_max_x){
		speed_x = -speed_max_x;
	} else {
		speed_x = speedX;
	}
}

//Sets the y speed, if the y speed is greater then the max speed value it is set to max speed value
void Moveable::setSpeedY(GLfloat speedY)
{
	if(speedY > speed_max_y){
		speed_y = speed_max_y;
	} else if(speedY < -speed_max_y){
		speed_y = -speed_max_y;
	} else {
		speed_y = speedY;
	}
}


void Moveable::setSpeedMaxX(GLfloat speedMaxX)
{
	speed_max_x = speedMaxX;
}


void Moveable::setSpeedMaxY(GLfloat speedMaxY)
{
	speed_max_y = speedMaxY;
}

GLfloat Moveable::getSpeedX(){
	return speed_x;
}

GLfloat Moveable::getSpeedY(){
	return speed_y;
}

GLfloat Moveable::getSpeedMaxX(){
	return speed_max_x;
}

GLfloat Moveable::getSpeedMaxY(){
	return speed_max_y;
}