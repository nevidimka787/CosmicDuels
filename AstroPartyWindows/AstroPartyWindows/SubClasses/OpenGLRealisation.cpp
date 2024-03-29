#include "OpenGLRealisation.h"
#include <iostream>

#pragma warning(disable : 6011)

OpenGL::OpenGL(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share, GLFWframebuffersizefun Function, GLFWwindow** window)
    : cursore_press_position (new Vec2D()), cursore_release_position(new Vec2D()), cursore_last_position(new Vec2D()), cursore_current_position(new Vec2D())
{
    update_menu = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT;
    flag_update_menu_can_change = true;
    
    InitMemory();

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

void OpenGL::CallMenuFunction(Menu* menu, const Vec2D* glob_clk_pos, uint8_t clk_status)
{
    if (menu == game_p__ships_control_menu)
    {
        return;
    }

    Vec2F clk_pos = Vec2F(
        ((float)glob_clk_pos->x / window_width - 0.5f) * 2.0f,
        ((float)glob_clk_pos->y / -window_height) * 2.0f / window_scale + 1.0f);
        
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

void OpenGL::CallControleMenuFunction(Menu* menu, const Vec2D* glob_clk_pos, uint8_t clk_status)
{
    if (menu != game_p__ships_control_menu)
    {
        return;
    }

    Vec2F clk_pos = Vec2F(
        ((float)glob_clk_pos->x / window_width - 0.5f) * 2.0f,
        ((float)glob_clk_pos->y / window_height - 0.5f) * -2.0f);

    if (window_scale > 1.0f)
    {
        if (clk_pos.x > 0.0f)
        {
            clk_pos.x = (clk_pos.x - 1.0f) * window_scale + 1.0f;
        }
        else
        {
            clk_pos.x = (clk_pos.x + 1.0f) * window_scale - 1.0f;
        }
    }
    else
    {
        if (clk_pos.y > 0.0f)
        {
            clk_pos.y = (clk_pos.y - 1.0f) / window_scale + 1.0f;
        }
        else
        {
            clk_pos.y = (clk_pos.y + 1.0f) / window_scale - 1.0f;
        }
    }

    object_p__menu_functions->ShipsControlMenuFunction(
        clk_pos,
        clk_status,
        game_p__control_flags->burnout_flags,
        game_p__control_flags->shoot_flags,
        *game_p__double_clk_timers,
        game_p__control_flags->burnout_flags);
}

void OpenGL::FirstUpdatePlayersFlags(GameTypes::players_count_t player)
{
    (game_p__control_flags->rotate_flags)[player] = true;
    if ((button_commands & (OPEN_GL_REALISATION_COMMAND_SHIP_ROTATE << player)) == OPEN_GL_REALISATION_COMMAND_NOTHING)
    {
        button_commands |= (OPEN_GL_REALISATION_COMMAND_SHIP_ROTATE << player);
        if ((*game_p__double_clk_timers)[player] > 0)
        {
            (game_p__control_flags->burnout_flags)[player] = true;
            (*game_p__double_clk_timers)[player] = -GAME_BURNOUT_COULDOWN;
        }
    }
}

void OpenGL::SecondUpdatePlayersFlags(GameTypes::players_count_t player)
{
    (game_p__control_flags->rotate_flags)[player] = false;
    if ((button_commands & (OPEN_GL_REALISATION_COMMAND_SHIP_ROTATE << player)) != OPEN_GL_REALISATION_COMMAND_NOTHING)
    {
        button_commands &= OPEN_GL_REALISATION_COMMAND_FULL - (OPEN_GL_REALISATION_COMMAND_SHIP_ROTATE << player);
        if ((*game_p__double_clk_timers)[player] == 0)
        {
            (*game_p__double_clk_timers)[player] = GAME_DOUBLE_CLK_TIME;
        }
    }
}

void OpenGL::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    if (height < 0)
    {
        height = 1;
    }
    if (width < 0)
    {
        width = 1;
    }

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
    const float& egde_y = -menu->GetSize().y - 2.0f / window_scale;
    if (menu->GetPosition().y > egde_y)
    {
        menu->SetPosition(Vec2F(0.0f, egde_y));
    }
    if (menu->GetPosition().y < 0.0f)
    {
        menu->SetPosition(Vec2F(0.0f, 0.0f));
    }
}

#define PLAYER_0    0
#define PLAYER_1    1
#define PLAYER_2    2
#define PLAYER_3    3

void OpenGL::ProcessInput(GLFWwindow* window)
{
    int left_mouse_button_pressed_status = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

    if (object_p__menu_functions->ShouldExit())
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return;
    }
    if (glfwWindowShouldClose(window))
    {
        object_p__menu_functions->EndMatch();
    }

    if (update_menu > 0 && flag_update_menu_can_change)
    {
        update_menu--;
        (*game_p__current_active_menu)->Recalculate();
    }
    if (*game_p__play_round)
    {
#if OPEN_GL_REALISATION__SHIPS_CONTROLED_BY_KEYBOARD == true
        rotate_key_flags[0] = (glfwGetKey(window, SHIP_0_ROTATE_BUTTON) == GLFW_PRESS) ? true : false;
        rotate_key_flags[1] = (glfwGetKey(window, SHIP_1_ROTATE_BUTTON) == GLFW_PRESS) ? true : false;
        rotate_key_flags[2] = (glfwGetKey(window, SHIP_2_ROTATE_BUTTON) == GLFW_PRESS) ? true : false;
        rotate_key_flags[3] = (glfwGetKey(window, SHIP_3_ROTATE_BUTTON) == GLFW_PRESS) ? true : false;
        shoot_key_flags[0] = (glfwGetKey(window, SHIP_0_SHOOT_BUTTON) == GLFW_PRESS) ? true : false;
        shoot_key_flags[1] = (glfwGetKey(window, SHIP_1_SHOOT_BUTTON) == GLFW_PRESS) ? true : false;
        shoot_key_flags[2] = (glfwGetKey(window, SHIP_2_SHOOT_BUTTON) == GLFW_PRESS) ? true : false;
        shoot_key_flags[3] = (glfwGetKey(window, SHIP_3_SHOOT_BUTTON) == GLFW_PRESS) ? true : false;
        object_p__menu_functions->ShipsControlMenuFunction(
            rotate_key_flags,
            shoot_key_flags,
            game_p__control_flags->rotate_flags,
            game_p__control_flags->shoot_flags,
            *game_p__double_clk_timers,
            game_p__control_flags->burnout_flags);
#endif //OPEN_GL_REALISATION__SHIPS_CONTROLED_BY_KEYBOARD == true
#if  OPEN_GL_REALISATION__SHIPS_CONTROLED_BY_SCREEN_BUTTONS == true
        glfwGetCursorPos(window, &cursore_current_position->x, &cursore_current_position->y);
        CallControleMenuFunction(*game_p__current_active_menu, cursore_current_position, left_mouse_button_pressed_status);
#endif //OPEN_GL_REALISATION__SHIPS_CONTROLED_BY_SCREEN_BUTTONS == true
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
    if (left_mouse_button_pressed_status == GLFW_PRESS && (button_commands & OPEN_GL_REALISATION_COMMAND_SELECT) == OPEN_GL_REALISATION_COMMAND_NOTHING)
    {
        update_menu = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT;
        flag_update_menu_can_change = false;
        flag_move_menu = true;

        button_commands |= OPEN_GL_REALISATION_COMMAND_SELECT;
        glfwGetCursorPos(window, &cursore_press_position->x, &cursore_press_position->y);
        *cursore_last_position = *cursore_press_position;

        CallMenuFunction(*game_p__current_active_menu, cursore_press_position, GLFW_PRESS);
    }
    else if (left_mouse_button_pressed_status == GLFW_RELEASE && (button_commands & OPEN_GL_REALISATION_COMMAND_SELECT) != OPEN_GL_REALISATION_COMMAND_NOTHING)
    {
        update_menu = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT;
        flag_update_menu_can_change = true;
        flag_move_menu = false;

        button_commands &= OPEN_GL_REALISATION_COMMAND_FULL - OPEN_GL_REALISATION_COMMAND_SELECT;
        glfwGetCursorPos(window, &cursore_release_position->x, &cursore_release_position->y);
        *cursore_press_position -= *cursore_release_position;
        if (fabs(cursore_press_position->x) < window_width / 100.0 && fabs(cursore_press_position->y) < window_height / 100.0)
        {
            CallMenuFunction(*game_p__current_active_menu, cursore_release_position, GLFW_RELEASE);
        }
        else
        {
            CallMenuFunction(*game_p__current_active_menu, cursore_release_position, OPEN_GL_REALISATION_BUTTON_LOST);
        }
    }
    if (flag_move_menu)
    {
        glfwGetCursorPos(window, &cursore_current_position->x, &cursore_current_position->y);

        const auto& move_vector_d = *cursore_current_position - *cursore_last_position;
        const auto& move_vector = Vec2F(0.0f, static_cast<float>(move_vector_d.y) / window_height * -2.0f / window_scale);

        (*game_p__current_active_menu)->Move(move_vector);
        LimitMenuPosition(*game_p__current_active_menu);
        *cursore_last_position = *cursore_current_position;
    }
}

//Callback functions



//Initialisation functions

GLFWwindow* OpenGL::CreateWindows(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share, GLFWframebuffersizefun Function)
{
    window = glfwCreateWindow(width, height, title, NULL, NULL);
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

    annih_area_generator_buffer.Initialisate(points, 6);
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
    portal_buffer.Initialisate(points, 6);

    ship_bullet_buffer.Initialisate(points, 6);

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

    turret_buffer.Initialisate(points, 6);

    points[0].Set(1.0f, 1.0f);
    points[1].Set(-1.0f, 1.0f);
    points[2].Set(1.0f, -1.0f);
    points[3].Set(-1.0f, -1.0f);
    points[4].Set(-1.0f, 1.0f);
    points[5].Set(1.0f, -1.0f);

    button_buffer.Initialisate(points, 6);

    points[0].Set(1.0f, 0.02f);
    points[1].Set(0.0f, 0.02f);
    points[2].Set(1.0f,-0.02f);
    points[3].Set(0.0f,-0.02f);
    points[4].Set(0.0f, 0.02f);
    points[5].Set(1.0f,-0.02f);

    knife_buffer.Initialisate(points, 6);

    points[0].Set(sqrt(3.0f) / 4.0f, 0.40f);
    points[1].Set(sqrt(3.0f) / 4.0f, -0.35f);
    points[2].Set(-points[1].x, -0.35f);
    points[3].Set(sqrt(3.0f) / 4.0f, 0.40f);
    points[4].Set(-sqrt(3.0f) / 4.0f, 0.35f);
    points[5].Set(-points[3].x, -0.35f);

    Vec2F t_points[6] =
    {
        Vec2F(1.0f, 0.0f),
        Vec2F(1.0f, 1.0f),
        Vec2F(0.0f, 1.0f),
        Vec2F(1.0f, 0.0f),
        Vec2F(0.0f, 0.0f),
        Vec2F(0.0f, 1.0f)
    };

    pilot_buffer.Initialisate(points, t_points, 6);
    ship_buffer.Initialisate(points, t_points, 6);

    //InitButtonsBuffers();

    //main_buffer.Initialisate(window_width, window_height);
}

void OpenGL::InitGlad()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }
}

