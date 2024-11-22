#pragma once

#include "CollisionDetector.h"

class CollisionHandler
{
public:
	void operator()(PacMan& pacman, const Map& map);
	void operator()(PacMan& pacman, Ghost& ghost);
	void operator()(PacMan& pacman, CoinCollection& coins);
};
