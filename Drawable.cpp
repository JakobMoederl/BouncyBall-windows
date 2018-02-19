#include "Drawable.h"

//standerd konstruktor
Drawable::Drawable()
{
	x = 0.0f;
	y = 0.0f;
	layer = -10;
	active = true;
}

//destruktor
Drawable::~Drawable(void)
{
	
}

GLfloat Drawable::getX(void){
	return x;
}
GLfloat Drawable::getY(void){
	return y;
}

//returns the drawing layer (disabled)
GLfloat Drawable::getLayer(void){
	return -layer;
}

//returns if the object is active
bool Drawable::isActive(){
	return active;
}

GLuint Drawable::getTexture(void)
{
	return texture;
}

void Drawable::setX(GLfloat x){
	this->x = x;
}
void Drawable::setY(GLfloat y){
	this->y = y;
}
void Drawable::setLayer(GLfloat layer){
	this->layer = -layer;
}

void Drawable::setActive(bool active){
	this->active = active;
}

void Drawable::setTexture(GLuint texture)
{
	this->texture = texture;
}