#include "Level.h"

#include "Game.h"

#include <string>
#include <SOIL.h>

using namespace std;

//standard Konstruktor
Level::Level(void)
{
	width = 100;
	height = 100;
	
	stoneBlocks = NULL;
	stoneBlocksAnz = 0;
	
	spikeBlocks = NULL;
	spikeBlocksAnz = 0;

	goodSmileys = NULL;
	goodSmileysAnz = 0;

	enemys = NULL;
	enemysAnz = 0;

	texture = NULL;
	textureAnz = 0;

	bg_repeated_y = true;
	bg_enabled = true;
	bg_faktor_x = 3.0f;
	bg_faktor_y = 1.0f;

	player.setSpeedX(0);
	player.setSpeedY(0);
	player.setOnFloor(false, 0, 0);
	player.setLayer(10.5f);
	player.setRotation(0);
	player.stopJumpAnimation();
	player.stopDeathAnimation();
	player.setActive(true);
	player.setRadius(0.4f);
	player.setSpeedMaxX(7.5f);
	player.setSpeedMaxY(20);
}

//destruktor (calls a cleanup)
Level::~Level(void)
{
	cleanup();
	deleteTextures();
}

//loads the lvl file from the given ifstream
bool Level::loadLvl(ifstream& lvlFile){
	cleanup();

	extern Game game;			//reference to the main game object
	string line;
	Symbol* symbols;
	int symbolsAnz;
	if(!lvlFile.is_open()){
		return false;
	}

	//reading the configurations of the lvl
	while(lvlFile.good()){
		getline(lvlFile, line);
		if(line.find('{') != string::npos){
			if(line.find("textures") != string::npos){
				readTextures(lvlFile);
			} else if(line.find("background") != string::npos){
				readBackground(lvlFile);
			} else if(line.find("symbols") != string::npos){
				symbolsAnz = getSymbolAnzahl(lvlFile);
				symbols = readSymbols(lvlFile, symbolsAnz);
				for(int i=0; i < symbolsAnz; i++){
					if(symbols[i].type == BLOCK_STONE){
						stoneBlocksAnz += symbols[i].anzahl;
					} else if (symbols[i].type == BLOCK_SPIKE){
						spikeBlocksAnz += symbols[i].anzahl;
					} else if (symbols[i].type == SMILEY_CATCH){
						goodSmileysAnz += symbols[i].anzahl;
					} else if(symbols[i].type == SMILEY_ENEMY){
						enemysAnz += symbols[i].anzahl;
					}
				}
				
				stoneBlocksAnz += 4;
				stoneBlocks = new StoneBlock[stoneBlocksAnz];
				spikeBlocks = new SpikeBlock[spikeBlocksAnz];
				goodSmileys = new GoodSmiley[goodSmileysAnz];
				enemys = new Enemy[enemysAnz];
						
			} else if(line.find("level") != string::npos){
				readLvl(lvlFile, symbols, symbolsAnz);
			}
		}
	}
	
	stoneBlocks[0] = StoneBlock(-1.0f, 0, 1.0f, (float)height);
	stoneBlocks[0].setTexture(stoneBlocks[4].getTexture());
	stoneBlocks[0].setLayer(0);
	stoneBlocks[0].setDepth(20);

	stoneBlocks[1] = StoneBlock(0.0f, -1.0f, (float)width, 1.0f);
	stoneBlocks[1].setTexture(stoneBlocks[4].getTexture());
	stoneBlocks[1].setLayer(0);
	stoneBlocks[1].setDepth(20);
	
	stoneBlocks[2] = StoneBlock((float)width, 0, 1.0f, (float)height);
	stoneBlocks[2].setTexture(stoneBlocks[4].getTexture());
	stoneBlocks[2].setLayer(0);
	stoneBlocks[2].setDepth(15);

	stoneBlocks[3] = StoneBlock((float)width, 0, 1.0f, (float)height);
	stoneBlocks[3].setTexture(stoneBlocks[4].getTexture());
	stoneBlocks[3].setLayer(15);
	stoneBlocks[3].setDepth(6);

	for(int i=0; i<symbolsAnz; i++){
		if(symbols[i].object.stoneBlock != NULL){
			delete (void*)symbols[i].object.stoneBlock;
		}
	}
	delete [] symbols;
	lvlFile.close();
	return true;
}

