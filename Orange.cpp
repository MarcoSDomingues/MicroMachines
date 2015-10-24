#include "Orange.h"

void Orange::setSpeed(float speed)
{
	_speed = speed;
}

void Orange::setDelayDraw(bool delay)
{
	_delayDraw = delay;
}

float Orange::getSpeed() {
	return _speed;
}

bool Orange::getDelayDraw()
{
	return _delayDraw;
}

float Orange::randomVelocity() {
	float auxV = ((float(rand()) / float(RAND_MAX)) * (0.001 - (-0.001))) + (-0.001);
	if (auxV == 0) {
		randomVelocity();
	}
	else
		return auxV;
}

void Orange::init() {
	
	int orangeX = rand() %9 - 4;
	int orangeZ = rand() %9 - 4;
	_position.set(orangeX, 1.0f, orangeZ);
	_velocity = randomVelocity();
	setDelayDraw(false);
}

void Orange::draw(VSShaderLib shader, GLint pvm_uniformId, GLint vm_uniformId, GLint normal_uniformId) {

	loadMesh(_meshes.at(0), shader);

	pushMatrix(MODEL);
	translate(MODEL, _position.getX(), _position.getY(), _position.getZ());
	scale(MODEL, 0.6f, 0.6f, 0.6f);
	rotate(MODEL, _yRotation, 0, 0, _yRotation);
	renderMesh(_meshes.at(0), pvm_uniformId, vm_uniformId, normal_uniformId);
	popMatrix(MODEL);
}

void Orange::update(double delta_t)
{
	//aumentar a velocidade depois de um certo tempo
	if (glutGet(GLUT_ELAPSED_TIME) > _speed_timer + 20000) {
		_speed_timer = glutGet(GLUT_ELAPSED_TIME);
		_speedInc += 1;
	}

	_xPosition = _xPosition + _velocity * _speedInc * delta_t;
	_yRotation = _yRotation + (_velocity * 100 * _speedInc) * delta_t; //*100 to rotate slower

	_position.set(_xPosition, _position.getY(), _position.getZ());
}

