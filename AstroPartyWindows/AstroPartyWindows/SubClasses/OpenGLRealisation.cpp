#include "OpenGLRealisation.h"
#include <iostream>

OpenGL::OpenGL(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share, GLFWframebuffersizefun Function, GLFWwindow** window)
{
    InitOpenGL();
    *window = CreateWindows(width, height, title, monitor, share, Function);
    InitGlad();
    InitBuffers();
    InitShaders();
}

GLFWwindow* OpenGL::CreateWindows(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share, GLFWframebuffersizefun Function)
{
    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, Function);

    return window;
}

bool OpenGL::CanDrawFrame(GLFWwindow* window)
{
    return glfwWindowShouldClose(window) == false;
}

void OpenGL::DrawFrame()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    DrawObjectCurrentMenu();
}

void OpenGL::ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        exit(0);
    }
}

void OpenGL::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void OpenGL::DrawObjectCurrentMenu()
{
    bullet_shader->Use();
    glBindVertexArray(basic_triangle);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void OpenGL::DrawObjectIndicatedMenu(Menu* menu)
{
    Menu* last_menu = *game_p__current_active_menu;
    *game_p__current_active_menu = menu;
    DrawObjectCurrentMenu();
    *game_p__current_active_menu = last_menu;
}

#define VERTEX_ARRAY_LENGTH 6

void OpenGL::InitBuffers()
{
    float vertexes[VERTEX_ARRAY_LENGTH];

    //basic triangle

    vertexes[0] = 0.0f;                 //x1
    vertexes[1] = 0.5f;                 //y1
    vertexes[2] = sqrtf(3.0f) / 4.0f;   //x2
    vertexes[3] = -0.25f;               //y2
    vertexes[4] = -vertexes[2];         //x3
    vertexes[5] = -0.25f;               //y3

    glGenVertexArrays(1, &basic_triangle);
    glGenBuffers(1, &basic_triangle_buffer);

    glBindVertexArray(basic_triangle);

    glBindBuffer(GL_ARRAY_BUFFER, basic_triangle_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);       //disable vertex array
    glBindBuffer(GL_ARRAY_BUFFER, 0);   //unbind buffer
    glBindVertexArray(0);               //unbind array

    //basic triangle
}

void OpenGL::InitGlad()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }
}

void OpenGL::InitOpenGL()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void OpenGL::InitShaders()
{
    bullet_shader = new Shader("Shaders/Objects/Vertex/Bullet.glsl", "Shaders/Objects/Fragment/Bullet.glsl");
}

