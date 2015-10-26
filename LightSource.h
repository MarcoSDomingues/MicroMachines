#pragma once

// include GLEW to access OpenGL 3.3 functions
#include <GL/glew.h>
// GLUT is the toolkit to interface with the OS
#include <GL/freeglut.h>

class LightSource {

private:

	GLfloat _ambient[4];
	GLfloat _diffuse[4];
	GLfloat _specular[4];
	GLfloat _position[4];
	GLfloat _spotDirection[3];
	GLfloat _direction[3];

	GLfloat _cut_off;
	GLfloat _exponent;
	GLfloat _shine;

	GLenum _num;

	bool _enabled;
	bool _local;
	bool _spot;

public:

	LightSource();
	~LightSource();

	bool isEnabled();
	void setEnabled(bool State);
	bool isLocal();
	void setLocal(bool local);
	bool isSpot();
	void setSpot(bool spot);
	GLenum getNum();
	void setPosition(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	float * getPosition();
	void setSpotDirection(GLfloat x, GLfloat y, GLfloat z);
	void setDirection(GLfloat x, GLfloat y, GLfloat z);
	void setCutOff(GLfloat cut_off);
	float getCutOff();
	void setExponent(GLfloat exponent);
	float getExponent();
	void setShine(GLfloat shine);
	void setAmbient(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void setDiffuse(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void setSpecular(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void draw();

};