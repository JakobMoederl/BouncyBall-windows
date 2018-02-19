#include "Enemy.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "Game.h"

Enemy::Enemy(void)
{
	type = BALL_BAD;
	radius = 0.38f;

	travel_length = 0;
	start_x = 0;
	start_y = 0;

	start_speed_x = 0;
	start_speed_y = 0;

	reflection_x = 1.0f;
	reflection_y = 1.0f;
	
	kollision_x = true;
	turn_on_cliff = false;
}


Enemy::~Enemy(void)
{
}

void Enemy::doKollision(Ball& other){
	if(other.getClass() == BALL_PLAYER){
		if(other.getSpeedY() < getSpeedY()){
			death();
			if(other.getX() == x){
				other.setX(x + 0.001f);
			}
			GLfloat betrag = sqrtf(powf(other.getSpeedX(), 2) + powf(other.getSpeedY(), 2));
			GLfloat alpha = atanf((other.getY() - y)/(other.getX() - x));
			if(other.getX() - x < 0){
				alpha += M_PI;
			}
			if(other.getSpeedX() == 0){
				other.setSpeedX(0.001f);
			}
			GLfloat beta = atanf(-other.getSpeedY()/other.getSpeedX());
			if(other.getSpeedX() < 0){
				beta += M_PI;
			}
			beta = alpha - (beta - alpha);

			other.setSpeedX(cos(beta) * betrag);
			other.setSpeedY(sin(beta) * betrag);
		} else {
			other.death();
		}
	}
}


void Enemy::setX(GLfloat x){
	x_last = this->x;
	this->x = x;

	rotation -= 360.0f*(x-x_last)/(2.0f* (float)M_PI*radius);

	if(rotation >360.0f){
		rotation -= 360.0f;
	}

	if(travel_length != 0){
		if(this->x > start_x + travel_length){
			speed_x = abs(speed_x);
		} else if(this->x < start_x){
			speed_x = -abs(speed_x);
		}
	}
}

void Enemy::move(GLfloat time){
	setX(x + (time*speed_x));
	setY(y + (time*speed_y));
	
	if(onFloor && x < onFloor_begin){
		if(turn_on_cliff){
			speed_x = abs(speed_x);
		} else {
			onFloor = false;
		}
	} else if(onFloor && x > onFloor_ende){
		if(turn_on_cliff){
			speed_x = -abs(speed_x);
		} else {
			onFloor = false;
		}
	}
}

void Enemy::reset(){
	active = true;
	x = start_x;
	y = start_y;
	speed_x = start_speed_x;
	speed_y = start_speed_y;
	setRotation(0);
}

void Enemy::setTravelLength(GLfloat travel_lenght){
	this->travel_length = travel_lenght;
}

GLfloat Enemy::getTravelLength(){
	return travel_length;
}

GLfloat Enemy::getStartX(){
	return start_x;
}
GLfloat Enemy::getStartSpeedX(){
	return start_speed_x;
}
GLfloat Enemy::getStartY(){
	return start_y;
}
GLfloat Enemy::getStartSpeedY(){
	return start_speed_y;
}

void Enemy::setStartX(GLfloat x){
	this->start_x = x;
}
void Enemy::setStartSpeedX(GLfloat speed_x){
	this->start_speed_x = speed_x;
}
void Enemy::setStartY(GLfloat y){
	this->start_y = y;
}
void Enemy::setStartSpeedY(GLfloat speed_y){
	this->start_speed_y = speed_y;
}

void Enemy::setKollisionXEnable(bool kollision_x){
	this->kollision_x = kollision_x;
}

bool Enemy::isKolliosionXEnable(){
	return kollision_x;
}

bool Enemy::turnsOnCliff(){
	return turn_on_cliff;
}

void Enemy::setTurnOnCliff(bool turn_on_cliff){
	this->turn_on_cliff = turn_on_cliff;
}