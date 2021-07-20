#include "OpenGL.h"

OpenGL::OpenGL()
{
		//������������� GLFW
		glfwInit();
		//��������� GLFW
		//�������� ����������� ��������� ������ OpenGL. 
		//�������� 
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		//��������
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		//��������� �������� ��� �������� ��������� ��������
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//���������� ����������� ��������� ������� ����
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		window = nullptr;

}

void OpenGL::CreateWindow(uint32_t width, uint32_t height, const char* name, GLFWmonitor* monitor, GLFWwindow* share)
{
	GLFWwindow* window = glfwCreateWindow(width, height, name, monitor, share);
	if (window == nullptr)
	{
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);

	int n_width, n_height;
	glfwGetFramebufferSize(window, &n_width, &n_height);

	glViewport(0, 0, n_width, n_height);

	this->window = window;
}

uint8_t OpenGL::GetStatus()
{
	return !glfwWindowShouldClose(window);
}

GLFWwindow* OpenGL::GetWindow()
{
	return window;
}



