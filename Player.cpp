#include "Player.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "Game.h"

Player::Player(void)
{
	radius = 0.4f;
	type = BALL_PLAYER;

	verzerrung_x = 1.0f;
	verzerrung_y = 1.0f;
	last_speed_y = speed_y;
	jump_animation_max_time = 0.3f;
	jumpAnimation = false;
	rotation_on = true;
}


Player::~Player(void)
{
	
}

//kollisions methode of player, catches smileys up
void Player::doKollision(Ball& object){
	
}

//does bounce animation
void Player::doAnimations(GLfloat time){
	if(!active && !deathAnimation){
		return;
	}
	if(deathAnimation){
		//death animation
		setSpeedY(speed_y - 15*time);
		rotation += + 1500.0f * time;
		layer += 3.0f * time;
		move(time);
	} else if(jumpAnimation){
		jump_animation_time += time;
		if(jump_animation_time > jump_animation_max_time){
			jumpAnimation = false;
			verzerrung_x = 1.0f;
			verzerrung_y = 1.0f;
		} else {
			verzerrung_x = 0.66f + abs((jump_animation_time - jump_animation_max_time*0.5f))*2.0f/(3*jump_animation_max_time);
			verzerrung_y = 1.33f - abs((jump_animation_time - jump_animation_max_time*0.5f))*2.0f/(3*jump_animation_max_time);
		}
	}
}

//death methode (looses game and starts death animation)
void Player::death(){
	extern Game game;
	if(game.isWon()){
		return;
	}
	active = false;
	deathAnimation = true;
	stopJumpAnimation();
	game.loose();
}

//starts the animation to bounce when jumping
void Player::startJumpAnimation(){
	jumpAnimation = true;
	jump_animation_time = 0;
	verzerrung_x = 1.0f;
	verzerrung_y = 1.0f;
}

//stops the jumping animation (sets the verzerrung to 1)
void Player::stopJumpAnimation(){
	jumpAnimation = false;
	verzerrung_x = 1.0f;
	verzerrung_y = 1.0f;
}