void OpenGL::InitMemory()
{
    rotate_key_flags = std::vector<bool>(GAME_PLAYERS_MAX_COUNT);
    shoot_key_flags = std::vector<bool>(GAME_PLAYERS_MAX_COUNT);
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
    annih_area_gen_shader.Initialisate(     "Shaders/Objects/Vertex/AnnihAreaGen.glsl"      ,   "Shaders/Objects/Fragment/AnnihAreaGen.glsl");
    asteroid_shader.Initialisate(           "Shaders/Objects/Vertex/Asteroid.glsl"          ,   "Shaders/Objects/Fragment/Asteroid.glsl");
    bomb_shader.Initialisate(               "Shaders/Objects/Vertex/Bomb.glsl"              ,   "Shaders/Objects/Fragment/Bomb.glsl");
    bonus_shader.Initialisate(              "Shaders/Objects/Vertex/Bonus.glsl"             ,   "Shaders/Objects/Fragment/Bonus.glsl");
    bullet_shader.Initialisate(             "Shaders/Objects/Vertex/Bullet.glsl"            ,   "Shaders/Objects/Fragment/Bullet.glsl");
    deceler_area_shader.Initialisate(       "Shaders/Objects/Vertex/Deceler.glsl"           ,   "Shaders/Objects/Fragment/Deceler.glsl");
    dynamic_particle_shader.Initialisate(   "Shaders/Objects/Vertex/DynamicParticle.glsl"   ,   "Shaders/Objects/Fragment/DynamicParticle.glsl");
    grav_gen_shader.Initialisate(           "Shaders/Objects/Vertex/GravGen.glsl"           ,   "Shaders/Objects/Fragment/GravGen.glsl");
    knife_shader.Initialisate(              "Shaders/Objects/Vertex/Knife.glsl"             ,   "Shaders/Objects/Fragment/Knife.glsl");
    laser_shader.Initialisate(              "Shaders/Objects/Vertex/Laser.glsl"             ,   "Shaders/Objects/Fragment/Laser.glsl");
    mega_laser_shader.Initialisate(         "Shaders/Objects/Vertex/MegaLaser.glsl"         ,   "Shaders/Objects/Fragment/MegaLaser.glsl");
    particle_shader.Initialisate(           "Shaders/Objects/Vertex/Particle.glsl"          ,   "Shaders/Objects/Fragment/Particle.glsl");
    portal_shader.Initialisate(             "Shaders/Objects/Vertex/Portal.glsl"            ,   "Shaders/Objects/Fragment/Portal.glsl");
    pilot_shader.Initialisate(              "Shaders/Objects/Vertex/Pilot.glsl"             ,   "Shaders/Objects/Fragment/Pilot.glsl");
    ship_shader.Initialisate(               "Shaders/Objects/Vertex/Ship.glsl"              ,   "Shaders/Objects/Fragment/Ship.glsl");
    turret_shader.Initialisate(              "Shaders/Objects/Vertex/Turret.glsl"           ,   "Shaders/Objects/Fragment/Turret.glsl");
    
    rectangle_shader.Initialisate(          "Shaders/Map/Vertex/Rectangle.glsl"             ,   "Shaders/Map/Fragment/Rectangle.glsl");
    cyrcle_shader.Initialisate(             "Shaders/Map/Vertex/Cyrcle.glsl"                ,   "Shaders/Map/Fragment/Cyrcle.glsl");
    polygon_shader.Initialisate(            "Shaders/Map/Vertex/Polygon.glsl"               ,   "Shaders/Map/Fragment/Polygon.glsl");
    
    button_shader.Initialisate(             "Shaders/Menu/Vertex/Button.glsl"               ,   "Shaders/Menu/Fragment/Button.glsl");
    controler_shader.Initialisate(          "Shaders/Menu/Vertex/Controler.glsl"            ,   "Shaders/Menu/Fragment/Controler.glsl");
}

