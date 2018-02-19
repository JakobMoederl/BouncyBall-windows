#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <fstream>
#include "StoneBlock.h"
#include "SpikeBlock.h"
#include "Player.h"
#include "Enemy.h"
#include "GoodSmiley.h"

using namespace std;

enum ObjectType{
	BLOCK_STONE, BLOCK_SPIKE, SMILEY_CATCH, SMILEY_PLAYER, SMILEY_ENEMY
};

union Object{
	SpikeBlock* spikeBlock;
	StoneBlock* stoneBlock;
	GoodSmiley* goodSmiley;
	Enemy* enemy;
	Object(){
		spikeBlock = NULL;
	};
};

struct Symbol{
	ObjectType type;
	int anzahl;
	char symbol;
	int texture;
	Object object;
};

class Level
{
public:
	//standartkonstruktor, no other avialble
	Level(void);
	//destruktor
	~Level(void);
	//loads a lvl from a txt file
	bool loadLvl(ifstream& lvlFile);
	//additonal lvlreading functions
	void readTextures(ifstream& in);
	void readBackground(ifstream& in);
	int getSymbolAnzahl(ifstream& in);
	Symbol* readSymbols(ifstream& in, int anzahl);
	void readLvl(ifstream& in, Symbol* symbols, int anzahl);

	//cleans all objects
	void cleanup();
	void deleteTextures();

	//loads a texture from a file
	bool loadTexture(const char* textureFileName, int index);

	//gets the background image
	GLuint getBackground();
	bool isBackgroundEnabled();
	
	//returns the block with index <index>
	StoneBlock& getStoneBlock(int index);
	//returns how many blocks the lvl contains
	int getStoneBlockAnzahl();
	
	//returns the block with index <index>
	SpikeBlock& getSpikeBlock(int index);
	//returns how many blocks the lvl contains
	int getSpikeBlockAnz();
	
	//returns the ball with index <index>
	GoodSmiley& getGoodSmiley(int index);
	//returns how many smileys the lvl contains
	int getGoodSmileyAnzahl(void);

	//returns the ball with index <index>
	Enemy& getEnemy(int index);
	//returns how many smileys the lvl contains
	int getEnemyAnzahl(void);

	//returns the object with index i of the lvl
	Drawable& getObject(int index);
	//returns the amount of objects in this lvl
	int getObjectAnzahl();

	//returns the width of the lvl (in units)
	int getWidth();
	//returns the height of the lvl (default 100, in units)
	int getHeight();

	void setWidth(int width);

	//returns width of the background image
	GLfloat getBackgroundFaktorX();
	//Returns height of the background image
	GLfloat getBackgroundFaktorY();
	bool isBackgroundRepeatedY();

	//returns the number of good smileys left in the lvl 
	int getActiveSmileys(void);

	GLfloat getTimePlaying();
	void addTimePlaying(GLfloat time);
	void setTimePlaying(GLfloat time);
	
	void setPlayerToStart();
	Player& getPlayer();
	GLfloat getStartX();
	GLfloat getStartY();

private:
	//lvl width and height in units
	int width;
	int height;

	//background width
	GLfloat bg_width;
	//bakcground height
	GLfloat bg_height;
	//background skalierungsfaktor x
	GLfloat bg_faktor_x;
	//background skalierungsfaktor y
	GLfloat bg_faktor_y;
	//background repeat modus on/off
	bool bg_repeated_y;
	//background enable
	bool bg_enabled;

	//Time since the level was started
	GLfloat time_playing;

	//stoneblocks which the lvl contains
	StoneBlock* stoneBlocks;
	//number of stoneblocks it the lvl
	int		  stoneBlocksAnz;

	//SpikeBlocks whicht the lvl contains
	SpikeBlock* spikeBlocks;
	//number of death blocks in the lvl
	int		    spikeBlocksAnz;

	//lvl Player Object
	Player player;
	//player startposition x
	GLfloat start_x;
	//player startposition y
	GLfloat start_y;

	//smileys which the lvl contains
	GoodSmiley* goodSmileys;
	//number of smileys in the lvl
	int   goodSmileysAnz;

	//enmys of the lvl
	Enemy* enemys;
	//amount of enemys
	int enemysAnz;
	
	//textures needed for this lvl
	GLuint* texture;
	int textureAnz;
};

#endif