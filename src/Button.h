#pragma once
#include <functional>
#include <string>
#include <GL/glew.h>
#include "Shader.h"
#include "TextRenderer.h"
#include "Vector4f.h"
#include "Vector2f.h"
#include "Rectangle.h"
#include "MyString.h"

struct TextRenderer;
struct SceneUIController;

struct Button {
	String label;
	GLfloat padding = 0;
	Vector2f position;
	GLfloat scale = 1.0;
	Vector4f buttonColor;
	Vector4f hoverColor;
	Vector4f textColor;
	Vector4f borderColor;
	Vector4f clickColor;
	GLfloat width = 128.f;
	int focusToken = -1;
	bool isHovered = false;
	bool isClicked = false;
	Rectangle boundingRect;

	int data = -1;
	void (*mOnClick)(int data) = nullptr;
	
	void update(const TextRenderer& textRenderer);
	void render(const Shader& shader, const TextRenderer& textRenderer) const;
	void free();
};

inline GLfloat getButtonHeight(const Button& button, const TextRenderer& textRenderer) {
	return (textRenderer.getFontSize() + 2 * button.padding) * button.scale;
}
