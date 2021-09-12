#pragma once

#include "MenuFunctions.h"

#include "../Classes/GameEngine.h"

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

	//buffers

	StaticBuffer asteroid_buffer;
	StaticBuffer bomb_buffer;
	StaticBuffer bonus_buffer;
	StaticBuffer bullet_buffer;
	StaticBuffer knife_buffer;
	StaticBuffer laser_buffer;
	StaticBuffer mega_laser_buffer;
	StaticBuffer pilot_buffer;
	StaticBuffer ship_buffer;
	StaticBuffer turel_buffer;

	StaticBuffer cyrcle_buffer;
	StaticBuffer polygon_buffer;
	StaticBuffer rectangle_buffer;

	StaticBuffer controler_buffer;
	StaticBuffer button_buffer;

	//buffers
	
	//data

	Vec2F temp__game__camera_position;
	float temp__game__camera_size;

	//data

	//flags

	//This values is set by defaultafter each click on the menu.
	//Each frame this value will be decrease until it reached zero.
	//Menu will be updating while the value is greater then zero.
	uint8_t update_menu;
	//If this flag is active then value of "update_menu" can be decrease.
	bool flag_update_menu_can_change;
	//The flag actives then menu is moving by cursore.
	bool flag_move_menu;

	int clear_frame;
