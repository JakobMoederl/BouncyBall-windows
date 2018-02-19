#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "ball.h"
class Enemy : public Ball
{
public:
	Enemy(void);
	~Enemy(void);

	void doKollision(Ball& object);

	void move(GLfloat time);
	void setX(GLfloat x);

	void setTravelLength(GLfloat travel_length);
	GLfloat getTravelLength();
	
	GLfloat getStartX();
	GLfloat getStartY();
	GLfloat getStartSpeedX();
	GLfloat getStartSpeedY();

	void setStartX(GLfloat);
	void setStartY(GLfloat);
	void setStartSpeedX(GLfloat);
	void setStartSpeedY(GLfloat);
	
	void setKollisionXEnable(bool kollision_x);
	bool isKolliosionXEnable();

	void reset();

	bool turnsOnCliff();
	void setTurnOnCliff(bool);

private:
	GLfloat travel_length;
	
	GLfloat start_x;
	GLfloat start_y;
	GLfloat start_speed_y;
	GLfloat start_speed_x;

	bool kollision_x;

	bool turn_on_cliff;
};
#endif
