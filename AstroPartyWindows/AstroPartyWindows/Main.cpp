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

#define SCR_WIDTH   800
#define SCR_HEIGHT  1000

#define TIK_UPDATE_INIT 0x01
#define PHYSICS_CALCULATION_INIT 0x02

std::shared_mutex init_mtx;
uint8_t init = 0x00;

std::shared_mutex input_output_mtx;
std::shared_mutex physic_calculation_mtx;

std::chrono::system_clock::time_point global_time_point;

//This thread update 100 times per second and use for show information about app.
void InputOutputUpdate();
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

        if (main_game->play_match == true)
        {
            main_game->InitMatch();
            while (main_game->play_match == true)
            {
                std::thread timer_thread(InputOutputUpdate);
                while (tik_update_thread_flag == false || physic_thread_flag == false)
                {
                    physic_calculation_mtx.lock();
                    main_draw_functions->DrawFrame();
                    physic_calculation_mtx.unlock();
                    glfwSwapBuffers(window);
                    glfwPollEvents();
                    glfwSwapInterval(1);
                }
                while (tik_update_thread_flag == true || physic_thread_flag == true)
                {
                    physic_calculation_mtx.lock();
                    main_draw_functions->DrawFrame();
                    physic_calculation_mtx.unlock();
                    glfwSwapBuffers(window);
                    glfwPollEvents();
                    glfwSwapInterval(1);
                    frame++;
                }
                timer_thread.join();
                main_draw_functions->update_menu = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT;
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

void InputOutputUpdate()
{
    global_time_point = std::chrono::system_clock::now();
    init_mtx.lock();
    //std::cout << "Thread TikUpdate start init.";
    init |= TIK_UPDATE_INIT;
    tik_update_thread_flag = true;
    std::chrono::system_clock::time_point local_time_point = global_time_point;
    //std::cout << "Thread TikUpdate end init.";
    init_mtx.unlock();

    unsigned lock_timer = 0;
    main_game->play_round = true;
    std::thread physics_calculation(PhysicsCalculation);

    while (main_game->play_round == true)
    {
        input_output_mtx.lock();
        main_draw_functions->ProcessInput(window);
        if (main_game->pause_round == false)
        {
            lock_timer++;
            if (!(lock_timer % 100) && false)
            {
                std::cout << "Frame: " << frame  << std::endl 
                    << "Physic: " << physic << std::endl << std::endl;
                for (GameTypes::players_count_t team = 0; team < GAME_PLAYERS_MAX_COUNT; team++)
                {
                    std::cout << "Team: " << (int)team << " Score: " << (int)main_game->scores[team] << std::endl;
                }
                std::cout << "Rounds: " << (int)main_game->end_match_score - main_game->GetMaxScore() << std::endl;
                std::cout << "Bombs count: " << main_game->bombs_count << std::endl;
                std::cout << std::endl << std::endl << std::endl;
                frame = 0;
                physic = 0;
            }
        }
        input_output_mtx.unlock();

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
    //std::cout << "Thread PhysicsCalculation start init." << std::endl;
    //start initialisate all entities and variables

        main_game->InitLevel();

    //start initialisate all entities and variables
    //std::cout << "Thread PhysicsCalculation end init." << std::endl;
    physic_calculation_mtx.unlock();
    init |= PHYSICS_CALCULATION_INIT;
    //std::cout << "Threads init." << std::endl;
    physic_thread_flag = true;
    init_mtx.unlock();

    std::chrono::system_clock::time_point local_time_point = global_time_point;

    while (main_game->play_round == true)
    {
        if (main_game->pause_round == false)
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

    //std::cout << "Thread PhysicsCalculation exit." << std::endl;
    physic_thread_flag = false;
}


void LocalFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    main_draw_functions->FramebufferSizeCallback(window, width, height);
}
