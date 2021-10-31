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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//Callback functions

void OpenGL::CallMenuFunction(Menu* menu, Vec2F* clk_pos, uint8_t clk_status)
{
    if (menu == game_p__bonus_pull_menu)
    {
        object_p__menu_functions->BonusPullSelectMenuFunction(clk_pos, clk_status);
    }
    else if (menu == game_p__main_menu)
    {
        object_p__menu_functions->MainMenuFunction(clk_pos, clk_status);
    }
    else if (menu == game_p__map_pull_select_menu)
    {
        object_p__menu_functions->MapPullSelectMenuFunction(clk_pos, clk_status);
    }
    else if (menu == game_p__option_menu)
    {
        object_p__menu_functions->OptionMenuFunction(clk_pos, clk_status);
    }
    else if (menu == game_p__pause_menu)
    {
        object_p__menu_functions->PauseMenuFunction(clk_pos, clk_status);
    }
    else if (menu == game_p__spawning_objects_select_menu)
    {
        object_p__menu_functions->SpawnObjectsSelectMenuFunction(clk_pos, clk_status);
    }
    else if (menu == game_p__ships_select_menu)
    {
        object_p__menu_functions->ShipsSelectMenuFunction(clk_pos, clk_status);
    }
}

void OpenGL::FirstUpdatePlayersFlags(GameTypes::players_count_t player)
{
    (*game_p__rotate_flags)[player] = true;
    if ((button_commands & (OPEN_GL_REALISATION_COMMAND_SHIP_ROTATE << player)) == OPEN_GL_REALISATION_COMMAND_NOTHING)
    {
        button_commands |= (OPEN_GL_REALISATION_COMMAND_SHIP_ROTATE << player);
        if ((*game_p__burnout_double_clk_timer)[player] > 0)
        {
            (*game_p__burnout_flag)[player] = true;
            (*game_p__burnout_double_clk_timer)[player] = -GAME_BURNOUT_COULDOWN;
        }
    }
}

