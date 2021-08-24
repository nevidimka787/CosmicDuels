//#define SAMPLE

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <thread>
#include <shared_mutex>

#include "SubClasses/Game.h"
#include "SubClasses/MenuFunctions.h"
#include "SubClasses/OpenGLRealisation.h"
#include "SubClasses/Linker.h"

#define SCR_WIDTH 1600
#define SCR_HEIGHT 800

#define TIK_UPDATE_INIT 0x01
#define PHYSICS_CALCULATION_INIT 0x02

std::shared_mutex init_mtx;
uint8_t init = 0x00;

std::shared_mutex timer_mtx;
std::shared_mutex physic_calculation_mtx;

void TikUpdate();
void PhysicsCalculation();
void Draw(GLFWwindow* window);

void LocalFramebufferSizeCallback(GLFWwindow* window, int width, int height);

Game* main_game = nullptr;
MenuFunctions* main_menu_functions = nullptr;
OpenGL* main_draw_functions = nullptr;

Linker* main_linker;

int main()
{
    GLFWwindow* window = nullptr;
    main_game = new Game();
    main_menu_functions = new MenuFunctions();
    main_draw_functions = new OpenGL(SCR_WIDTH, SCR_HEIGHT, "AstroParty", nullptr, nullptr, LocalFramebufferSizeCallback, &window);

    main_linker = new Linker(main_game, main_menu_functions, main_draw_functions);

    //game cycle
    while (true)
    {
        main_draw_functions->ProcessInput(window);
        if (main_draw_functions->flag_update_menu > 0)
        {
            main_draw_functions->DrawFrame();
        }
        
        //Frame();
        
        if (main_game->start_game == true)
        {
            main_game->InitMach();
            while (main_game->start_game == true)
            {
                std::thread physic(PhysicsCalculation);
                std::thread draw(Draw, window);
                physic.join();
                draw.join();
            }
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
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
        main_draw_functions->ProcessInput(window);

        physic_calculation_mtx.lock();
        std::cout << "Draw Begin" << std::endl;
        main_draw_functions->DrawFrame();
        std::cout << "Draw End" << std::endl;
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
        if (main_game->pause_game == false)
        {
            main_game->global_timer++;
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

        main_game->InitLevel();

    //start initialisate all entities and variables
    physic_calculation_mtx.unlock();
    init |= PHYSICS_CALCULATION_INIT;
    init_mtx.unlock();


    timer_mtx.lock();
    main_game->current_tic = main_game->global_timer;
    timer_mtx.unlock();

    while (true)
    {
        physic_calculation_mtx.lock();
        //start physics calculation

        std::cout << "Update Begin" << std::endl;
        main_game->Update();
        std::cout << "Update End" << std::endl;

        //end physics calculation
        physic_calculation_mtx.unlock();

        timer_mtx.lock();
        while (main_game->current_tic >= main_game->global_timer)
        {
            timer_mtx.unlock();
            timer_mtx.lock();
        }
        main_game->current_tic = main_game->global_timer;
        timer_mtx.unlock();

    }
}


void LocalFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    main_draw_functions->FramebufferSizeCallback(window, width, height);
}
