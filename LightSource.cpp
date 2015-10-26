#include "LightSource.h"

LightSource::LightSource() {
	_enabled = false;
	_local = false;
	_spot = false;
}

LightSource::~LightSource() {}

bool LightSource::isEnabled() {
	return _enabled;
}

void LightSource::setEnabled(bool state) {
	_enabled = state;
}

bool LightSource::isLocal() {
	return _local;
}

void LightSource::setLocal(bool local) {
	_local = local;
}

bool LightSource::isSpot() {
	return _spot;
}

void LightSource::setSpot(bool spot) {
	_spot = spot;
}

GLenum LightSource::getNum() {
	return _num;
}

void LightSource::setPosition(GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
	_position[0] = x;
	_position[1] = y;
	_position[2] = z;
	_position[3] = w;
}

float * LightSource::getPosition() {
	return _position;
}

void LightSource::setSpotDirection(GLfloat x, GLfloat y, GLfloat z) {
	_spotDirection[0] = x;
	_spotDirection[1] = y;
	_spotDirection[2] = z;
}


void LightSource::setDirection(GLfloat x, GLfloat y, GLfloat z) {
	_direction[0] = x;
	_direction[1] = y;
	_direction[2] = z;
}

void LightSource::setCutOff(GLfloat cut_off) {
	_cut_off = cut_off;
}

float LightSource::getCutOff() {
	return _cut_off;
}

void LightSource::setExponent(GLfloat exponent) {
	_exponent = exponent;
}

float LightSource::getExponent() {
	return _exponent;
}

void LightSource::setShine(GLfloat shine) {
	_shine = shine;
}

void LightSource::setAmbient(GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
	_ambient[0] = x;
	_ambient[1] = y;
	_ambient[2] = z;
	_ambient[3] = w;
}

void LightSource::setDiffuse(GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
	_diffuse[0] = x;
	_diffuse[1] = y;
	_diffuse[2] = z;
	_diffuse[3] = w;
}

void LightSource::setSpecular(GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
	_specular[0] = x;
	_specular[1] = y;
	_specular[2] = z;
	_specular[3] = w;
}

void LightSource::draw() {

}