#include "Map.h"

Block::Block() : x(0), y(0), z(0), width(0), height(0), isWall(0) {}
Block::Block(float x, float y, float z, float w, float h) : x(x), y(y), z(z), width(w), height(h), isWall(0) {}

void Block::setWidth(float w) {
	width = w;
}

void Block::setHeight(float h) {
	height = h;
}

void Block::setIsWall(int i) {
	isWall = i;
}

float Block::getWidth() const {
	return width;
}

float Block::getHeight() const {
	return height;
}

int Block::getIsWall() const {
	return isWall;
}

void Block::draw() const {
	glBegin(GL_QUADS);
	if (isWall) {
		glColor3f(0.0f, 0.0f, 1.0f); // Blue color for walls
	}
	else {
		glColor3f(0.0f, 0.0f, 0.0f); // Black color for paths
	}

	// Draw the tile
	glVertex2f(x, y);
	glVertex2f(x + TILE_SIZE, y);
	glVertex2f(x + TILE_SIZE, y + TILE_SIZE);
	glVertex2f(x, y + TILE_SIZE);
	glEnd();
}

Map::Map() {}

void Map::createMap(const std::array<std::array<int, MAP_WIDTH>, MAP_HEIGHT>& idxTiles) {
	for (int row = 0; row < MAP_HEIGHT; row++) {
		for (int col = 0; col < MAP_WIDTH; col++) {
			tiles[row][col] = Block(TILE_SIZE * col, TILE_SIZE * row, 0, TILE_SIZE, TILE_SIZE);
			switch (idxTiles[row][col]) {
			case 0:
				tiles[row][col].setIsWall(0);
				break;
			case 1:
				tiles[row][col].setIsWall(1);
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