#include "UIContext.h"
#include "GlobalApplicationState.h"

void updateUIContext(UIContext& context, const TextRenderer& textRenderer) {
	if (context.shouldOpen && !context.isActive) {
		context.isActive = true;
		context.shouldOpen = false;
	} else if (context.shouldClose && context.isActive) {
		context.shouldClose = false;
		context.isActive = false;
	}

	if (!context.isActive) {
		return;
	}


	setPanelPosition(context.panel);
	GLfloat yOffset = GlobalAppState.floatHeight - context.panel.boundingRect.y - context.panel.padding;
	GLfloat xPosition = context.panel.boundingRect.x + context.panel.padding;
	
	for (size_t elementIndex = 0; elementIndex < context.numUiElements; elementIndex++) {
		UIElement& element = context.uiElements[elementIndex];
		switch (element.type) {
		case UIElement::BUTTON: {
			Button& button = std::get<Button>(element.element);
			if (elementIndex == 0) {
				yOffset -= getButtonHeight(button, textRenderer);
			}
			button.position = glm::vec2(xPosition, yOffset);
			button.width = GlobalAppState.floatWidth * context.panel.percentageWidth - 2 * button.padding;
			updateButton(button, textRenderer);
			yOffset -= getButtonHeight(button, textRenderer);
			break;
		}
		case UIElement::TEXT_INPUT: {
			TextInput& textInput = std::get<TextInput>(element.element);
			if (elementIndex == 0) {
				yOffset -= getBoundTextHeight(textInput.bt, textRenderer);
			}
			textInput.bt.rect.x = xPosition;
			textInput.bt.rect.y = yOffset;
			textInput.bt.rect.w = GlobalAppState.floatWidth * context.panel.percentageWidth - 2 * textInput.bt.padding;
			updateTextInput(textInput, textRenderer);
			yOffset -= getBoundTextHeight(textInput.bt, textRenderer);
			break;
		}
		case UIElement::LABEL: {
			Label& label = std::get<Label>(element.element);
			GLfloat btHeight = getBoundTextHeight(label.bt, textRenderer);
			if (elementIndex == 0) {
				yOffset -= btHeight;
			}

			label.bt.rect.x = xPosition;
			label.bt.rect.y = yOffset;
			label.bt.rect.w = GlobalAppState.floatWidth * context.panel.percentageWidth - 2 * label.bt.padding;
			label.bt.rect.h = btHeight;
			yOffset -= btHeight;
			break;
		}
		}

		yOffset -= context.spaceBetweenElements;
	}
}

void renderUIContext(const UIContext& context, const Shader& shader, const TextRenderer& textRenderer) {
	if (!context.isActive) {
		return;
	}

	renderPanel(context.panel, shader);

	for (size_t elementIndex = 0; elementIndex < context.numUiElements; elementIndex++) {
		const UIElement& element = context.uiElements[elementIndex];
		switch (element.type) {
		case UIElement::BUTTON:
			renderButton(std::get<Button>(element.element), shader, textRenderer);
			break;
		case UIElement::TEXT_INPUT:
			renderTextInput(std::get<TextInput>(element.element), shader, textRenderer);
			break;
		case UIElement::LABEL:
			renderLabel(std::get<Label>(element.element), shader, textRenderer);
			break;
		}
	}
}

void freeUIContext(UIContext& context) {
	delete[] context.uiElements;
	context.numUiElements = 0;
}