#define CLEAR_FRAME_PERIOD	30;

	//flags

	//shaders

	//The shader use for draw asteroids.
	Shader asteroid_shader;
	//The shader use for draw bombs.
	Shader bomb_shader;
	//The shader use for draw bonuses.
	Shader bonus_shader;
	//The shader use for draw bullets.
	Shader bullet_shader;
	//The shader use for draw knifes.
	Shader knife_shader;
	//The shader use for draw lasers.
	Shader laser_shader;
	//The shader use for draw mega lasers.
	Shader mega_laser_shader;
	//The shader use for draw pilots.
	Shader pilot_shader;
	//The shader use for draw shipd.
	Shader ship_shader;
	//The shader use for draw turels.
	Shader turel_shader;

	//The shader use for draw rectangles.
	Shader rectangle_shader;
	//The shader use for draw cyrcles.
	Shader cyrcle_shader;
	//The shader use for draw polygons.
	Shader polygon_shader;

	//The shader use for draw buttons.
	Shader button_shader;
	//The shader use for draw control buttons of ships.
	Shader controler_shader;

	//shaders

	//textures

	//The texture stores symbols and signs that will be drawn on the buttons.
	Texture2D symbols_texture;

	//textures

	OpenGL(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share, GLFWframebuffersizefun Function, GLFWwindow** window);

	//Callback functions

	void CallMenuFunction(Menu* menu, Vec2F* clk_pos, uint8_t clk_statuse);
	void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	void LimitMenuPosition(Menu* menu);
	void ProcessInput(GLFWwindow* window);

	//Callback functions

	//Draw functions

	//The function is main draw function and called every draw cycle.
	void DrawFrame();

	//The function draw line.
	void DrawObject(Line* line, bool update_shader = false);
	//The function draw beam.
	void DrawObject(Beam* beam, bool update_shader = false);
	//The function draw segment.
	void DrawObject(Segment* segment, bool update_shader = false);

	//The function draw entity as vague object.
	void DrawObject(Entity* entity, bool update_shader = false);
	//The function draw static entity as vague object.
	void DrawObject(StaticEntity* static_entity, bool update_shader = false);
	//The function draw dynamic entity as vague object.
	void DrawObject(DynamicEntity* dynamic_entity, bool update_shader = false);

	//The function draw asteroid.
	void DrawObject(Asteroid* asteroid, bool update_shader = false);
	//The function draw asteroid.
	void DrawObject(Bomb* bomb, bool update_shader = false);
	//The function draw bonus.
	void DrawObject(Bonus* bonus, bool update_shader = false);
	//The function draw bullet.
	void DrawObject(Bullet* bullet, bool update_shader = false);
	//The function draw knife.
	void DrawObject(Knife* knife, bool update_shader = false);
	//The function draw laser.
	void DrawObject(Laser* mega_laser, bool update_shader = false);
	//The function draw mega laser.
	void DrawObject(MegaLaser* mega_laser, bool update_shader = false);
	//The function draw pilot.
	void DrawObject(Pilot* pilot, bool update_shader = false);
	//The function draw ship.
	void DrawObject(Ship* ship, bool update_shader = false);
	//The function draw turel.
	void DrawObject(Turel* turel, bool update_shader = false);

	//The function draw map element as vague object.
	void DrawObject(MapElement* map_element, bool update_shader = false);
	//The function draw map's rectangle.
	void DrawObject(Rectangle* rectangle, bool update_shader = false);
	//The function draw map's cyrcle.
	void DrawObject(Cyrcle* cyrcle, bool update_shader = false);
	//The function draw map's polygon.
	void DrawObject(Polygon* polygon, bool update_shader = false);

	//The function draw menu's button.
	void DrawObject(Button* button, bool button_is_controller = false, bool update_shader = false);

	//Draw functions

	//Multydraw functions

	//The function draw all asteroids on the map.
	void DrawAsteroids();
	//The function draw all bombs on the map.
	void DrawBombs();
	//The function draw all bonuses on the map.
	void DrawBonuses();
	//The function draw all bullets on the map.
	void DrawBullets();
	//The function draw all knifes on the map.
	void DrawKnifes();
	//The function draw all lasers on the map.
	void DrawLasers();
	//The function draw all mega lasers on the map.
	void DrawMegaLasers();
	//The function draw all pilots on the map.
	void DrawPilots();
	//The function draw all ships on the map.
	void DrawShips();
	//The function draw all turels on the map.
	void DrawTurels();

	//The function draww current map.
	void DrawCurrentMap();
	//The function draw current menu.
	void DrawCurrentMenu();
	//The function draww indicated map.
	void DrawIndicatedMap(Map* map);
	//The function draw indicated menu.
	void DrawIndicatedMenu(Menu* menu);

	//Multydraw functions

	//Get data functions

	bool CanDrawFrame(GLFWwindow* window);
	float GetScale();

	//Get data functions

	//Initialisations functions

	GLFWwindow* CreateWindows(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share, GLFWframebuffersizefun Function);

	void InitBuffers();
	void InitGlad();
	void InitOpenGL();
	void InitShaders();
	void InitTextures();

	//Initialisations functions

	//game pointers

	bool* game_p__play_match;
	bool* game_p__play_round;
	bool* game_p__flag_all_entities_initialisate;

	//Count of gravity generators on the map.
	GameTypes::map_elements_count_t* game_p__grav_gens_count;
	//Count of players on the map.
	GameTypes::players_count_t* game_p__players_count;
	//Count of ships on the map.
	GameTypes::players_count_t* game_p__ships_count;
	//Count of pilots on the map.
	GameTypes::players_count_t* game_p__pilots_count;
	//Count of knifes on the map.
	GameTypes::entities_count_t* game_p__knifes_count;
	//Count of lazers on the map.
	GameTypes::entities_count_t* game_p__lasers_count;
	//Count of bombs on the map.
	GameTypes::entities_count_t* game_p__bombs_count;
	//Count of turels on the map.
	GameTypes::entities_count_t* game_p__turels_count;
	//Count of map's lazers on the map.
	GameTypes::entities_count_t* game_p__mega_lasers_count;
	//Count of asteroids on the map.
	GameTypes::entities_count_t* game_p__asteroids_count;
	//Count of bonuses on the map.
	GameTypes::entities_count_t* game_p__bonuses_count;
	//Count of bullets on the map.
	GameTypes::entities_count_t* game_p__bullets_count;
	//Count of particles on the map.
	GameTypes::entities_count_t* game_p__particles_count;
	//Count of map's rectangles on the map.
	GameTypes::map_elements_count_t* game_p__rectangles_count;

	//Array of ships
	Ship** game_p__ships;
	//Array of pilots.
	Pilot** game_p__pilots;
	//Array of asteroids.
	Asteroid** game_p__asteroids;
	//Array of bonuses.
	Bonus** game_p__bonuses;
	//Array of bombs.
	Bomb** game_p__bombs;
	//Array of bullets.
	Bullet** game_p__bullets;
	//Array of knifes.
	Knife** game_p__knifes;
	//Array of lazers.
	Laser** game_p__lasers;
	//Array of particles.
	Particle** game_p__particles;

	//Object stores data about map on current level.
	Map* game_p__map;
	//Array of rectangles.
	Rectangle** game_p__rectangles;
	//Array of gravity generators.
	GravGen** game_p__grav_gens;
	//Array of turels.
	Turel** game_p__turels;
	//Array of lasers of the map.
	MegaLaser** game_p__mega_lasers;

	Menu** game_p__current_active_menu;

	Menu* game_p__bonus_pull_menu;
	Menu* game_p__main_menu;
	Menu* game_p__map_pull_select_menu;
	Menu* game_p__option_menu;
	Menu* game_p__pause_menu;
	Menu* game_p__ships_select_menu;
	Menu* game_p__ships_control_menu;
	Menu* game_p__spawning_objects_select_menu;

	Camera* game_p__camera;

	bool** game_p__rotate_flags;
	bool** game_p__shoot_flags;


	//menu functions pointers

	MenuFunctions* object_p__menu_functions;

};