#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GameRealisation.h"
#include "Classes/Menu.h"
#include "Classes/Line.h"
#include "Classes/GameEngine.h"
#include "Classes/Shader.h"

namespace OpenGL
{
	typedef unsigned int buffer_t;
	//buffers

	static buffer_t basic_triangle;
	static buffer_t basic_triangle_vertex;
	static buffer_t basic_square;
	static buffer_t basic_square_vertex;
	static buffer_t long_triangle;
	static buffer_t long_triangle_vertex;

	//buffers

	//start functions
	
	GLFWwindow* CreateWindows(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);
	bool CanDrawFrame(GLFWwindow* window);
	namespace Init
	{
		void Glad();
		void OpneGL();
		void Buffers();
		void Shaders();
	}
	//start functions

	//callback functions

	void ProcessInput(GLFWwindow* window);
	void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	
	//callback functions

	//shaders

	static Shader* asteroid_shader;
	static Shader* bomb_shader;
	static Shader* bonus_shader;
	static Shader* bullet_shader;
	static Shader* knife_shader;
	static Shader* mega_laser_sahder;
	static Shader* pilot_shader;
	static Shader* ship_shader;
	static Shader* turel_shader;
	
	static Shader* rectangle_shader;
	static Shader* cyrcle_shader;
	static Shader* polygon_shader;

	static Shader* button_shader;

	//shaders

	//hight level funct

	void DrawFrame();
	void DrawAll();

	namespace DrawObject
	{
		void CurrentMenu();
		void CurrentMap();
		void IndicatedMenu(Menu* menu);

		void Object(Line* line, bool update_shader);
		void Object(Beam* beam, bool update_shader);
		void Object(Segment* segment, bool update_shader);

		void Object(Entity* entity, bool update_shader);
		void Object(StaticEntity* static_entity, bool update_shader);
		void Object(DynamicEntity* dynamic_entity, bool update_shader);

		void Object(Asteroid* asteroid, bool update_shader);
		void Object(Bomb* mine, bool update_shader);
		void Object(Bonus* bonus, bool update_shader);
		void Object(Bullet* bullet, bool update_shader);
		void Object(Knife* knife, bool update_shader);
		void Object(MegaLaser* mega_laser, bool update_shader);
		void Object(Pilot* pilot, bool update_shader);
		void Object(Ship* ship, bool update_shader);
		void Object(Turel* turel, bool update_shader);

		void Object(MapElement* map_element, bool update_shader);
		void Object(Rectangle* rectangle, bool update_shader);
		void Object(Cyrcle* cyrcle, bool update_shader);
		void Object(Polygon* polygon, bool update_shader);

		void Object(Button* button, bool update_shader);

		void Asteroids();
		void Bombs();
		void Bonuses();
		void Bullets();
		void Knifes();
		void MegaLasers();
		void Pilots();
		void Ships();
		void Turels();
	}
	//hight level functions
};