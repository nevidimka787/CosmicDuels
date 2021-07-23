#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <thread>
#include <shared_mutex>

#include "OpenGLRealisation.h"
#include "Vec.h"
#include "Line.h"
#include "Mat.h"
#include "GameEngine.h"
#include "GameRealisation.h"
#include "Menu.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

#define TIK_UPDATE_INIT 0x01
#define PHYSICS_CALCULATION_INIT 0x02

std::shared_mutex init_mtx;
uint8_t init = 0x00;

std::shared_mutex timer_mtx;
std::shared_mutex physic_calculation_mtx;
uint32_t global_timer = 0;

void TikUpdate();
void PhysicsCalculation(uint8_t* level_id);
void Draw(GLFWwindow* window);

int main()
{
    OpenGL::InitOpneGL();
    GLFWwindow* window = OpenGL::CreateWindows(SCR_WIDTH, SCR_HEIGHT, "AstroParty", nullptr, nullptr);
    OpenGL::InitGlad();

    bool start_game = false;
    uint8_t level;
    Game::MenusInit();
    Game::GameInit();

    while (true)
    {
        OpenGL::DrawObject(Game::current_active_menu, false);
        if (start_game == true)
        {
            Game::MachInit(2, GAME_RULE_PLAYERS_SPAWN_THIS_BONUS | GAME_RULE_START_BONUS_RANDOMIZE | GAME_RULE_NEED_KILL_PILOT | GAME_RULE_ASTEROIDS_IS_SPAWNING | GAME_RULE_BONUSES_IS_SPAWNING, 0);
            std::thread physic(PhysicsCalculation, &level);
            std::thread draw(Draw, window);
            physic.join();
        }
    }

    glfwTerminate();
    return 0;
}

void Draw(GLFWwindow* window)
{
    init_mtx.lock();
    while ((init & (TIK_UPDATE_INIT | PHYSICS_CALCULATION_INIT)) != (TIK_UPDATE_INIT | PHYSICS_CALCULATION_INIT))
    {
        init_mtx.unlock();
        init_mtx.lock();
    }
    init_mtx.unlock();

    while (!glfwWindowShouldClose(window))
    {
        OpenGL::ProcessInput(window);

        physic_calculation_mtx.lock();
        OpenGL::DrawFrame();
        physic_calculation_mtx.unlock();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void TikUpdate()
{
    init_mtx.lock();
    timer_mtx.lock();
    global_timer = 0;
    timer_mtx.unlock();
    init |= TIK_UPDATE_INIT;
    init_mtx.unlock();
    while (true)
    {
        timer_mtx.lock();

        global_timer++;

        timer_mtx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void PhysicsCalculation(uint8_t* level_id)
{
    std::thread timer(TikUpdate);

    init_mtx.lock();
    while (!(init & TIK_UPDATE_INIT))
    {
        init_mtx.unlock();
        init_mtx.lock();
    }
    physic_calculation_mtx.lock();
    //start initialisate all entities and variables

    if (Game::game_rules & GAME_RULE_RANDOMLY_MAP)
    {
        Game::LevelInit(Game::GetRandomMap());
    }
    else
    {
        Game::LevelInit(*level_id);
    }


    //start initialisate all entities and variables
    physic_calculation_mtx.unlock();
    init |= PHYSICS_CALCULATION_INIT;
    init_mtx.unlock();


    timer_mtx.lock();
    uint64_t current_tik = global_timer;
    timer_mtx.unlock();

    while (true)
    {
        physic_calculation_mtx.lock();
        //start physics calculation

        Game::Recalculate();

        //end physics calculation
        physic_calculation_mtx.unlock();

        timer_mtx.lock();
        while (current_tik >= global_timer)
        {
            timer_mtx.unlock();
            timer_mtx.lock();
        }
        current_tik = global_timer;
        timer_mtx.unlock();

    }
}