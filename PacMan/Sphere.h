#pragma once

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

	virtual void draw() const;

private:
	bool bCollided;
};

/* Ghost class */
class Ghost : public Sphere
{
public:
	enum STATE { CHASE, SCATTER, FRIGHTENED, EATEN };

	Ghost(float r, int sl, int st, STATE s, STATE ps);

	void setState(STATE s);
	void saveState();
	STATE getState() const;
	void updateVelocity();
	STATE getprevState() const;

	virtual void draw() const;

private:
	STATE state;
	STATE prevstate;
};

class Coin : public Sphere {
public:
	Coin(float r, int sl, int st, const Vector3f& pos);

	virtual void draw() const;
private:

};
