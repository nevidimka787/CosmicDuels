#include "OpenGLRealisation.h"
#include <iostream>

OpenGL::OpenGL(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share, GLFWframebuffersizefun Function, GLFWwindow** window)
{
    flag_update_menu = false;
    
    InitOpenGL();
    *window = CreateWindows(width, height, title, monitor, share, Function);
    window_height = height;
    window_width = width;
    InitGlad();
    InitBuffers();
    InitShaders();
    InitTextures();
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
    if (flag_update_menu > 0)
    {
        flag_update_menu--;
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        flag_update_menu = 10;
        glfwSetWindowShouldClose(window, true);
        exit(0);
    }
    if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS && (mouse_buttons & OPEN_GL_REALISATION_BACK) == OPEN_GL_REALISATION_NOTHING)
    {
        flag_update_menu = 10;
        mouse_buttons |= OPEN_GL_REALISATION_BACK;
        object_p__menu_functions->Back();
    }
    else if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_RELEASE && (mouse_buttons & OPEN_GL_REALISATION_BACK) != OPEN_GL_REALISATION_NOTHING)
    {
        flag_update_menu = 10;
        mouse_buttons &= OPEN_GL_REALISATION_FULL - OPEN_GL_REALISATION_BACK;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && (mouse_buttons & OPEN_GL_REALISATION_SELECT) == OPEN_GL_REALISATION_NOTHING)
    {
        flag_update_menu = 10;
        mouse_buttons |= OPEN_GL_REALISATION_SELECT;
        Vec2D clk_pos_d;
        glfwGetCursorPos(window, &clk_pos_d.x, &clk_pos_d.y);
        Vec2F clk_pos = Vec2F(((float)clk_pos_d.x / window_width - 0.5f) * 2.0f, ((float)clk_pos_d.y / -window_height + 0.5f) * 2.0f);
        if (*game_p__current_active_menu == game_p__main_menu)
        {
            object_p__menu_functions->MainMenuFunction(&clk_pos);
        }
        else if (*game_p__current_active_menu == game_p__option_menu)
        {
            object_p__menu_functions->OptionMenuFunction(&clk_pos);
        }
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && (mouse_buttons & OPEN_GL_REALISATION_SELECT) != OPEN_GL_REALISATION_NOTHING)
    {
        flag_update_menu = 10;
        mouse_buttons &= OPEN_GL_REALISATION_FULL - OPEN_GL_REALISATION_SELECT;
    }
}

void OpenGL::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    flag_update_menu = 10;
    window_height = height;
    window_width = width;
    glViewport(0, 0, width, height);
}

void OpenGL::DrawObjectCurrentMenu()
{
    DrawObjectIndicatedMenu(*game_p__current_active_menu);
}

void OpenGL::DrawObjectIndicatedMenu(Menu* menu)
{
    button_shader->Use();
    symbols_texture->Use();
    for (ClassTypes::Menu::buttons_count_t button = 0; button < menu->GetButtonsCount(); button++)
    {
        DrawObject(&menu->current_buttons[button], false);
    }
}

#define VERTEX_ARRAY_LENGTH 12

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

    //basic square

    vertexes[0] = 1.0f;                 //x1
    vertexes[1] = 1.0f;                 //y1
    vertexes[2] = 0.0f;                 //x2
    vertexes[3] = 1.0f;                 //y2
    vertexes[4] = 1.0f;                 //x3
    vertexes[5] = 0.0f;                 //y3
    vertexes[6] = 0.0f;                 //x1
    vertexes[7] = 0.0f;                 //y1
    vertexes[8] = 0.0f;                 //x4
    vertexes[9] = 1.0f;                 //y4
    vertexes[10] = 1.0f;                //x3
    vertexes[11] = 0.0f;                //y3
    
    glGenVertexArrays(1, &basic_square);
    glGenBuffers(1, &basic_square_buffer);

    glBindVertexArray(basic_square);

    glBindBuffer(GL_ARRAY_BUFFER, basic_square_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);       //disable vertex array
    glBindBuffer(GL_ARRAY_BUFFER, 0);   //unbind buffer
    glBindVertexArray(0);               //unbind array

    //basic square

    //long triangle

    vertexes[0] = 0.0f;                 //x1
    vertexes[1] = 1.0f;                 //y1
    vertexes[2] = -0.5f;                //x2
    vertexes[3] = -1.0f;                //y2
    vertexes[4] = 0.5f;                 //x3
    vertexes[5] = -1.0f;                //y3

    glGenVertexArrays(1, &long_triangle);
    glGenBuffers(1, &long_triangle_buffer);

    glBindVertexArray(long_triangle);

    glBindBuffer(GL_ARRAY_BUFFER, long_triangle_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);       //disable vertex array
    glBindBuffer(GL_ARRAY_BUFFER, 0);   //unbind buffer
    glBindVertexArray(0);               //unbind array

    //long trriangle
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
    asteroid_shader = new Shader("Shaders/Objects/Vertex/Asteroid.glsl", "Shaders/Objects/Fragment/Asteroid.glsl");
    bomb_shader = new Shader("Shaders/Objects/Vertex/Bomb.glsl", "Shaders/Objects/Fragment/Bomb.glsl");
    bonus_shader = new Shader("Shaders/Objects/Vertex/Bonus.glsl", "Shaders/Objects/Fragment/Bonus.glsl");
    bullet_shader = new Shader("Shaders/Objects/Vertex/Bullet.glsl", "Shaders/Objects/Fragment/Bullet.glsl");
    knife_shader = new Shader("Shaders/Objects/Vertex/Knife.glsl", "Shaders/Objects/Fragment/Knife.glsl");
    mega_laser_sahder = new Shader("Shaders/Objects/Vertex/MegaLaser.glsl", "Shaders/Objects/Fragment/MegaLaser.glsl");
    pilot_shader = new Shader("Shaders/Objects/Vertex/Pilot.glsl", "Shaders/Objects/Fragment/Pilot.glsl");
    ship_shader = new Shader("Shaders/Objects/Vertex/Ship.glsl", "Shaders/Objects/Fragment/Ship.glsl");
    turel_shader = new Shader("Shaders/Objects/Vertex/Turel.glsl", "Shaders/Objects/Fragment/Turel.glsl");

    rectangle_shader = new Shader("Shaders/Map/Vertex/Rectangle.glsl", "Shaders/Map/Fragment/Rectangle.glsl");
    cyrcle_shader = new Shader("Shaders/Map/Vertex/Cyrcle.glsl", "Shaders/Map/Fragment/Cyrcle.glsl");
    polygon_shader = new Shader("Shaders/Map/Vertex/Polygon.glsl", "Shaders/Map/Fragment/Polygon.glsl");

    button_shader = new Shader("Shaders/Menu/Vertex/Button.glsl", "Shaders/Menu/Fragment/Button.glsl");
}

