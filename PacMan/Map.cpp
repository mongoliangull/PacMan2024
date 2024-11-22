#include "Map.h"

Block::Block() : x(0), y(0), z(0), width(0), height(0), bPassable(0), bHalfWall(0), tCoin(noCoin) {}
Block::Block(float x, float y, float z, float w, float h) : x(x), y(y), z(z), width(w), height(h), bPassable(0), bHalfWall(0), tCoin(noCoin) {}

void Block::setWidth(float w) {
	width = w;
}

void Block::setHeight(float h) {
	height = h;
}

void Block::setIsPassable(bool i) {
	bPassable = i;
}

void Block::setHalfWall(bool b) {
	bHalfWall = b;
}

void Block::setIsCoin(Block::typeCoin c) {
	tCoin = c;
}

float Block::getWidth() const {
	return width;
}

float Block::getHeight() const {
	return height;
}

bool Block::isPassable() const {
	return bPassable;
}

bool Block::isHalfWall() const {
	return bHalfWall;
}

Block::typeCoin Block::isCoin() const {
	return tCoin;
}

void Block::draw() const {
		glBegin(GL_QUADS);
	if (bHalfWall) {
		glColor3f(1.0f, 0.647f, 0.0f);
		glVertex2f(x, y - 0.8 * TILE_SIZE);
		glVertex2f(x + TILE_SIZE, y - 0.8 * TILE_SIZE);
		glVertex2f(x + TILE_SIZE, y - TILE_SIZE);
		glVertex2f(x, y - TILE_SIZE);
		glEnd();
	}
	else {
		if (bPassable) {
			glColor3f(0.0f, 0.0f, 0.0f); // Black color for paths
		}
		else {
			glColor3f(0.0f, 0.0f, 1.0f); // Blue color for walls
		}

		glVertex2f(x, y);
		glVertex2f(x + TILE_SIZE, y);
		glVertex2f(x + TILE_SIZE, y - TILE_SIZE);
		glVertex2f(x, y - TILE_SIZE);
		glEnd();
	}
}

Map::Map() {}

void Map::createMap(const std::array<std::array<tileType, MAP_WIDTH>, MAP_HEIGHT>& idxTiles) {
	for (int row = 0; row < MAP_HEIGHT; row++) {
		for (int col = 0; col < MAP_WIDTH; col++) {
			Vector3f posCenter(LEFT_BOUNDARY + TILE_SIZE * (col + 0.5f), TOP_BOUNDARY - TILE_SIZE * (row + 0.5f), 1.0f);
			tiles[row][col] = Block(LEFT_BOUNDARY + TILE_SIZE * col, TOP_BOUNDARY - TILE_SIZE * row, 0, TILE_SIZE, TILE_SIZE);
			switch (idxTiles[row][col]) {
			case u:
				tiles[row][col].setIsPassable(false);
				break;
			case w:
				tiles[row][col].setIsPassable(false);
				break;
			case h:
				tiles[row][col].setIsPassable(true);
				tiles[row][col].setHalfWall(true);
				break;
			case p:
				tiles[row][col].setIsPassable(true);
				break;
			case o:
				tiles[row][col].setIsPassable(true);
				tiles[row][col].setIsCoin(Block::sCoin);
				smallCoins.addCoin(2.5f, 20, 20, posCenter, row, col, false);
				break;
			case O:
				tiles[row][col].setIsPassable(true);
				tiles[row][col].setHeight(Block::bCoin);
				bigCoins.addCoin(6.0f, 20, 20, posCenter, row, col, true);
				break;
			}
		}
	}
}

const Block& Map::getBlock(int r, int c) const {
	return tiles[r][c];
}

void Map::draw() const {
	for (int row = 0; row < MAP_HEIGHT; row++) {
		for (int col = 0; col < MAP_WIDTH; col++) {
			tiles[row][col].draw();
		}
	}
}

void Map::drawCoins() const {
	for (const auto& coin : smallCoins.coins) {
		coin.draw();
	}
	
	for (const auto& COIN : bigCoins.coins) {
		COIN.draw();
	}
}