void OpenGL::InitTextures()
{
    annih_area_gen_basic_texture.Initialisate("Textures/Entities/AnnihAreaGen/Basic.png", GL_RGBA,    GL_RGBA);

    asteroid_small_texture.Initialisate(    "Textures/Entities/Asteroid/Small.png",     GL_RGBA,    GL_RGBA);
    asteroid_medium_texture.Initialisate(   "Textures/Entities/Asteroid/Medium.png",    GL_RGBA,    GL_RGBA);
    asteroid_large_texture.Initialisate(    "Textures/Entities/Asteroid/Large.png",     GL_RGBA,    GL_RGBA);
    asteroid_sublimation_texture.Initialisate("Textures/Entities/Asteroid/Sublimation.png",GL_RGBA, GL_RGBA);

    bonus_bomb_texture.Initialisate(    "Textures/Entities/Bonus/Bomb.png",     GL_RGBA,    GL_RGBA);
    bonus_knife_texture.Initialisate(   "Textures/Entities/Bonus/Knife.png",    GL_RGBA,    GL_RGBA);
    bonus_laser_texture.Initialisate(   "Textures/Entities/Bonus/Laser.png",    GL_RGBA,    GL_RGBA);
    bonus_loop_texture.Initialisate(    "Textures/Entities/Bonus/Loop.png",     GL_RGBA,    GL_RGBA);
    bonus_shield_texture.Initialisate(  "Textures/Entities/Bonus/Shield.png",   GL_RGBA,    GL_RGBA);
    bonus_stream_texture.Initialisate(  "Textures/Entities/Bonus/Stream.png",   GL_RGBA,    GL_RGBA);
    bonus_triple_texture.Initialisate(  "Textures/Entities/Bonus/Triple.png",   GL_RGBA,    GL_RGBA);
    bonus_revers_texture.Initialisate(  "Textures/Entities/Bonus/Revers.png",   GL_RGBA,    GL_RGBA);

    bomb_basic_texture.Initialisate(    "Textures/Entities/Bomb/Basic.png",     GL_RGBA,    GL_RGBA);
    bomb_lighting_texture.Initialisate( "Textures/Entities/Bomb/Lighting.png",  GL_RGBA,    GL_RGBA);

    bullet_small_texture.Initialisate(  "Textures/Entities/Bullet/Small.png",   GL_RGBA,    GL_RGBA);
    bullet_medium_texture.Initialisate( "Textures/Entities/Bullet/Medium.png",  GL_RGBA,    GL_RGBA);
    bullet_large_texture.Initialisate(  "Textures/Entities/Bullet/Large.png",   GL_RGBA,    GL_RGBA);

    pilot_basic_texture.Initialisate(   "Textures/Entities/Pilot/Basic.png",    GL_RGBA,    GL_RGBA);

    ship_basic_texture.Initialisate(    "Textures/Entities/Ship/Basic.png",     GL_RGBA,    GL_RGBA);
    ship_triple_texture.Initialisate(   "Textures/Entities/Ship/Triple.png",    GL_RGBA,    GL_RGBA);

    turret_basic_texture.Initialisate(  "Textures/Entities/Turret/Basic.png",   GL_RGBA,    GL_RGBA);

    symbols_texture.Initialisate("Textures/Menu/Buttons/Symbols.bmp");
}

//Draw functions

void OpenGL::DrawFrame()
{
    //glClearColor(0.07f, 0.07f, 0.4f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);
    //draw_lock0_mtx.lock();
    //game_p__input_values_mtx->lock();

    //while (drawing_lock)
    //{
        //std::this_thread::sleep_for(std::chrono::microseconds(10));
    //}

    if (glfwWindowShouldClose(window))
    {
        *game_p__play_match = false;
        *game_p__play_round = false;
    }

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
        game_p__portals_array_mtx->lock();
        if (*game_p__portals_count > 0)
        {
            DrawPortals();
        }
        game_p__portals_array_mtx->unlock();
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
        game_p__turrets_array_mtx->lock();
        if (*game_p__turrets_count > 0)
        {
            DrawTurrets();
        }
        game_p__turrets_array_mtx->unlock();

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
        game_p__annih_area_gens_array_mtx->lock();
        if (*game_p__annih_area_gens_count > 0)
        {
            DrawAnnihAreaGens();
        }
        game_p__annih_area_gens_array_mtx->unlock();
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

    //game_p__input_values_mtx->unlock();
    //draw_lock0_mtx.unlock();
}

