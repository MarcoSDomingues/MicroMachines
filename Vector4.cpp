#include "Vector4.h"

Vector4::Vector4() {

}

Vector4::Vector4(double x, double y, double z, double w) {
	_x = x;
	_y = y;
	_z = z;
	_w = w;
}

Vector4::~Vector4() {}

double Vector4::getW() {
	return _w;
}

void Vector4::set(double x, double y, double z, double w) {
	_x = x;
	_y = y;
	_z = z;
	_w = w;
}

Vector4& Vector4::operator=(const Vector4 &vec) {
	_x = vec._x;
	_y = vec._y;
	_z = vec._z;
	_w = vec._w;
	return *this;
}

Vector4& Vector4::operator*(double num) {
	_x = _x * num;
	_y = _y * num;
	_z = _z * num;
	_w = _w * num;
	return *this;
}

Vector4& Vector4::operator+(const Vector4 &vec) {
	_x += vec._x;
	_y += vec._y;
	_z += vec._z;
	_w += vec._w;
	return *this;
}

Vector4& Vector4::operator-(const Vector4 &vec) {
	_x -= vec._x;
	_y -= vec._y;
	_z -= vec._z;
	_w -= vec._w;
	return *this;

}