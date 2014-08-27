#include "Vector3f.h"

void Vector3f::normalize()
{
	float len = _x * _x + _y * _y + _z * _z;
	if (len > 0.0f) {
		_x /= len;
		_y /= len;
		_z /= len;
	}
}

Vector3f Vector3f::crossProduct(const Vector3f& a, const Vector3f& b)
{
	Vector3f ret;

	ret.setX(a.y() * b.z() - a.z() * b.y());
	ret.setY(a.z() * b.x() - a.x() * b.z());
	ret.setZ(a.x() * b.y() - a.y() * b.x());

	return ret;
}

const Vector3f operator-(const Vector3f& v1, const Vector3f& v2)
{
	Vector3f ret;
	ret.setX(v1.x() - v2.x());
	ret.setY(v1.y() - v2.y());
	ret.setZ(v1.z() - v2.z());

	return ret;
}