void OpenGL::DrawObject(const AnnihAreaGen& annih_area_gen, bool update_shader)
{
    if(update_shader)
    {
        annih_area_generator_buffer.Use();
        annih_area_gen_shader.Use();
        annih_area_gen_basic_texture.Use();
        annih_area_gen_shader.SetUniform("scale", window_scale);
        annih_area_gen_shader.SetUniform("camera_position", temp__game__camera_position);
        annih_area_gen_shader.SetUniform("camera_size", temp__game__camera_size);
    }
    annih_area_gen_shader.SetUniform("shield", 0);
    annih_area_gen_shader.SetUniform("position", annih_area_gen.GetGlobalPosition());
    annih_area_gen_shader.SetUniform("angle", annih_area_gen.GetGlobalAngle());
    annih_area_gen_shader.SetUniform("radius", annih_area_gen.radius);
    annih_area_generator_buffer.Draw();

    if (annih_area_gen.IsHaveShield())
    {
        annih_area_gen_shader.SetUniform("shield", 1);
        annih_area_gen_shader.SetUniform("radius", annih_area_gen.radius * 1.1f);
        annih_area_generator_buffer.Draw();
    }
}

void OpenGL::DrawObject(const Asteroid& asteroid, bool update_shader)
{
    if (update_shader)
    {
        asteroid_buffer.Use();
        asteroid_shader.Use();
        asteroid_small_texture.Use(0);
        asteroid_shader.SetUniform("small_txtr", 0);
        asteroid_medium_texture.Use(1);
        asteroid_shader.SetUniform("medium_txtr", 1);
        asteroid_large_texture.Use(2);
        asteroid_shader.SetUniform("large_txtr", 2);
        asteroid_sublimation_texture.Use(3);
        asteroid_shader.SetUniform("sblmtn_txtr", 3);
        asteroid_shader.SetUniform("scale", window_scale);
        asteroid_shader.SetUniform("camera_position", temp__game__camera_position);
        asteroid_shader.SetUniform("camera_size", temp__game__camera_size);
    }
    asteroid_shader.SetUniform("position", asteroid.GetPosition());
    asteroid_shader.SetUniform("radius", asteroid.radius);
    asteroid_shader.SetUniform("size", asteroid.GetSize());
    asteroid_shader.SetUniform("inventory", asteroid.bonus_inventory);
    asteroid_buffer.Draw();
}

void OpenGL::DrawObject(const Bomb& bomb, bool update_shader)
{
    if (update_shader)
    {
        bomb_buffer.Use();
        bomb_shader.Use();
        bomb_basic_texture.Use(0);
        bomb_shader.SetUniform("basic_txtr", 0);
        bomb_lighting_texture.Use(1);
        bomb_shader.SetUniform("lighting_txtr", 1);
        bomb_shader.SetUniform("scale", window_scale);
        bomb_shader.SetUniform("camera_position", temp__game__camera_position);
        bomb_shader.SetUniform("camera_size", temp__game__camera_size);
    }
    bomb_shader.SetUniform("position", bomb.GetPosition());
    bomb_shader.SetUniform("angle", bomb.GetAngle());
    bomb_shader.SetUniform("size", bomb.radius);
    bomb_shader.SetUniform("animation", 1.0f - (float)bomb.GetAnimationTic() / (float)bomb.blinking_period);
    bomb_shader.SetUniform("status", (int)bomb.IsActive() + (int)bomb.IsBoom() * 2);
    bomb_buffer.Draw();
}

void OpenGL::DrawObject(const Bonus& bonus, bool update_shader)
{
    if (update_shader)
    {
        bonus_buffer.Use();
        bonus_shader.Use();
        bonus_bomb_texture.Use(0);
        bonus_shader.SetUniform("bomb_texture", 0);
        bonus_knife_texture.Use(1);
        bonus_shader.SetUniform("knife_texture", 1);
        bonus_laser_texture.Use(2);
        bonus_shader.SetUniform("laser_texture", 2);
        bonus_loop_texture.Use(3);
        bonus_shader.SetUniform("loop_texture", 3);
        bonus_shield_texture.Use(4);
        bonus_shader.SetUniform("shield_texture", 4);
        bonus_stream_texture.Use(5);
        bonus_shader.SetUniform("stream_texture", 5);
        bonus_triple_texture.Use(6);
        bonus_shader.SetUniform("triple_texture", 6);
        bonus_revers_texture.Use(7);
        bonus_shader.SetUniform("revers_texture", 7);
        bonus_shader.SetUniform("scale", window_scale);
        bonus_shader.SetUniform("camera_position", temp__game__camera_position);
        bonus_shader.SetUniform("camera_size", temp__game__camera_size);
    }
    bonus_shader.SetUniform("angle", bonus.GetAngle());
    bonus_shader.SetUniform("position", bonus.GetPosition());
    bonus_shader.SetUniform("size", bonus.radius);
    bonus_shader.SetUniform("inventory", bonus.bonus_inventory);
    bonus_buffer.Draw();
}

void OpenGL::DrawObject(const Bullet& bullet, bool update_shader)
{
    if (update_shader)
    {
        bullet_buffer.Use();
        bullet_shader.Use();
        bullet_small_texture.Use(0);
        bullet_shader.SetUniform("small_txtr", 0);
        bullet_medium_texture.Use(1);
        bullet_shader.SetUniform("medium_txtr", 1);
        bullet_large_texture.Use(2);
        bullet_shader.SetUniform("large_txtr", 2);
        bullet_shader.SetUniform("scale", window_scale);
        bullet_shader.SetUniform("camera_position", temp__game__camera_position);
        bullet_shader.SetUniform("camera_size", temp__game__camera_size);
    }
    bullet_shader.SetUniform("angle", bullet.GetAngle());
    bullet_shader.SetUniform("position", bullet.GetPosition());
    bullet_shader.SetUniform("radius", bullet.radius);
    bullet_buffer.Draw();
}

void OpenGL::DrawObject(const DecelerationArea& deceler_area, bool update_shader)
{
    if (update_shader)
    {
        deceler_area_buffer.Use();
        deceler_area_shader.Use();
        deceler_area_shader.SetUniform("scale", window_scale);
        deceler_area_shader.SetUniform("camera_position", temp__game__camera_position);
        deceler_area_shader.SetUniform("camera_size", temp__game__camera_size);
    }
    deceler_area_shader.SetUniform("position", deceler_area.GetPosition());
    deceler_area_shader.SetUniform("size", deceler_area.radius);
    deceler_area_shader.SetUniform("decel_param", deceler_area.deceleration_parameter);
    deceler_area_buffer.Draw();
}

void OpenGL::DrawObject(const DynamicParticle& dynamic_particle, bool update_shader)
{
    if (update_shader)
    {
        dynamic_particle_buffer.Use();
        dynamic_particle_shader.Use();
        dynamic_particle_shader.SetUniform("scale", window_scale);
        dynamic_particle_shader.SetUniform("camera_position", temp__game__camera_position);
        dynamic_particle_shader.SetUniform("camera_size", temp__game__camera_size);
    }

    dynamic_particle_shader.SetUniform("position", dynamic_particle.GetPosition());
    dynamic_particle_shader.SetUniform("angle", dynamic_particle.GetAngle());
    dynamic_particle_shader.SetUniform("radius", dynamic_particle.radius);
    dynamic_particle_shader.SetUniform("type", (int)dynamic_particle.GetType());
    dynamic_particle_shader.SetUniform("animation", dynamic_particle.animation);
    dynamic_particle_shader.SetUniform("color", dynamic_particle.color);
    dynamic_particle_buffer.Draw();
}

