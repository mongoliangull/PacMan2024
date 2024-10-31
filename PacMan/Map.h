#pragma once
#include <GL/freeglut.h>
#include <array>
#include <vector>

const int MAP_WIDTH = 28;
const int MAP_HEIGHT = 31;
const int TILE_SIZE = 20;

class Block {
public:
	Block();
	Block(float x, float y, float z, float w, float h);

	void setWidth(float w);
	void setHeight(float h);
	void setIsWall(int i);

	float getWidth() const;
	float getHeight() const;
	int getIsWall() const;

	void draw() const;
private:
	float x, y, z;
	float width, height;
	int isWall;
};

class Map {
public:
	Map();

	void createMap(const std::array<std::array<int, MAP_WIDTH>, MAP_HEIGHT>& idxTiles);

	const Block& getBlock(int r, int c) const;

	void draw() const;
private:
	std::array<std::array<Block, MAP_WIDTH>, MAP_HEIGHT> tiles;
};