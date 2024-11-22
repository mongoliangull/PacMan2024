#pragma once
#include "Constants.h"
#include "Sphere.h"

#include <array>
#include <vector>

class Block {
public:
	Block();
	Block(float x, float y, float z, float w, float h);

	typedef enum {noCoin, sCoin, bCoin} typeCoin;

	void setWidth(float w);
	void setHeight(float h);
	void setIsPassable(bool i);
	void setIsCoin(typeCoin c);
	

	float getWidth() const;
	float getHeight() const;
	bool isPassable() const;
	typeCoin isCoin() const;

	void draw() const;

private:
	float x, y, z;
	float width, height;
	bool bPassable;
	typeCoin tCoin;
};

class Map {
public:
	Map();

	void createMap(const std::array<std::array<tileType, MAP_WIDTH>, MAP_HEIGHT>& idxTiles);

	const Block& getBlock(int r, int c) const;

	void draw() const;

	void drawCoins() const;

	CoinCollection smallCoins;
	CoinCollection bigCoins;

private:
	std::array<std::array<Block, MAP_WIDTH>, MAP_HEIGHT> tiles;
};