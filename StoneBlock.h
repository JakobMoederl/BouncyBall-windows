#ifndef _STONEBLOCK_H_
#define _STONEBLOCK_H_

#include "block.h"

class StoneBlock : public Block
{
public:
	StoneBlock(void);
	StoneBlock(GLfloat x, GLfloat y, GLfloat width, GLfloat height);
	~StoneBlock(void);

	//individual kollisions method (do let the ball bounce back correct (FUCKING EDGES!!!)
	void doKollision(Ball& object);

	void setJumpSpeed(GLfloat);
	GLfloat getJumpSpeed();
private:
	GLfloat jump_speed;
};

#endif