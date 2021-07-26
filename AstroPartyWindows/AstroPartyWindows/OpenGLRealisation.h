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
public:
	Shader(const char* geometric_file_name, const char* fragment_file_name);

	void Use();
	void SetGeometric(const char* geometric_file_name);
	void SetFragment(const char* fragment_file_name);
	~Shader();

};

namespace OpenGL
{
	//low level functions

	void InitOpneGL();
	void InitGlad();
	GLFWwindow* CreateWindows(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);
	bool CanDrawFrame(GLFWwindow* window);
	void DrawFrame();

	void BindShader();

	void ProcessInput(GLFWwindow* window);
	void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

	//low level functions

	//hight level functions

	void DrawMenu(Menu* menu);

	void DrawObject(Line* line, bool update_shader);
	void DrawObject(Beam* beam, bool update_shader);
	void DrawObject(Segment* segment, bool update_shader);

	void DrawObject(Entity* entity, bool update_shader);
	void DrawObject(StaticEntity* static_entity, bool update_shader);
	void DrawObject(DynamicEntity* dynamic_entity, bool update_shader);
	void DrawObject(Bonus* bonus, bool update_shader);
	void DrawObject(Asteroid* asteroid, bool update_shader);
	void DrawObject(ControledEntity controled_entity, bool update_shader);
	void DrawObject(Sheep* sheep, bool update_shader);
	void DrawObject(Pilot* pilot, bool update_shader);
	void DrawObject(Turel* turel, bool update_shader);
	void DrawObject(MegaLazer* mega_lazer, bool update_shader);
	void DrawObject(Mine* mine, bool update_shader);
	void DrawObject(MapElement* map_element, bool update_shader);
	void DrawObject(Rectangle* rectangle, bool update_shader);
	void DrawObject(Cyrcle* cyrcle, bool update_shader);
	void DrawObject(Polygon* polygon, bool update_shader);
	void DrawObject(Map* map, bool update_shader);

	//hight level functions
};

#endif //OPENGL_REALISASTION_H