void OpenGL::DrawObject(const GravGen& grav_gen, bool update_shader)
{
    if (update_shader)
    {
        grav_gen_buffer.Use();
        grav_gen_shader.Use();
        grav_gen_shader.SetUniform("scale", window_scale);
        grav_gen_shader.SetUniform("camera_position", temp__game__camera_position);
        grav_gen_shader.SetUniform("camera_size", temp__game__camera_size);
    }
    grav_gen_shader.SetUniform("position", grav_gen.GetPosition());
    grav_gen_shader.SetUniform("size", grav_gen.radius);
    grav_gen_shader.SetUniform("gravity", grav_gen.gravity);
    grav_gen_buffer.Draw();
}

void OpenGL::DrawObject(const Knife& knife, bool update_shader)
{
    if (update_shader)
    {
        knife_buffer.Use();
        knife_shader.Use();
        knife_shader.SetUniform("scale", window_scale);
        knife_shader.SetUniform("camera_position", temp__game__camera_position);
        knife_shader.SetUniform("camera_size", temp__game__camera_size);
    }
    knife_shader.SetUniform("segment", knife.GetSegment());
    knife_buffer.Draw();
}

void OpenGL::DrawObject(const Laser& laser, bool update_shader)
{
    if (update_shader)
    {
        laser_buffer.Use();
        laser_shader.Use();
        laser_shader.SetUniform("scale", window_scale);
        laser_shader.SetUniform("camera_position", temp__game__camera_position);
        laser_shader.SetUniform("camera_size", temp__game__camera_size);
    }
    laser_shader.SetUniform("segment", laser.GetSegment());
    laser_shader.SetUniform("life", (float)laser.GetLifeTime() / (float)LASER_DEFAULT_SHOOT_TIME);
    laser_shader.SetUniform("width", laser.width);
    laser_shader.SetUniform("properties", (int)laser.properties);
    laser_buffer.Draw();
}

void OpenGL::DrawObject(const Particle& particle, bool update_shader)
{
    if (update_shader)
    {
        particle_buffer.Use();
        particle_shader.Use();
        particle_shader.SetUniform("scale", window_scale);
        particle_shader.SetUniform("camera_position", temp__game__camera_position);
        particle_shader.SetUniform("camera_size", temp__game__camera_size);
    }

    particle_shader.SetUniform("position", particle.GetPosition());
    particle_shader.SetUniform("angle", (particle.GetType() == PARTICLE_TYPE_BACKGROUND) ? (float)*game_p__stuning_timer / (float)GAME_BOMB_STUNING_TIME : particle.GetAngle());
    particle_shader.SetUniform("radius", particle.radius);
    particle_shader.SetUniform("type", (int)particle.GetType());
    particle_shader.SetUniform("animation", particle.animation);
    particle_shader.SetUniform("color", particle.color);
    particle_buffer.Draw();
}

void OpenGL::DrawObject(const Portal& portal, bool update_shader)
{
    if (update_shader)
    {
        portal_buffer.Use();
        portal_shader.Use();
        portal_shader.SetUniform("scale", window_scale);
        portal_shader.SetUniform("camera_position", temp__game__camera_position);
        portal_shader.SetUniform("camera_size", temp__game__camera_size);
        portal_shader.SetUniform("glob_time", (int)(*game_p__global_timer % (unsigned)INT32_MAX));
    }

    portal_shader.SetUniform("position", portal.GetPosition());
    portal_shader.SetUniform("radius", portal.radius);
    portal_shader.SetUniform("connected", (int)portal.IsConnected());
    portal_buffer.Draw();
}

void OpenGL::DrawObject(const Pilot& pilot, bool update_shader)
{
    if (update_shader)
    {
        pilot_buffer.Use();
        pilot_shader.Use();
        pilot_basic_texture.Use();
        pilot_shader.SetUniform("scale", window_scale);
        pilot_shader.SetUniform("camera_position", temp__game__camera_position);
        pilot_shader.SetUniform("camera_size", temp__game__camera_size);
    }
    GameTypes::players_count_t number_of_player_in_team = 0;
    for (auto& ship : *game_p__ships)
    {
        if (ship.GetPlayerNumber() == pilot.GetPlayerNumber()) break;
        if (ship.GetTeamNumber() == pilot.GetTeamNumber()) number_of_player_in_team++;
    }

    pilot_shader.SetUniform("model", pilot.GetModelMatrixPointerConst());
    pilot_shader.SetUniform("team", pilot.GetTeamNumber());
    pilot_shader.SetUniform("player", number_of_player_in_team);
    pilot_shader.SetUniform("life", (float)pilot.GetRespawnDellay() / (float)PILOT_DEFAULT_RESPAWN_TIMER);
    pilot_buffer.Draw();
}

void OpenGL::DrawObject(const Ship& ship, bool update_shader)
{
#define PLAYER_SHIELD 0x0F00
    if (update_shader)
    {
        //ship_buffer.Use();//Every draw buffer is resseted.
        ship_shader.Use();
        ship_shader.SetUniform("scale", window_scale);
        ship_shader.SetUniform("camera_position", temp__game__camera_position);
        ship_shader.SetUniform("camera_size", temp__game__camera_size);
    }

    ship_buffer.Use();

    GameTypes::players_count_t number_of_player_in_team = 0;
    for (auto& pilot : *game_p__pilots)
    {
        if (ship.GetPlayerNumber() == pilot.GetPlayerNumber()) break;
        if (ship.GetTeamNumber() == pilot.GetTeamNumber()) number_of_player_in_team++;
    }

    ship_shader.SetUniform("model", ship.GetModelMatrixPointerConst());
    ship_shader.SetUniform("team", ship.GetTeamNumber());

    ship_shader.SetUniform("current_tic", (int)((*game_p__global_timer) & ((1u << 31u) - 1u)));
    ship_shader.SetUniform("inventory", ship.GetBonusInventoryAsBoolList() + ((*game_p__rotation_inverse) ? 256 : 0));
    ship_shader.SetUniform("bullets_count", ship.GetBulletsCountInMagasine());
    ship_shader.SetUniform("magazine_size", ship.GetSizeOfMagazine());

    ship_shader.SetUniform("player", number_of_player_in_team | (ship.IsUnbrakable() ? 0xF000 : 0x0000));

    ship_shader.SetUniform("type", (int)0);//0 - is ship

    if (ship.IsHaveBuff(SHIP_BUFF_TRIPLE))
    {
        ship_triple_texture.Use();
    }
    else
    {
        ship_basic_texture.Use();
    }

    ship_buffer.Draw();

    if (ship.IsHaveBuff(SHIP_BUFF_SHIELD))
    {
        ship_shader.SetUniform("player", PLAYER_SHIELD);
        ship_buffer.Draw();
    }

    ship_bullet_buffer.Use();
    ship_shader.SetUniform("position", ship.GetPosition());
    ship_shader.SetUniform("type", (int)1);//1 - is bullets
    ship_bullet_buffer.Draw();
}

