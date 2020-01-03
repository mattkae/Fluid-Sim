#pragma once
#include <GL/glew.h>
#include <cmath>
#include "Vector4f.h"

enum UnitDirectionVector {
	X = 0,
	Y = 1,
	Z = 2
};

struct Vector3f {
	GLfloat x = 0;
	GLfloat y = 0;
	GLfloat z = 0;
};

Vector3f addVector(const Vector3f& first, const Vector3f& second);
GLfloat dotProduct(const Vector3f& first, const Vector3f& second);
GLfloat length(const Vector3f& v);
Vector3f scale(const Vector3f&v, GLfloat scalar);
Vector3f subtractVector(const Vector3f& first, const Vector3f& second);
Vector3f subtractVector(const Vector3f& first, const Vector4f& second);
Vector3f negate(const Vector3f& v);
Vector3f cross(const Vector3f& first, const Vector3f& second);
Vector3f normalize(const Vector3f& v);
Vector3f fromVec4(const Vector4f& v);