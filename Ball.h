#ifndef _BALL_H_
#define _BALL_H_

#include "Moveable.h"

enum BallDerivates{
	BALL_BASE, BALL_PLAYER, BALL_BAD, BALL_GOOD
};

class Ball : public Moveable
{
public:
	Ball(void);
	Ball(GLfloat x, GLfloat y);
	~Ball(void);

	//advanced move function which also calculates the ange the ball has to rotate 
	virtual void move(GLfloat time);

	//returns true if the Ball has a kollision with the other ball
	bool getKollision(Ball& object);

	//standart draw funtion, overide for advanced drawing (animations)
	virtual void draw(void);
	//does all the animations dependig on the time spent
	virtual void doAnimations(GLfloat time);
	//should override to compute on a kollion
	virtual void doKollision(Ball& object) = 0;
	//override if the ball is able to die (player, bad smiley ...)
	virtual void death(void);

	//returns the refelktionskoefizient (x direction)
	GLfloat getReflectionX(void);
	//returns the refelktionskoefizient (y direction)
	GLfloat getReflectionY(void);
	//returns the angel of the ball
	GLfloat getRotation();
	//returns the radius if the ball (in units)
	GLfloat getRadius(void);
	
	//sets the refelectionkoefizient (x direction)
	void setReflectionX(GLfloat reflection_x);
	//sets the refelectionkkoefizient (y direction)
	void setReflectionY(GLfloat reflection_y);
	//sets the angel of the ball
	void setRotation(GLfloat radius);
	//sets the radius of the ball (in units)
	void setRadius(GLfloat radius);
	
	//returns true if the ball is rolling on a block or the bottom of the lvl
	bool isOnFloor(void);
	//sets the onFloor flag in the specified area
	void setOnFloor(bool onFloor, GLfloat begin, GLfloat ende);
	
	virtual void setX(GLfloat x);

	void stopDeathAnimation();

	void setRotationEnable(bool rotation);

	bool isRotationEnable();

	//returns the class (type) of the ball
	BallDerivates getClass();
protected:
	//reflectionskoefizient (x direction)
	GLfloat reflection_x;
	//reflectionskoefizient (y direction)
	GLfloat reflection_y;
	
	GLfloat verzerrung_x;
	GLfloat verzerrung_y;

	//last x position for angle calculation
	GLfloat x_last;
	//angle
	GLfloat rotation;
	//sets if rotation is on or of
	bool rotation_on;
	//radius
	GLfloat radius;

	//on floor flag
	bool onFloor;
	//x where the floor begins on which the ball rolls
	GLfloat onFloor_begin;
	//x where the floor ends on wihch the ball rolls
	GLfloat onFloor_ende;

	//sets true when the death animation should be displayed
	bool deathAnimation;

	//the type of the ball
	BallDerivates type;
};

#endif