void OpenGL::SecondUpdatePlayersFlags(GameTypes::players_count_t player)
{
    (*game_p__rotate_flags)[player] = false;
    if ((button_commands & (OPEN_GL_REALISATION_COMMAND_SHIP_ROTATE << player)) != OPEN_GL_REALISATION_COMMAND_NOTHING)
    {
        button_commands &= OPEN_GL_REALISATION_COMMAND_FULL - (OPEN_GL_REALISATION_COMMAND_SHIP_ROTATE << player);
        if ((*game_p__burnout_double_clk_timer)[player] == 0)
        {
            (*game_p__burnout_double_clk_timer)[player] = GAME_DOUBLE_CLK_TIME;
        }
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

#define PLAYER_0    0
#define PLAYER_1    1
#define PLAYER_2    2
#define PLAYER_3    3

void OpenGL::ProcessInput(GLFWwindow* window)
{
    if (update_menu > 0 && flag_update_menu_can_change)
    {
        update_menu--;
        (*game_p__current_active_menu)->Recalculate();
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        std::cout << "Debug pause." << std::endl;
        int i = 0;
    }
    if (*game_p__play_round)
    {
        if (glfwGetKey(window, SHIP_0_ROTATE_BUTTON) == GLFW_PRESS)
        {
            FirstUpdatePlayersFlags(PLAYER_0);
        }
        else
        {
            SecondUpdatePlayersFlags(PLAYER_0);
        }
        if (glfwGetKey(window, SHIP_0_SHOOT_BUTTON) == GLFW_PRESS)
        {
            (*game_p__shoot_flags)[PLAYER_0] = true;
        }
        else
        {
            (*game_p__shoot_flags)[PLAYER_0] = false;
        }
        if (glfwGetKey(window, SHIP_1_ROTATE_BUTTON) == GLFW_PRESS)
        {
            FirstUpdatePlayersFlags(PLAYER_1);
        }
        else
        {
            SecondUpdatePlayersFlags(PLAYER_1);
        }
        if (glfwGetKey(window, SHIP_1_SHOOT_BUTTON) == GLFW_PRESS)
        {
            (*game_p__shoot_flags)[PLAYER_1] = true;
        }
        else
        {
            (*game_p__shoot_flags)[PLAYER_1] = false;
        }
        if (glfwGetKey(window, SHIP_2_ROTATE_BUTTON) == GLFW_PRESS)
        {
            FirstUpdatePlayersFlags(PLAYER_2);
        }
        else
        {
            SecondUpdatePlayersFlags(PLAYER_2);
        }
        if (glfwGetKey(window, SHIP_2_SHOOT_BUTTON) == GLFW_PRESS)
        {
            (*game_p__shoot_flags)[PLAYER_2] = true;
        }
        else
        {
            (*game_p__shoot_flags)[PLAYER_2] = false;
        }
        if (glfwGetKey(window, SHIP_3_ROTATE_BUTTON) == GLFW_PRESS)
        {
            FirstUpdatePlayersFlags(PLAYER_3);
        }
        else
        {
            SecondUpdatePlayersFlags(PLAYER_3);
        }
        if (glfwGetKey(window, SHIP_3_SHOOT_BUTTON) == GLFW_PRESS)
        {
            (*game_p__shoot_flags)[PLAYER_3] = true;
        }
        else
        {
            (*game_p__shoot_flags)[PLAYER_3] = false;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        object_p__menu_functions->Exit();
    }
    if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS && (button_commands & OPEN_GL_REALISATION_COMMAND_BACK) == OPEN_GL_REALISATION_COMMAND_NOTHING)
    {
        update_menu = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT;
        button_commands |= OPEN_GL_REALISATION_COMMAND_BACK;
        object_p__menu_functions->Back();
    }
    else if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_RELEASE && (button_commands & OPEN_GL_REALISATION_COMMAND_BACK) != OPEN_GL_REALISATION_COMMAND_NOTHING)
    {
        update_menu = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT;
        button_commands &= OPEN_GL_REALISATION_COMMAND_FULL - OPEN_GL_REALISATION_COMMAND_BACK;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && *game_p__current_active_menu == game_p__ships_control_menu)
    {
        glfwGetCursorPos(window, &cursore_press_position->x, &cursore_press_position->y);
        Vec2F clk_pos;
        clk_pos.Set(cursore_press_position->x / window_width - 0.5f, cursore_press_position->y / window_height - 0.5f);
        object_p__menu_functions->ShipsControlMenuFunction(&clk_pos, GLFW_PRESS);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && (button_commands & OPEN_GL_REALISATION_COMMAND_SELECT) == OPEN_GL_REALISATION_COMMAND_NOTHING)
    {
        update_menu = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT;
        flag_update_menu_can_change = false;
        flag_move_menu = true;

        button_commands |= OPEN_GL_REALISATION_COMMAND_SELECT;
        glfwGetCursorPos(window, &cursore_press_position->x, &cursore_press_position->y);
        *cursore_last_position = *cursore_press_position;

        Vec2F clk_pos = Vec2F(((float)cursore_press_position->x / window_width - 0.5f) * 2.0f, ((float)cursore_press_position->y / -window_height) * 2.0f / window_scale + 1.0f);
        CallMenuFunction(*game_p__current_active_menu, &clk_pos, GLFW_PRESS);
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && (button_commands & OPEN_GL_REALISATION_COMMAND_SELECT) != OPEN_GL_REALISATION_COMMAND_NOTHING)
    {
        update_menu = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT;
        flag_update_menu_can_change = true;
        flag_move_menu = false;

        button_commands &= OPEN_GL_REALISATION_COMMAND_FULL - OPEN_GL_REALISATION_COMMAND_SELECT;
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

    asteroid_buffer.Initialisate(points, 6);
    bomb_buffer.Initialisate(points, 6);
    bonus_buffer.Initialisate(points, 6);
    bullet_buffer.Initialisate(points, 6);

    cyrcle_buffer.Initialisate(points, 6);
    deceler_area_buffer.Initialisate(points, 6);
    grav_gen_buffer.Initialisate(points, 6);
    rectangle_buffer.Initialisate(points, 6);

    dynamic_particle_buffer.Initialisate(points, 6);
    particle_buffer.Initialisate(points, 6);

    points[0].Set(1.0f, 1.0f);
    points[1].Set(0.0f, 1.0f);
    points[2].Set(1.0f, -1.0f);
    points[3].Set(0.0f, -1.0f);
    points[4].Set(0.0f, 1.0f);
    points[5].Set(1.0f, -1.0f);

    laser_buffer.Initialisate(points, 6);
    mega_laser_buffer.Initialisate(points, 6);
    polygon_buffer.Initialisate(points, 6);

    points[0].Set(2.0f, 2.0f);
    points[1].Set(-2.0f, 2.0f);
    points[2].Set(2.0f, -2.0f);
    points[3].Set(-2.0f, -2.0f);
    points[4].Set(-2.0f, 2.0f);
    points[5].Set(2.0f, -2.0f);

    turel_buffer.Initialisate(points, 6);

    points[0].Set(1.0f, 1.0f);
    points[1].Set(0.0f, 1.0f);
    points[2].Set(1.0f, 0.0f);
    points[3].Set(0.0f, 0.0f);
    points[4].Set(0.0f, 1.0f);
    points[5].Set(1.0f, 0.0f);

    button_buffer.Initialisate(points, 6);
 
    points[0].Set(-1.0f, 0.0f);
    points[1].Set(0.0f, 1.0f);
    points[2].Set(0.0f, -1.0f);
    points[3].Set(1.0f, 0.0f);
    points[4].Set(0.0f, -1.0f);
    points[5].Set(0.0f, 1.0f);

    controler_buffer.Initialisate(points, 6);

    points[0].Set(1.0f, 0.02f);
    points[1].Set(0.0f, 0.02f);
    points[2].Set(1.0f,-0.02f);
    points[3].Set(0.0f,-0.02f);
    points[4].Set(0.0f, 0.02f);
    points[5].Set(1.0f,-0.02f);

    knife_buffer.Initialisate(points, 6);

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
    glfwWindowHint(GLFW_SAMPLES, 4);
}

void OpenGL::InitShaders()
{
    asteroid_shader.Initialisate(           "Shaders/Objects/Vertex/Asteroid.glsl"          ,   "Shaders/Objects/Fragment/Asteroid.glsl");
    bomb_shader.Initialisate(               "Shaders/Objects/Vertex/Bomb.glsl"              ,   "Shaders/Objects/Fragment/Bomb.glsl");
    bonus_shader.Initialisate(              "Shaders/Objects/Vertex/Bonus.glsl"             ,   "Shaders/Objects/Fragment/Bonus.glsl");
    bullet_shader.Initialisate(             "Shaders/Objects/Vertex/Bullet.glsl"            ,   "Shaders/Objects/Fragment/Bullet.glsl");
    deceler_area_shader.Initialisate(       "Shaders/Objects/Vertex/Deceler.glsl"           ,   "Shaders/Objects/Fragment/Deceler.glsl");
    dynamic_particle_shader.Initialisate(   "Shaders/Objects/Vertex/Particle.glsl"          ,   "Shaders/Objects/Fragment/Particle.glsl");
    grav_gen_shader.Initialisate(           "Shaders/Objects/Vertex/GravGen.glsl"           ,   "Shaders/Objects/Fragment/GravGen.glsl");
    knife_shader.Initialisate(              "Shaders/Objects/Vertex/Knife.glsl"             ,   "Shaders/Objects/Fragment/Knife.glsl");
    laser_shader.Initialisate(              "Shaders/Objects/Vertex/Laser.glsl"             ,   "Shaders/Objects/Fragment/Laser.glsl");
    mega_laser_shader.Initialisate(         "Shaders/Objects/Vertex/MegaLaser.glsl"         ,   "Shaders/Objects/Fragment/MegaLaser.glsl");
    particle_shader.Initialisate(           "Shaders/Objects/Vertex/Particle.glsl"          ,   "Shaders/Objects/Fragment/Particle.glsl");
    pilot_shader.Initialisate(              "Shaders/Objects/Vertex/Pilot.glsl"             ,   "Shaders/Objects/Fragment/Pilot.glsl");
    ship_shader.Initialisate(               "Shaders/Objects/Vertex/Ship.glsl"              ,   "Shaders/Objects/Fragment/Ship.glsl");
    turel_shader.Initialisate(              "Shaders/Objects/Vertex/Turel.glsl"             ,   "Shaders/Objects/Fragment/Turel.glsl");
    
    rectangle_shader.Initialisate(          "Shaders/Map/Vertex/Rectangle.glsl"             ,   "Shaders/Map/Fragment/Rectangle.glsl");
    cyrcle_shader.Initialisate(             "Shaders/Map/Vertex/Cyrcle.glsl"                ,   "Shaders/Map/Fragment/Cyrcle.glsl");
    polygon_shader.Initialisate(            "Shaders/Map/Vertex/Polygon.glsl"               ,   "Shaders/Map/Fragment/Polygon.glsl");
    
    button_shader.Initialisate(             "Shaders/Menu/Vertex/Button.glsl"               ,   "Shaders/Menu/Fragment/Button.glsl");
    controler_shader.Initialisate(          "Shaders/Menu/Vertex/Controler.glsl"            ,   "Shaders/Menu/Fragment/Controler.glsl");
}

void OpenGL::InitTextures()
{
    symbols_texture.Initialisate("Textures/Sample.bmp");
}

//Draw functions

void OpenGL::DrawFrame()
{
    if (*game_p__flag_all_entities_initialisate == false || *game_p__flag_round_results == true)
    {
        glClearColor(0.1f, 0.1f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    if (*game_p__flag_all_entities_initialisate == true && *game_p__flag_round_results == false)
    {
        game_p__camera_data_mtx->lock();
        temp__game__camera_position = game_p__camera->GetPosition();
        temp__game__camera_size = game_p__camera->GetSize();
        game_p__camera_data_mtx->unlock();

        game_p__particles_array_mtx->lock();
        if (*game_p__particles_count > 0)
        {
            DrawParticles();
        }
        game_p__particles_array_mtx->unlock();
        game_p__dynamic_particles_array_mtx->lock();
        if (*game_p__particles_count > 0)
        {
            DrawDynamicParticles();
        }
        game_p__dynamic_particles_array_mtx->unlock();
        game_p__map_data_mtx->lock();
        DrawCurrentMap();
        game_p__map_data_mtx->unlock();
        game_p__grav_gens_array_mtx->lock();
        if (*game_p__grav_gens_count > 0)
        {
            DrawGravityGenerators();
        }
        game_p__grav_gens_array_mtx->unlock();
        game_p__turels_array_mtx->lock();
        if (*game_p__turels_count > 0)
        {
            DrawTurels();
        }
        game_p__turels_array_mtx->unlock();

        game_p__bombs_array_mtx->lock();
        if (*game_p__bombs_count > 0)
        {
            DrawBombs();
        }
        game_p__bombs_array_mtx->unlock();
        game_p__bonuses_array_mtx->lock();
        if (*game_p__bonuses_count > 0)
        {
            DrawBonuses();
        }
        game_p__bonuses_array_mtx->unlock();
        game_p__mega_lasers_array_mtx->lock();
        if (*game_p__mega_lasers_count > 0)
        {
            DrawMegaLasers();
        }
        game_p__mega_lasers_array_mtx->unlock();
        game_p__bullets_array_mtx->lock();
        if (*game_p__bullets_count > 0)
        {
            DrawBullets();
        }
        game_p__bullets_array_mtx->unlock();
        game_p__knifes_array_mtx->lock();
        if (*game_p__knifes_count > 0)
        {
            DrawKnifes();
        }
        game_p__knifes_array_mtx->unlock();
        game_p__lasers_array_mtx->lock();
        if (*game_p__lasers_count > 0)
        {
            DrawLasers();
        }
        game_p__lasers_array_mtx->unlock();
        game_p__asteroids_array_mtx->lock();
        if (*game_p__asteroids_count > 0)
        {
            DrawAsteroids();
        }
        game_p__asteroids_array_mtx->unlock();

        game_p__pilots_array_mtx->lock();
        if (*game_p__pilots_count > 0)
        {
            DrawPilots();
        }
        game_p__pilots_array_mtx->unlock();
        game_p__ships_array_mtx->lock();
        if (*game_p__ships_count > 0)
        {
            DrawShips();
        }
        game_p__ships_array_mtx->unlock();
        game_p__deceler_areas_array_mtx->lock();
        if (*game_p__deceler_areas_count > 0)
        {
            DrawDecelerationAreas();
        }
        game_p__deceler_areas_array_mtx->unlock();
    }
    else if (*game_p__flag_round_results == true)
    {
        game_p__camera_data_mtx->lock();
        temp__game__camera_position = game_p__camera->GetPosition();
        temp__game__camera_size = game_p__camera->GetSize();
        game_p__camera_data_mtx->unlock();
        game_p__map_data_mtx->lock();
        DrawCurrentMap();
        game_p__map_data_mtx->unlock();
        game_p__ships_array_mtx->lock();
        DrawShips();
        game_p__ships_array_mtx->unlock();
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
    asteroid_shader.SetUniform("size", asteroid->radius);
    asteroid_shader.SetUniform("inventory", asteroid->bonus_inventory);
    asteroid_buffer.Draw();
}

void OpenGL::DrawObject(Bomb* bomb, bool update_shader)
{
    if (update_shader)
    {
        bomb_buffer.Use();
        bomb_shader.Use();
        bomb_shader.SetUniform("scale", window_scale);
        bomb_shader.SetUniform("camera_position", temp__game__camera_position);
        bomb_shader.SetUniform("camera_size", temp__game__camera_size);
    }
    bomb_shader.SetUniform("position", bomb->GetPosition());
    bomb_shader.SetUniform("angle", bomb->GetAngle());
    bomb_shader.SetUniform("size", bomb->radius);
    bomb_shader.SetUniform("animation", (float)bomb->GetAnimationTic() / (float)BOMB_BOOM_TIME);
    bomb_buffer.Draw();
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
    bonus_shader.SetUniform("inventory", bonus->bonus_inventory);
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

void OpenGL::DrawObject(DecelerationArea* deceler_area, bool update_shader)
{
    if (update_shader)
    {
        deceler_area_buffer.Use();
        deceler_area_shader.Use();
        deceler_area_shader.SetUniform("scale", window_scale);
        deceler_area_shader.SetUniform("camera_position", temp__game__camera_position);
        deceler_area_shader.SetUniform("camera_size", temp__game__camera_size);
    }
    deceler_area_shader.SetUniform("position", deceler_area->GetPosition());
    deceler_area_shader.SetUniform("size", deceler_area->radius);
    deceler_area_buffer.Draw();
}

void OpenGL::DrawObject(DynamicParticle* dynamic_particle, bool update_shader)
{
    if (update_shader)
    {
        dynamic_particle_buffer.Use();
        dynamic_particle_shader.Use();
        dynamic_particle_shader.SetUniform("scale", window_scale);
        dynamic_particle_shader.SetUniform("camera_position", temp__game__camera_position);
        dynamic_particle_shader.SetUniform("camera_size", temp__game__camera_size);
    }

    dynamic_particle_shader.SetUniform("position", dynamic_particle->GetPosition());
    dynamic_particle_shader.SetUniform("angle", dynamic_particle->GetAngle());
    dynamic_particle_shader.SetUniform("radius", dynamic_particle->radius);
    dynamic_particle_shader.SetUniform("type", (int)dynamic_particle->GetType());
    dynamic_particle_shader.SetUniform("animation", dynamic_particle->animation);
    dynamic_particle_shader.SetUniform("color", dynamic_particle->color);
    dynamic_particle_buffer.Draw();
}

void OpenGL::DrawObject(GravGen* grav_gen, bool update_shader)
{
    if (update_shader)
    {
        grav_gen_buffer.Use();
        grav_gen_shader.Use();
        grav_gen_shader.SetUniform("scale", window_scale);
        grav_gen_shader.SetUniform("camera_position", temp__game__camera_position);
        grav_gen_shader.SetUniform("camera_size", temp__game__camera_size);
    }
    grav_gen_shader.SetUniform("position", grav_gen->GetPosition());
    grav_gen_shader.SetUniform("size", grav_gen->radius);
    grav_gen_shader.SetUniform("gravity", grav_gen->gravity);
    grav_gen_buffer.Draw();
}

void OpenGL::DrawObject(Knife* knife, bool update_shader)
{
    if (update_shader)
    {
        knife_buffer.Use();
        knife_shader.Use();
        knife_shader.SetUniform("scale", window_scale);
        knife_shader.SetUniform("camera_position", temp__game__camera_position);
        knife_shader.SetUniform("camera_size", temp__game__camera_size);
    }
    knife_shader.SetUniform("beam", knife->GetSegment());
    knife_buffer.Draw();
}

void OpenGL::DrawObject(Laser* laser, bool update_shader)
{
    if (update_shader)
    {
        laser_buffer.Use();
        laser_shader.Use();
        laser_shader.SetUniform("scale", window_scale);
        laser_shader.SetUniform("camera_position", temp__game__camera_position);
        laser_shader.SetUniform("camera_size", temp__game__camera_size);
    }
    laser_shader.SetUniform("beam", laser->GetBeam());
    laser_shader.SetUniform("life", (float)laser->GetLifeTime() / (float)LASER_DEFAULT_SHOOT_TIME);
    laser_shader.SetUniform("width", laser->width);
    laser_buffer.Draw();
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
    mega_laser_shader.SetUniform("is_active", mega_laser->IsShooting(*game_p__global_timer));
    mega_laser_shader.SetUniform("angle", mega_laser->GetDirectionNotNormalize().AbsoluteAngle());
    mega_laser_shader.SetUniform("position", mega_laser->GetPosition());
    mega_laser_shader.SetUniform("vector", mega_laser->GetDirectionNotNormalize() * mega_laser->radius);
    mega_laser_shader.SetUniform("width", mega_laser->width);
    mega_laser_buffer.Draw();
}

void OpenGL::DrawObject(Particle* particle, bool update_shader)
{
    if (update_shader)
    {
        particle_buffer.Use();
        particle_shader.Use();
        particle_shader.SetUniform("scale", window_scale);
        particle_shader.SetUniform("camera_position", temp__game__camera_position);
        particle_shader.SetUniform("camera_size", temp__game__camera_size);
    }

    particle_shader.SetUniform("position", particle->GetPosition());
    particle_shader.SetUniform("angle", (particle->GetType() == PARTICLE_TYPE_BACKGROUND) ? (float)*game_p__stuning_timer / (float)GAME_BOMB_STUNING_TIME : particle->GetAngle());
    particle_shader.SetUniform("radius", particle->radius);
    particle_shader.SetUniform("type", (int)particle->GetType());
    particle_shader.SetUniform("animation", particle->animation);
    particle_shader.SetUniform("color", particle->color);
    particle_buffer.Draw();
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
    GameTypes::players_count_t number_of_player_in_team = 0;
    for (GameTypes::players_count_t player = 0; player < GAME_PLAYERS_MAX_COUNT; player++)
    {
        if ((*game_p__ships)[player].GetPlayerNumber() == pilot->GetPlayerNumber())
        {
            break;
        }
        if ((*game_p__ships)[player].GetTeamNumber() == pilot->GetTeamNumber())
        {
            number_of_player_in_team++;
        }
    }

    pilot_shader.SetUniform("model", pilot->GetModelMatrixPointer());
    pilot_shader.SetUniform("team", pilot->GetTeamNumber());
    pilot_shader.SetUniform("player", number_of_player_in_team);
    pilot_shader.SetUniform("life", (float)pilot->GetRespawnDellay() / (float)PILOT_DEFAULT_RESPAWN_TIMER);
    pilot_buffer.Draw();
}

void OpenGL::DrawObject(Ship* ship, bool update_shader)
{
#define PLAYER_SHIELD 0x0F00
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

    ship_shader.SetUniform("model", ship->GetModelMatrixPointer());
    ship_shader.SetUniform("team", ship->GetTeamNumber());

    ship_shader.SetUniform("player", number_of_player_in_team | (ship->IsUnbrakable() ? 0xF000 : 0x0000));
    ship_buffer.Draw();

    if (ship->HaveBuff(SHIP_BUFF_SHIELD))
    {
        ship_shader.SetUniform("player", PLAYER_SHIELD);
        ship_buffer.Draw();
    }
}

void OpenGL::DrawObject(Turel* turel, bool update_shader)
{

    if (update_shader)
    {
        turel_buffer.Use();
        turel_shader.Use();
        turel_shader.SetUniform("scale", window_scale);
        turel_shader.SetUniform("camera_position", temp__game__camera_position);
        turel_shader.SetUniform("camera_size", temp__game__camera_size);
    }
    turel_shader.SetUniform("angle", turel->GetAngle());
    turel_shader.SetUniform("position", turel->GetPosition());
    turel_shader.SetUniform("size", turel->radius);
    turel_buffer.Draw();
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
    rectangle_shader.SetUniform("properties", rectangle->Prorerties());
    rectangle_shader.SetUniform("position",rectangle->UpLeftPoint());
    rectangle_shader.SetUniform("point2", rectangle->DownRightPoint());
    rectangle_buffer.Draw();
}

void OpenGL::DrawObject(Cyrcle* cyrcle, bool update_shader)
{
    if (update_shader)
    {
        cyrcle_buffer.Use();
        cyrcle_shader.SetUniform("scale", window_scale);
        cyrcle_shader.SetUniform("camera_position", temp__game__camera_position);
        cyrcle_shader.SetUniform("camera_size", temp__game__camera_size);
    }
    cyrcle_shader.SetUniform("properties", cyrcle->Prorerties());
    cyrcle_shader.SetUniform("position", cyrcle->Position());
    cyrcle_shader.SetUniform("size", cyrcle->Radius());
    cyrcle_buffer.Draw();
}

void OpenGL::DrawObject(Polygon* polygon, bool update_shader)
{
    if (update_shader)
    {
        polygon_buffer.Use();
        polygon_shader.Use();
        polygon_shader.SetUniform("scale", window_scale);
        polygon_shader.SetUniform("camera_position", temp__game__camera_position);
        polygon_shader.SetUniform("camera_size", temp__game__camera_size);
    }
    EngineTypes::Map::array_length_t p_count = polygon->PointsCount();
    if (p_count <= 1)
    {
        return;
    }
    polygon_shader.SetUniform("properties", polygon->Prorerties());
    if (p_count > 2 && polygon->IsClosed())
    {
        polygon_shader.SetUniform("side", Segment(polygon->points_array[p_count - 1], polygon->points_array[0], true));
        polygon_buffer.Draw();
    }
    for (EngineTypes::Map::array_length_t p = 1; p < p_count; p++)
    {
        polygon_shader.SetUniform("side", Segment(polygon->points_array[p - 1], polygon->points_array[p], true));
        polygon_buffer.Draw();
    }
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
        if (button->GetStatus(BUTTON_STATUS_TRUE))
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
    }
    else
    {
        color = Color3F(0.3f, 0.3f, 0.3f);
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
        if ((*game_p__asteroids)[asteroid].exist)
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
        if ((*game_p__bombs)[bomb].exist)
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
        if ((*game_p__bonuses)[bonus].exist)
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
        if ((*game_p__bullets)[bullet].exist)
        {
            found_bullets++;
            DrawObject(&(*game_p__bullets)[bullet]);
        }
    }
}

void OpenGL::DrawGravityGenerators()
{
    grav_gen_buffer.Use();
    grav_gen_shader.Use();
    grav_gen_shader.SetUniform("scale", window_scale);
    grav_gen_shader.SetUniform("camera_position", temp__game__camera_position);
    grav_gen_shader.SetUniform("camera_size", temp__game__camera_size);
    for (GameTypes::entities_count_t grav_gen = 0, found_grav_gens = 0; found_grav_gens < *game_p__grav_gens_count; grav_gen++)
    {
        if ((*game_p__grav_gens)[grav_gen].exist)
        {
            found_grav_gens++;
            DrawObject(&(*game_p__grav_gens)[grav_gen]);
        }
    }
}

void OpenGL::DrawDecelerationAreas()
{
    deceler_area_buffer.Use();
    deceler_area_shader.Use();
    deceler_area_shader.SetUniform("scale", window_scale);
    deceler_area_shader.SetUniform("camera_position", temp__game__camera_position);
    deceler_area_shader.SetUniform("camera_size", temp__game__camera_size);
    for (GameTypes::entities_count_t deceler_area = 0, found_deceler_area = 0; found_deceler_area < *game_p__deceler_areas_count; deceler_area++)
    {
        if ((*game_p__deceler_areas)[deceler_area].exist)
        {
            found_deceler_area++;
            DrawObject(&(*game_p__deceler_areas)[deceler_area]);
        }
    }
}

void OpenGL::DrawDynamicParticles()
{
    dynamic_particle_buffer.Use();
    dynamic_particle_shader.Use();
    dynamic_particle_shader.SetUniform("scale", window_scale);
    dynamic_particle_shader.SetUniform("camera_position", temp__game__camera_position);
    dynamic_particle_shader.SetUniform("camera_size", temp__game__camera_size);
    for (GameTypes::entities_count_t dynamic_particle = 0, found_dynamic_particles = 0; found_dynamic_particles < *game_p__dynamic_particles_count; dynamic_particle++)
    {
        if ((*game_p__dynamic_particles)[dynamic_particle].exist)
        {
            found_dynamic_particles++;
            if ((*game_p__dynamic_particles)[dynamic_particle].IsActive())
            {
                DrawObject(&(*game_p__dynamic_particles)[dynamic_particle]);
            }
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
        if ((*game_p__knifes)[knife].exist)
        {
            found_knifes++;
            DrawObject(&(*game_p__knifes)[knife]);
        }
    }
}

void OpenGL::DrawLasers()
{
    laser_buffer.Use();
    laser_shader.Use();
    laser_shader.SetUniform("scale", window_scale);
    laser_shader.SetUniform("camera_position", temp__game__camera_position);
    laser_shader.SetUniform("camera_size", temp__game__camera_size);
    for (GameTypes::entities_count_t laser = 0, found_lasers = 0; found_lasers < *game_p__lasers_count; laser++)
    {
        if ((*game_p__lasers)[laser].exist)
        {
            found_lasers++;
            DrawObject(&(*game_p__lasers)[laser]);
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
        if ((*game_p__mega_lasers)[mega_laser].exist)
        {
            found_mega_lasers++;
            DrawObject(&(*game_p__mega_lasers)[mega_laser]);
        }
    }
}

void OpenGL::DrawParticles()
{
    particle_buffer.Use();
    particle_shader.Use();
    particle_shader.SetUniform("scale", window_scale);
    particle_shader.SetUniform("camera_position", temp__game__camera_position);
    particle_shader.SetUniform("camera_size", temp__game__camera_size);
    for (GameTypes::entities_count_t particle = 0, found_particles = 0; found_particles < *game_p__particles_count; particle++)
    {
        if ((*game_p__particles)[particle].exist)
        {
            found_particles++;
            if ((*game_p__particles)[particle].IsActive())
            {
                DrawObject(&(*game_p__particles)[particle]);
            }
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
    for (GameTypes::players_count_t pilot = 0, found_pilots = 0; found_pilots < *game_p__pilots_count; pilot++)
    {
        if ((*game_p__pilots)[pilot].exist == true)
        {
            found_pilots++;
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
    for (GameTypes::players_count_t ship = 0, found_ships = 0; found_ships < *game_p__ships_count; ship++)
    {
        if ((*game_p__ships)[ship].exist == true)
        {
            found_ships++;
            DrawObject(&(*game_p__ships)[ship]);
        }
    }
}

void OpenGL::DrawTurels()
{
    turel_buffer.Use();
    turel_shader.Use();
    turel_shader.SetUniform("scale", window_scale);
    turel_shader.SetUniform("camera_position", temp__game__camera_position);
    turel_shader.SetUniform("camera_size", temp__game__camera_size);
    for (GameTypes::entities_count_t turel = 0, found_turels = 0; found_turels < *game_p__turels_count; turel++)
    {
        if ((*game_p__turels)[turel].exist)
        {
            found_turels++;
            DrawObject(&(*game_p__turels)[turel]);
        }
    }
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
        for (EngineTypes::Map::array_length_t rectangle = 0; rectangle < map->rectangles_array_length; rectangle++)
        {
            map_element_p = (void*)map->RectanglePointer(rectangle);
            if (((Rectangle*)map_element_p)->exist)
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
        for (EngineTypes::Map::array_length_t cyrcle = 0; cyrcle < map->cyrcles_array_length; cyrcle++)
        {
            map_element_p = (void*)map->CyrclePointer(cyrcle);
            if (((Cyrcle*)map_element_p)->exist)
            {
                DrawObject((Cyrcle*)map_element_p);
            }
        }
    }
    
    if (map->polygons_array_length > 0)
    {
        polygon_buffer.Use();
        polygon_shader.Use();
        polygon_shader.SetUniform("scale", window_scale);
        polygon_shader.SetUniform("camera_position", temp__game__camera_position);
        polygon_shader.SetUniform("camera_size", temp__game__camera_size);
        for (EngineTypes::Map::array_length_t polygon = 0; polygon < map->polygons_array_length; polygon++)
        {
            map_element_p = (void*)map->PolygonPointer(polygon);
            if (((Polygon*)map_element_p)->exist)
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
