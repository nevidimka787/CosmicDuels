#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../Types/OpenGLRealisationTypes.h"
#include "Game.h"
#include "../Classes/Menu.h"
#include "../Classes/Line.h"
#include "../Classes/GameEngine.h"
#include "../Classes/Shader.h"

class OpenGL
{
public:
	void (*FramebufferSizeCallbackPointer)(GLFWwindow* window, int width, int height);

	OpenGL(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share, GLFWframebuffersizefun Function, GLFWwindow** window);
	//buffers

	OpenGLTypes::buffer_t basic_triangle;
	OpenGLTypes::buffer_t basic_triangle_buffer;
	OpenGLTypes::buffer_t basic_square;
	OpenGLTypes::buffer_t basic_square_buffer;
	OpenGLTypes::buffer_t long_triangle;
	OpenGLTypes::buffer_t long_triangle_buffer;

	//buffers

	//start functions
	
	GLFWwindow* CreateWindows(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share, GLFWframebuffersizefun Function);
	bool CanDrawFrame(GLFWwindow* window);

	void InitOpenGL();
	void InitGlad();
	void InitBuffers();
	void InitShaders();

	//start functions

	//callback functions

	void ProcessInput(GLFWwindow* window);
	void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	
	//callback functions

	//shaders

	Shader* asteroid_shader;
	Shader* bomb_shader;
	Shader* bonus_shader;
	Shader* bullet_shader;
	Shader* knife_shader;
	Shader* mega_laser_sahder;
	Shader* pilot_shader;
	Shader* ship_shader;
	Shader* turel_shader;
	
	Shader* rectangle_shader;
	Shader* cyrcle_shader;
	Shader* polygon_shader;

	Shader* button_shader;

	//shaders

	//hight level funct

	void DrawFrame();
	void DrawAll();

	void DrawObjectCurrentMenu();
	void DrawObjectCurrentMap();
	void DrawObjectIndicatedMenu(Menu* menu);

	void DrawObject(Line* line, bool update_shader);
	void DrawObject(Beam* beam, bool update_shader);
	void DrawObject(Segment* segment, bool update_shader);

	void DrawObject(Entity* entity, bool update_shader);
	void DrawObject(StaticEntity* static_entity, bool update_shader);
	void DrawObject(DynamicEntity* dynamic_entity, bool update_shader);

	void DrawObject(Asteroid* asteroid, bool update_shader);
	void DrawObject(Bomb* mine, bool update_shader);
	void DrawObject(Bonus* bonus, bool update_shader);
	void DrawObject(Bullet* bullet, bool update_shader);
	void DrawObject(Knife* knife, bool update_shader);
	void DrawObject(MegaLaser* mega_laser, bool update_shader);
	void DrawObject(Pilot* pilot, bool update_shader);
	void DrawObject(Ship* ship, bool update_shader);
	void DrawObject(Turel* turel, bool update_shader);

	void DrawObject(MapElement* map_element, bool update_shader);
	void DrawObject(Rectangle* rectangle, bool update_shader);
	void DrawObject(Cyrcle* cyrcle, bool update_shader);
	void DrawObject(Polygon* polygon, bool update_shader);

	void DrawObject(Button* button, bool update_shader);

	void DrawAsteroids();
	void DrawBombs();
	void DrawBonuses();
	void DrawBullets();
	void DrawKnifes();
	void DrawMegaLasers();
	void DrawPilots();
	void DrawShips();
	void DrawTurels();
	//hight level functions

	//game pointers

	Menu** game_p__current_active_menu;

};