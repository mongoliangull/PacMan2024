#include "CollisionHandler.h"

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
	else {
		pacman.setCollided(false);
	}
}