#ifndef OPENGL_REALISASTION_H
#define OPENGL_REALISASTION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GameRealisation.h"
#include "Menu.h"
#include "Line.h"
#include "GameEngine.h"
#include "MenuFunctions.h"

class Shader
{
private:
	uint32_t id;
public:
	Shader(const char* vertex_file_name, const char* fragment_file_name);

	void SetValue(char* name, float value);
	void SetValue(char* name, int value);
	void SetValue(char* name, unsigned value);
	void SetValue(char* name, Vec2F* vector);

	void Use();

	~Shader();

};

namespace OpenGL
{
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

	//shaders

	//hight level funct

	void DrawFrame();
	void DrawAll();

	namespace DrawObject
	{

		void IndicatedMenu(Menu* menu);
		void CurrentMenu();

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
		void Object(Map* map, bool update_shader);

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

#endif //OPENGL_REALISASTION_H