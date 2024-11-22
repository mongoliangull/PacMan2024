#include "CollisionHandler.h"

#include <iostream>

void CollisionHandler::operator()(PacMan& sphere, const Map& map) {
	CollisionDetector coldector;
	if (coldector(sphere, map)) {
		if (sphere.getCurrentDirection() == Sphere::DIRECTION::LEFT) {
			sphere.setVelocity(-0.1f * MOVE_SPEED, 0.0f * MOVE_SPEED, 0);
		}
		else if (sphere.getCurrentDirection() == Sphere::DIRECTION::RIGHT) {
			sphere.setVelocity(0.1f * MOVE_SPEED, 0.0f * MOVE_SPEED, 0);
		}
		else if (sphere.getCurrentDirection() == Sphere::DIRECTION::UP) {
			sphere.setVelocity(0.0f * MOVE_SPEED, 0.1f * MOVE_SPEED, 0);
		}
		else if (sphere.getCurrentDirection() == Sphere::DIRECTION::DOWN) {
			sphere.setVelocity(0.0f * MOVE_SPEED, -0.1f * MOVE_SPEED, 0);
		}
	}
	else {
		sphere.setCurrentDirection(Sphere::DIRECTION::NONE);
		sphere.setVelocity(0.0f * MOVE_SPEED, 0.0f * MOVE_SPEED, 0);
	}
}

void CollisionHandler::operator()(PacMan& pacman, Ghost& ghost) {
	
	CollisionDetector coldector;
	if (coldector(pacman,ghost)) {
		pacman.setCollided(true);
		if (ghost.getState() == Ghost::STATE::FRIGHTENED) {
			ghost.setState(Ghost::STATE::EATEN);
		}
	}
}

void CollisionHandler::operator()(PacMan& pacman, CoinCollection& coins) {
	CollisionDetector coldector;
	int idxX = 0, idxY = 0;
	switch (pacman.getCurrentDirection()) {
	case Sphere::DIRECTION::LEFT:
		idxX = -1;
		idxY = 0;
		break;
	case Sphere::DIRECTION::RIGHT:
		idxX = 1;
		idxY = 0;
		break;
	case Sphere::DIRECTION::UP:
		idxX = 0;
		idxY = -1;
		break;
	case Sphere::DIRECTION::DOWN:
		idxX = 0;
		idxY = 1;
		break;
	}
	idxX += pacman.getXIndex();
	idxY += pacman.getYIndex();

	idxX %= NUM_ROW;
	idxY %= NUM_COL;

	if (coldector(pacman, coins.findCoin(idxY, idxX))) {
		coins.removeCoin(coins.findCoin(idxY, idxX));
	}
}