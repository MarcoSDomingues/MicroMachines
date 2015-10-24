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

class GameObject : public Entity {

protected:

	std::vector<struct MyMesh *> _meshes;
	std::vector<GLuint> _textures;

	Vector4 _checkBox;
	std::string _colisionSide;

public:

	GameObject();
	~GameObject();

	void loadMesh(struct MyMesh* mesh, VSShaderLib shader);
	void renderMesh(struct MyMesh* mesh, GLint pvm_uniformId, GLint vm_uniformId, GLint normal_uniformId);
	void addMesh(struct MyMesh* mesh);
	void addTexture(GLuint texture);
	void drawTextures();

	virtual void draw(VSShaderLib shader,
		GLint pvm_uniformId, GLint vm_uniformId, GLint normal_uniformId) = 0;

	Vector4 getCheckBox();
	void setCheckBox(double x, double y, double z, double w);
	bool checkCollisions(GameObject *object);
	void collision();
};