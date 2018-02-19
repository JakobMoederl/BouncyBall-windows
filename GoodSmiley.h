#ifndef _GOODSMILEY_H_
#define _GOODSMILEY_H_

#include "ball.h"

class GoodSmiley :
	public Ball
{
public:
	GoodSmiley(void);
	~GoodSmiley(void);

	void doKollision(Ball& object);
};

#endif