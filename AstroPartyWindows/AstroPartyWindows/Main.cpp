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

std::chrono::system_clock::time_point global_time_point;

//This thread update 100 times per second and use for show information about app.
void InputOutputUpdate();
//This thread start physics calculation thread and wait when they stop.
void PhysicsCalculationStarter();
//This thread update 100 times per second and use for update all physical data of all entities.
void PhysicsCalculation0();
//This thread update 100 times per second and use for update all physical data of all entities.
void PhysicsCalculation1();
//This thread update 100 times per second and use for update all physical data of all entities.
void PhysicsCalculation2();
//This thread update 100 times per second and use for update all physical data of all entities.
void PhysicsCalculation3();

bool physic_thread_flag = false;
bool tik_update_thread_flag = false;

void LocalFramebufferSizeCallback(GLFWwindow* window, int width, int height);

GLFWwindow* window = nullptr;
Game* main_game = new Game();
MenuFunctions* main_menu_functions = new MenuFunctions();
OpenGL* main_draw_functions = new OpenGL(SCR_WIDTH, SCR_HEIGHT, "AstroParty", nullptr, nullptr, LocalFramebufferSizeCallback, &window);

Linker* main_linker;

unsigned frame = 0;
unsigned ph0 = 0;
unsigned ph1 = 0;
unsigned ph2 = 0;
unsigned ph3 = 0;

int main()
{
    main_linker = new Linker(main_game, main_menu_functions, main_draw_functions);
    glEnable(GL_MULTISAMPLE);
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
                    main_draw_functions->DrawFrame();
                    glfwSwapBuffers(window);
                    glfwPollEvents();
                    glfwSwapInterval(1);
                }
                while (tik_update_thread_flag == true || physic_thread_flag == true)
                {
                    main_draw_functions->DrawFrame();
                    glfwSwapBuffers(window);
                    glfwPollEvents();
                    glfwSwapInterval(1);
                    frame++;
                }
                timer_thread.join();
                main_draw_functions->update_menu = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT * 3;
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
    std::thread physics_calculation(PhysicsCalculationStarter);

    while (main_game->play_round == true)
    {
        main_game->input_values_mtx.lock();
        main_draw_functions->ProcessInput(window);
        main_game->input_values_mtx.unlock();
        if (main_game->pause_round == false)
        {
            lock_timer++;
            if (!(lock_timer % 100))
            {
                std::cout << "Frame: " << frame << std::endl;
                printf("Ph0:%4u Ph1:%4u Ph2:%4u Ph3:%4u\n", ph0, ph1, ph2, ph3);
                for (GameTypes::players_count_t team = 0; team < GAME_PLAYERS_MAX_COUNT; team++)
                {
                    std::cout << "Team: " << (int)team << " Score: " << (int)main_game->scores[team] << std::endl;
                }
                frame = 0;
                ph0 = 0;
                ph1 = 0;
                ph2 = 0;
                ph3 = 0;
            }
            else if (!((lock_timer + 30) % 100))
            {
                if (ph0 == 0 || ph1 == 0 || ph2 == 0 || ph3 == 0)
                {
                    std::cout << "Thread lock detected." << std::endl;
                    std::cout << "Checking mutexes..." << std::endl;
                    main_game->DebugLog__CheckMutexeslLock();
                }
            }
            else if (!((lock_timer + 60) % 100))
            {
                if (!(ph0 == ph1 && ph1 == ph2 && ph2 == ph3))
                {
                    std::cout << "Thread desinchronisation detected." << std::endl;
                }
            }
        }

        local_time_point += std::chrono::milliseconds(THREAD_INPUT_TIK_PERIOD);
        std::this_thread::sleep_until(local_time_point);
    }
    physics_calculation.join();

    tik_update_thread_flag = false;
}

std::shared_mutex physic_start;

