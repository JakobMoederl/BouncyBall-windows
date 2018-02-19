#ifndef _SPIKEBLOCK_H_
#define _SPIKEBLOCK_H_
#include "block.h"

#include "Ball.h"
class SpikeBlock : public Block
{
public:
	SpikeBlock(void);
	SpikeBlock(GLfloat x, GLfloat y, GLfloat width, GLfloat height);
	~SpikeBlock(void);

	void draw();

	//if ball is the player objects it uses ball.death
	void doKollision(Ball& object);

	SpikeBlock& operator=(SpikeBlock& other);
	
	void setSpikeTexture(GLuint texture);
	GLuint getSpikeTexture();
private:
	GLUquadricObj *sphere;
	GLuint spikeTexture;
	int i, j, k, l;
};
#endif
