#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "Ball.h"
#include "Moveable.h"

enum HittingType{
	HT_NO_KOLLISION, HT_TOP, HT_BOTTOM, HT_LEFT, HT_RIGHT, HT_LOWER_LEFT_CORNER, HT_UPPER_LEFT_CORNER, HT_UPPER_RIGHT_CORNER, HT_LOWER_RIGHT_CORNER
};

class Block : public Moveable
{
public:
	Block(void);
	Block(GLfloat x, GLfloat y, GLfloat width, GLfloat height);
	~Block(void);

	//returns true if there is a kollision with the Ball
	//sets the KollisionsType value to indicate in which area is of the block is the kollision
	bool getKollisionEdge(Ball& object);
	bool getKollisionCorner(Ball& object);
	HittingType getKollisionType();
	//standert methode of the block
	virtual void draw(void);
	//abstract kollisions methode
	virtual void doKollision(Ball& object) = 0;

	//sets the width of the block
	void setWidth(GLfloat width);
	//sets the height of the block
	void setHeight(GLfloat height);
	void setDepth(GLfloat depth);

	//sets the reflectionkoefizient (x direction)
	void setReflectionX(GLfloat reflection_x);
	//sets the reflectionkoefizient (y direction)
	void setReflectionY(GLfloat reflection_y);

	//returns the width of the block
	GLfloat getWidth();
	//returns the height of the block
	GLfloat getHeight();
	GLfloat getDepth();

	//returns the reflectionkoefizient (x direction)
	GLfloat getReflectionX();
	//returns the reflectionkoefizient (y direction)
	GLfloat getReflectionY();
protected:
	//width of the block
	GLfloat width;
	//height of the block
	GLfloat height;
	//deapth of the block
	GLfloat depth;

	//reflectionkoefizient (x direction)
	GLfloat reflection_x;
	//reflectionkoefizient (y direction)
	GLfloat reflection_y;

	//indicates in which area of the block is a kollision (top, bottom, corner...)
	HittingType kollisionsType;
};

#endif