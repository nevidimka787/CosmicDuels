#include "OpenGLRealisation.h"
#include <iostream>

OpenGL::OpenGL(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share, GLFWframebuffersizefun Function, GLFWwindow** window)
    : cursore_press_position (new Vec2D()), cursore_release_position(new Vec2D()), cursore_last_position(new Vec2D()), cursore_current_position(new Vec2D())
{
    flag_update_menu = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT;
    flag_update_menu_can_change = true;
    
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
    window_scale = (float)width / (float)height;
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
    glClearColor(0.1f, 0.1f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (*game_p__flag_all_entities_initialisate == true)
    {
        DrawShips();
    }


    DrawObjectCurrentMenu();
}

void OpenGL::ProcessInput(GLFWwindow* window)
{
    if (flag_update_menu > 0 && flag_update_menu_can_change)
    {
        flag_update_menu--;
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        flag_update_menu = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT;
        glfwSetWindowShouldClose(window, true);
        exit(0);
    }
    if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS && (mouse_buttons & OPEN_GL_REALISATION_BACK) == OPEN_GL_REALISATION_NOTHING)
    {
        flag_update_menu = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT;
        mouse_buttons |= OPEN_GL_REALISATION_BACK;
        object_p__menu_functions->Back();
    }
    else if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_RELEASE && (mouse_buttons & OPEN_GL_REALISATION_BACK) != OPEN_GL_REALISATION_NOTHING)
    {
        flag_update_menu = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT;
        mouse_buttons &= OPEN_GL_REALISATION_FULL - OPEN_GL_REALISATION_BACK;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && (mouse_buttons & OPEN_GL_REALISATION_SELECT) == OPEN_GL_REALISATION_NOTHING)
    {
        flag_update_menu = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT;
        flag_update_menu_can_change = false;
        flag_move_menu = true;

        mouse_buttons |= OPEN_GL_REALISATION_SELECT;
        glfwGetCursorPos(window, &cursore_press_position->x, &cursore_press_position->y);
        *cursore_last_position = *cursore_press_position;

        Vec2F clk_pos = Vec2F(((float)cursore_press_position->x / window_width - 0.5f) * 2.0f, ((float)cursore_press_position->y / -window_height) * 2.0f / window_scale + 1.0f);
        //std::cout << clk_pos.y << std::endl;
        CallMenuFunction(*game_p__current_active_menu, &clk_pos, GLFW_PRESS);
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && (mouse_buttons & OPEN_GL_REALISATION_SELECT) != OPEN_GL_REALISATION_NOTHING)
    {
        flag_update_menu = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT;
        flag_update_menu_can_change = true;
        flag_move_menu = false;

        mouse_buttons &= OPEN_GL_REALISATION_FULL - OPEN_GL_REALISATION_SELECT;
        glfwGetCursorPos(window, &cursore_release_position->x, &cursore_release_position->y);
        *cursore_press_position -= *cursore_release_position;
        if (fabs(cursore_press_position->x) < window_width / 100.0 && fabs(cursore_press_position->y) < window_height / 100.0)
        {
            Vec2F clk_pos = Vec2F(((float)cursore_release_position->x / window_width - 0.5f) * 2.0f, ((float)cursore_release_position->y / -window_height) * 2.0f / window_scale + 1.0f);
            CallMenuFunction(*game_p__current_active_menu, &clk_pos, GLFW_RELEASE);
        }
        else
        {
            CallMenuFunction(*game_p__current_active_menu, nullptr, OPEN_GL_REALISATION_BUTTON_LOST);
        }
    }
    if (flag_move_menu)
    {
        glfwGetCursorPos(window, &cursore_current_position->x, &cursore_current_position->y);

        Vec2D move_vector_d = *cursore_current_position - *cursore_last_position;
        Vec2F move_vector = Vec2F(0.0f, (float)move_vector_d.y / window_height * -2.0f / window_scale);
        
        (*game_p__current_active_menu)->Move(&move_vector);
        LimitMenuPosition(*game_p__current_active_menu);
        *cursore_last_position = *cursore_current_position;
    }
}

void OpenGL::LimitMenuPosition(Menu* menu)
{
    Vec2F new_position = Vec2F();
    new_position.y = -menu->GetSize().y - 2.0f / window_scale;
    //std::cout << new_position.y << std::endl;
    if (menu->GetPosition().y > new_position.y)
    {
        menu->SetPosition(&new_position);
    }
    if (menu->GetPosition().y < 0.0f)
    {
        new_position.y = 0.0f;
        menu->SetPosition(&new_position);
    }
}

