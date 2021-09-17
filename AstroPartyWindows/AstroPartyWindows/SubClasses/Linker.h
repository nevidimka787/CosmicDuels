#pragma once
#include "Game.h"
#include "MenuFunctions.h"
#include "OpenGLRealisation.h"

class Linker
{
public:
	Linker(Game* game_object, MenuFunctions* menu_functins_object, OpenGL* open_gl_object);
	~Linker();
};