#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../Types/OpenGLRealisationTypes.h"
#include "../Constants/OpenGLRealisationConstants.h"
#include "Game.h"
#include "../Classes/Menu.h"
#include "../Classes/Line.h"
#include "../Classes/GameEngine.h"
#include "../Classes/Shader.h"
#include "../Classes/Texture.h"

class OpenGL
{
protected:
	uint8_t mouse_buttons = 0x00;
	GLint window_height;
	GLint window_width;
	//width / height
	float window_scale;
	Vec2D* cursore_current_position;
	Vec2D* cursore_last_position;
	Vec2D* cursore_press_position;
	Vec2D* cursore_release_position;
public:
	void (*FramebufferSizeCallbackPointer)(GLFWwindow* window, int width, int height);

	OpenGL(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share, GLFWframebuffersizefun Function, GLFWwindow** window);
	
	//buffers

	//3 vertexes
	OpenGLTypes::buffer_t basic_triangle;
	OpenGLTypes::buffer_t basic_triangle_buffer;
	//6 vertexes
	OpenGLTypes::buffer_t basic_square;
	OpenGLTypes::buffer_t basic_square_buffer;
	//3 vertexes
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
	void InitTextures();

	//start functions

	//callback functions

	void ProcessInput(GLFWwindow* window);
	void LimitMenuPosition(Menu* menu);
	void CallMenuFunction(Menu* menu, Vec2F* clk_pos, uint8_t clk_statuse);
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

	Texture2D* symbols_texture;

	uint8_t flag_update_menu;
	bool flag_update_menu_can_change;
	bool flag_move_menu;

	//shaders

	//hight level funct

	void DrawFrame();

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
	Menu* game_p__option_menu;
	Menu* game_p__main_menu;
	Menu* game_p__pause_menu;
	Menu* game_p__ships_select_menu;
	Menu* game_p__map_pull_select_menu;
	Menu* game_p__spawning_objects_select_menu;
	Menu* game_p__ships_control_menu;


	//menu functions pointers

	MenuFunctions* object_p__menu_functions;

};