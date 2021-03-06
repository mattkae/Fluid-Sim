#ifndef INPUT_H
#define INPUT_H
#include "Vector2f.h"

const int DEFAULT_FOCUS_TOKEN = 0;

struct GLFWwindow;
void initializeInputSystem(GLFWwindow* window);
void deallocateInputSystem();
bool isKeyUp(int key, int focusToken);
bool isKeyJustDown(int key, int focusToken);
bool isKeyDown(int key, int focusToken);
bool isKeyJustUp(int key, int focusToken);
bool isLeftClickDown(int focusToken = DEFAULT_FOCUS_TOKEN);
bool isRightClickDown();
Vector2f getCursorPosition();
void resetFocus();
int grabFocus();
void returnFocus(int focusToken);
int getInputFocus();
int getCurrentKeyDown(int focusToken);
int getCurrentScancode(int focusToken);
void getWindowDimensions(int& width, int& height);
bool isDefaultFocused();
void setCursorToText();
void setCursorToPointer();
void resetCursor();

#endif
