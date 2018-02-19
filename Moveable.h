#ifndef _MOVEABLE_H_
#define _MOVEABLE_H_

#include "Drawable.h"
class Moveable : public Drawable
{
public:
	Moveable(void);
	~Moveable(void);

	//moves the object deping on the time difference
	virtual void move(GLfloat time);

	//sets the speed in x direction
	void setSpeedX(GLfloat speedX);
	//sets the speed in y direction
	void setSpeedY(GLfloat speedY);
	//sets the maximum speed in x direction
	void setSpeedMaxX(GLfloat speedMaxX);
	//sets the maximum speed in x direction
	void setSpeedMaxY(GLfloat speedMaxY);
	
	//returns the speed in x direction
	GLfloat getSpeedX();
	//returns the speed in y direction
	GLfloat getSpeedY();
	//returns the maximum speed (x direction)
	GLfloat getSpeedMaxX();
	//returns the maximum speed (y direction)
	GLfloat getSpeedMaxY();
protected:
	//speed in x direction
	GLfloat speed_x;
	//speed in y direction
	GLfloat speed_y;
	//maximum speed in x direction
	GLfloat speed_max_x;
	//maximum speed in y direction
	GLfloat speed_max_y;
	
};

#endif