void OpenGL::DrawObject(const Turret& turret, bool update_shader)
{

    if (update_shader)
    {
        turret_buffer.Use();
        turret_shader.Use();
        turret_basic_texture.Use();
        turret_shader.SetUniform("scale", window_scale);
        turret_shader.SetUniform("camera_position", temp__game__camera_position);
        turret_shader.SetUniform("camera_size", temp__game__camera_size);
    }
    turret_shader.SetUniform("angle", turret.GetAngle());
    turret_shader.SetUniform("position", turret.GetPosition());
    turret_shader.SetUniform("size", turret.radius);
    turret_buffer.Draw();
}

void OpenGL::DrawObject(const Map::Rectangle* rectangle, bool update_shader)
{
    if (update_shader)
    {
        rectangle_shader.Use();
        rectangle_shader.SetUniform("scale", window_scale);
        rectangle_shader.SetUniform("camera_position", temp__game__camera_position);
        rectangle_shader.SetUniform("camera_size", temp__game__camera_size);
    }
    rectangle_shader.SetUniform("properties", rectangle->Prorerties());
    rectangle_shader.SetUniform("position",rectangle->GetUpLeftPoint());
    rectangle_shader.SetUniform("point2", rectangle->GetDownRightPoint());
    rectangle_buffer.Draw();
}

void OpenGL::DrawObject(const Map::Cyrcle* cyrcle, bool update_shader)
{
    if (update_shader)
    {
        cyrcle_buffer.Use();
        cyrcle_shader.SetUniform("scale", window_scale);
        cyrcle_shader.SetUniform("camera_position", temp__game__camera_position);
        cyrcle_shader.SetUniform("camera_size", temp__game__camera_size);
    }
    cyrcle_shader.SetUniform("properties", cyrcle->Prorerties());
    cyrcle_shader.SetUniform("position", cyrcle->GetPosition());
    cyrcle_shader.SetUniform("size", cyrcle->GetRadius());
    cyrcle_buffer.Draw();
}

void OpenGL::DrawObject(const Map::Polygon* polygon, bool update_shader)
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


void OpenGL::DrawObject(const Button& button, bool update_shader)
{
    if (update_shader)
    {
        button_buffer.Use();
        button_shader.Use();
        symbols_texture.Use();
        button_shader.SetUniform("scale", window_scale);
    }
    Color3F color;
    if (button.GetStatus(BUTTON_STATUS_ACTIVE))
    {
        if (button.GetStatus(BUTTON_STATUS_TRUE))
        {
            color = Color3F(0.1f, 0.6f, 0.1f);
        }
        else if (button.GetStatus(BUTTON_STATUS_CUSTOM_RED))
        {
            color = Color3F(1.0f, 0.0f, 0.0f);
        }
        else if (button.GetStatus(BUTTON_STATUS_CUSTOM_GREEN))
        {
            color = Color3F(0.0f, 1.0f, 0.0f);
        }
        else if (button.GetStatus(BUTTON_STATUS_CUSTOM_BLUE))
        {
            color = Color3F(0.0f, 0.0f, 1.0f);
        }
        else if (button.GetStatus(BUTTON_STATUS_CUSTOM_PURPURE))
        {
            color = Color3F(1.0f, 0.0f, 1.0f);
        }
        else
        {
            color = Color3F(0.6f, 0.1f, 0.1f);
        }
        if (button.GetStatus(BUTTON_STATUS_SELECT))
        {
            color *= 0.6f;
        }
    }
    else
    {
        color = Color3F(0.3f, 0.3f, 0.3f);
    }

    //StaticBuffer buffer = button_buffers.button_buffers[GetButtonIdInArray(&button_buffers, button.GetId())];

    button_shader.SetUniform("position", button.GetPosition());
    button_shader.SetUniform("size", button.GetSize());
    button_shader.SetUniform("color", &color);
    button_shader.SetUniform("text", button.GetText(), button.GetTextLength());
    button_shader.SetUniform("text_length", button.GetTextLength());
    button_shader.SetUniform("text_size", button.GetTexSize());
    button_shader.SetUniform("corners", button.GetAreaP()->GetArrayP());
    button_shader.SetUniform("corners_count", static_cast<int>(button.GetAreaP()->GetArrayP().size()));
    button_buffer.Draw();
}

//Draw functions

//Multydraw functions

void OpenGL::DrawAnnihAreaGens()
{
    annih_area_generator_buffer.Use();
    annih_area_gen_shader.Use();
    annih_area_gen_basic_texture.Use();
    annih_area_gen_shader.SetUniform("scale", window_scale);
    annih_area_gen_shader.SetUniform("camera_position", temp__game__camera_position);
    annih_area_gen_shader.SetUniform("camera_size", temp__game__camera_size);

    DrawObjects(game_p__annih_area_gens, game_p__annih_area_gens_count);
}

void OpenGL::DrawAsteroids()
{
    asteroid_buffer.Use();
    asteroid_shader.Use();
    asteroid_small_texture.Use(0);
    asteroid_shader.SetUniform("small_txtr", 0);
    asteroid_medium_texture.Use(1);
    asteroid_shader.SetUniform("medium_txtr", 1);
    asteroid_large_texture.Use(2);
    asteroid_shader.SetUniform("large_txtr", 2);
    asteroid_sublimation_texture.Use(3);
    asteroid_shader.SetUniform("sblmtn_txtr", 3);
    asteroid_shader.SetUniform("scale", window_scale);
    asteroid_shader.SetUniform("camera_position", temp__game__camera_position);
    asteroid_shader.SetUniform("camera_size", temp__game__camera_size);

    DrawObjects(game_p__asteroids, game_p__asteroids_count);
}

void OpenGL::DrawBombs()
{
    bomb_buffer.Use();
    bomb_shader.Use();
    bomb_basic_texture.Use(0);
    bomb_shader.SetUniform("basic_txtr", 0);
    bomb_lighting_texture.Use(1);
    bomb_shader.SetUniform("lighting_txtr", 1);
    bomb_shader.SetUniform("scale", window_scale);
    bomb_shader.SetUniform("camera_position", temp__game__camera_position);
    bomb_shader.SetUniform("camera_size", temp__game__camera_size);

    DrawObjects(game_p__bombs, game_p__bombs_count);
}

