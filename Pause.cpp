#include "Pause.h"

Pause::Pause() {
	_camera = new OrtogonalCamera(0, 800, 0, 600, -10, 10);
}

void Pause::draw(VSShaderLib shader, GLint pvm_uniformId, GLint vm_uniformId, GLint normal_uniformId)
{
	
	float ratio = ratio = (1.0f * glutGet(GLUT_WINDOW_WIDTH)) / glutGet(GLUT_WINDOW_HEIGHT);
	pushMatrix(PROJECTION); // Save the current matrix
	loadIdentity(PROJECTION); // We initialize the projection matrix as identity
	_camera->update(ratio);
	pushMatrix(VIEW); // Save the current matrix
	loadIdentity(VIEW); // Initialize the model matrix as identity
	GameObject::drawTextures();
	loadMesh(_meshes.at(0), shader);
	pushMatrix(MODEL);

	translate(MODEL, _position.getX(), _position.getY(), _position.getZ());
	scale(MODEL, 5.0f, 5.0f, 1.0f);

	renderMesh(_meshes.at(0), pvm_uniformId, vm_uniformId, normal_uniformId);
	popMatrix(MODEL);

	popMatrix(VIEW); // Restore the previous matrix
	popMatrix(PROJECTION); // Restore the previous matrix


}