#include "OpenGLRealisation.h"
#include <iostream>

OpenGL::OpenGL(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share, GLFWframebuffersizefun Function, GLFWwindow** window)
    : cursore_press_position (new Vec2D()), cursore_release_position(new Vec2D()), cursore_last_position(new Vec2D()), cursore_current_position(new Vec2D())
{
    update_menu = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT;
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

//Callback functions

void OpenGL::CallMenuFunction(Menu* menu, Vec2F* clk_pos, uint8_t clk_status)
{
    if (menu == game_p__main_menu)
    {
        object_p__menu_functions->MainMenuFunction(clk_pos, clk_status);
    }
    else if (menu == game_p__option_menu)
    {
        object_p__menu_functions->OptionMenuFunction(clk_pos, clk_status);
    }
    else if (menu == game_p__spawning_objects_select_menu)
    {
        object_p__menu_functions->SpawnObjectsSelectMenuFunction(clk_pos, clk_status);
    }
    else if (menu == game_p__ships_select_menu)
    {
        object_p__menu_functions->ShipsSelectMenuFunction(clk_pos, clk_status);
    }
    else if (menu == game_p__map_pull_select_menu)
    {
        object_p__menu_functions->MapPullSelectMenuFunction(clk_pos, clk_status);
    }
    else if (menu == game_p__pause_menu)
    {
        object_p__menu_functions->PauseMenuFunction(clk_pos, clk_status);
    }
}

void OpenGL::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    update_menu = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT;
    window_height = height;
    window_width = width;
    window_scale = (float)width / (float)height;
    LimitMenuPosition(*game_p__current_active_menu);
    game_p__camera->SetScale(window_scale);
    glViewport(0, 0, width, height);
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

void OpenGL::ProcessInput(GLFWwindow* window)
{
    if (update_menu > 0 && flag_update_menu_can_change)
    {
        update_menu--;
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        std::cout << "Debug pause." << std::endl;
        int i = 0;
    }
    if (*game_p__play_round)
    {
        if (glfwGetKey(window, SHIP_0_ROTATE_BUTTON))
        {
            (*game_p__rotate_flags)[0] = true;
        }
        else
        {
            (*game_p__rotate_flags)[0] = false;
        }
        if (glfwGetKey(window, SHIP_0_SHOOT_BUTTON))
        {
            (*game_p__shoot_flags)[0] = true;
        }
        else
        {
            (*game_p__shoot_flags)[0] = false;
        }
        if (glfwGetKey(window, SHIP_1_ROTATE_BUTTON))
        {
            (*game_p__rotate_flags)[1] = true;
        }
        else
        {
            (*game_p__rotate_flags)[1] = false;
        }
        if (glfwGetKey(window, SHIP_1_SHOOT_BUTTON))
        {
            (*game_p__shoot_flags)[1] = true;
        }
        else
        {
            (*game_p__shoot_flags)[1] = false;
        }
        if (glfwGetKey(window, SHIP_2_ROTATE_BUTTON))
        {
            (*game_p__rotate_flags)[2] = true;
        }
        else
        {
            (*game_p__rotate_flags)[2] = false;
        }
        if (glfwGetKey(window, SHIP_2_SHOOT_BUTTON))
        {
            (*game_p__shoot_flags)[2] = true;
        }
        else
        {
            (*game_p__shoot_flags)[2] = false;
        }
        if (glfwGetKey(window, SHIP_3_ROTATE_BUTTON))
        {
            (*game_p__rotate_flags)[3] = true;
        }
        else
        {
            (*game_p__rotate_flags)[3] = false;
        }
        if (glfwGetKey(window, SHIP_3_SHOOT_BUTTON))
        {
            (*game_p__shoot_flags)[3] = true;
        }
        else
        {
            (*game_p__shoot_flags)[3] = false;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        update_menu = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT;
        glfwSetWindowShouldClose(window, true);
        exit(0);
    }
    if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS && (mouse_buttons & OPEN_GL_REALISATION_BACK) == OPEN_GL_REALISATION_NOTHING)
    {
        update_menu = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT;
        mouse_buttons |= OPEN_GL_REALISATION_BACK;
        object_p__menu_functions->Back();
    }
    else if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_RELEASE && (mouse_buttons & OPEN_GL_REALISATION_BACK) != OPEN_GL_REALISATION_NOTHING)
    {
        update_menu = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT;
        mouse_buttons &= OPEN_GL_REALISATION_FULL - OPEN_GL_REALISATION_BACK;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && *game_p__current_active_menu == game_p__ships_control_menu)
    {
        glfwGetCursorPos(window, &cursore_press_position->x, &cursore_press_position->y);
        Vec2F clk_pos;
        clk_pos.Set(cursore_press_position->x / window_width - 0.5f, cursore_press_position->y / window_height - 0.5f);
        object_p__menu_functions->ShipsControlMenuFunction(&clk_pos, GLFW_PRESS);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && (mouse_buttons & OPEN_GL_REALISATION_SELECT) == OPEN_GL_REALISATION_NOTHING)
    {
        update_menu = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT;
        flag_update_menu_can_change = false;
        flag_move_menu = true;

        mouse_buttons |= OPEN_GL_REALISATION_SELECT;
        glfwGetCursorPos(window, &cursore_press_position->x, &cursore_press_position->y);
        *cursore_last_position = *cursore_press_position;

        Vec2F clk_pos = Vec2F(((float)cursore_press_position->x / window_width - 0.5f) * 2.0f, ((float)cursore_press_position->y / -window_height) * 2.0f / window_scale + 1.0f);
        CallMenuFunction(*game_p__current_active_menu, &clk_pos, GLFW_PRESS);
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && (mouse_buttons & OPEN_GL_REALISATION_SELECT) != OPEN_GL_REALISATION_NOTHING)
    {
        update_menu = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT;
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

//Callback functions

//Initialisation functions

GLFWwindow* OpenGL::CreateWindows(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share, GLFWframebuffersizefun Function)
{
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
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

#define VERTEX_ARRAY_LENGTH 6

void OpenGL::InitBuffers()
{
    Vec2F points[VERTEX_ARRAY_LENGTH];

    points[0].Set(1.0f, 1.0f);
    points[1].Set(-1.0f, 1.0f);
    points[2].Set(1.0f, -1.0f);
    points[3].Set(-1.0f, -1.0f);
    points[4].Set(-1.0f, 1.0f);
    points[5].Set(1.0f, -1.0f);

    bonus_buffer.Initialisate(points, 6);
    bullet_buffer.Initialisate(points, 6);

    points[0].Set(1.0f, 1.0f);
    points[1].Set(0.0f, 1.0f);
    points[2].Set(1.0f, 0.0f);
    points[3].Set(0.0f, 0.0f);
    points[4].Set(0.0f, 1.0f);
    points[5].Set(1.0f, 0.0f);

    button_buffer.Initialisate(points, 6);
    rectangle_buffer.Initialisate(points, 6);
 
    points[0].Set(-1.0f, 0.0f);
    points[1].Set(0.0f, 1.0f);
    points[2].Set(0.0f, -1.0f);
    points[3].Set(1.0f, 0.0f);
    points[4].Set(0.0f, -1.0f);
    points[5].Set(0.0f, 1.0f);

    asteroid_buffer.Initialisate(points, 6);
    controler_buffer.Initialisate(points, 6);

    points[0].Set(1.0f, 0.02f);
    points[1].Set(0.0f, 0.02f);
    points[2].Set(1.0f,-0.02f);
    points[3].Set(0.0f,-0.02f);
    points[4].Set(0.0f, 0.02f);
    points[5].Set(1.0f,-0.02f);

    mega_laser_buffer.Initialisate(points, 6);

    points[0].Set(0.0f, 0.5f);
    points[1].Set(sqrt(3.0f) / 4.0f, -0.25f);
    points[2].Set(-points[1].x, -0.25f);

    pilot_buffer.Initialisate(points, 3);
    ship_buffer.Initialisate(points, 3);
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
    asteroid_shader.Initialisate("Shaders/Objects/Vertex/Asteroid.glsl", "Shaders/Objects/Fragment/Asteroid.glsl");
    bomb_shader.Initialisate("Shaders/Objects/Vertex/Bomb.glsl", "Shaders/Objects/Fragment/Bomb.glsl");
    bonus_shader.Initialisate("Shaders/Objects/Vertex/Bonus.glsl", "Shaders/Objects/Fragment/Bonus.glsl");
    bullet_shader.Initialisate("Shaders/Objects/Vertex/Bullet.glsl", "Shaders/Objects/Fragment/Bullet.glsl");
    knife_shader.Initialisate("Shaders/Objects/Vertex/Knife.glsl", "Shaders/Objects/Fragment/Knife.glsl");
    mega_laser_shader.Initialisate("Shaders/Objects/Vertex/MegaLaser.glsl", "Shaders/Objects/Fragment/MegaLaser.glsl");
    pilot_shader.Initialisate("Shaders/Objects/Vertex/Pilot.glsl", "Shaders/Objects/Fragment/Pilot.glsl");
    ship_shader.Initialisate("Shaders/Objects/Vertex/Ship.glsl", "Shaders/Objects/Fragment/Ship.glsl");
    turel_shader.Initialisate("Shaders/Objects/Vertex/Turel.glsl", "Shaders/Objects/Fragment/Turel.glsl");
    
    rectangle_shader.Initialisate("Shaders/Map/Vertex/Rectangle.glsl", "Shaders/Map/Fragment/Rectangle.glsl");
    cyrcle_shader.Initialisate("Shaders/Map/Vertex/Cyrcle.glsl", "Shaders/Map/Fragment/Cyrcle.glsl");
    polygon_shader.Initialisate("Shaders/Map/Vertex/Polygon.glsl", "Shaders/Map/Fragment/Polygon.glsl");
    
    button_shader.Initialisate("Shaders/Menu/Vertex/Button.glsl", "Shaders/Menu/Fragment/Button.glsl");
    controler_shader.Initialisate("Shaders/Menu/Vertex/Controler.glsl", "Shaders/Menu/Fragment/Controler.glsl");
}

void OpenGL::InitTextures()
{
    symbols_texture.Initialisate("Textures/Sample.bmp");
}

//Draw functions

void OpenGL::DrawFrame()
{
    glClearColor(0.1f, 0.1f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (*game_p__flag_all_entities_initialisate == true)
    {
        temp__game__camera_position = game_p__camera->GetPosition();
        temp__game__camera_size = game_p__camera->GetSize();

        DrawCurrentMap();
        if (*game_p__turels_count > 0)
        {
            DrawTurels();
        }

        if (*game_p__bombs_count > 0)
        {
            DrawBombs();
        }
        if (*game_p__bonuses_count > 0)
        {
            DrawBonuses();
        }
        if (*game_p__bullets_count > 0)
        {
            DrawBullets();
        }
        if (*game_p__knifes_count > 0)
        {
            DrawKnifes();
        }
        if (*game_p__mega_lasers_count > 0)
        {
            DrawMegaLasers();
        }
        if (*game_p__asteroids_count > 0)
        {
            DrawAsteroids();
        }

        if (*game_p__pilots_count > 0)
        {
            DrawPilots();
        }
        if (*game_p__ships_count > 0)
        {
            DrawShips();
        }
    }

    DrawCurrentMenu();
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
    if (update_shader)
    {
        asteroid_buffer.Use();
        asteroid_shader.Use();
        asteroid_shader.SetUniform("scale", window_scale);
        asteroid_shader.SetUniform("camera_position", temp__game__camera_position);
        asteroid_shader.SetUniform("camera_size", temp__game__camera_size);
    }
    asteroid_shader.SetUniform("position", asteroid->GetPosition());
    asteroid_shader.SetUniform("size", 0.05f);
    asteroid_buffer.Draw();
}

void OpenGL::DrawObject(Bomb* mine, bool update_shader)
{

}

void OpenGL::DrawObject(Bonus* bonus, bool update_shader)
{
    if (update_shader)
    {
        bonus_buffer.Use();
        bonus_shader.Use();
        bonus_shader.SetUniform("scale", window_scale);
        bonus_shader.SetUniform("camera_position", temp__game__camera_position);
        bonus_shader.SetUniform("camera_size", temp__game__camera_size);
    }
    bonus_shader.SetUniform("position", bonus->GetPosition());
    bonus_shader.SetUniform("size", bonus->radius);
    bonus_buffer.Draw();
}

void OpenGL::DrawObject(Bullet* bullet, bool update_shader)
{
    if (update_shader)
    {
        bullet_buffer.Use();
        bullet_shader.Use();
        bullet_shader.SetUniform("scale", window_scale);
        bullet_shader.SetUniform("camera_position", temp__game__camera_position);
        bullet_shader.SetUniform("camera_size", temp__game__camera_size);
    }
    bullet_shader.SetUniform("position", bullet->GetPosition());
    bullet_shader.SetUniform("size", bullet->radius);
    bullet_buffer.Draw();
}

void OpenGL::DrawObject(Knife* knife, bool update_shader)
{

}

void OpenGL::DrawObject(MegaLaser* mega_laser, bool update_shader)
{
    if (update_shader)
    {
        mega_laser_buffer.Use();
        mega_laser_shader.Use();
        mega_laser_shader.SetUniform("scale", window_scale);
        mega_laser_shader.SetUniform("camera_position", temp__game__camera_position);
        mega_laser_shader.SetUniform("camera_size", temp__game__camera_size);
    }
    mega_laser_shader.SetUniform("angle", mega_laser->GetDirection().GetAbsoluteAngle());
    mega_laser_shader.SetUniform("position", mega_laser->GetPosition());
    mega_laser_shader.SetUniform("vector", mega_laser->GetDirection());
    mega_laser_buffer.Draw();
}

void OpenGL::DrawObject(Pilot* pilot, bool update_shader)
{
    if (update_shader)
    {
        pilot_buffer.Use();
        pilot_shader.Use();
        pilot_shader.SetUniform("scale", window_scale);
        pilot_shader.SetUniform("camera_position", temp__game__camera_position);
        pilot_shader.SetUniform("camera_size", temp__game__camera_size);
    }
    pilot_shader.SetUniform("position", pilot->GetPosition());
    pilot_shader.SetUniform("angle", pilot->GetAngle());
    pilot_shader.SetUniform("size", pilot->radius);
    pilot_buffer.Draw();
}

void OpenGL::DrawObject(Ship* ship, bool update_shader)
{
    if (update_shader)
    {
        ship_buffer.Use();
        ship_shader.Use();
        ship_shader.SetUniform("scale", window_scale);
        ship_shader.SetUniform("camera_position", temp__game__camera_position);
        ship_shader.SetUniform("camera_size", temp__game__camera_size);
    }

    GameTypes::players_count_t number_of_player_in_team = 0;
    for (GameTypes::players_count_t player = 0; player < GAME_PLAYERS_MAX_COUNT; player++)
    {
        if ((*game_p__ships)[player].GetPlayerNumber() == ship->GetPlayerNumber())
        {
            break;
        }
        if ((*game_p__ships)[player].GetTeamNumber() == ship->GetTeamNumber())
        {
            number_of_player_in_team++;
        }
    }

    ship_shader.SetUniform("position", ship->GetPosition());
    ship_shader.SetUniform("angle", ship->GetAngle());
    ship_shader.SetUniform("size", ship->radius);
    ship_shader.SetUniform("team", ship->GetTeamNumber());
    ship_shader.SetUniform("player", number_of_player_in_team);
    ship_buffer.Draw();
}

void OpenGL::DrawObject(Turel* turel, bool update_shader)
{

}

void OpenGL::DrawObject(MapElement* map_element, bool update_shader)
{

}

void OpenGL::DrawObject(Rectangle* rectangle, bool update_shader)
{
    if (update_shader)
    {
        rectangle_shader.Use();
        rectangle_shader.SetUniform("scale", window_scale);
        rectangle_shader.SetUniform("camera_position", temp__game__camera_position);
        rectangle_shader.SetUniform("camera_size", temp__game__camera_size);
    }
    rectangle_shader.SetUniform("position",rectangle->GetUpLeftPoint());
    rectangle_shader.SetUniform("point2", rectangle->GetDownRightPoint());
    rectangle_buffer.Draw();
}

void OpenGL::DrawObject(Cyrcle* cyrcle, bool update_shader)
{

}

void OpenGL::DrawObject(Polygon* polygon, bool update_shader)
{

}


void OpenGL::DrawObject(Button* button, bool button_is_controller, bool update_shader)
{
    if (update_shader)
    {
        if (button_is_controller)
        {
            controler_buffer.Use();
            controler_shader.Use();
            controler_shader.SetUniform("scale", window_scale);
        }
        else
        {
            button_buffer.Use();
            button_shader.Use();
            symbols_texture.Use();
            button_shader.SetUniform("scale", window_scale);
        }
    }
    Color3F color;
    if (button->GetStatus(BUTTON_STATUS_ACTIVE))
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
    if (button_is_controller)
    {
        controler_shader.SetUniform("position", button->GetPosition());
        controler_shader.SetUniform("size", button->GetSize());
        controler_shader.SetUniform("color", &color);
        controler_buffer.Draw();
    }
    else
    {
        button_shader.SetUniform("position", button->GetPosition());
        button_shader.SetUniform("size", button->GetSize());
        button_shader.SetUniform("color", &color);
        button_shader.SetUniform("text", button->GetText(), button->GetTextLength());
        button_shader.SetUniform("text_length", button->GetTextLength());
        button_shader.SetUniform("text_size", button->text_size);
        button_buffer.Draw();
    }
}

//Draw functions

//Multydraw functions

void OpenGL::DrawAsteroids()
{
    asteroid_buffer.Use();
    asteroid_shader.Use();
    asteroid_shader.SetUniform("scale", window_scale);
    asteroid_shader.SetUniform("camera_position", temp__game__camera_position);
    asteroid_shader.SetUniform("camera_size", temp__game__camera_size);
    for (GameTypes::entities_count_t asteroid = 0, found_ateroids = 0; found_ateroids < *game_p__asteroids_count; asteroid++)
    {
        if ((*game_p__asteroids)[asteroid].exist == true)
        {
            found_ateroids++;
            DrawObject(&(*game_p__asteroids)[asteroid]);
        }
    }
}

void OpenGL::DrawBombs()
{
    bomb_buffer.Use();
    bomb_shader.Use();
    bomb_shader.SetUniform("scale", window_scale);
    bomb_shader.SetUniform("camera_position", temp__game__camera_position);
    bomb_shader.SetUniform("camera_size", temp__game__camera_size);
    for (GameTypes::entities_count_t bomb = 0, found_bombs = 0; found_bombs < *game_p__bombs_count; bomb++)
    {
        if ((*game_p__bombs)[bomb].exist == true)
        {
            found_bombs++;
            DrawObject(&(*game_p__bombs)[bomb]);
        }
    }
}

void OpenGL::DrawBonuses()
{
    bonus_buffer.Use();
    bonus_shader.Use();
    bonus_shader.SetUniform("scale", window_scale);
    bonus_shader.SetUniform("camera_position", temp__game__camera_position);
    bonus_shader.SetUniform("camera_size", temp__game__camera_size);
    for (GameTypes::entities_count_t bonus = 0, found_bonus = 0; found_bonus < *game_p__bonuses_count; bonus++)
    {
        if ((*game_p__bonuses)[bonus].exist == true)
        {
            found_bonus++;
            DrawObject(&(*game_p__bonuses)[bonus]);
        }
    }
}

void OpenGL::DrawBullets()
{
    bullet_buffer.Use();
    bullet_shader.Use();
    bullet_shader.SetUniform("scale", window_scale);
    bullet_shader.SetUniform("camera_position", temp__game__camera_position);
    bullet_shader.SetUniform("camera_size", temp__game__camera_size);
    for (GameTypes::entities_count_t bullet = 0, found_bullets = 0; found_bullets < *game_p__bullets_count; bullet++)
    {
        if ((*game_p__bullets)[bullet].exist == true)
        {
            found_bullets++;
            DrawObject(&(*game_p__bullets)[bullet]);
        }
    }
}

void OpenGL::DrawKnifes()
{
    knife_buffer.Use();
    knife_shader.Use();
    knife_shader.SetUniform("scale", window_scale);
    knife_shader.SetUniform("camera_position", temp__game__camera_position);
    knife_shader.SetUniform("camera_size", temp__game__camera_size);
    for (GameTypes::entities_count_t knife = 0, found_knifes = 0; found_knifes < *game_p__knifes_count; knife++)
    {
        if ((*game_p__knifes)[knife].exist == true)
        {
            found_knifes++;
            DrawObject(&(*game_p__knifes)[knife]);
        }
    }
}

void OpenGL::DrawMegaLasers()
{
    mega_laser_buffer.Use();
    mega_laser_shader.Use();
    mega_laser_shader.SetUniform("scale", window_scale);
    mega_laser_shader.SetUniform("camera_position", temp__game__camera_position);
    mega_laser_shader.SetUniform("camera_size", temp__game__camera_size);
    for (GameTypes::entities_count_t mega_laser = 0, found_mega_lasers = 0; found_mega_lasers < *game_p__mega_lasers_count; mega_laser++)
    {
        if ((*game_p__mega_lasers)[mega_laser].exist == true)
        {
            found_mega_lasers++;
            DrawObject(&(*game_p__mega_lasers)[mega_laser]);
        }
    }
}

void OpenGL::DrawPilots()
{
    pilot_buffer.Use();
    pilot_shader.Use();
    pilot_shader.SetUniform("scale", window_scale);
    pilot_shader.SetUniform("camera_position", temp__game__camera_position);
    pilot_shader.SetUniform("camera_size", temp__game__camera_size);
    for (GameTypes::players_count_t pilot = 0; pilot < GAME_PLAYERS_MAX_COUNT; pilot++)
    {
        if ((*game_p__pilots)[pilot].exist == true)
        {
            DrawObject(&(*game_p__pilots)[pilot]);
        }
    }
}

void OpenGL::DrawShips()
{
    ship_buffer.Use();
    ship_shader.Use();
    ship_shader.SetUniform("scale", window_scale);
    ship_shader.SetUniform("camera_position", temp__game__camera_position);
    ship_shader.SetUniform("camera_size", temp__game__camera_size);
    for (GameTypes::players_count_t ship = 0; ship < GAME_PLAYERS_MAX_COUNT; ship++)
    {
        if ((*game_p__ships)[ship].exist == true)
        {
            DrawObject(&(*game_p__ships)[ship]);
        }
    }
}

void OpenGL::DrawTurels()
{
    int i = 0;
}


void OpenGL::DrawCurrentMap()
{
    DrawIndicatedMap(game_p__map);
}

void OpenGL::DrawCurrentMenu()
{
    if (*game_p__current_active_menu == game_p__pause_menu)
    {
        DrawIndicatedMenu(game_p__ships_control_menu);
    }
    DrawIndicatedMenu(*game_p__current_active_menu);
}

void OpenGL::DrawIndicatedMap(Map* map)
{
    void* map_element_p;
    if (map->rectangles_array_length > 0)
    {
        rectangle_buffer.Use();
        rectangle_shader.Use();
        rectangle_shader.SetUniform("scale", window_scale);
        rectangle_shader.SetUniform("camera_position", temp__game__camera_position);
        rectangle_shader.SetUniform("camera_size", temp__game__camera_size);
        for (EngineTypes::Map::elements_array_length_t rectangle = 0; rectangle < map->rectangles_array_length; rectangle++)
        {
            if (((Rectangle*)(map_element_p = (void*)map->GetRectanglePointer(rectangle)))->exist == true)
            {
                DrawObject((Rectangle*)map_element_p);
            }
        }
    }

    if (map->cyrcles_array_length > 0)
    {
        cyrcle_buffer.Use();
        cyrcle_shader.Use();
        cyrcle_shader.SetUniform("scale", window_scale);
        cyrcle_shader.SetUniform("camera_position", temp__game__camera_position);
        cyrcle_shader.SetUniform("camera_size", temp__game__camera_size);
        for (EngineTypes::Map::elements_array_length_t cyrcle = 0; cyrcle < map->cyrcles_array_length; cyrcle++)
        {
            if (((Cyrcle*)(map_element_p = (void*)map->GetRectanglePointer(cyrcle)))->exist == true)
            {
                DrawObject((Cyrcle*)map_element_p);
            }
        }
    }

    if (map->polygons_array_length > 0)
    {
        polygon_shader.Use();
        polygon_shader.Use();
        polygon_shader.SetUniform("scale", window_scale);
        polygon_shader.SetUniform("camera_position", temp__game__camera_position);
        polygon_shader.SetUniform("camera_size", temp__game__camera_size);
        for (EngineTypes::Map::elements_array_length_t polygon = 0; polygon < map->polygons_array_length; polygon++)
        {
            if (((Polygon*)(map_element_p = (void*)map->GetRectanglePointer(polygon)))->exist == true)
            {
                DrawObject((Polygon*)map_element_p);
            }
        }
    }
}

void OpenGL::DrawIndicatedMenu(Menu* menu)
{
    if (menu == game_p__ships_control_menu)
    {
        controler_buffer.Use();
        controler_shader.Use();
        controler_shader.SetUniform("scale", window_scale);
        for (EngineTypes::Menu::buttons_count_t button = 0; button < menu->GetButtonsCount(); button++)
        {
        //    DrawObject(&menu->current_buttons[button], true);
        }
    }
    else
    {
        button_buffer.Use();
        button_shader.Use();
        symbols_texture.Use();
        button_shader.SetUniform("scale", window_scale);
        for (EngineTypes::Menu::buttons_count_t button = 0; button < menu->GetButtonsCount(); button++)
        {
            DrawObject(&menu->current_buttons[button]);
        }
    }
}

//Multydraw functions

//Get data functions

bool OpenGL::CanDrawFrame(GLFWwindow* window)
{
    return glfwWindowShouldClose(window) == false;
}

float OpenGL::GetScale()
{
    return window_scale;
}

//Get data functions
