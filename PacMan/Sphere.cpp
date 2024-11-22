#include "Constants.h"
#include "Sphere.h"

using namespace std;

GLfloat ambient[4];
GLfloat emission[4];
GLfloat diffuse[4];
GLfloat specular[4];
GLfloat shininess[1];

Sphere::Sphere() {

}

Sphere::Sphere(float r, int sl, int st) {
	radius = r;
	slice = sl;
	stack = st;
	center[0] = 0;
	center[1] = 0;
	center[2] = 0;
	velocity[0] = 0;
	velocity[1] = 0;
	velocity[2] = 0;
	idxPos[0] = 0;
	idxPos[1] = 1;
	bInxPosUpdated = 0;

	currDirection = Sphere::DIRECTION::NONE;
	nextDirection = Sphere::DIRECTION::NONE;
}
void Sphere::setRadius(float r) {
	radius = r;
}
float Sphere::getRadius() const {
	return radius;
}
void Sphere::setSlice(int sl) {
	slice = sl;
}
void Sphere::setStack(int st) {
	stack = st;
}

void Sphere::setIndexPosition(int x, int y) {
	idxPos[0] = x;
	idxPos[1] = y;
}
bool Sphere::isIndexPositionUpdated() const {
	return bInxPosUpdated;
}

void Sphere::updateCheck() {
	/*cout << center[0] << " " << center[1] << endl;
	cout << (idxPos[0] - 4) * 40 << " " << (idxPos[1] - 4) * 40 << endl;*/
	if (velocity[0] > 0.0f) {
		if (center[0] - radius >= (idxPos[0] + 1) * BLOCK_SIZE + LEFT_BOUNDARY) { //idxPos[0] + 1 - NUM_ROW / 2.0f) * BLOCK_SIZE
			if (idxPos[0] < NUM_ROW - 1)
				setIndexPosition(idxPos[0] + 1, idxPos[1]);
			else {
				setIndexPosition(0, idxPos[1]);
				center[0] = LEFT_BOUNDARY + BLOCK_SIZE / 2.0f;
			}
			bInxPosUpdated = true;
		}
		else
			bInxPosUpdated = false;
	}
	else if (velocity[0] < 0.0f) {
		if (center[0] + radius <= (idxPos[0]) * BLOCK_SIZE + LEFT_BOUNDARY) {
			if (idxPos[0] > 0)
				setIndexPosition(idxPos[0] - 1, idxPos[1]);
			else {
				setIndexPosition(NUM_ROW - 1, idxPos[1]);
				center[0] = BLOCK_SIZE * (NUM_ROW / 2.0f - 0.5f);
			}
			bInxPosUpdated = true;
		}
		else
			bInxPosUpdated = false;
	}
	else if (velocity[1] > 0.0f) {
		if (center[1] - radius >= (- idxPos[1] * BLOCK_SIZE + TOP_BOUNDARY)) {
			if (idxPos[1] > 0)
				setIndexPosition(idxPos[0], idxPos[1] - 1);
			else {
				setIndexPosition(idxPos[0], NUM_COL - 1);
				center[1] = -BLOCK_SIZE * (NUM_COL / 2.0f);
			}
			bInxPosUpdated = true;
		}
		else
			bInxPosUpdated = false;
	}
	else if (velocity[1] < 0.0f) {
		if (center[1] + radius <= (-idxPos[1] - 1) * BLOCK_SIZE + TOP_BOUNDARY) {
			if (idxPos[1] < NUM_COL - 1)
				setIndexPosition(idxPos[0], idxPos[1] + 1);
			else {
				setIndexPosition(idxPos[1], 0);
				center[1] = +BLOCK_SIZE * (NUM_COL / 2.0f);
			}
			bInxPosUpdated = true;
		}
		else
			bInxPosUpdated = false;
	}
}

int Sphere::getXIndex() const {
	return idxPos[0];
}

int Sphere::getYIndex() const {
	return idxPos[1];
}

void Sphere::setCurrentDirection(DIRECTION d) {
	currDirection = d;
}
void Sphere::setNextDirection(DIRECTION d) {
	nextDirection = d;
}
Sphere::DIRECTION Sphere::getCurrentDirection() const {
	return currDirection;
}
Sphere::DIRECTION Sphere::getNextDirection() const {
	return nextDirection;
}
void Sphere::updateDirection() {
	currDirection = nextDirection;
}

