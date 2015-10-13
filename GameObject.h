#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "Vector3.h"
#include "VSShaderlib.h"
#include "basic_geometry.h"
#include "AVTmathLib.h"

class GameObject {

protected:

	Vector3 _position;
	std::vector<struct MyMesh *> _meshes;

public:

	GameObject();
	~GameObject();

	void setPosition(float x, float y, float z);

	Vector3 getPosition();

	void loadMesh(struct MyMesh* mesh, VSShaderLib shader);
	void renderMesh(struct MyMesh* mesh, GLint pvm_uniformId, GLint vm_uniformId, GLint normal_uniformId, GLint lPos_uniformId);
	void addMesh(struct MyMesh* mesh);
};