//reads the textures of a lvl File
void Level::readTextures(ifstream& in){
	streamoff pos = in.tellg();
	string line;
	int i = 1; 
	textureAnz = 1;
	getline(in, line);
	while(in.good() && line.find('}') == string::npos){
		while(!line.empty() && (line[0] == '\t' || line[0] == ' ')){
			line.erase(0, 1);
		}
		if(!line.empty()){
			textureAnz++;
		}
		getline(in, line);
	}
	in.seekg(pos);

	texture = new GLuint[textureAnz];

	getline(in, line);
	for(i = 1; i < textureAnz && line.find('}') == string::npos && in.good(); ){
		while(!line.empty() && (line[0] == '\t' || line[0] == ' ')){
			line.erase(0, 1);
		}
		if(!line.empty()){
			loadTexture(line.c_str(), i);
			i++;
		}
		getline(in, line);
	}
}

//reads the information of the background (image, repeted, darkend and size)
void Level::readBackground(ifstream& in){
	extern Game game;
	string line;
	bg_repeated_y = true;

	getline(in, line);
	while(line.empty() && in.good()){ //skip empty lines
		getline(in, line);
	}
	while(line[0] == '\t' || line[0] == ' '){
		line.erase(0, 1);
	}
	loadTexture(line.c_str(), 0);

	getline(in, line);
	while(line.find('}') == string::npos && in.good()){
		while(!line.empty() && (line[0] == '\t' || line[0] == ' ')){
			line.erase(0, 1);
		}
		if(isdigit(line[0])){
			sscanf(line.c_str(), "%fx%f", &bg_width, &bg_height);
		} else if (line.find("repeat") != string::npos){
			sscanf(line.c_str(), "repeat %f", &bg_faktor_x);
		} else if (line.find("no_repeat_y") != string::npos){
			bg_repeated_y = false;
		}
		getline(in, line);
	}

	bg_faktor_y = bg_faktor_x * -(float)height/(float)width * bg_width/bg_height;
}

//reads how many different symbols are used in the lvl File
int Level::getSymbolAnzahl(ifstream& in){
	streamoff pos = in.tellg();
	string line;
	int anzahl = 0;
	getline(in, line);
	while(line.find('}') == string::npos && in.good()){
		while(!line.empty() && (line[0] == '\t' || line[0] == ' ')){
			line.erase(0, 1);
		}
		if(!line.empty()){
			anzahl++;
			if(line.find('{') != string::npos){
				while(line.find('}') == string::npos){
					getline(in,line);
				}
			}
		}
		getline(in, line);
	}
	in.seekg(pos);

	return anzahl;
}