void PhysicsCalculationStarter()
{

    init_mtx.lock();
    while (!(init & TIK_UPDATE_INIT))
    {
        init_mtx.unlock();
        init_mtx.lock();
    }
    //std::cout << "Thread PhysicsCalculation start init." << std::endl;
    //start initialisate all entities and variables

        main_game->InitLevel();

    //start initialisate all entities and variables
    //std::cout << "Thread PhysicsCalculation end init." << std::endl;
    init |= PHYSICS_CALCULATION_INIT;
    //std::cout << "Threads init." << std::endl;
    physic_thread_flag = true;
    init_mtx.unlock();

    physic_start.lock();
    std::thread physics_calculation0(PhysicsCalculation0);
    std::thread physics_calculation1(PhysicsCalculation1);
    std::thread physics_calculation2(PhysicsCalculation2);
    std::thread physics_calculation3(PhysicsCalculation3);
    physic_start.unlock();

    physics_calculation0.join();
    physics_calculation1.join();
    physics_calculation2.join();
    physics_calculation3.join();

    std::chrono::system_clock::time_point local_time_point = std::chrono::system_clock::now();

    main_game->RoundResultsInit();

    if (main_game->flag_end_match == false)
    {
        while (true)
        {
            local_time_point += std::chrono::seconds(1);
            std::this_thread::sleep_until(local_time_point);

            main_game->ships_array_mtx.lock();
            main_game->log_data_mtx.lock();
            if (!main_game->RoundResults())
            {
                main_game->ships_array_mtx.unlock();
                main_game->log_data_mtx.unlock();
                break;
            }
            main_game->ships_array_mtx.unlock();
            main_game->log_data_mtx.unlock();
        }

        main_game->NextLevel();

        local_time_point += std::chrono::seconds(1);
        std::this_thread::sleep_until(local_time_point);
    }
    if(main_game->flag_end_match == true)
    {
        main_game->EndMatch();
    }
    main_game->flag_round_results = false;

    physic_thread_flag = false;
}

void PhysicsCalculation0()
{
    std::chrono::system_clock::time_point local_time_point = global_time_point;
    physic_start.lock();
    physic_start.unlock();
    while (main_game->play_round)
    {
        if (!main_game->pause_round)
        {
            main_game->PhysicThread0();
            ph0++;
        }
        local_time_point += std::chrono::microseconds(THREAD_PHYSIC_TIK_PERIOD);
        std::this_thread::sleep_until(local_time_point);
    }
}

void PhysicsCalculation1()
{
    std::chrono::system_clock::time_point local_time_point = global_time_point;
    physic_start.lock();
    physic_start.unlock();
    while (main_game->play_round)
    {
        if (!main_game->pause_round)
        {
            main_game->PhysicThread1();
            ph1++;
        }
        local_time_point += std::chrono::microseconds(THREAD_PHYSIC_TIK_PERIOD);
        std::this_thread::sleep_until(local_time_point);
    }
}

void PhysicsCalculation2()
{
    std::chrono::system_clock::time_point local_time_point = global_time_point;
    physic_start.lock();
    physic_start.unlock();
    while (main_game->play_round)
    {
        if (!main_game->pause_round)
        {
            main_game->PhysicThread2();
            ph2++;
        }
        local_time_point += std::chrono::microseconds(THREAD_PHYSIC_TIK_PERIOD);
        std::this_thread::sleep_until(local_time_point);
    }
}

void PhysicsCalculation3()
{
    std::chrono::system_clock::time_point local_time_point = global_time_point;
    physic_start.lock();
    physic_start.unlock();
    while (main_game->play_round)
    {
        if (!main_game->pause_round)
        {
            main_game->PhysicThread3();
            ph3++;
        }
        local_time_point += std::chrono::microseconds(THREAD_PHYSIC_TIK_PERIOD);
        std::this_thread::sleep_until(local_time_point);
    }
}


void LocalFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    main_draw_functions->FramebufferSizeCallback(window, width, height);
}
