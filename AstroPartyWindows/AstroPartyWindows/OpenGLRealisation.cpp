#include "OpenGLRealisation.h"
#include <iostream>

GLFWwindow* OpenGL::CreateWindows(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
{
    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, OpenGL::FramebufferSizeCallback);

    return window;
}

bool OpenGL::CanDrawFrame(GLFWwindow* window)
{
    return !glfwWindowShouldClose(window);
}

void OpenGL::DrawFrame()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    DrawObject::CurrentMenu();
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

void OpenGL::DrawObject::CurrentMenu()
{
    bullet_shader->Use();
    glBindVertexArray(basic_triangle);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void OpenGL::DrawObject::IndicatedMenu(Menu* menu)
{
    Menu* last_menu = Game::current_active_menu;
    Game::current_active_menu = menu;
    CurrentMenu();
    Game::current_active_menu = last_menu;
}

#define VERTEX_ARRAY_LENGTH 12

void OpenGL::Init::Buffers()
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
    glGenBuffers(1, &basic_triangle_vertex);

    glBindVertexArray(basic_triangle);

    glBindBuffer(GL_ARRAY_BUFFER, basic_triangle_vertex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);       //disable vertex array
    glBindBuffer(GL_ARRAY_BUFFER, 0);   //unbind buffer
    glBindVertexArray(0);               //unbind array

    //basic triangle
}

void OpenGL::Init::Glad()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }
}

void OpenGL::Init::OpneGL()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void OpenGL::Init::Shaders()
{
    bullet_shader = new Shader("Shaders/Objects/Vertex/Bullet.glsl", "Shaders/Objects/Fragment/Bullet.glsl");
}