void OpenGL::CallMenuFunction(Menu* menu, Vec2F* clk_pos, uint8_t clk_status)
{
    if (menu == *game_p__main_menu)
    {
        object_p__menu_functions->MainMenuFunction(clk_pos, clk_status);
    }
    else if (menu == *game_p__option_menu)
    {
        object_p__menu_functions->OptionMenuFunction(clk_pos, clk_status);
    }
    else if (menu == *game_p__spawning_objects_select_menu)
    {
        object_p__menu_functions->SpawnObjectsSelectMenuFunction(clk_pos, clk_status);
    }
    else if (menu == *game_p__ships_select_menu)
    {
        object_p__menu_functions->ShipsSelectMenuFunction(clk_pos, clk_status);
    }
    else if (menu == *game_p__map_pull_select_menu)
    {
        object_p__menu_functions->MapPullSelectMenuFunction(clk_pos, clk_status);
    }
}

void OpenGL::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    flag_update_menu = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT;
    window_height = height;
    window_width = width;
    window_scale = (float)width / (float)height;
    LimitMenuPosition(*game_p__current_active_menu);
    glViewport(0, 0, width, height);
}

void OpenGL::DrawObjectCurrentMenu()
{
    DrawObjectIndicatedMenu(*game_p__current_active_menu);
}

