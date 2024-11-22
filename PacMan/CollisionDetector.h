#pragma once

#include "Sphere.h"
#include "Map.h"

class CollisionDetector
{
public:
	bool operator()(const PacMan& pacman, const Map& map);
	bool operator()(const PacMan& pacman, const Ghost& sphere);
	bool operator() (const PacMan& pacman, const Coin* coin);
};

