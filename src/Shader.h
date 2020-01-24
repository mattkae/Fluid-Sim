#ifndef SHADER_H
#define SHADER_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include "Matrix4x4f.h"
#include "Matrix3x3f.h"
#include "Vector4f.h"
#include "Logger.h"

typedef GLuint Shader;

Shader loadShader(const GLchar* vertexPath, const GLchar* fragmentPath = nullptr, const GLchar* geomPath = nullptr);
Shader reloadShader(Shader shader);
void watchForDirectorychanges(std::vector<Shader>& shadersToReload, const bool& isDying);

inline GLint getShaderUniform(const Shader& shader, const GLchar *name) {
    GLint uid = glGetUniformLocation(shader, name);
	if (uid < 0) {
		Logger::logError("Failed to find uniform for: " + std::string(name));
	}
	return uid;
}

inline void useShader(const Shader& shader) {
	glUseProgram(shader);
}

inline void setShaderFloat(const Shader& shader, const GLchar* name, GLfloat value) {
    glUniform1f(getShaderUniform(shader, name), value);
}

inline void setShaderInt(const Shader& shader, const GLchar* name, GLint value) {
	glUniform1i(getShaderUniform(shader, name), value);
}

inline void setShaderUint(const Shader& shader, const GLchar* name, GLuint value) {
	glUniform1ui(getShaderUniform(shader, name), value);
}

inline void setShaderVec2(const Shader& shader, const GLchar* name, glm::vec2 value) {
	glUniform2f(getShaderUniform(shader, name), value.x, value.y);
}

inline void setShaderVec3(const Shader& shader, const GLchar* name, glm::vec3 value) {
	glUniform3f(getShaderUniform(shader, name), value.x, value.y, value.z);
}

inline void setShaderVec4(const Shader& shader, const GLchar* name, glm::vec4 value) {
	glUniform4f(getShaderUniform(shader, name), value.x, value.y, value.z, value.w);
}

inline void setShaderVec4(const Shader& shader, const GLchar* name, const Vector4f& value) {
	glUniform4f(getShaderUniform(shader, name), value.x, value.y, value.z, value.w);
}

inline void setShaderMat3(const Shader& shader, const GLchar* name, glm::mat3 matrix) {
	glUniformMatrix3fv(getShaderUniform(shader, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

inline void setShaderMat3(const Shader& shader, const GLchar* name, const Matrix3x3f& matrix) {
	glUniformMatrix3fv(getShaderUniform(shader, name), 1, GL_FALSE, matrix.values);
}

inline void setShaderMat4(const Shader& shader, const GLchar* name, glm::mat4 matrix) {
	glUniformMatrix4fv(getShaderUniform(shader, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

inline void setShaderMat4(const Shader& shader, const GLchar* name, const Matrix4x4f& matrix) {
	glUniformMatrix4fv(getShaderUniform(shader, name), 1, GL_FALSE, matrix.values);
}

inline void setShaderVec3(const Shader& shader, const GLchar* name, const Vector3f& value) {
	glUniform3f(getShaderUniform(shader, name), value.x, value.y, value.z);
}

inline void setShaderBVec3(const Shader& shader, const GLchar* name, bool first, bool second, bool third) {
	glUniform3i(getShaderUniform(shader, name), first, second, third);
}

inline void setShaderVec3WithUniform(const Shader& shader, GLint uniform, glm::vec3 value) {
	glUniform3f(uniform, value.x, value.y, value.z);
}

inline void setShaderFloatWithUniform(const Shader& shader, GLint uniform, GLfloat value) {
    glUniform1f(uniform, value);
}

inline void setShaderIntWithUniform(const Shader& shader, GLint uniform, GLint value) {
	glUniform1i(uniform, value);
}

inline void setShaderMat4WithUniform(const Shader& shader, GLint uniform, glm::mat4 matrix) {
	glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(matrix));
}
#endif
