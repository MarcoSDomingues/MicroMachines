#include "LightSource.h"

LightSource::LightSource(GLenum number) {
	_num = number;
	_state = false;
}

LightSource::~LightSource() {}

bool LightSource::getState() {
	return _state;
}

void LightSource::setState(bool state) {
	_state = state;
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

void LightSource::setExponent(GLfloat exponent) {
	_exponent = exponent;
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

	if (_state) {
		glEnable(_num);

		if (_num == GL_LIGHT0) {
			glLightfv(_num, GL_POSITION, _position);
			glLightfv(_num, GL_AMBIENT, _ambient);
			glLightfv(_num, GL_DIFFUSE, _diffuse);
			glLightfv(_num, GL_SPECULAR, _specular);
		}
		else {
			glLightfv(_num, GL_POSITION, _position);
			glLightfv(_num, GL_SPOT_DIRECTION, _spotDirection);
			glLightf(_num, GL_SPOT_CUTOFF, _cut_off);
			glLightf(_num, GL_SPOT_EXPONENT, _exponent);
			glLightfv(_num, GL_AMBIENT, _ambient);
			glLightfv(_num, GL_DIFFUSE, _diffuse);
			glLightfv(_num, GL_SPECULAR, _specular);
		}
	}
	else
		glDisable(_num);
}