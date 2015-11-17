#include "Sun.h"
void Sun::draw(VSShaderLib shader, GLint pvm_uniformId, GLint vm_uniformId, GLint normal_uniformId)
{
	GameObject::drawTextures();
	loadMesh(_meshes.at(0), shader);
	pushMatrix(MODEL);

	translate(MODEL, _position.getX(), _position.getY(), _position.getZ());
	rotate(MODEL, -45, 0, 1, 0);
	scale(MODEL, 1.0f, 1.0f, 0.0001f);

	renderMesh(_meshes.at(0), pvm_uniformId, vm_uniformId, normal_uniformId);
	popMatrix(MODEL);
}