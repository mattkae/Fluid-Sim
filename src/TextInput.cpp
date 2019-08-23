#include "TextInput.h"
#include "Input.h"
#include "TextRenderer.h"
#include "Logger.h"
#include <GLFW/glfw3.h>

bool initializeTextInput(TextInput& input) {
	input.r = {
		input.position.x,
		input.position.y,
		input.dimensions.x,
		input.dimensions.y
	};
	input.r.backgroundColor = input.backgroundColor;
	input.r.borderColor = input.borderColor;
	input.focusToken = getNextFocusToken();
	initializeRenderableRectangle(input.r);
	input.cursorPosition = input.str.size();
	return true;
}

void updateTextInput(TextInput& textInput) {
	updateRenderableRectangle(textInput.r);
	if (textInput.r.isFocused && textInput.r.isJustClicked) {
		setInputFocus(textInput.focusToken);
	} else if (getInputFocus() == textInput.focusToken && !textInput.r.isFocused) {
		resetInputFocus();
	}

	int key = getCurrentKeyDown(textInput.focusToken);
	int scancode = getCurrentScancode(textInput.focusToken);

	if (key > -1 && isKeyJustDown(key, textInput.focusToken)) {
		switch (key) {
		case GLFW_KEY_LEFT:
			textInput.cursorPosition = textInput.cursorPosition == 0 ? 0 : textInput.cursorPosition - 1;
			break;
		case GLFW_KEY_RIGHT:
			textInput.cursorPosition = textInput.cursorPosition == textInput.str.size() ? textInput.str.size() : textInput.cursorPosition + 1;
			break;
		case GLFW_KEY_BACKSPACE:
			if (!textInput.str.empty()) {
				textInput.str.erase(textInput.cursorPosition - 1, textInput.cursorPosition);
				if (textInput.cursorPosition != 0) {
					textInput.cursorPosition--;
				}
			}
			break;
		case GLFW_KEY_LEFT_SHIFT:
		case GLFW_KEY_RIGHT_SHIFT:
			break;
		default: {
			char c = (char) key;
			
			if (!isKeyDown(GLFW_KEY_LEFT_SHIFT, textInput.focusToken)) {
				c = std::tolower(c);
			}

			if (textInput.cursorPosition == textInput.str.size()) {
				textInput.str += c;
			} else {
				textInput.str.insert(textInput.cursorPosition, &c);
			}

			textInput.cursorPosition++;
		}
		}
	}
}

void renderTextInput(const TextInput& textInput, const Shader& shader) {
	renderRenderableRectangle(textInput.r, shader);
	textInput.textRenderer->renderText(textInput.str, textInput.position, textInput.scale, textInput.textColor);
}