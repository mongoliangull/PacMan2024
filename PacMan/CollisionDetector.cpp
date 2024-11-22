#include "CollisionDetector.h"

bool CollisionDetector::operator()(const PacMan& pacman, const Map& map) {
	switch (pacman.getCurrentDirection())
	{
	case Sphere::DIRECTION::NONE:
		return false;
		break;
	case Sphere::DIRECTION::LEFT:
		if (map.getBlock(pacman.getYIndex() % NUM_COL, (pacman.getXIndex() - 1) % NUM_ROW).isPassable())
			return true;
		else
			return false;
		break;
	case Sphere::DIRECTION::RIGHT:
		if (map.getBlock(pacman.getYIndex() % NUM_COL, (pacman.getXIndex() + 1) % NUM_ROW).isPassable())
			return true;
		else
			return false;
		break;
	case Sphere::DIRECTION::UP:
		if (map.getBlock((pacman.getYIndex() - 1) % NUM_COL, pacman.getXIndex() % NUM_ROW).isPassable())
			return true;
		else
			return false;
		break;
	case Sphere::DIRECTION::DOWN:
		if (map.getBlock((pacman.getYIndex() + 1) % NUM_COL, pacman.getXIndex() % NUM_ROW).isPassable())
			return true;
		else
			return false;
		break;
	}
}

bool CollisionDetector::operator()(const PacMan& pacman, const Ghost& ghost) {
	float distanceBetweenCenterSq = (pacman.getCenter()[0] - ghost.getCenter()[0])* (pacman.getCenter()[0] - ghost.getCenter()[0]) 
		+ (pacman.getCenter()[1] - ghost.getCenter()[1])* (pacman.getCenter()[1] - ghost.getCenter()[1]);
	float DiameterSq = BLOCK_SIZE * BLOCK_SIZE;
	
	if (distanceBetweenCenterSq < DiameterSq){
		return true;
	}
	else{
		return false;
	}
}

bool CollisionDetector::operator() (const PacMan& pacman, const Coin* coin) {
	if (coin == nullptr) {
		return false;
	}

	float distanceBetweenCenterSq = (pacman.getCenter()[0] - coin->getCenter()[0]) * (pacman.getCenter()[0] - coin->getCenter()[0])
		+ (pacman.getCenter()[1] - coin->getCenter()[1]) * (pacman.getCenter()[1] - coin->getCenter()[1]);
	float DiameterSq = (pacman.getRadius() + coin->getRadius()) * (pacman.getRadius() + coin->getRadius());

	if (distanceBetweenCenterSq < DiameterSq) {
		return true;
	}
	else {
		return false;
	}
}