#include "SpikeBlock.h"

#include "Game.h"
#define _USE_MATH_DEFINES
#include <math.h>
SpikeBlock::SpikeBlock(void)
{
	sphere=gluNewQuadric();
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluQuadricTexture(sphere, GL_TRUE);
	gluQuadricDrawStyle(sphere, GLU_FILL);
}

SpikeBlock::SpikeBlock(GLfloat x, GLfloat y, GLfloat width, GLfloat height){
	sphere=gluNewQuadric();
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluQuadricTexture(sphere, GL_TRUE);
	gluQuadricDrawStyle(sphere, GLU_FILL);
	this->x = x;
	this->y = y;
	this->width = width;
	this->height =height;
}

SpikeBlock::~SpikeBlock(void)
{
	if(sphere != NULL){
		gluDeleteQuadric(sphere);
		sphere = NULL;
	}
}

SpikeBlock& SpikeBlock::operator=(SpikeBlock& other){
	x = other.getX();
	y = other.getY();
	setLayer(other.getLayer());

	width = other.getWidth();
	height = other.getHeight();
	depth = other.getDepth();

	texture = other.getTexture();
	spikeTexture = other.getSpikeTexture();
	active = other.isActive();

	speed_x = other.getSpeedX();
	speed_y = other.getSpeedY();
	speed_max_x = other.getSpeedMaxX();
	speed_max_y = other.getSpeedMaxY();

	reflection_x = other.getReflectionX();
	reflection_y = other.getReflectionY();

	sphere = gluNewQuadric();
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluQuadricTexture(sphere, GL_TRUE);
	gluQuadricDrawStyle(sphere, GLU_FILL);
	return *this;
}
void SpikeBlock::draw(){
	glPushMatrix();
	glTranslatef(x + 0.5f, y + 0.5f, -10 - 0.5f);
	for(k=0; k<depth; k++){
		for(j=0; j<height; j++){
			for(i=0; i<width; i++){
				glPushMatrix();
				//kugel in der mitte
				glBindTexture(GL_TEXTURE_2D, texture);
				gluSphere(sphere, 0.35f,16,16);
				
				//spitzen
				glBindTexture(GL_TEXTURE_2D, spikeTexture);
				//acht Spitzen um die x achse Zeichnen
				gluCylinder(sphere,0.2f,0.0f,0.5f,6,1);
				glRotatef(45.0f, -1.0f, 0.0f, 0.0f);
				gluCylinder(sphere,0.15f,0.0f,0.71f,6,1);
				glRotatef(45.0f, -1.0f, 0.0f, 0.0f);
				gluCylinder(sphere,0.2f,0.0f,0.5f,6,1);
				glRotatef(45.0f, -1.0f, 0.0f, 0.0f);
				gluCylinder(sphere,0.15f,0.0f,0.71f,6,1);
				glRotatef(45.0f, -1.0f, 0.0f, 0.0f);
				gluCylinder(sphere,0.2f,0.0f,0.5f,6,1);
				glRotatef(45.0f, -1.0f, 0.0f, 0.0f);
				gluCylinder(sphere,0.15f,0.0f,0.71f,6,1);
				glRotatef(45.0f, -1.0f, 0.0f, 0.0f);
				gluCylinder(sphere,0.2f,0.0f,0.5f,6,1);
				glRotatef(45.0f, -1.0f, 0.0f, 0.0f);
				gluCylinder(sphere,0.15f,0.0f,0.71f,6,1);
				glRotatef(45.0f, -1.0f, 0.0f, 0.0f);
				
				//sechs spitzen um die y achse zeichnen
				glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
				gluCylinder(sphere,0.2f,0.0f,0.5f,6,1);
				glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
				gluCylinder(sphere,0.15f,0.0f,0.71f,6,1);
				glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
				gluCylinder(sphere,0.15f,0.0f,0.71f,6,1);
				glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
				gluCylinder(sphere,0.2f,0.0f,0.5f,6,1);
				glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
				gluCylinder(sphere,0.15f,0.0f,0.71f,6,1);
				glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
				gluCylinder(sphere,0.15f,0.0f,0.71f,6,1);
				glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
				
				glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
				gluCylinder(sphere,0.15f,0.0f,0.71f,6,1);
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				gluCylinder(sphere,0.15f,0.0f,0.71f,6,1);
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				gluCylinder(sphere,0.15f,0.0f,0.71f,6,1);
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				gluCylinder(sphere,0.15f,0.0f,0.71f,6,1);

				glPopMatrix();
				glTranslatef(1.0f, 0.0f, 0.0f);
			}
			glTranslatef((float)i, 1.0f, 0.0f);
		}
		glTranslatef(0.0f, (float)-j, 1.0f);
	}
	glPopMatrix();
}

//specific kollision methode (sets the player death on kollision)
void SpikeBlock::doKollision(Ball& object){
	extern Game game;
	if(object.getClass() == BALL_PLAYER){
		game.getPlayer().death();
	} else if(object.getClass() == BALL_BAD){
		extern Game game;
		GLfloat alpha, beta, betrag;

		betrag = sqrtf(powf(object.getSpeedX(), 2) + powf(object.getSpeedY(), 2));
		if(object.getClass() == BALL_PLAYER && game.wantJump() && betrag < 7.0f){
			betrag = 7.0f;
		}
		switch(kollisionsType){
		case HT_TOP:
			if(game.wantJump() && object.getClass() == BALL_PLAYER){
				game.moveJump(7.0f);
			} else {
				object.setSpeedY(-object.getSpeedY() * object.getReflectionY());
				if(object.getSpeedY() < 1.5f){
					object.setSpeedY(0);
					object.setOnFloor(true, x, x+width);
				}
			}
			object.setY(y + height + object.getRadius());
			break;
		case HT_BOTTOM:
			object.setSpeedY(-object.getSpeedY() * object.getReflectionY());
			object.setY(y - object.getRadius());
			break;
		case HT_LEFT:
			object.setSpeedX(-object.getSpeedX() * object.getReflectionX());
			object.setX(x - object.getRadius());
			break;
		case HT_RIGHT:
			object.setSpeedX(-object.getSpeedX() * object.getReflectionX());
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

			object.setSpeedX(betrag * cosf(beta) * object.getReflectionX());
			object.setSpeedY(betrag * sinf(beta) * object.getReflectionY());
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

			object.setSpeedX(betrag * cosf(beta) * object.getReflectionX());
			object.setSpeedY(betrag * sinf(beta) * object.getReflectionY());
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

			object.setSpeedX(betrag * cosf(beta) * object.getReflectionX());
			object.setSpeedY(betrag * sinf(beta) * object.getReflectionY());
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

			object.setSpeedX(betrag * cosf(beta) * object.getReflectionX());
			object.setSpeedY(betrag * sinf(beta) * object.getReflectionY());
			break;
		}
	}
}

void SpikeBlock::setSpikeTexture(GLuint texture){
	spikeTexture = texture;
}

GLuint SpikeBlock::getSpikeTexture(){
	return spikeTexture;
}

