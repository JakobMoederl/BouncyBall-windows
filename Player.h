#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Ball.h"

class Player : public Ball
{
public:
	Player(void);
	~Player(void);

	//individual kollisions method
	void doKollision(Ball& object);
	//death method (used when de player dies)
	void death(void);

	void doAnimations(GLfloat time);

	void startJumpAnimation();
	void stopJumpAnimation();
private:
	GLfloat jump_animation_time;
	GLfloat jump_animation_max_time;

	GLfloat last_speed_y;

	bool jumpAnimation;

};

#endif