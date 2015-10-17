#include "Cheerio.h"

void Cheerio::draw(VSShaderLib shader, GLint pvm_uniformId,
	GLint vm_uniformId, GLint normal_uniformId, GLint lPos_uniformId) {

	float xBot = 2.4f;		float yBot = 2.5f;
	float xTop = -2.4f;		float yTop = -3.6f;
	float xRight = 2.4f;	float yRight = -2.1f;
	float xLeft = -2.4f;	float yLeft = -2.1f;

	for (int i = 0; i < 17; i++) {
		cheerio(MODEL, xBot - 0.3f * i, 0.52f, yBot, shader, pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
		cheerio(MODEL, xBot - 0.3f * i, 0.52f, yBot + 1.2f, shader, pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
		cheerio(MODEL, xTop + 0.3f * i, 0.52f, yTop, shader, pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
		cheerio(MODEL, xTop + 0.3f * i, 0.52f, yTop + 1.2f, shader, pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
	}

	for (int i = 0; i < 15; i++) {
		cheerio(MODEL, xRight, 0.52f, yRight + 0.3f * i, shader, pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
		cheerio(MODEL, xRight + 1.2f, 0.52f, yRight + 0.3f * i, shader, pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
		cheerio(MODEL, xLeft, 0.52f, yLeft + 0.3f * i, shader, pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
		cheerio(MODEL, xLeft - 1.2f, 0.52f, yLeft + 0.3f * i, shader, pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
	}

}

void Cheerio::cheerio(MatrixTypes aType, float x, float y, float z, VSShaderLib shader, GLint pvm_uniformId,
	GLint vm_uniformId, GLint normal_uniformId, GLint lPos_uniformId) {

	loadMesh(_meshes.at(3), shader);
	pushMatrix(aType);

	translate(aType, x, y, z);

	renderMesh(_meshes.at(3), pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
	popMatrix(aType);

}