#ifndef OPENGL_REALISASTION_H
#define OPENGL_REALISASTION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GameRealisation.h"

class Shader
{
private:
public:
	Shader(const char* geometric_file_name, const char* fragment_file_name);

	void Use();
	void SetGeometric(const char* geometric_file_name);
	void SetFragment(const char* fragment_file_name);
	~Shader();

};

namespace OpenGL
{
	void InitOpneGL();
	void InitGlad();
	GLFWwindow* CreateWindows(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);
	bool CanDrawFrame(GLFWwindow* window);
	void DrawFrame();

	void BindShader();

	void ProcessInput(GLFWwindow* window);
	void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
};
#endif //OPENGL_REALISASTION_H