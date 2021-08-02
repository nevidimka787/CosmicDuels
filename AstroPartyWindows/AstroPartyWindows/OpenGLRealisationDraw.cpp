#include "OpenGLRealisation.h"



void OpenGL::DrawObject::IndicatedMenu(Menu* menu)
{
    Menu* last_menu = Game::current_active_menu;
    Game::current_active_menu = menu;
    CurrentMenu();
    Game::current_active_menu = last_menu;
}

