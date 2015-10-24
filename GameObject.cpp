#include "GameObject.h"

extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
extern float mNormal3x3[9];

GameObject::GameObject() {}

GameObject::~GameObject() {}

void GameObject::loadMesh(struct MyMesh* mesh, VSShaderLib shader) {
	GLint loc;
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, mesh->mat.ambient);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, mesh->mat.diffuse);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, mesh->mat.specular);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc, mesh->mat.shininess);
}

void GameObject::renderMesh(struct MyMesh* mesh, GLint pvm_uniformId, GLint vm_uniformId, GLint normal_uniformId) {
	// send matrices to OGL
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	computeNormalMatrix3x3();
	glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

	// Render mesh
	glBindVertexArray(mesh->vao);
	glDrawElements(mesh->type, mesh->numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void GameObject::addTexture(GLuint texture) {
	_textures.push_back(texture);
}

void GameObject::drawTextures() {
	glBindTexture(GL_TEXTURE_2D, 0);

	int i = 0;
	for (GLuint tex : _textures) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, _textures.at(i));
		i++;
	}

}

void GameObject::addMesh(struct MyMesh* mesh)
{
	_meshes.push_back(mesh);
}

Vector4 GameObject::getCheckBox() {
	return _checkBox;
}

void GameObject::setCheckBox(double x, double y, double z, double w) {
	_checkBox.set(x, y, z, w);
}

bool GameObject::checkCollisions(GameObject *object) {

	Vector4 checkBoxObject;
	Vector4 checkBoxCar;

	checkBoxObject = getCheckBox();
	checkBoxCar = object->getCheckBox();

	if ((checkBoxCar.getX() >= checkBoxObject.getX()) && (checkBoxCar.getX() <= checkBoxObject.getY()) && (checkBoxCar.getW() >= checkBoxObject.getZ()) && (checkBoxCar.getW() <= checkBoxObject.getW())) {
		_colisionSide = "down";
		std::cout << (_colisionSide) << std::endl;
		return true;
	}

	else if ((checkBoxCar.getY() >= checkBoxObject.getX()) && (checkBoxCar.getY() <= checkBoxObject.getY()) && (checkBoxCar.getW() >= checkBoxObject.getZ()) && (checkBoxCar.getW() <= checkBoxObject.getW())) {
		_colisionSide = "up";
		std::cout << (_colisionSide) << std::endl;
		return true;
	}

	else if ((checkBoxCar.getX() >= checkBoxObject.getX()) && (checkBoxCar.getX() <= checkBoxObject.getY()) && (checkBoxCar.getZ() >= checkBoxObject.getZ()) && (checkBoxCar.getZ() <= checkBoxObject.getW())) {
		_colisionSide = "right";
		std::cout << (_colisionSide) << std::endl;
		return true;
	}

	else if ((checkBoxCar.getY() >= checkBoxObject.getX()) && (checkBoxCar.getY() <= checkBoxObject.getY()) && (checkBoxCar.getZ() >= checkBoxObject.getZ()) && (checkBoxCar.getZ() <= checkBoxObject.getW())) {
		_colisionSide = "left";
		std::cout << (_colisionSide) << std::endl;
		return true;
	}

	else {
		return false;
	}
}

void GameObject::collision() {
	if (_colisionSide == "left") {
		setPosition(getPosition().getX()+10, getPosition().getY(), getPosition().getZ());
	}
	else if (_colisionSide == "up") {
		setPosition(getPosition().getX(), getPosition().getY(), getPosition().getZ()-10);
	}
	else if (_colisionSide == "right") {
		setPosition(getPosition().getX()-10, getPosition().getY(), getPosition().getZ());
	}
	else if (_colisionSide == "down") {
		setPosition(getPosition().getX(), getPosition().getY(), getPosition().getZ()+10);
	}
}
