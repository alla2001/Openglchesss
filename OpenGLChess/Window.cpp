#include "Window.h"

int Window::InitWindow()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (glfwInit()) 
	{
		return -1;
	}

	return 0;
}

int Window::CreateWindow(int width,int height)
{
	m_width = width;
	m_height = height;
	m_windowPtr  = glfwCreateWindow(m_width,m_height,"Gorgeous Chess",NULL,NULL);
	return 0;
}

int Window::CloseWindow()
{
	glfwSetWindowShouldClose(m_windowPtr, GLFW_TRUE);
	return 0;
}

int Window::SwapBuffers()
{
	glfwSwapBuffers(m_windowPtr);
	return 0;
}

int Window::MakeWindowCurrentCtx()
{
	glfwMakeContextCurrent(m_windowPtr);
	return 0;
}

int Window::ShouldClose()
{
	return glfwWindowShouldClose(m_windowPtr);
}

int Window::SwapBuffer()
{
	glfwSwapBuffers(m_windowPtr);
	return 0;
}

void Window::ResizeCallBack(void(*callback)(GLFWwindow*,int, int))
{
	glfwSetWindowSizeCallback(m_windowPtr, callback);
}

GLFWwindow* Window::GetWindowID()
{
		
	return (m_windowPtr);
}



