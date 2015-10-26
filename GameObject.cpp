#include "GameObject.h"
#include "Car.h"

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
	glBindTexture(GL_TEXTURE_2D, _textures.at(0));

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
	Car* car = (Car*)object;

	float dir = 1;
	if (car->getAcceleration() < 0)
		dir = -1;
	else
		dir = 1;

	if ((checkBoxCar.getX() >= checkBoxObject.getX()) && (checkBoxCar.getX() <= checkBoxObject.getY()) && (checkBoxCar.getW() >= checkBoxObject.getZ()) && (checkBoxCar.getW() <= checkBoxObject.getW())) {
		setPosition(getPosition().getX() + car->getDirection().getX()*0.03*dir, getPosition().getY(), getPosition().getZ() + car->getDirection().getZ()*0.03*dir);
		return true;
	}

	else if ((checkBoxCar.getY() >= checkBoxObject.getX()) && (checkBoxCar.getY() <= checkBoxObject.getY()) && (checkBoxCar.getW() >= checkBoxObject.getZ()) && (checkBoxCar.getW() <= checkBoxObject.getW())) {
		setPosition(getPosition().getX() + car->getDirection().getX()*0.03*dir, getPosition().getY(), getPosition().getZ() + car->getDirection().getZ()*0.03*dir);
		return true;
	}

	else if ((checkBoxCar.getX() >= checkBoxObject.getX()) && (checkBoxCar.getX() <= checkBoxObject.getY()) && (checkBoxCar.getZ() >= checkBoxObject.getZ()) && (checkBoxCar.getZ() <= checkBoxObject.getW())) {
		setPosition(getPosition().getX() + car->getDirection().getX()*0.03*dir, getPosition().getY(), getPosition().getZ() + car->getDirection().getZ()*0.03*dir);
		return true;
	}

	else if ((checkBoxCar.getY() >= checkBoxObject.getX()) && (checkBoxCar.getY() <= checkBoxObject.getY()) && (checkBoxCar.getZ() >= checkBoxObject.getZ()) && (checkBoxCar.getZ() <= checkBoxObject.getW())) {
		setPosition(getPosition().getX() + car->getDirection().getX()*0.03*dir, getPosition().getY(), getPosition().getZ() + car->getDirection().getZ()*0.03*dir);
		return true;
	}

	else {
		return false;
	}
}