void OpenGL::DrawBonuses()
{
    bonus_buffer.Use();
    bonus_shader.Use();
    bonus_bomb_texture.Use(0);
    bonus_shader.SetUniform("bomb_texture", 0);
    bonus_knife_texture.Use(1);
    bonus_shader.SetUniform("knife_texture", 1);
    bonus_laser_texture.Use(2);
    bonus_shader.SetUniform("laser_texture", 2);
    bonus_loop_texture.Use(3);
    bonus_shader.SetUniform("loop_texture", 3);
    bonus_shield_texture.Use(4);
    bonus_shader.SetUniform("shield_texture", 4);
    bonus_stream_texture.Use(5);
    bonus_shader.SetUniform("stream_texture", 5);
    bonus_triple_texture.Use(6);
    bonus_shader.SetUniform("triple_texture", 6);
    bonus_revers_texture.Use(7);
    bonus_shader.SetUniform("revers_texture", 7);
    bonus_shader.SetUniform("scale", window_scale);
    bonus_shader.SetUniform("camera_position", temp__game__camera_position);
    bonus_shader.SetUniform("camera_size", temp__game__camera_size);

    DrawObjects(game_p__bonuses, game_p__bonuses_count);
}

void OpenGL::DrawBullets()
{
    bullet_buffer.Use();
    bullet_shader.Use();
    bullet_small_texture.Use(0);
    bullet_shader.SetUniform("small_txtr", 0);
    bullet_medium_texture.Use(1);
    bullet_shader.SetUniform("medium_txtr", 1);
    bullet_large_texture.Use(2);
    bullet_shader.SetUniform("large_txtr", 2);
    bullet_shader.SetUniform("scale", window_scale);
    bullet_shader.SetUniform("camera_position", temp__game__camera_position);
    bullet_shader.SetUniform("camera_size", temp__game__camera_size);

    DrawObjects(game_p__bullets, game_p__bullets_count);
}

void OpenGL::DrawGravityGenerators()
{
    grav_gen_buffer.Use();
    grav_gen_shader.Use();
    grav_gen_shader.SetUniform("scale", window_scale);
    grav_gen_shader.SetUniform("camera_position", temp__game__camera_position);
    grav_gen_shader.SetUniform("camera_size", temp__game__camera_size);

    DrawObjects(game_p__grav_gens, game_p__grav_gens_count);
}

void OpenGL::DrawDecelerationAreas()
{
    deceler_area_buffer.Use();
    deceler_area_shader.Use();
    deceler_area_shader.SetUniform("scale", window_scale);
    deceler_area_shader.SetUniform("camera_position", temp__game__camera_position);
    deceler_area_shader.SetUniform("camera_size", temp__game__camera_size);

    DrawObjects(game_p__deceler_areas, game_p__deceler_areas_count);
}

void OpenGL::DrawDynamicParticles()
{
    dynamic_particle_buffer.Use();
    dynamic_particle_shader.Use();
    dynamic_particle_shader.SetUniform("scale", window_scale);
    dynamic_particle_shader.SetUniform("camera_position", temp__game__camera_position);
    dynamic_particle_shader.SetUniform("camera_size", temp__game__camera_size);

    DrawObjects(game_p__dynamic_particles, game_p__dynamic_particles_count);
}

void OpenGL::DrawKnifes()
{
    knife_buffer.Use();
    knife_shader.Use();
    knife_shader.SetUniform("scale", window_scale);
    knife_shader.SetUniform("camera_position", temp__game__camera_position);
    knife_shader.SetUniform("camera_size", temp__game__camera_size);

    DrawObjects(game_p__knifes, game_p__knifes_count);
}

void OpenGL::DrawLasers()
{
    laser_buffer.Use();
    laser_shader.Use();
    laser_shader.SetUniform("scale", window_scale);
    laser_shader.SetUniform("camera_position", temp__game__camera_position);
    laser_shader.SetUniform("camera_size", temp__game__camera_size);

    DrawObjects(game_p__lasers, game_p__lasers_count);
}

void OpenGL::DrawParticles()
{
    particle_buffer.Use();
    particle_shader.Use();
    particle_shader.SetUniform("scale", window_scale);
    particle_shader.SetUniform("camera_position", temp__game__camera_position);
    particle_shader.SetUniform("camera_size", temp__game__camera_size);

    GameTypes::entities_count_t found_particles = 0;
    for (auto& particle : *game_p__particles)
    {
        if (found_particles >= *game_p__particles_count) break;
        if (!particle.exist) continue;
        ++found_particles;
        if (!particle.IsActive()) continue;

        DrawObject(particle);
    }
}

void OpenGL::DrawPortals()
{
    portal_buffer.Use();
    portal_shader.Use();
    portal_shader.SetUniform("scale", window_scale);
    portal_shader.SetUniform("camera_position", temp__game__camera_position);
    portal_shader.SetUniform("camera_size", temp__game__camera_size);

    DrawObjects(game_p__portals, game_p__portals_count);
}

void OpenGL::DrawPilots()
{
    pilot_buffer.Use();
    pilot_shader.Use();
    pilot_basic_texture.Use();
    pilot_shader.SetUniform("scale", window_scale);
    pilot_shader.SetUniform("camera_position", temp__game__camera_position);
    pilot_shader.SetUniform("camera_size", temp__game__camera_size);

    DrawObjects(game_p__pilots, game_p__pilots_count);
}

void OpenGL::DrawShips()
{
    //ship_buffer.Use();//Every draw buffer is resseted.
    ship_shader.Use();
    ship_shader.SetUniform("scale", window_scale);
    ship_shader.SetUniform("camera_position", temp__game__camera_position);
    ship_shader.SetUniform("camera_size", temp__game__camera_size);

    DrawObjects(game_p__ships, game_p__ships_count);
}

void OpenGL::DrawTurrets()
{
    turret_buffer.Use();
    turret_shader.Use();
    turret_basic_texture.Use();
    turret_shader.SetUniform("scale", window_scale);
    turret_shader.SetUniform("camera_position", temp__game__camera_position);
    turret_shader.SetUniform("camera_size", temp__game__camera_size);

    DrawObjects(game_p__turrets, game_p__turrets_count);
}

