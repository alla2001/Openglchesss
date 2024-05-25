#pragma once
#include <stdint.h>
#include "GLFW/glfw3.h"
class Window {
	
	
	GLFWwindow* m_windowPtr = nullptr;
public :
	uint16_t m_width = 450;
	uint16_t m_height = 450;
	int InitWindow();
	int CreateWindow(int width, int height);
	int CloseWindow();
	int SwapBuffers();
	int MakeWindowCurrentCtx();
	int ShouldClose();
	int SwapBuffer();
	void ResizeCallBack(void(*callback)(GLFWwindow*, int, int));
	GLFWwindow* GetWindowID();
	
};