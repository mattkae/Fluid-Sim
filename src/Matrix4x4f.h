#pragma once
#include "Vector3f.h"
#include "Vector4f.h"
#include <GL/glew.h>
#include <cstring>

constexpr int DIM_OF_MAT = 4;
constexpr int SIZE_OF_4_X_4 = DIM_OF_MAT * DIM_OF_MAT;

// Using coumn major, because OpenGL uses it internally, and we don't need that confusion
struct Matrix4x4f {
    GLfloat values[SIZE_OF_4_X_4] = { 0 };
};

inline Matrix4x4f copyMatrix(const Matrix4x4f matrix) {
    Matrix4x4f result;
    memcpy(result.values, matrix.values, 16 * sizeof(GLfloat));
    return result;
}

constexpr Matrix4x4f getIdentity();
Matrix4x4f scaleMatrix(const Matrix4x4f& matrix, const GLfloat scale);
Matrix4x4f scaleMatrix(const Matrix4x4f& matrix, const Vector3f& scale);
Matrix4x4f mult(const Matrix4x4f& first, const Matrix4x4f& second);
Matrix4x4f getScalarMatrix(const GLfloat scale);
Vector4f mult(const Matrix4x4f& matrix, const Vector4f& vec);
Matrix4x4f getXRotationMatrix(GLfloat angleRadians);
Matrix4x4f getYRotationMatrix(GLfloat angleRadians);
Matrix4x4f getZRotationMatrix(GLfloat angleRadians);
Matrix4x4f rotate(const Matrix4x4f matrix, GLfloat xRadians = 0, GLfloat yRadians = 0, GLfloat zRadians = 0);
Matrix4x4f translateMatrix(const Matrix4x4f& matrix, const Vector3f& translate);
Matrix4x4f transpose(const Matrix4x4f& matrix);
Matrix4x4f getLookAt(const Vector3f& eye, const Vector3f& pointToLookAt, const Vector3f& up);
Matrix4x4f getProjection(GLfloat near, GLfloat far, GLfloat fieldOfViewRadians, GLfloat aspectRatio);
bool inverse(const Matrix4x4f& matrix, Matrix4x4f& result);
