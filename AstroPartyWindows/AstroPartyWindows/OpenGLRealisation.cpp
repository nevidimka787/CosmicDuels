#include "OpenGLRealisation.h"
#include <iostream>

Shader::Shader(const char* geometric_file_name, const char* fragment_file_name)
{

}

void Shader::SetFragment(const char* fragment_file_name)
{

}

void Shader::SetGeometric(const char* geometric_file_name)
{

}

void Shader::Use()
{

}

Shader::~Shader()
{

}

void OpenGL::InitOpneGL()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void OpenGL::InitGlad()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }
}

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
}

void OpenGL::BindShader()
{

}

void OpenGL::ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void OpenGL::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}



void OpenGL::DrawMenu(Menu* menu)
{
    Menu* last_menu = Game::current_active_menu;
    Game::current_active_menu = menu;
    DrawCurrentMenu();
    Game::current_active_menu = last_menu;
}

void OpenGL::DrawCurrentMenu()
{}

void OpenGL::DrawObject(Line* line, bool update_shader)
{}

void OpenGL::DrawObject(Beam* beam, bool update_shader)
{}

void OpenGL::DrawObject(Segment* segment, bool update_shader)
{}

void OpenGL::DrawObject(Entity* entity, bool update_shader)
{}

void OpenGL::DrawObject(StaticEntity* static_entity, bool update_shader)
{}

void OpenGL::DrawObject(DynamicEntity* dynamic_entity, bool update_shader)
{}

void OpenGL::DrawObject(Bonus* bonus, bool update_shader)
{}

void OpenGL::DrawObject(Asteroid* asteroid, bool update_shader)
{}

void OpenGL::DrawObject(ControledEntity controled_entity, bool update_shader)
{}

void OpenGL::DrawObject(Sheep* sheep, bool update_shader)
{}

void OpenGL::DrawObject(Pilot* pilot, bool update_shader)
{}

void OpenGL::DrawObject(Turel* turel, bool update_shader)
{}

void OpenGL::DrawObject(MegaLazer* mega_lazer, bool update_shader)
{}

void OpenGL::DrawObject(Mine* mine, bool update_shader)
{}

void OpenGL::DrawObject(MapElement* map_element, bool update_shader)
{}

void OpenGL::DrawObject(Rectangle* rectangle, bool update_shader)
{}

void OpenGL::DrawObject(Cyrcle* cyrcle, bool update_shader)
{}

void OpenGL::DrawObject(Polygon* polygon, bool update_shader)
{}

void OpenGL::DrawObject(Map* map, bool update_shader)
{}