void Sphere::move() {
	center = center + velocity;
}

void Sphere::draw() const {
	glPushMatrix();
	glTranslatef(center[0], center[1], center[2]);
	glShadeModel(GL_SMOOTH);
	shininess[0] = mtl.getShininess();
	for (int i = 0; i < 4; i++) {
		ambient[i] = mtl.getAmbient()[i];
		emission[i] = mtl.getEmission()[i];
		diffuse[i] = mtl.getDiffuse()[i];
		specular[i] = mtl.getSpecular()[i];
	}

	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glutSolidSphere(radius, slice, stack);
	glPopMatrix();
}


PacMan::PacMan(float r, int sl, int st, bool bCol) {
	radius = r;
	slice = sl;
	stack = st;
	center[0] = 10.0f;
	center[1] = -160.0f;
	center[2] = 0;
	velocity[0] = 0;
	velocity[1] = 0;
	velocity[2] = 0;
	idxPos[0] = 0;
	idxPos[1] = 0;
	bInxPosUpdated = 0;

	currDirection = Sphere::DIRECTION::NONE;
	nextDirection = Sphere::DIRECTION::NONE;
	bCollided = 0;
}

void PacMan::setCollided(bool bCol) {
	bCollided = bCol;
}

void PacMan::draw() const {
	if (bCollided) {
		glPushMatrix();
		glTranslatef(center[0], center[1], center[2]);
		glShadeModel(GL_SMOOTH);
		shininess[0] = mtl.getShininess();
		for (int i = 0; i < 4; i++) {
			ambient[3-i] = mtl.getAmbient()[i];
			emission[3-i] = mtl.getEmission()[i];
			diffuse[3-i] = mtl.getDiffuse()[i];
			specular[i] = mtl.getSpecular()[i]/2;
		}

		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
		glutSolidSphere(radius, slice, stack);
		glPopMatrix();
	}
	else {
		glPushMatrix();
		glTranslatef(center[0], center[1], center[2]);
		glShadeModel(GL_SMOOTH);
		shininess[0] = mtl.getShininess();
		for (int i = 0; i < 4; i++) {
			ambient[i] = mtl.getAmbient()[i];
			emission[i] = mtl.getEmission()[i];
			diffuse[i] = mtl.getDiffuse()[i];
			specular[i] = mtl.getSpecular()[i];
		}

		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
		glutSolidSphere(radius, slice, stack);
		glPopMatrix();
	}
}

Ghost::Ghost(float r, int sl, int st, STATE s, STATE ps) {
	radius = r;
	slice = sl;
	stack = st;
	center[0] = -250;
	center[1] = 280;
	center[2] = 0;
	velocity[0] = 0;
	velocity[1] = 0;
	velocity[2] = 0;
	idxPos[0] = 1;
	idxPos[1] = 1;
	bInxPosUpdated = 0;

	currDirection = Sphere::DIRECTION::NONE;
	nextDirection = Sphere::DIRECTION::NONE;
	state = s;
	prevstate = ps;
}

void Ghost::setState(STATE s) {
	state = s;
}

void Ghost::saveState() {
	prevstate = state;
}

Ghost::STATE Ghost::getState() const {
	return state;
}

Ghost::STATE Ghost::getprevState() const {
	return prevstate;
}

void Ghost::updateVelocity() {
	if (state != Ghost::STATE::EATEN) {
		if (currDirection == Sphere::DIRECTION::LEFT) {
			setVelocity(-0.1f * MOVE_SPEED, 0.0f * MOVE_SPEED, 0);
		}
		else if (currDirection == Sphere::DIRECTION::RIGHT) {
			setVelocity(+0.1f * MOVE_SPEED, 0.0f * MOVE_SPEED, 0);
		}
		else if (currDirection == Sphere::DIRECTION::UP) {
			setVelocity(0.0f * MOVE_SPEED, 0.1f * MOVE_SPEED, 0);
		}
		else if (currDirection == Sphere::DIRECTION::DOWN) {
			setVelocity(0.0f * MOVE_SPEED, -0.1f * MOVE_SPEED, 0);
		}
		else {
			setVelocity(0.0f * MOVE_SPEED, 0.0f * MOVE_SPEED, 0);
		}
	}
	else {
		if (currDirection == Sphere::DIRECTION::LEFT) {
			setVelocity(-0.5f * MOVE_SPEED, 0.0f * MOVE_SPEED, 0);
		}
		else if (currDirection == Sphere::DIRECTION::RIGHT) {
			setVelocity(+0.5f * MOVE_SPEED, 0.0f * MOVE_SPEED, 0);
		}
		else if (currDirection == Sphere::DIRECTION::UP) {
			setVelocity(0.0f * MOVE_SPEED, 0.5f * MOVE_SPEED, 0);
		}
		else if (currDirection == Sphere::DIRECTION::DOWN) {
			setVelocity(0.0f * MOVE_SPEED, -0.5f * MOVE_SPEED, 0);
		}
		else {
			setVelocity(0.0f * MOVE_SPEED, 0.0f * MOVE_SPEED, 0);
		}
	}
}

