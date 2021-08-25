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

bool physic_thread_flag = false;
bool tik_update_thread_flag = false;

void LocalFramebufferSizeCallback(GLFWwindow* window, int width, int height);

GLFWwindow* window = nullptr;
Game* main_game = new Game();
MenuFunctions* main_menu_functions = new MenuFunctions();
OpenGL* main_draw_functions = new OpenGL(SCR_WIDTH, SCR_HEIGHT, "AstroParty", nullptr, nullptr, LocalFramebufferSizeCallback, &window);

Linker* main_linker;

int main()
{
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
                while (tik_update_thread_flag == false || physic_thread_flag == false)
                {
                    main_draw_functions->ProcessInput(window);
                    main_draw_functions->DrawFrame();
                    glfwSwapBuffers(window);
                    glfwPollEvents();
                }
                while (tik_update_thread_flag == true || physic_thread_flag == true)
                {
                    main_draw_functions->ProcessInput(window);
                    main_draw_functions->DrawFrame();
                    glfwSwapBuffers(window);
                    glfwPollEvents();
                }
                physic.join();
            }
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void TikUpdate()
{
    init_mtx.lock();
    init |= TIK_UPDATE_INIT;
    tik_update_thread_flag = true;
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
    tik_update_thread_flag = false;
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
    physic_thread_flag = true;
    init_mtx.unlock();


    timer_mtx.lock();
    main_game->current_tic = main_game->global_timer;
    timer_mtx.unlock();

    while (true)
    {
        physic_calculation_mtx.lock();
        //start physics calculation

        main_game->Update();

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
    physic_thread_flag = false;
}


void LocalFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    main_draw_functions->FramebufferSizeCallback(window, width, height);
}
