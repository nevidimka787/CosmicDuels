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

std::chrono::system_clock::time_point global_time_point;

//This thread update 100 times per second and use for show information about app.
void TikUpdate();
//This thread update 100 times per second and use for update all physical data of all entities.
void PhysicsCalculation();

bool physic_thread_flag = false;
bool tik_update_thread_flag = false;

void LocalFramebufferSizeCallback(GLFWwindow* window, int width, int height);

GLFWwindow* window = nullptr;
Game* main_game = new Game();
MenuFunctions* main_menu_functions = new MenuFunctions();
OpenGL* main_draw_functions = new OpenGL(SCR_WIDTH, SCR_HEIGHT, "AstroParty", nullptr, nullptr, LocalFramebufferSizeCallback, &window);

Linker* main_linker;

unsigned frame = 0;
unsigned physic = 0;

int main()
{
    main_linker = new Linker(main_game, main_menu_functions, main_draw_functions);
    //game cycle
    while (true)
    {
        main_draw_functions->ProcessInput(window);
        if (main_draw_functions->update_menu > 0)
        {
            main_draw_functions->DrawFrame();
        }
        
        //Frame();
        
        //std::cout << main_game->start_game << std::endl;

        if (main_game->start_game == true)
        {
            main_game->InitMach();
            while (main_game->start_game == true)
            {
                std::thread timer_thread(TikUpdate);
                while (tik_update_thread_flag == false || physic_thread_flag == false)
                {
                    physic_calculation_mtx.lock();
                    main_draw_functions->ProcessInput(window);
                    main_draw_functions->DrawFrame();
                    physic_calculation_mtx.unlock();
                    glfwSwapBuffers(window);
                    glfwPollEvents();
                    glfwSwapInterval(1);
                }
                while (tik_update_thread_flag == true || physic_thread_flag == true)
                {
                    physic_calculation_mtx.lock();
                    main_draw_functions->ProcessInput(window);
                    main_draw_functions->DrawFrame();
                    physic_calculation_mtx.unlock();
                    glfwSwapBuffers(window);
                    glfwPollEvents();
                    //std::cout << main_game->ships[0].GetAngle() << std::endl;
                    glfwSwapInterval(1);
                    frame++;
                }
                timer_thread.join();
            }
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
        glfwSwapInterval(1);
    }

    glfwTerminate();
    return 0;
}

#pragma warning(disable : 6269)

#define SECOND  100
#define MINUTE  6000
#define HOUR    360000


#define LOGS_UPDATE_PERIOD

void TikUpdate()
{
    global_time_point = std::chrono::system_clock::now();
    init_mtx.lock();
    init |= TIK_UPDATE_INIT;
    tik_update_thread_flag = true;
    std::chrono::system_clock::time_point local_time_point = global_time_point;
    init_mtx.unlock();

    unsigned lock_timer = 0;

    std::thread physics_calculation(PhysicsCalculation);

    while (main_game->start_game == true)
    {
        timer_mtx.lock();
        if (main_game->pause_game == false)
        {
            lock_timer++;
            if (!(lock_timer % 100))
            {
                std::cout << "Frame: " << frame  << std::endl 
                    << "Physic: " << physic << std::endl
                    << "Ship: " << main_game->ships[0].GetDirection() << std::endl
                    << "Camera: " << main_game->camera.GetPosition() << std::endl << std::endl;
                frame = 0;
                physic = 0;
            }
        }
        timer_mtx.unlock();

        local_time_point += std::chrono::milliseconds(10);
        std::this_thread::sleep_until(local_time_point);
    }
    physics_calculation.join();
    tik_update_thread_flag = false;
}

void PhysicsCalculation()
{

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

    std::chrono::system_clock::time_point local_time_point = global_time_point;

    while (main_game->start_game == true)
    {
        if (main_game->pause_game == false)
        {
            physic_calculation_mtx.lock();
            //start physics calculation

            main_game->Update();
            physic++;
            //end physics calculation
            physic_calculation_mtx.unlock();
        }

        local_time_point += std::chrono::milliseconds(10);
        std::this_thread::sleep_until(local_time_point);
    }
    physic_thread_flag = false;

    physic_calculation_mtx.lock();

    main_game->NextLevel();

    physic_calculation_mtx.unlock();
}


void LocalFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    main_draw_functions->FramebufferSizeCallback(window, width, height);
}
