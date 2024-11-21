#pragma once
#include "Constants.h"
#include "Sphere.h"

#include <array>
#include <vector>

class Block {
public:
	Block();
	Block(float x, float y, float z, float w, float h);

	void setWidth(float w);
	void setHeight(float h);
	void setIsPassable(bool i);

	float getWidth() const;
	float getHeight() const;
	bool isPassable() const;

	void draw() const;

private:
	float x, y, z;
	float width, height;
	bool bPassable;
};

class Map {
public:
	Map();

	void createMap(const std::array<std::array<tileType, MAP_WIDTH>, MAP_HEIGHT>& idxTiles);

	const Block& getBlock(int r, int c) const;

	void draw() const;

	void drawCoins() const;

private:
	std::array<std::array<Block, MAP_WIDTH>, MAP_HEIGHT> tiles;
	std::vector<Coin> smallCoins;
	std::vector<Coin> bigCoins;
};