# Level Files
the game loads the levels according to their file name. 1.lvl as first level, 2.lvl as second and so on.

the level file conists of two components. first the definition of blocks, textures, background etc. and secondly the layout of the blocks.

## definitions.

there are several things that need to be defined in order to load everything correctly.
* The textures used in this level
* The background image and if it will be stretched or repeated
* And the different block types used in this level.

### Textures
Every .lvl file needs to include a textures { ... } block which defines which texture files need to be loaded for this levels
Every line in this block should be a path to a .tga file. Later the first line can be refernced with 1, the second with 2 and so on.

### Background
The Background image of this level int the background { ... } block. the first line is the path to the .tga file
The second line is the resolution of the image.
With an additional "repeat 1" you the background gets repeated instead of stretched out over the whole level.

### Symbols
In the symbols { ... } block the different objects used in the level layout are described.
the syntax for a symbol block is <symbol> <type> <texture> [{ optional parameters }].
* <symbol> is a single character used to represent the block in the level layout.
* <type> is the type of the object. possible types are:
  - Player - Player start position and image.
  - Enemy - Enemy position and parameters.
  - Smiley - Simleys that the player has to collect.
  - StoneBlock - A Stone Wall/Block that the player cannot pass trough.
  - SpikeBlock - A Spike block that will kill the player if he touches it.
 * <texture> is the line in the texture { ... } block that should be uesed as textures for this object.
 
 additionally you can add additional parameters by extending the block with curly brackets { ... }
 every block has different keywords/parameters.
 
 Enemy:
 speed <number> - gives the start speed of the enemy
 rotation_on - changes the diplay of the enemy from sliding to rolling
 turn_on_cliff - when the enemy reaches a cliff it should turn and go back instead of falling down.
 
 StoneBlock:
 bounce_x <number> -  how much speed is reflected when the player bounces against the wall in the x-direction
 bounce_y <number> - same as bounce_x
 jump_speed <number> the jumping speed of the player when jumping when on this block.
 
 SpikeBlock:
spiketexture <number> - texture of the spikes of the block 

## Layout
the level {... } block contains the layout of the level. it should consist of lines with equal characters.
every character stands for a position in the level. the possible characters are the characters defined in the symbols { ... } block and space for an empty area.
Every Level must at least contain the symbol definition of the player, simley and StoneWall. The level layout must also contain atleast the player start position and one simley.
The bottom of the level is always a StoneWall.