void Ghost::draw() const {
	if (state == Ghost::STATE::FRIGHTENED) {
		glPushMatrix();
		glTranslatef(center[0], center[1], center[2]);
		glShadeModel(GL_SMOOTH);
		shininess[0] = mtl.getShininess();
		for (int i = 0; i < 4; i++) {
			ambient[3 - i] = mtl.getAmbient()[i];
			emission[3 - i] = 2*mtl.getEmission()[i];
			diffuse[3 - i] = mtl.getDiffuse()[i];
			specular[i] = mtl.getSpecular()[i] / 3;
		}

		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
		glutSolidSphere(radius, slice, stack);
		glPopMatrix();
	}
	else if(state == Ghost::STATE::EATEN) {
		glPushMatrix();
		glTranslatef(center[0], center[1], center[2]);
		glShadeModel(GL_SMOOTH);
		shininess[0] = mtl.getShininess();
		for (int i = 0; i < 4; i++) {
			ambient[i] = mtl.getAmbient()[i]/3;
			emission[i] = mtl.getEmission()[i]/3;
			diffuse[i] = mtl.getDiffuse()[i]/3;
			specular[i] = mtl.getSpecular()[i]/3;
		}

		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
		glutSolidSphere(radius, slice, stack);
		glPopMatrix();
	}
	else {
		glPushMatrix();
		glTranslatef(center[0], center[1], center[2]);
		glShadeModel(GL_SMOOTH);
		shininess[0] = mtl.getShininess();
		for (int i = 0; i < 4; i++) {
			ambient[i] = mtl.getAmbient()[i];
			emission[i] = mtl.getEmission()[i];
			diffuse[i] = mtl.getDiffuse()[i];
			specular[i] = mtl.getSpecular()[i];
		}

		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
		glutSolidSphere(radius, slice, stack);
		glPopMatrix();
	}
}

Coin::Coin(float r, int sl, int st, const Vector3f& pos, int idxX, int idxY, bool big) {
	radius = r;
	slice = sl;
	stack = st;
	center = pos;
	idxPos[0] = idxX;
	idxPos[1] = idxY;
	bBig = big;
	mtl.setEmission(0.2f, 0.2f, 0.2f, 1.0f);
	mtl.setAmbient(0.6f, 0.6f, 0.0f, 1.0f);
	mtl.setDiffuse(0.8f, 0.8f, 0.0f, 1.0f);
	mtl.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	mtl.setShininess(30.0f);
}

void Coin::draw() const {
	glPushMatrix();
	glTranslatef(center[0], center[1], center[2]);
	glShadeModel(GL_SMOOTH);
	shininess[0] = mtl.getShininess();
	for (int i = 0; i < 4; i++) {
		ambient[i] = mtl.getAmbient()[i];
		emission[i] = mtl.getEmission()[i];
		diffuse[i] = mtl.getDiffuse()[i];
		specular[i] = mtl.getSpecular()[i];
	}

	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glutSolidSphere(radius, slice, stack);
	glPopMatrix();
}

void Coin::setIsBig(bool big) {
	bBig = big;
}

void Coin::setIdxPos(int x, int y) {
	idxPos[0] = x;
	idxPos[1] = y;
}

bool Coin::isBig() const {
	return bBig;
}

int Coin::getIdxX() const {
	return idxPos[0];
}

int Coin::getIdxY() const {
	return idxPos[1];
}