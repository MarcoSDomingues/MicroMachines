#pragma once
#include "GameObject.h"
class Camera :
	public GameObject
{
protected:
	Vector3 _up;
	Vector3* _at;
	double _near;
	double _far;

public:
	Camera(double near, double far);
	~Camera();
	virtual void update();
	virtual void computeProjectionMatrix();
	virtual void computeVisualizationMatrix();
	void setAt(double x, double y, double z);

};