#include "Shape3D.h"

using namespace std;

Shape3D::Shape3D() {
	center[0] = 0;
	center[1] = 0;
	center[2] = 0;
	velocity[0] = 0;
	velocity[1] = 0;
	velocity[2] = 0;
}
Shape3D::Shape3D(float x, float y, float z) {
	center[0] = x;
	center[1] = y;
	center[2] = z;
	velocity[0] = 0;
	velocity[1] = 0;
	velocity[2] = 0;
}

void Shape3D::setCenter(const Vector3f& c) {
	center[0] = c[0];
	center[1] = c[1];
	center[2] = c[2];
}

void Shape3D::setCenter(float x, float y, float z) {
	center[0] = x;
	center[1] = y;
	center[2] = z;
}

Vector3f Shape3D::getCenter() const {
	return center;
}

void Shape3D::setVelocity(const Vector3f& v) {
	velocity[0] = v[0];
	velocity[1] = v[1];
	velocity[2] = v[2];
}
void Shape3D::setVelocity(float x, float y, float z) {
	velocity[0] = x;
	velocity[1] = y;
	velocity[2] = z;
}

Vector3f Shape3D::getVelocity() const {
	return velocity;
}

void Shape3D::setMTL(const Material& m) {
	mtl = m;
}

const Material& Shape3D::getMTL() const {
	return mtl;
}

void Shape3D::move() {
	center = center + velocity;
}

void Shape3D::draw() const {

}