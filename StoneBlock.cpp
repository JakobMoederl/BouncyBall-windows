#include "StoneBlock.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "Game.h"

StoneBlock::StoneBlock(void)
{
	jump_speed = 10.0f;
}

StoneBlock::StoneBlock(GLfloat x, GLfloat y, GLfloat width, GLfloat height){
	this->x = x;
	this->y = y;
	this->width = width;
	this->height =height;
	jump_speed = 10.0f;
}

StoneBlock::~StoneBlock(void)
{
}

//specific kollisions methode (reflects the ball)
void StoneBlock::doKollision(Ball& object){
	extern Game game;
	GLfloat alpha, beta, betrag;

	betrag = sqrtf(powf(object.getSpeedX(), 2) + powf(object.getSpeedY(), 2));
	if(object.getClass() == BALL_PLAYER && game.wantJump() && betrag < jump_speed && (kollisionsType == HT_UPPER_LEFT_CORNER || kollisionsType == HT_UPPER_RIGHT_CORNER)){
		betrag = jump_speed;
	}
	switch(kollisionsType){
	case HT_TOP:
		if(game.wantJump() && object.getClass() == BALL_PLAYER){
			game.moveJump(-jump_speed);
		} else {
			object.setSpeedY(-object.getSpeedY() * object.getReflectionY() * reflection_y);
			if(object.getSpeedY() < 1.5f){
				object.setSpeedY(0);
				object.setOnFloor(true, x, x+width);
				game.setJumpSpeed(jump_speed);
			}
		}
		object.setY(y + height + object.getRadius());
		break;
	case HT_BOTTOM:
		object.setSpeedY(-object.getSpeedY() * object.getReflectionY() * reflection_y);
		object.setY(y - object.getRadius());
		break;
	case HT_LEFT:
		object.setSpeedX(-object.getSpeedX() * object.getReflectionX() * reflection_x);
		object.setX(x - object.getRadius());
		break;
	case HT_RIGHT:
		object.setSpeedX(-object.getSpeedX() * object.getReflectionX() * reflection_x);
		object.setX(x + width + object.getRadius());
		break;
	case HT_LOWER_LEFT_CORNER:
		if(object.getX() > x){
			object.setX(x - 0.001f);
		}
		if(object.getY() > y){
			object.setY(y);
		}

		alpha = (float) (M_PI) + atanf( (object.getY() - y) / (object.getX() - x) );
		object.setX(x + object.getRadius() * cosf(alpha));
		object.setY(y + object.getRadius() * sinf(alpha));
		
		if(object.getSpeedX() == 0){
			object.setSpeedX(0.001f);
		}
		beta = atanf(object.getSpeedY() / object.getSpeedX());
		if(object.getSpeedX() > 0){
			beta += (float) M_PI;
		}
		beta = alpha - (beta - alpha);

		object.setSpeedX(betrag * cosf(beta) * object.getReflectionX() * reflection_x);
		object.setSpeedY(betrag * sinf(beta) * object.getReflectionY() * reflection_y);
		break;
	case HT_UPPER_LEFT_CORNER:
		if(object.getX() > x){
			object.setX(x - 0.001f);
		}
		if(object.getY() < y + height){
			object.setY(y + height);
		}
		alpha = (float) M_PI + atanf((object.getY() - (y + height)) / (object.getX() - x));
		
		object.setX(x + object.getRadius() * cosf(alpha));
		object.setY(y + height + object.getRadius() * sinf(alpha));
		
		if(object.getSpeedX() == 0){
			object.setSpeedX(0.001f);
		}
		beta = atanf(object.getSpeedY() / object.getSpeedX());
		if(object.getSpeedX() > 0){
			beta += (float) M_PI;
		}
		beta = alpha - (beta - alpha);

		object.setSpeedX(betrag * cosf(beta) * object.getReflectionX() * reflection_x);
		object.setSpeedY(betrag * sinf(beta) * object.getReflectionY() * reflection_y);
		break;
	case HT_UPPER_RIGHT_CORNER:
		if(object.getX() < x + width){
			object.setX(x + width + 0.001f);
		}
		if(object.getY() < y + height){
			object.setY(y + height);
		}
		alpha = atanf( (object.getY() - (y + height)) / (object.getX() - (x + width)) );

		object.setX(x + width + object.getRadius() * cosf(alpha));
		object.setY(y + height + object.getRadius() * sinf(alpha));
		
		if(object.getSpeedX() == 0){
			object.setSpeedX(-0.001f);
		}
		beta = atanf(object.getSpeedY() / object.getSpeedX());
		if(object.getSpeedX() > 0){
			beta += (float) M_PI;
		}
		beta = alpha - (beta - alpha);

		object.setSpeedX(betrag * cosf(beta) * object.getReflectionX() * reflection_x);
		object.setSpeedY(betrag * sinf(beta) * object.getReflectionY() * reflection_y);
		break;
	case HT_LOWER_RIGHT_CORNER:
		if(object.getX() < x + width){
			object.setX(x + width + 0.001f);
		}
		if(object.getY() > y){
			object.setY(y);
		}
		alpha = atanf( (object.getY() - y) / (object.getX() - (x + width)) );
		object.setX(x + width + object.getRadius() * cosf(alpha));
		object.setY(y + object.getRadius() * sinf(alpha));
		
		if(object.getSpeedX() == 0){
			object.setSpeedX(-0.001f);
		}
		beta = atanf(object.getSpeedY() / object.getSpeedX());
		if(object.getSpeedX() > 0){
			beta += (float) M_PI;
		}
		beta = alpha - (beta - alpha);

		object.setSpeedX(betrag * cosf(beta) * object.getReflectionX() * reflection_x);
		object.setSpeedY(betrag * sinf(beta) * object.getReflectionY() * reflection_y);
		break;
	}
}

void StoneBlock::setJumpSpeed(GLfloat jump_speed){
	this->jump_speed = jump_speed;
}

GLfloat StoneBlock::getJumpSpeed(){
	return jump_speed;
}