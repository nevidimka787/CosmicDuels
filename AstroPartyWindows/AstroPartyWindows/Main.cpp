﻿//#define SAMPLE

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

//This function begin then window is resizing.
void LocalFramebufferSizeCallback(GLFWwindow* window, int width, int height);

//Pointer to main window of app.
GLFWwindow* window = nullptr;
//Pointer to, storing and ruling all of the data of game, object.
Game* main_game = new Game();
//Pointer to, storing and ruling all of the data of game's menus, object.
MenuFunctions* main_menu_functions = new MenuFunctions();
//Pointer to, storing and ruling all of the data of drawing, object.
OpenGL* main_draw_functions = new OpenGL(SCR_WIDTH, SCR_HEIGHT, "AstroParty", nullptr, nullptr, LocalFramebufferSizeCallback, &window);

//Link Game, MenuFunctions and OpenG.
Linker* main_linker;

//FPS
unsigned frame = 0;
//count of updates of first physic thread per second
unsigned ph0 = 0;
//count of updates of second physic thread per second
unsigned ph1 = 0;
//count of updates of third physic thread per second
unsigned ph2 = 0;
//count of updates of fourth physic thread per second
unsigned ph3 = 0;

int main()
{
    main_linker = new Linker(main_game, main_menu_functions, main_draw_functions); //link Game, MainMenuFunctions and OpenGL
    //glfwWindowHint(GLFW_SAMPLES, 32);
    //glEnable(GL_MULTISAMPLE); //enable multisample
    //game cycle

    printf("-------------------\n");

    while (!glfwWindowShouldClose(window))
    {
        main_draw_functions->ProcessInput(window); //check input data
        if (main_draw_functions->update_menu > 0) //some event was detected in main_draw_functions->ProcessInput(window)
        {
            main_draw_functions->DrawFrame(); //draw menus
            //in main_draw_functions->DrawFrame() main_draw_functions->update_menu is decrementing
        }

        if (main_game->play_match == true) //flag "playing matc is active"
        {
            main_game->InitMatch();
            while (main_game->play_match == true && !glfwWindowShouldClose(window)) //while winner not detected or player do not wont go to main menu
            {
                std::thread timer_thread(InputOutputUpdate); //init console thread
                while (tik_update_thread_flag == false || physic_thread_flag == false)
                {
                    main_draw_functions->DrawFrame(); //draw loading screen
                    glfwSwapBuffers(window);
                    glfwPollEvents();
                    glfwSwapInterval(1);
                }

                main_draw_functions->flag_move_menu = true;
                main_draw_functions->flag_update_menu_can_change = true;
                main_draw_functions->update_menu = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT;

                while (tik_update_thread_flag == true || physic_thread_flag == true)
                {
                    main_draw_functions->DrawFrame(); //draw game proces
                    glfwSwapBuffers(window);
                    glfwPollEvents();
                    glfwSwapInterval(1);
                    frame++;
                }
                timer_thread.join(); //wait then raund full complete
                main_draw_functions->update_menu = OPEN_GL_REALISATION_FRAMES_AFTER_CALLBAC_COUNT * 3;
            }
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
        glfwSwapInterval(1);
    }

    main_draw_functions->Free();
    glfwTerminate(); //clear memory locating for OpenGL 
    return 0;
}

#pragma warning(disable : 6269)

void InputOutputUpdate()
{
    global_time_point = std::chrono::system_clock::now(); //get system time
    init_mtx.lock();
    init |= TIK_UPDATE_INIT;
    tik_update_thread_flag = true;
    std::chrono::system_clock::time_point local_time_point = global_time_point; //set begining time for console
    init_mtx.unlock();

    //lockal timer for console
    unsigned lock_timer = 0;
    main_game->play_round = true; //start raund

    //The thread start the round and is analising events of the round.
    std::thread physics_calculation(PhysicsCalculationStarter);

    while (main_game->play_round == true) //while round is gone
    {
        main_game->input_values_mtx.lock();
        main_draw_functions->ProcessInput(window); //check input from main window
        main_game->input_values_mtx.unlock();
        if (main_game->pause_round == false) //if round not pausing
        {
            lock_timer++;//update local timer of console
            if (!(lock_timer % 100)) //every second
            {//draw data to console
                uint32_t sum[2] =
                {
                    (uint32_t)main_game->thread_durations[0][0].count() + (uint32_t)main_game->thread_durations[1][0].count() + (uint32_t)main_game->thread_durations[2][0].count() + (uint32_t)main_game->thread_durations[3][0].count(),
                    (uint32_t)main_game->thread_durations[0][1].count() + (uint32_t)main_game->thread_durations[1][1].count() + (uint32_t)main_game->thread_durations[2][1].count() + (uint32_t)main_game->thread_durations[3][1].count()
                };
                if (sum[0] == 0) { sum[0] = 1; }
                if (sum[1] == 0) { sum[1] = 1; }
                uint32_t percent[4][2] = 
                {
                    {(uint32_t)main_game->thread_durations[0][0].count() * 400u / sum[0], (uint32_t)main_game->thread_durations[0][1].count() * 400u / sum[1]},
                    {(uint32_t)main_game->thread_durations[1][0].count() * 400u / sum[0], (uint32_t)main_game->thread_durations[1][1].count() * 400u / sum[1]},
                    {(uint32_t)main_game->thread_durations[2][0].count() * 400u / sum[0], (uint32_t)main_game->thread_durations[2][1].count() * 400u / sum[1]},
                    {(uint32_t)main_game->thread_durations[3][0].count() * 400u / sum[0], (uint32_t)main_game->thread_durations[3][1].count() * 400u / sum[1]}
                };

                printf("Ph:       Th:0       Th:1       Th:2       Th:3\n");
                printf("0: %11u%%%11u%%%11u%%%11u%%\n1: %11u%%%11u%%%11u%%%11u%%\n\n",
                    percent[0][0], percent[1][0], percent[2][0], percent[3][0],
                    percent[0][1], percent[1][1], percent[2][1], percent[3][1]);

                //reset counters
                main_game->ResetThreadDurations();
                frame = 0;
                ph0 = 0;
                ph1 = 0;
                ph2 = 0;
                ph3 = 0;
            }
            else if (!((lock_timer + 90) % 100)) //every second
            {//check thread lock
                if (ph0 == 0 || ph1 == 0 || ph2 == 0 || ph3 == 0)
                {
                    std::cout << "Thread lock detected." << std::endl;
                    std::cout << "Checking mutexes..." << std::endl;
                    //main_game->DebugLog__CheckMutexeslLock();
                    std::cout << "WARNING::Checking mutexes function was disabled." << std::endl;
                }
            }
            else if (!((lock_timer + 10) % 100)) //every second
            {//check threads desinchronisation
                if (!(ph0 == ph1 && ph1 == ph2 && ph2 == ph3))
                {
                    std::cout << "Thread desinchronisation detected." << std::endl;
                    printf("Ph0:%4u Ph1:%4u Ph2:%4u Ph3:%4u Ans:%1u\n", ph0, ph1, ph2, ph3, (unsigned)(!(ph0 == ph1 && ph1 == ph2 && ph2 == ph3)));
                }
            }
        }

        local_time_point += std::chrono::milliseconds(THREAD_INPUT_TIK_PERIOD); //set waking up point
        std::this_thread::sleep_until(local_time_point); //wait waking up point
    }
    physics_calculation.join(); //wait then physic calculation was completed

    tik_update_thread_flag = false;
}

std::shared_mutex physic_start;

void PhysicsCalculationStarter()
{
    init_mtx.lock();
    while (!(init & TIK_UPDATE_INIT)) //wait full initialisation of the console thread
    {
        init_mtx.unlock();
        init_mtx.lock();
    }

    main_game->InitLevel(); //initialisate all entities and variables for begining the round

    init |= PHYSICS_CALCULATION_INIT;

    physic_thread_flag = true;
    init_mtx.unlock();

    physic_start.lock();
    //calculate and update game physic
    std::thread physics_calculation0(PhysicsCalculation0);
    //calculate and update game physic
    std::thread physics_calculation1(PhysicsCalculation1);
    //calculate and update game physic
    std::thread physics_calculation2(PhysicsCalculation2);
    //calculate and update game physic
    std::thread physics_calculation3(PhysicsCalculation3);
    physic_start.unlock();

    physics_calculation0.join(); //wait completing of the physic calculation
    physics_calculation1.join(); //wait completing of the physic calculation
    physics_calculation2.join(); //wait completing of the physic calculation
    physics_calculation3.join(); //wait completing of the physic calculation

    //waking up point
    std::chrono::system_clock::time_point local_time_point = std::chrono::system_clock::now();

    main_game->RoundResultsInit();

    if (main_game->flag_end_match == false) //if round was completed
    {
        while (true)//round results menu update
        {
            local_time_point += std::chrono::seconds(1); //update waking up point
            std::this_thread::sleep_until(local_time_point); //wait waking up point

            main_game->ships_array_mtx.lock();
            main_game->log_data_mtx.lock();
            if (!main_game->RoundResults()) //if logs are empty
            {
                main_game->ships_array_mtx.unlock();
                main_game->log_data_mtx.unlock();
                break;
            }
            main_game->ships_array_mtx.unlock();
            main_game->log_data_mtx.unlock();
        }

        main_game->NextLevel(); //deside initialisating the next level or completing the match

        local_time_point += std::chrono::seconds(1); //update waking up point
        std::this_thread::sleep_until(local_time_point); //wait waking up point
    }
    if(main_game->flag_end_match == true)
    {
        main_game->EndMatch(); //completig the match
    }
    main_game->flag_round_results = false;

    physic_thread_flag = false;
}

void PhysicsCalculation0()
{
    //local time point
    std::chrono::system_clock::time_point local_time_point = global_time_point;
    physic_start.lock();
    physic_start.unlock();
    while (main_game->play_round) //while game gone
    {
        if (!main_game->pause_round) //roung not paused
        {
            main_game->PhysicThread0();
            ph0++; //increment count of updates of first thread
        }
        local_time_point += std::chrono::microseconds(THREAD_PHYSIC_TIK_PERIOD); //update waking up point
        std::this_thread::sleep_until(local_time_point); //wait waking up point
    }
}

void PhysicsCalculation1()
{
    //local time point
    std::chrono::system_clock::time_point local_time_point = global_time_point;
    physic_start.lock();
    physic_start.unlock();
    while (main_game->play_round) //while game gone
    {
        if (!main_game->pause_round) //roung not paused
        {
            main_game->PhysicThread1();
            ph1++; //increment count of updates of second thread
        }
        local_time_point += std::chrono::microseconds(THREAD_PHYSIC_TIK_PERIOD); //update waking up point
        std::this_thread::sleep_until(local_time_point); //wait waking up point
    }
}

void PhysicsCalculation2()
{
    //local time point
    std::chrono::system_clock::time_point local_time_point = global_time_point;
    physic_start.lock();
    physic_start.unlock();
    while (main_game->play_round) //while game gone
    {
        if (!main_game->pause_round) //roung not paused
        {
            main_game->PhysicThread2();
            ph2++; //increment count of updates of third thread
        }
        local_time_point += std::chrono::microseconds(THREAD_PHYSIC_TIK_PERIOD); //update waking up point
        std::this_thread::sleep_until(local_time_point); //wait waking up point
    }
}

void PhysicsCalculation3()
{
    //local time point
    std::chrono::system_clock::time_point local_time_point = global_time_point;
    physic_start.lock();
    physic_start.unlock();
    while (main_game->play_round) //while game gone
    {
        if (!main_game->pause_round) //roung not paused
        {
            main_game->PhysicThread3();
            ph3++; //increment count of updates of fourth thread
        }
        local_time_point += std::chrono::microseconds(THREAD_PHYSIC_TIK_PERIOD); //update waking up point
        std::this_thread::sleep_until(local_time_point); //wait waking up point
    }
}


void LocalFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    main_draw_functions->FramebufferSizeCallback(window, width, height);
}
