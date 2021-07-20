#pragma once

#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class OpenGL
{
private:
	GLFWwindow* window;
public:

#define OPEN_GL_STATUSE_NORMAL 0x00
#define OPEN_GL_STATUSE_TERMINADE 0x01

	OpenGL();
	void CreateWindow(uint32_t width, uint32_t height, const char* name, GLFWmonitor* monitor, GLFWwindow* share);
	uint8_t GetStatus();
	GLFWwindow* GetWindow();
};