template<typename Object_T, typename counter_t>
void OpenGL::DrawObjects(const std::vector<Object_T>* objects, const counter_t* objects_count)
{
    GameTypes::entities_count_t objects_found = 0;
    for (auto& object : *objects)
    {
        if (objects_found >= *objects_count) break;
        if (!object.exist) continue;
        ++objects_found;

        DrawObject(object);
    }
}
template void OpenGL::DrawObjects(const std::vector<AnnihAreaGen>* objects, const GameTypes::entities_count_t* objects_count);
template void OpenGL::DrawObjects(const std::vector<Asteroid>* objects, const GameTypes::entities_count_t* objects_count);
template void OpenGL::DrawObjects(const std::vector<Bomb>* objects, const GameTypes::entities_count_t* objects_count);
template void OpenGL::DrawObjects(const std::vector<Bonus>* objects, const GameTypes::entities_count_t* objects_count);
template void OpenGL::DrawObjects(const std::vector<Bullet>* objects, const GameTypes::entities_count_t* objects_count);
template void OpenGL::DrawObjects(const std::vector<GravGen>* objects, const GameTypes::map_elements_count_t* objects_count);
template void OpenGL::DrawObjects(const std::vector<DecelerationArea>* objects, const GameTypes::map_elements_count_t* objects_count);
template void OpenGL::DrawObjects(const std::vector<DynamicParticle>* objects, const GameTypes::entities_count_t* objects_count);
template void OpenGL::DrawObjects(const std::vector<Knife>* objects, const GameTypes::entities_count_t* objects_count);
template void OpenGL::DrawObjects(const std::vector<Laser>* objects, const GameTypes::entities_count_t* objects_count);
template void OpenGL::DrawObjects(const std::vector<Portal>* objects, const GameTypes::map_elements_count_t* objects_count);
template void OpenGL::DrawObjects(const std::vector<Pilot>* objects, const GameTypes::players_count_t* objects_count);
template void OpenGL::DrawObjects(const std::vector<Ship>* objects, const GameTypes::players_count_t* objects_count);
template void OpenGL::DrawObjects(const std::vector<Turret>* objects, const GameTypes::map_elements_count_t* objects_count);

void OpenGL::DrawCurrentMap()
{
    DrawIndicatedMap(game_p__map);
}

void OpenGL::DrawCurrentMenu()
{
#if OPEN_GL_REALISATION__SHIPS_CONTROLED_BY_SCREEN_BUTTONS == true
    if (*game_p__current_active_menu == game_p__pause_menu)
    {
        DrawIndicatedMenu(game_p__ships_control_menu);
    }
#endif
    DrawIndicatedMenu(*game_p__current_active_menu);
}

void OpenGL::DrawIndicatedMap(const Map::MapData* map)
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
            if (((Map::Rectangle*)map_element_p)->exist)
            {
                DrawObject((Map::Rectangle*)map_element_p);
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
            if (((Map::Cyrcle*)map_element_p)->exist)
            {
                DrawObject((Map::Cyrcle*)map_element_p);
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
            if (((Map::Polygon*)map_element_p)->exist)
            {
                DrawObject((Map::Polygon*)map_element_p);
            }
        }
    }
}

void OpenGL::DrawIndicatedMenu(const Menu* menu)
{
#if OPEN_GL_REALISATION__SHIPS_CONTROLED_BY_SCREEN_BUTTONS == true
    if (menu == game_p__ships_control_menu)
    {
        controler_shader.Use();
        controler_shader.SetUniform("scale", window_scale);
        for (EngineTypes::Menu::buttons_count_t button = 0; button < menu->GetButtonsCount(); button++)
        {
            DrawObject(&menu->current_buttons[button], true);
        }
    }
    else
    {
#endif
        button_buffer.Use();
        button_shader.Use();
        symbols_texture.Use();
        button_shader.SetUniform("scale", window_scale);
        for (EngineTypes::Menu::buttons_count_t button = 0; button < menu->GetButtonsCount(); button++)
        {
            DrawObject(menu->current_buttons[button]);
        }
#if OPEN_GL_REALISATION__SHIPS_CONTROLED_BY_SCREEN_BUTTONS == true
    }
#endif
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

//destructor and free memory functions

void OpenGL::Free()
{
    FreeBuffers();
    FreeMemory();
    FreeShaders();
    FreeTextures();
}

void OpenGL::FreeBuffers()
{
    annih_area_generator_buffer.Delete();
    asteroid_buffer.Delete();
    bomb_buffer.Delete();
    bonus_buffer.Delete();
    bullet_buffer.Delete();
    cyrcle_buffer.Delete();
    deceler_area_buffer.Delete();
    dynamic_particle_buffer.Delete();
    grav_gen_buffer.Delete();
    rectangle_buffer.Delete();
    particle_buffer.Delete();
    portal_buffer.Delete();
    ship_bullet_buffer.Delete();
    laser_buffer.Delete();
    mega_laser_buffer.Delete();
    polygon_buffer.Delete();
    turret_buffer.Delete();
    button_buffer.Delete();
    button_horisontal_buffer.Delete();
    button_vertical_buffer.Delete();
    knife_buffer.Delete();
    pilot_buffer.Delete();
    ship_buffer.Delete();
}

void OpenGL::FreeMemory()
{
}

void OpenGL::FreeShaders()
{
    annih_area_gen_shader.Delete();
    asteroid_shader.Delete();
    bomb_shader.Delete();
    bonus_shader.Delete();
    bullet_shader.Delete();
    deceler_area_shader.Delete();
    dynamic_particle_shader.Delete();
    grav_gen_shader.Delete();
    knife_shader.Delete();
    laser_shader.Delete();
    mega_laser_shader.Delete();
    particle_shader.Delete();
    portal_shader.Delete();
    pilot_shader.Delete();
    ship_shader.Delete();
    turret_shader.Delete();
    rectangle_shader.Delete();
    cyrcle_shader.Delete();
    polygon_shader.Delete();
    button_shader.Delete();
    controler_shader.Delete();
}

void OpenGL::FreeTextures()
{
    symbols_texture.Delete();
    annih_area_gen_basic_texture.Delete();
    asteroid_small_texture.Delete();
    asteroid_medium_texture.Delete();
    asteroid_large_texture.Delete();
    asteroid_sublimation_texture.Delete();
    bonus_bomb_texture.Delete();
    bonus_knife_texture.Delete();
    bonus_laser_texture.Delete();
    bonus_loop_texture.Delete();
    bonus_shield_texture.Delete();
    bonus_stream_texture.Delete();
    bonus_triple_texture.Delete();
    bonus_revers_texture.Delete();
    bomb_basic_texture.Delete();
    bomb_lighting_texture.Delete();
    bullet_small_texture.Delete();
    bullet_medium_texture.Delete();
    bullet_large_texture.Delete();
    pilot_basic_texture.Delete();
    ship_basic_texture.Delete();
    ship_triple_texture.Delete();
    turret_basic_texture.Delete();
}

OpenGL::~OpenGL()
{
}

//destructor and free memory functions