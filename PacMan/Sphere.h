#pragma once

#include <vector>
#include <algorithm>

#include "Shape3d.h"

class Sphere : public Shape3D
{
public:
	enum DIRECTION { NONE, LEFT, UP, RIGHT, DOWN };

	Sphere();
	Sphere(float r, int sl, int st);

	void setRadius(float r);
	float getRadius() const;
	void setSlice(int sl);
	void setStack(int st);

	void setIndexPosition(int x, int y);
	bool isIndexPositionUpdated() const;
	void updateCheck();
	int getXIndex() const;
	int getYIndex() const;

	void setCurrentDirection(DIRECTION d);
	void setNextDirection(DIRECTION d);
	DIRECTION getCurrentDirection() const;
	DIRECTION getNextDirection() const;
	void updateDirection();

	void move();
	virtual void draw() const;

protected:
	float radius;
	int slice;
	int stack;

	int idxPos[2];
	bool bInxPosUpdated;

	DIRECTION currDirection;
	DIRECTION nextDirection;
};

/* Pacman class */
class PacMan : public Sphere
{
public:
	PacMan(float r, int sl, int st, bool bCol);

	void setCollided(bool bCol);
	bool getCollided();

	virtual void draw() const;

private:
	bool bCollided;
};

/* Ghost class */
class Ghost : public Sphere
{
public:
	enum STATE { CHASE, SCATTER, FRIGHTENED, EATEN };

	Ghost(float r, int sl, int st, int ctX, int ctY, STATE s, STATE ps);

	void setState(STATE s);
	void saveState();
	STATE getState() const;
	void updateVelocity();
	STATE getprevState() const;
	void setdTime(int time);
	int getdTime();
	void setpTime(int time);
	int getpTime();
	int getOriginX();
	int getOriginY();

	virtual void draw() const;

private:
	int origin[2];
	int dTime;
	int pTime;
	STATE state;
	STATE prevstate;
};

class Coin : public Sphere {
public:
	Coin(float r, int sl, int st, const Vector3f& pos, int idxX, int idxY, bool big);

	void setIsBig(bool big);
	void setIdxPos(int x, int y);

	bool isBig() const;
	int getIdxX() const;
	int getIdxY() const;

	virtual void draw() const;

private:
	int idxPos[2];
	bool bBig;
};

class CoinCollection {
public:
	std::vector<Coin> coins;

	void addCoin(float r, int sl, int st, const Vector3f& pos, int idxX, int idxY, bool big) {
		coins.emplace_back(r, sl, st, pos, idxX, idxY, big);
	}

	const Coin* findCoin(int idxX, int idxY) const {
		auto it = std::find_if(coins.begin(), coins.end(), [idxX, idxY](const Coin& coin) {
			return coin.getIdxX() == idxX && coin.getIdxY() == idxY;
		});
		
		if (it != coins.end()) {
			return &(*it);
		}
		else {
			return nullptr;
		}
	}

	void removeCoin(const Coin* coinPtr) {
		if (!coinPtr) {
			return; // Invalid pointer, cannot remove
		}

		// Use std::find to locate the coin in the collection
		auto it = std::find_if(coins.begin(), coins.end(), [coinPtr](const Coin& coin) {
			return &coin == coinPtr; // Compare the address of the coin
			});

		if (it != coins.end()) {
			coins.erase(it); // Remove the coin from the collection
		}
	}
};