//reads and interprets the symbols
Symbol* Level::readSymbols(ifstream& in, int anzahl){
	string line;
	Symbol* symbols = new Symbol[anzahl];
	streamoff pos;
	int i;
	int j;
	int help;
	GLfloat helpf;
	char actualSymbol = '\0';

	getline(in, line);
	for(i = 0; i < anzahl && line.find('}') == string::npos && in.good(); ){
		while(!line.empty() && (line[0] == '\t' || line[0] == ' ')){
			line.erase(0, 1);
		}
		if(!line.empty()){
			symbols[i].symbol = line[0];
			line.erase(0, 2);
			if(line.find("StoneBlock") != string::npos){
				line.erase(0, 11);
				symbols[i].type = BLOCK_STONE;
				symbols[i].object.stoneBlock = new StoneBlock;
				sscanf(line.c_str(), "%i", &symbols[i].texture);
				symbols[i].object.stoneBlock->setTexture(texture[symbols[i].texture]);
			} else if (line.find("SpikeBlock") != string::npos){
				line.erase(0, 11);
				symbols[i].type = BLOCK_SPIKE;
				symbols[i].object.spikeBlock = new SpikeBlock;
				sscanf(line.c_str(), "%i", &symbols[i].texture);
				symbols[i].object.spikeBlock->setTexture(texture[symbols[i].texture]);
			} else if (line.find("Smiley") != string::npos){
				line.erase(0, 7);
				symbols[i].type = SMILEY_CATCH;
				symbols[i].object.goodSmiley = new GoodSmiley;
				sscanf(line.c_str(), "%i", &symbols[i].texture);
				symbols[i].object.goodSmiley->setTexture(texture[symbols[i].texture]);
				symbols[i].object.goodSmiley->setRotationEnable(false);
			} else if (line.find("Player") != string::npos){
				line.erase(0, 7);
				symbols[i].type = SMILEY_PLAYER;
				sscanf(line.c_str(), "%i", &symbols[i].texture);
			} else if (line.find("Enemy") != string::npos){
				line.erase(0, 6);
				symbols[i].type = SMILEY_ENEMY;
				symbols[i].object.enemy = new Enemy;
				sscanf(line.c_str(), "%i", &symbols[i].texture);
				symbols[i].object.enemy->setTexture(texture[symbols[i].texture]);
				symbols[i].object.enemy->setRotationEnable(false);
			}
			symbols[i].anzahl = 0;
			if(line.find('{') != string::npos){
				getline(in, line);
				while(line.find('}') == string::npos){
					while(!line.empty() && (line[0] == '\t' || line[0] == ' ')){
						line.erase(0, 1);
					}
					if(symbols[i].type == BLOCK_SPIKE){
						if(line.find("spiketexture") != string::npos){
							sscanf(line.c_str(), "spiketexture %i", &help);
							symbols[i].object.spikeBlock->setSpikeTexture(texture[help]);
						}
					} else if(symbols[i].type == SMILEY_ENEMY){
						if(line.find("speed") != string::npos){
							sscanf(line.c_str(), "speed %f", &helpf);
							symbols[i].object.enemy->setStartSpeedX(helpf);
						} else if(line.find("travel_length") != string::npos){
							sscanf(line.c_str(), "travel_length %f", &helpf);
							symbols[i].object.enemy->setTravelLength(helpf);
						} else if(line.find("rotation_on") != string::npos){
							symbols[i].object.enemy->setRotationEnable(true);
						} else if(line.find("turn_on_cliff") != string::npos){
							symbols[i].object.enemy->setTurnOnCliff(true);
						} else if(line.find("radius") != string::npos){
							sscanf(line.c_str(), "radius %f", &helpf);
							symbols[i].object.enemy->setRadius(helpf);
						}
					} else if(symbols[i].type == BLOCK_STONE){
						if(line.find("bounce_x") != string::npos){
							sscanf(line.c_str(), "bounce_x %f", &helpf);
							symbols[i].object.stoneBlock->setReflectionX(helpf);
						} else if(line.find("bounce_y") != string::npos){
							sscanf(line.c_str(), "bounce_y %f", &helpf);
							symbols[i].object.stoneBlock->setReflectionY(helpf);
						} else if(line.find("jump_speed") != string::npos){
							sscanf(line.c_str(), "jump_speed %f", &helpf);
							symbols[i].object.stoneBlock->setJumpSpeed(helpf);
						}
					}
					getline(in, line);
				}
			}
			i++;
		}
		getline(in, line);
	}

	pos = in.tellg();

	while(line.find("level") == string::npos && line.find('{') == string::npos && in.good()){
		getline(in, line);
	}

	//anzahl der anzahl der einzelnen Symbole lesen;
	getline(in, line);
	while(line.find('}') == string::npos && in.good()){
		for(i = 0; i < (signed int)line.length(); i++){
			if(line[i] != actualSymbol){
				actualSymbol = line[i];
				for(j = 0; j < anzahl; j++){
					if(symbols[j].symbol == actualSymbol){
						symbols[j].anzahl++;
						if(line.find('{') != string::npos){
							while(line.find('}') == string::npos){
								getline(in, line);
							}
						}
						goto end_loop;
					}
				}
				end_loop:;
			}
		}
		actualSymbol = '\0';
		getline(in, line);
	}

	in.seekg(pos);

	return symbols;
}

