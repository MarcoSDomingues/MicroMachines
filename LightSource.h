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

	bool _state;

public:

	LightSource(GLenum number);
	~LightSource();

	bool getState();
	void setState(bool State);
	GLenum getNum();
	void setPosition(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	float * getPosition();
	void setSpotDirection(GLfloat x, GLfloat y, GLfloat z);
	void setDirection(GLfloat x, GLfloat y, GLfloat z);
	void setCutOff(GLfloat cut_off);
	void setExponent(GLfloat exponent);
	void setShine(GLfloat shine);
	void setAmbient(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void setDiffuse(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void setSpecular(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void draw();

};