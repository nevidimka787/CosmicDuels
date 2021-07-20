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

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

std::shared_mutex timer_mtx;
uint64_t global_timer = 0;

void TikUpdate();
void PhysicsCalculation();

int main()
{
    OpenGL::InitOpneGL();
    GLFWwindow* window = OpenGL::CreateWindows(SCR_WIDTH, SCR_HEIGHT, "AstroParty", nullptr, nullptr);
    OpenGL::InitGlad();

    std::thread timer(TikUpdate);
    std::thread physic(PhysicsCalculation);

    while (!glfwWindowShouldClose(window))
    {
        OpenGL::ProcessInput(window);

        OpenGL::DrawFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void TikUpdate()
{
    while (true)
    {
        std::unique_lock<std::shared_mutex> ul(timer_mtx);

        global_timer++;

        timer_mtx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void PhysicsCalculation()
{
    std::shared_lock<std::shared_mutex> sl(timer_mtx);
    uint64_t current_tik = global_timer;
    timer_mtx.unlock();
    while (true)
    {
        std::shared_lock<std::shared_mutex> sl(timer_mtx);

        //start physics calculation



        //end physics calculation

        while (current_tik >= global_timer);
        current_tik = global_timer;

        timer_mtx.unlock();
    }
}