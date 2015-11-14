#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <typeinfo>
#include <string>

#include "Entity.h"
#include "Vector3.h"
#include "VSShaderlib.h"
#include "basic_geometry.h"
#include "AVTmathLib.h"
#include "Vector4.h"

#define MAX_PARTICULAS  50
#define frand()			((float)rand()/RAND_MAX)
#define M_PI			3.14159265
class Particles {

protected:
	typedef struct {
		float	life;		// vida
		float	fade;		// fade
		float	r, g, b;    // color
		GLfloat x, y, z;    // posição
		GLfloat vx, vy, vz; // velocidade 
		GLfloat ax, ay, az; // aceleração
	} Particle;

	Particle particula[MAX_PARTICULAS];


public:


	void Particle();

	void iterate(int value);

	void init(GLfloat x, GLfloat y, GLfloat z);

	void render(GLint texture, struct MyMesh mesh, GLint pvm_uniformId, GLint vm_uniformId, GLint normal_uniformId, VSShaderLib shader, float angle);

};