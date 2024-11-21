#include "Light.h"

using namespace std;

float position[3];
float ambients[4];
float diffuses[4];
float speculars[4];

Light::Light(float x, float y, float z, int L_ID) {
	pos[0] = x;
	pos[1] = y;
	pos[2] = z;
	lightID = L_ID;
	ambient[0] = 0;
	ambient[1] = 0;
	ambient[2] = 0;
	ambient[3] = 0;
	diffuse[0] = 0;
	diffuse[1] = 0;
	diffuse[2] = 0;
	diffuse[3] = 0;
	specular[0] = 0;
	specular[1] = 0;
	specular[2] = 0;
	specular[3] = 0;
}
void Light::setAmbient(float r, float g, float b, float a) {
	ambient[0] = r;
	ambient[1] = g;
	ambient[2] = b;
	ambient[3] = a;
}
void Light::setDiffuse(float r, float g, float b, float a) {
	diffuse[0] = r;
	diffuse[1] = g;
	diffuse[2] = b;
	diffuse[3] = a;
}
void Light::setSpecular(float r, float g, float b, float a) {
	specular[0] = r;
	specular[1] = g;
	specular[2] = b;
	specular[3] = a;
}

Vector4f Light::getAmbient() const {
	return ambient;
}
Vector4f Light::getDiffuse() const {
	return diffuse;
}
Vector4f Light::getSpecular() const {
	return specular;
}

void Light::draw() const {
	for (int i = 0; i < 3; i++) {
		position[i] = pos[i];
	}
	for (int i = 0; i < 4; i++) {
		diffuses[i] = diffuse[i];
		ambients[i] = ambient[i];
		speculars[i] = specular[i];
	}
	glLightfv(lightID, GL_AMBIENT, ambients);
	glLightfv(lightID, GL_DIFFUSE, diffuses);
	glLightfv(lightID, GL_SPECULAR, speculars);
	glLightfv(lightID, GL_POSITION, position);
}

int Light::getID() const {
	return lightID;
}