void OpenGL::DrawObjectIndicatedMenu(Menu* menu)
{
    if (menu == *game_p__ships_control_menu)
    {
        controler_shader->Use();
        controler_shader->SetUniform("scale", window_scale);
        glBindVertexArray(right_triangle_buffer);
        for (ClassTypes::Menu::buttons_count_t button = 0; button < menu->GetButtonsCount(); button++)
        {
            DrawObject((ControledButton*)&menu->current_buttons[button]);
        }
    }
    else
    {
        button_shader->Use();
        symbols_texture->Use();
        button_shader->SetUniform("scale", window_scale);
        glBindVertexArray(basic_square);
        for (ClassTypes::Menu::buttons_count_t button = 0; button < menu->GetButtonsCount(); button++)
        {
            DrawObject(&menu->current_buttons[button]);
        }
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

    //right åriangle

    vertexes[0] = 1.0f;                 //x1
    vertexes[1] = 0.0f;                 //y1
    vertexes[2] = 0.0f;                 //x2
    vertexes[3] = 0.0f;                 //y2
    vertexes[4] = 0.0f;                 //x3
    vertexes[5] = 1.0f;                 //y3

    glGenVertexArrays(1, &right_triangle);
    glGenBuffers(1, &right_triangle_buffer);

    glBindVertexArray(right_triangle);

    glBindBuffer(GL_ARRAY_BUFFER, right_triangle_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);       //disable vertex array
    glBindBuffer(GL_ARRAY_BUFFER, 0);   //unbind buffer
    glBindVertexArray(0);               //unbind array

    //right åriangle
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
    asteroid_shader = new Shader();
    asteroid_shader->Initialisate("Shaders/Objects/Vertex/Asteroid.glsl", "Shaders/Objects/Fragment/Asteroid.glsl");
    bomb_shader = new Shader();
    bomb_shader->Initialisate("Shaders/Objects/Vertex/Bomb.glsl", "Shaders/Objects/Fragment/Bomb.glsl");
    bonus_shader = new Shader();
    bonus_shader->Initialisate("Shaders/Objects/Vertex/Bonus.glsl", "Shaders/Objects/Fragment/Bonus.glsl");
    bullet_shader = new Shader();
    bullet_shader->Initialisate("Shaders/Objects/Vertex/Bullet.glsl", "Shaders/Objects/Fragment/Bullet.glsl");
    knife_shader = new Shader();
    knife_shader->Initialisate("Shaders/Objects/Vertex/Knife.glsl", "Shaders/Objects/Fragment/Knife.glsl");
    mega_laser_sahder = new Shader();
    mega_laser_sahder->Initialisate("Shaders/Objects/Vertex/MegaLaser.glsl", "Shaders/Objects/Fragment/MegaLaser.glsl");
    pilot_shader = new Shader();
    pilot_shader->Initialisate("Shaders/Objects/Vertex/Pilot.glsl", "Shaders/Objects/Fragment/Pilot.glsl");
    ship_shader = new Shader();
    ship_shader->Initialisate("Shaders/Objects/Vertex/Ship.glsl", "Shaders/Objects/Fragment/Ship.glsl");
    turel_shader = new Shader();
    turel_shader->Initialisate("Shaders/Objects/Vertex/Turel.glsl", "Shaders/Objects/Fragment/Turel.glsl");
    
    rectangle_shader = new Shader();
    rectangle_shader->Initialisate("Shaders/Map/Vertex/Rectangle.glsl", "Shaders/Map/Fragment/Rectangle.glsl");
    cyrcle_shader = new Shader();
    cyrcle_shader->Initialisate("Shaders/Map/Vertex/Cyrcle.glsl", "Shaders/Map/Fragment/Cyrcle.glsl");
    polygon_shader = new Shader();
    polygon_shader->Initialisate("Shaders/Map/Vertex/Polygon.glsl", "Shaders/Map/Fragment/Polygon.glsl");
    
    button_shader = new Shader();
    button_shader->Initialisate("Shaders/Menu/Vertex/Button.glsl", "Shaders/Menu/Fragment/Button.glsl");
    controler_shader = new Shader();
    controler_shader->Initialisate("Shaders/Menu/Vertex/Controler.glsl", "Shaders/Menu/Fragment/Controler.glsl");
}

void OpenGL::InitTextures()
{
    symbols_texture = new Texture2D();
    symbols_texture->Initialisate("Textures/Sample.bmp");
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
    if (update_shader)
    {
        ship_shader->Use();
        ship_shader->SetUniform("scale", window_scale);
        glBindVertexArray(basic_square);
    }
    ship_shader->SetUniform("position", ship->GetPosition());
    ship_shader->SetUniform("angle", ship->GetAngle());
    ship_shader->SetUniform("size", ship->radius);
    glDrawArrays(GL_TRIANGLES, 0, 6);
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
    if (update_shader)
    {
        button_shader->Use();
        symbols_texture->Use();
        button_shader->SetUniform("scale", window_scale);
        glBindVertexArray(basic_square);
    }
    Color3F color;
    if(button->GetStatus(BUTTON_STATUS_ACTIVE))
    {
        color = Color3F(0.1f, 0.6f, 0.1f);
    }
    else if (button->GetStatus(BUTTON_STATUS_CUSTOM_RED))
    {
        color = Color3F(1.0f, 0.0f, 0.0f);
    }
    else if (button->GetStatus(BUTTON_STATUS_CUSTOM_GREEN))
    {
        color = Color3F(0.0f, 1.0f, 0.0f);
    }
    else if (button->GetStatus(BUTTON_STATUS_CUSTOM_BLUE))
    {
        color = Color3F(0.0f, 0.0f, 1.0f);
    }
    else if (button->GetStatus(BUTTON_STATUS_CUSTOM_PURPURE))
    {
        color = Color3F(1.0f, 0.0f, 1.0f);
    }
    else
    {
        color = Color3F(0.6f, 0.1f, 0.1f);
    }
    if (button->GetStatus(BUTTON_STATUS_SELECT))
    {
        color *= 0.6f;
    }
    button_shader->SetUniform("position", button->GetPosition());
    button_shader->SetUniform("size", button->GetSize());
    button_shader->SetUniform("color", &color);
    button_shader->SetUniform("text", button->GetText(), button->GetTextLength());
    button_shader->SetUniform("text_length", button->GetTextLength());
    button_shader->SetUniform("text_size", button->text_size);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void OpenGL::DrawObject(ControledButton* button, bool update_shader)
{
    if (update_shader)
    {
        controler_shader->Use();
        controler_shader->SetUniform("scale", window_scale);
        glBindVertexArray(right_triangle_buffer);
    }
    Color3F color;
    if (((Button*)button)->GetStatus(BUTTON_STATUS_ACTIVE))
    {
        color = Color3F(0.1f, 0.6f, 0.1f);
    }
    else if (((Button*)button)->GetStatus(BUTTON_STATUS_CUSTOM_RED))
    {
        color = Color3F(1.0f, 0.0f, 0.0f);
    }
    else if (((Button*)button)->GetStatus(BUTTON_STATUS_CUSTOM_GREEN))
    {
        color = Color3F(0.0f, 1.0f, 0.0f);
    }
    else if (((Button*)button)->GetStatus(BUTTON_STATUS_CUSTOM_BLUE))
    {
        color = Color3F(0.0f, 0.0f, 1.0f);
    }
    else if (((Button*)button)->GetStatus(BUTTON_STATUS_CUSTOM_PURPURE))
    {
        color = Color3F(1.0f, 0.0f, 1.0f);
    }
    else
    {
        color = Color3F(0.6f, 0.1f, 0.1f);
    }
    if (((Button*)button)->GetStatus(BUTTON_STATUS_SELECT))
    {
        color *= 0.6f;
    }
    controler_shader->SetUniform("position", ((Button*)button)->GetPosition());
    controler_shader->SetUniform("size", ((Button*)button)->GetSize());
    controler_shader->SetUniform("color", &color);
    glDrawArrays(GL_TRIANGLES, 0, 3);
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
    ship_shader->Use();
    ship_shader->SetUniform("scale", window_scale);
    DrawObject(&(*game_p__ships)[0]);
}

void OpenGL::DrawTurels()
{

}