void OpenGL::InitTextures()
{
    symbols_texture = new Texture2D("Textures/Sample.bmp");
}

void OpenGL::DrawObjectCurrentMap()
{

}

void OpenGL::DrawObject(Line* line, bool update_shader)
{

}

void OpenGL::DrawObject(Beam* beam, bool update_shader)
{

}

void OpenGL::DrawObject(Segment* segment, bool update_shader)
{

}

void OpenGL::DrawObject(Entity* entity, bool update_shader)
{

}

void OpenGL::DrawObject(StaticEntity* static_entity, bool update_shader)
{

}

void OpenGL::DrawObject(DynamicEntity* dynamic_entity, bool update_shader)
{

}

void OpenGL::DrawObject(Asteroid* asteroid, bool update_shader)
{

}

void OpenGL::DrawObject(Bomb* mine, bool update_shader)
{

}

void OpenGL::DrawObject(Bonus* bonus, bool update_shader)
{

}

void OpenGL::DrawObject(Bullet* bullet, bool update_shader)
{

}

void OpenGL::DrawObject(Knife* knife, bool update_shader)
{

}

void OpenGL::DrawObject(MegaLaser* mega_laser, bool update_shader)
{

}

void OpenGL::DrawObject(Pilot* pilot, bool update_shader)
{

}

void OpenGL::DrawObject(Ship* ship, bool update_shader)
{

}

void OpenGL::DrawObject(Turel* turel, bool update_shader)
{

}

void OpenGL::DrawObject(MapElement* map_element, bool update_shader)
{

}

void OpenGL::DrawObject(Rectangle* rectangle, bool update_shader)
{

}

void OpenGL::DrawObject(Cyrcle* cyrcle, bool update_shader)
{

}

void OpenGL::DrawObject(Polygon* polygon, bool update_shader)
{

}


void OpenGL::DrawObject(Button* button, bool update_shader)
{
#define BUTTON_SHADER_VERTEX_UNIFORMS_COUNT             2
#define BUTTON_SHADER_FRAGMENT_UNIFORMS_COUNT           5
#define BUTTON_SHADER_VERTEX_UNIFORM_START_POSITION     0
#define BUTTON_SHADER_FRAGMENT_UNIFORM_START_POSITION   BUTTON_SHADER_VERTEX_UNIFORMS_COUNT

#define BUTTON_SHADER_UNIFORM_POSITION          (BUTTON_SHADER_VERTEX_UNIFORM_START_POSITION + 0)
#define BUTTON_SHADER_UNIFORM_SIZE              (BUTTON_SHADER_VERTEX_UNIFORM_START_POSITION + 1)

#define BUTTON_SHADER_UNIFORM_COLOR             (BUTTON_SHADER_FRAGMENT_UNIFORM_START_POSITION + 0)
#define BUTTON_SHADER_UNIFORM_TEXT_SIZE         (BUTTON_SHADER_FRAGMENT_UNIFORM_START_POSITION + 1)
#define BUTTON_SHADER_UNIFORM_TEXT_LENGTH       (BUTTON_SHADER_FRAGMENT_UNIFORM_START_POSITION + 2)

#define BUTTON_SHADER_UNIFORM_TEXT              (BUTTON_SHADER_FRAGMENT_UNIFORM_START_POSITION + 3)
#define BUTTON_SHADER_UNIFORM_TEXT_BUFFER_SIZE  500

    if (update_shader)
    {
        button_shader->Use();
        symbols_texture->Use();
    }
    button_shader->SetUniform("position", button->GetPosition());
    button_shader->SetUniform("size", button->GetSize());
    button_shader->SetUniform("color", (button->status & BUTTOM_STATUS_ACTIVE) ? Color3F(0.3f) : Color3F(0.7f));
    button_shader->SetUniform("text", button->GetText(), button->GetTextLength());
    button_shader->SetUniform("text_length", button->GetTextLength());
    button_shader->SetUniform("text_size", button->text_size);
    glBindVertexArray(basic_square);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void OpenGL::DrawAsteroids()
{

}

void OpenGL::DrawBombs()
{

}

void OpenGL::DrawBonuses()
{

}

void OpenGL::DrawBullets()
{

}

void OpenGL::DrawKnifes()
{

}

void OpenGL::DrawMegaLasers()
{

}

void OpenGL::DrawPilots()
{

}

void OpenGL::DrawShips()
{

}

void OpenGL::DrawTurels()
{

}
