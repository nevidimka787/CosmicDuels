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

void TikUpdate();
void PhysicsCalculation();
void Draw(GLFWwindow* window);

int main()
{
    OpenGL::Init::OpneGL();
    GLFWwindow* window = OpenGL::CreateWindows(SCR_WIDTH, SCR_HEIGHT, "AstroParty", nullptr, nullptr);
    OpenGL::Init::Glad();

    Game::Init::Menus();
    Game::Init::Game();

    //game cycle
    while (true)
    {
        OpenGL::DrawObject::CurrentMenu();
        if (Game::start_game == true)
        {
            Game::Init::Mach();
            while (Game::start_game == true)
            {
                std::thread physic(PhysicsCalculation);
                std::thread draw(Draw, window);
                physic.join();
                draw.join();
            }
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
        OpenGL::DrawObject::CurrentMenu();
        physic_calculation_mtx.unlock();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void TikUpdate()
{
    init_mtx.lock();
    init |= TIK_UPDATE_INIT;
    init_mtx.unlock();
    while (true)
    {
        timer_mtx.lock();
        if (Game::pause_game == false)
        {
            Game::global_timer++;
        }
        timer_mtx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void PhysicsCalculation()
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

        Game::Init::Level();

    //start initialisate all entities and variables
    physic_calculation_mtx.unlock();
    init |= PHYSICS_CALCULATION_INIT;
    init_mtx.unlock();


    timer_mtx.lock();
    Game::current_tic = Game::global_timer;
    timer_mtx.unlock();

    while (true)
    {
        physic_calculation_mtx.lock();
        //start physics calculation

        Game::Recalculate();

        //end physics calculation
        physic_calculation_mtx.unlock();

        timer_mtx.lock();
        while (Game::current_tic >= Game::global_timer)
        {
            timer_mtx.unlock();
            timer_mtx.lock();
        }
        Game::current_tic = Game::global_timer;
        timer_mtx.unlock();

    }
}