//reads the part of the lvl File where the lvl is designed
void Level::readLvl(ifstream& in, Symbol* symbols, int anzahl){
	extern Game game;
	string line;
	int i, j;
	int x, y;
	int stoneBlocksAdded = 4;
	int spikeBlocksAdded = 0;
	int goodSmileysAdded = 0;
	int enemysAdded = 0;
	char actualSymbol = '\0';
	y = 1;
	time_playing = 0;
	width = 1;

	getline(in, line);
	line.append("\n");
	while(line.find('}') == string::npos && in.good()){
		if((signed int)line.length() > width){
			width = line.length() - 1;
			bg_faktor_y = bg_faktor_x * -(float)height/(float)width * bg_width/bg_height;
		}
		actualSymbol = line[0];
		x = 0;
		y --;

		for(i = 0; i < (signed int)line.length(); i++){
			if(line[i] != actualSymbol){
				if(actualSymbol != ' '){
					for(j = 0; j < anzahl; j++){
						if(symbols[j].symbol == actualSymbol){
							if(symbols[j].type == BLOCK_STONE){
								stoneBlocks[stoneBlocksAdded] = *(symbols[j].object.stoneBlock);
								stoneBlocks[stoneBlocksAdded].setX((float)x);
								stoneBlocks[stoneBlocksAdded].setY((float)y);
								stoneBlocks[stoneBlocksAdded].setWidth((float)(i-x));
								stoneBlocks[stoneBlocksAdded].setHeight(1.0f);
								stoneBlocksAdded++;
							} else if(symbols[j].type == BLOCK_SPIKE){
								spikeBlocks[spikeBlocksAdded] = *(symbols[j].object.spikeBlock);
								spikeBlocks[spikeBlocksAdded].setX((float)x);
								spikeBlocks[spikeBlocksAdded].setY((float)y);
								spikeBlocks[spikeBlocksAdded].setWidth((float)(i-x));
								spikeBlocks[spikeBlocksAdded].setHeight(1.0f);
								spikeBlocksAdded++;
							} else if(symbols[j].type == SMILEY_CATCH){
								goodSmileys[goodSmileysAdded] = *(symbols[j].object.goodSmiley);
								goodSmileys[goodSmileysAdded].setX(x + 0.5f);
								goodSmileys[goodSmileysAdded].setY(y + goodSmileys[goodSmileysAdded].getRadius());
								goodSmileysAdded++;
							} else if(symbols[j].type == SMILEY_ENEMY){
								enemys[enemysAdded] = *(symbols[j].object.enemy);
								enemys[enemysAdded].setStartX(x + 0.5f);
								enemys[enemysAdded].setY(y + enemys[enemysAdded].getRadius());
								enemysAdded++;
							} else if(symbols[j].type == SMILEY_PLAYER){
								start_x = x + 0.5f;
								start_y = y + player.getRadius() + 0.01f;
								player.setTexture(texture[symbols[j].texture]);
							}
							goto end_loop;
						}
					}
					end_loop:;
				}
				x = i;
			}
			actualSymbol = line[i];
		}
		actualSymbol = '\0';
		getline(in, line);
		line.append("\n");
	}
	
	for(i = 3; i < getObjectAnzahl(); i++){
		getObject(i).setY(getObject(i).getY() - y);
	}
	for(i = 0; i < getEnemyAnzahl(); i++){
		getEnemy(i).setStartY(getEnemy(i).getY());
		getEnemy(i).reset();
	}
	start_y -= y;
	
	setPlayerToStart();
}

void Level::setPlayerToStart(){
	player.setX(start_x);
	player.setRotation(0);
	player.setY(start_y);
	player.setLayer(10.5f);
}

//loads a texture from the file spezified by "textureFileName" at the position <index> in the texture array
bool Level::loadTexture(const char* textureFileName, int index){
	texture[index] = SOIL_load_OGL_texture
        (
		textureFileName,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
		0
        );
 
    if(texture[index] == 0){
        return false;
	}
 
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, texture[index]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return true;
}

