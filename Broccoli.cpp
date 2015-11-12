#include "Broccoli.h"

void Broccoli::setAngle(float angle) {
	_angle = acos(angle) * 180 / M_PI;
}

void Broccoli::setDirection(float x, float y, float z) {
	_dir.set(x, y, z);
}

void Broccoli::draw(VSShaderLib shader, GLint pvm_uniformId, GLint vm_uniformId, GLint normal_uniformId)
{
	GameObject::drawTextures();
	loadMesh(_meshes.at(0), shader);
	pushMatrix(MODEL);

	translate(MODEL, _position.getX(), _position.getY(), _position.getZ());
	rotate(MODEL, _angle, _dir.getX(), _dir.getY(), _dir.getZ());
	scale(MODEL, 1.0f, 1.0f, 0.0000001f);

	renderMesh(_meshes.at(0), pvm_uniformId, vm_uniformId, normal_uniformId);
	popMatrix(MODEL);
}
