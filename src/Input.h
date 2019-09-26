#ifndef INPUT_H
#define INPUT_H
#include "BasicShapes.h"

struct GLFWwindow;
void initializeInputSystem(GLFWwindow* window);
bool isKeyUp(int key, int focusToken);
bool isKeyJustDown(int key, int focusToken);
bool isKeyDown(int key, int focusToken);
bool isKeyJustUp(int key, int focusToken);
bool isLeftClickDown();
bool isRightClickDown();
DoublePoint getCursorPosition();
int getNextFocusToken();
void setInputFocus(int focusToken);
void resetInputFocus();
int getInputFocus();
int getCurrentKeyDown(int focusToken);
int getCurrentScancode(int focusToken);
void getWindowDimensions(int& width, int& height);

#endif