//cleans the lvl Class up (deletes all arrays and textures)
void Level::cleanup(){
	if(stoneBlocks != NULL){
		delete[] stoneBlocks;
		stoneBlocks = NULL;
	}
	stoneBlocksAnz = 0;

	if(spikeBlocks != NULL){
		delete[] spikeBlocks;
		stoneBlocks = NULL;
	}
	spikeBlocksAnz = 0;
	
	if(goodSmileys != NULL){
		delete[] goodSmileys;
		goodSmileys = NULL;
	}
	goodSmileysAnz = 0;
	
	if(enemys != NULL){
		delete[] enemys;
		enemys = NULL;
	}
	enemysAnz = 0;
}

void Level::deleteTextures(){
	if(texture != NULL){
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(textureAnz, texture);
		delete[] texture;
		texture = NULL;
	}
	textureAnz = 0;
}

//returns the block number <index>
StoneBlock& Level::getStoneBlock(int index)
{
	return stoneBlocks[index];
}

//returns how many blocks are in this lvl
int Level::getStoneBlockAnzahl(void)
{
	return stoneBlocksAnz;
}

//returns the block number <index>
SpikeBlock& Level::getSpikeBlock(int index)
{
	return spikeBlocks[index];
}

//returns how many blocks are in this lvl
int Level::getSpikeBlockAnz(void)
{
	return spikeBlocksAnz;
}

//gets the simley number <index>
GoodSmiley& Level::getGoodSmiley(int index)
{
	return goodSmileys[index];
}

//returns how many smiles are in the lvl
int Level::getGoodSmileyAnzahl(void)
{
	return goodSmileysAnz;
}

//gets the simley number <index>
Enemy& Level::getEnemy(int index)
{
	return enemys[index];
}

//returns how many smiles are in the lvl
int Level::getEnemyAnzahl(void)
{
	return enemysAnz;
}

Drawable& Level::getObject(int index)
{
	if(index < stoneBlocksAnz){
		return stoneBlocks[index];
	}
	index -= stoneBlocksAnz;

	if(index < spikeBlocksAnz){
		return spikeBlocks[index];
	}
	index -= spikeBlocksAnz;

	if(index < goodSmileysAnz){
		return goodSmileys[index];
	}
	index -= goodSmileysAnz;

	if(index < enemysAnz){
		return enemys[index];
	}

	return stoneBlocks[0];
}

int Level::getObjectAnzahl(void)
{
	return stoneBlocksAnz + spikeBlocksAnz + goodSmileysAnz + enemysAnz;
}

//returns the width of the lvl
int Level::getWidth(void)
{
	return width;
}

//returns the height of the lvl (default hight is 100, heigt schouldnt be set du a custom value)
int Level::getHeight(void)
{
	return height;
}

void Level::setWidth(int width){
	this->width = width;
}

//returns the width of the background image
GLfloat Level::getBackgroundFaktorX(){
	return bg_faktor_x;
}

//returns the height of the background image
GLfloat Level::getBackgroundFaktorY(){
	return bg_faktor_y;
}

//returns the background repeat modus (true -> repeat, false -> strech over whole lvl)
bool Level::isBackgroundRepeatedY(void){
	return bg_repeated_y;
}

//gets the number of active (good) smileys in the lvl (if 0 the lvl is won)
int Level::getActiveSmileys(void){
	int x=0;
	for(int i=0; i < goodSmileysAnz; i++){
		if(goodSmileys[i].getClass() == BALL_GOOD && goodSmileys[i].isActive()){
			x++;
		}
	}
	return x;
}

//reutrns the background image as texture
GLuint Level::getBackground(){
	return texture[0];
}

//returns true if there is set a background image and it is enabled
bool Level::isBackgroundEnabled(){
	return bg_enabled;
}

//returns the time spent playing this lvl (since last death or reload)
GLfloat Level::getTimePlaying(){
	return time_playing;
}

//adds playing time to the time played this lvl
void Level::addTimePlaying(GLfloat time){
	time_playing += time;
}

//sets the time the player has been playing this lvl
void Level::setTimePlaying(GLfloat time){
	time_playing = time;
}

GLfloat Level::getStartX(){
	return start_x;
}

GLfloat Level::getStartY(){
	return start_y;
}

Player& Level::getPlayer(){
	return player;
}