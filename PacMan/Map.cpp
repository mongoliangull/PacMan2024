#include "Map.h"

Block::Block() : x(0), y(0), z(0), width(0), height(0), bPassable(0) {}
Block::Block(float x, float y, float z, float w, float h) : x(x), y(y), z(z), width(w), height(h), bPassable(0) {}

void Block::setWidth(float w) {
	width = w;
}

void Block::setHeight(float h) {
	height = h;
}

void Block::setIsPassable(bool i) {
	bPassable = i;
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

void Block::draw() const {
	glBegin(GL_QUADS);
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
				tiles[row][col].setIsPassable(false);
				break;
			case p:
				tiles[row][col].setIsPassable(true);
				break;
			case o:
				tiles[row][col].setIsPassable(true);
				smallCoins.emplace_back(2.5f, 20, 20, posCenter);
				break;
			case O:
				tiles[row][col].setIsPassable(true);
				bigCoins.emplace_back(6.0f, 20, 20, posCenter);
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
	for (const auto& coin : smallCoins) {
		coin.draw();
	}
	
	for (const auto& COIN : bigCoins) {
		COIN.draw();
	}
}