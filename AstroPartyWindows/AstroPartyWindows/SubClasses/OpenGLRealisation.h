#pragma once

#include <shared_mutex>
#include <vector>

#include "MenuFunctions.h"

#include "../Classes/GameEngine.h"

class OpenGL
{
protected:
	GLFWwindow* window;

	uint16_t button_commands = 0x0000;
	GLint window_height;
	GLint window_width;
	//width / height
	float window_scale;
	Vec2D* cursore_current_position;
	Vec2D* cursore_last_position;
	Vec2D* cursore_press_position;
	Vec2D* cursore_release_position;

	//The array store data about pushed keys on keyboard.
	std::vector<bool> rotate_key_flags;
	//The array store data about pushed keys on keyboard.
	std::vector<bool> shoot_key_flags;
public:

	//buffers

	StaticBuffer annih_area_generator_buffer;
	StaticBuffer asteroid_buffer;
	StaticBuffer bomb_buffer;
	StaticBuffer bonus_buffer;
	StaticBuffer bullet_buffer;
	StaticBuffer deceler_area_buffer;
	StaticBuffer dynamic_particle_buffer;
	StaticBuffer grav_gen_buffer;
	StaticBuffer knife_buffer;
	StaticBuffer laser_buffer;
	StaticBuffer mega_laser_buffer;
	StaticBuffer particle_buffer;
	StaticBuffer portal_buffer;
	StaticBuffer pilot_buffer;
	StaticBuffer ship_buffer;
	StaticBuffer ship_bullet_buffer;
	StaticBuffer turret_buffer;

	StaticBuffer cyrcle_buffer;
	StaticBuffer polygon_buffer;
	StaticBuffer rectangle_buffer;

	StaticBuffer button_horisontal_buffer;
	StaticBuffer button_vertical_buffer;
	StaticBuffer button_buffer;

	//FrameBuffer main_buffer;

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

	//The shader use for draw annihilatio area generator.
	Shader annih_area_gen_shader;
	//The shader use for draw asteroids.
	Shader asteroid_shader;
	//The shader use for draw bombs.
	Shader bomb_shader;
	//The shader use for draw bonuses.
	Shader bonus_shader;
	//The shader use for draw bullets.
	Shader bullet_shader;
	//The shader use for draw deceleration areas.
	Shader deceler_area_shader;
	//The shader use for draw dynamic particles.
	Shader dynamic_particle_shader;
	//The shader use for draw gravity generators.
	Shader grav_gen_shader;
	//The shader use for draw knifes.
	Shader knife_shader;
	//The shader use for draw lasers.
	Shader laser_shader;
	//The shader use for draw mega lasers.
	Shader mega_laser_shader;
	//The shader use for draw particles.
	Shader particle_shader;
	//The shader use for draw portals.
	Shader portal_shader;
	//The shader use for draw pilots.
	Shader pilot_shader;
	//The shader use for draw shipd.
	Shader ship_shader;
	//The shader use for draw turrets.
	Shader turret_shader;

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

	Texture2D annih_area_gen_basic_texture;

	Texture2D asteroid_small_texture;
	Texture2D asteroid_medium_texture;
	Texture2D asteroid_large_texture;
	Texture2D asteroid_sublimation_texture;

	Texture2D bonus_bomb_texture;
	Texture2D bonus_knife_texture;
	Texture2D bonus_laser_texture;
	Texture2D bonus_loop_texture;
	Texture2D bonus_shield_texture;
	Texture2D bonus_stream_texture;
	Texture2D bonus_triple_texture;
	Texture2D bonus_revers_texture;

	Texture2D bomb_basic_texture;
	Texture2D bomb_lighting_texture;

	Texture2D bullet_small_texture;
	Texture2D bullet_medium_texture;
	Texture2D bullet_large_texture;

	Texture2D pilot_basic_texture;

	Texture2D ship_basic_texture;
	Texture2D ship_triple_texture;

	Texture2D turret_basic_texture;

	//textures

	OpenGL(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share, GLFWframebuffersizefun Function, GLFWwindow** window);

	//Callback functions

	void CallMenuFunction(Menu* menu, const Vec2D* glob_clk_pos, uint8_t clk_status);
	void CallControleMenuFunction(Menu* menu, const Vec2D* glob_clk_pos, uint8_t clk_status);
	void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	void LimitMenuPosition(Menu* menu);
	void ProcessInput(GLFWwindow* window);
	void FirstUpdatePlayersFlags(GameTypes::players_count_t player);
	void SecondUpdatePlayersFlags(GameTypes::players_count_t player);

	//Callback functions

	//Draw functions

	//The function is main draw function and called every draw cycle.
	void DrawFrame();

	// Debug function draw a segment
	void DrawObject(const Segment& segment, bool update_shader = false);

	//The function draw annihilation area generator..
	void DrawObject(const AnnihAreaGen& annih_area_gen, bool update_shader = false);
	//The function draw asteroid.
	void DrawObject(const Asteroid& asteroid, bool update_shader = false);
	//The function draw asteroid.
	void DrawObject(const Bomb& bomb, bool update_shader = false);
	//The function draw bonus.
	void DrawObject(const Bonus& bonus, bool update_shader = false);
	//The function draw bullet.
	void DrawObject(const Bullet& bullet, bool update_shader = false);
	//The function draw deceleration area.
	void DrawObject(const DecelerationArea& deceler_area, bool update_shader = false);
	//The function draw dynamic particle.
	void DrawObject(const DynamicParticle& dynamic_particle, bool update_shader = false);
	//The function draw gravity generator.
	void DrawObject(const GravGen& grav_gen, bool update_shader = false);
	//The function draw knife.
	void DrawObject(const Knife& knife, bool update_shader = false);
	//The function draw laser.
	void DrawObject(const Laser& mega_laser, bool update_shader = false);
	//The function draw particle.
	void DrawObject(const Particle& particle, bool update_shader = false);
	//The function draw portal.
	void DrawObject(const Portal& portal, bool update_shader = false);
	//The function draw pilot.
	void DrawObject(const Pilot& pilot, bool update_shader = false);
	//The function draw ship.
	void DrawObject(const Ship& ship, bool update_shader = false);
	//The function draw turret.
	void DrawObject(const Turret& turret, bool update_shader = false);

	//The function draw map's rectangle.
	void DrawObject(const Map::Rectangle* rectangle, bool update_shader = false);
	//The function draw map's cyrcle.
	void DrawObject(const Map::Cyrcle* cyrcle, bool update_shader = false);
	//The function draw map's polygon.
	void DrawObject(const Map::Polygon* polygon, bool update_shader = false);

	//The function draw menu's button.
	void DrawObject(const Button& button, bool update_shader = false);

	template<typename Object_T, typename counter_t >
	void DrawObjects(const std::vector<Object_T>* objects, const counter_t* objects_count);

	//Draw functions

	//Multydraw functions

	//The function draw all annihilation area generators on the map.
	void DrawAnnihAreaGens();
	//The function draw all asteroids on the map.
	void DrawAsteroids();
	//The function draw all bombs on the map.
	void DrawBombs();
	//The function draw all bonuses on the map.
	void DrawBonuses();
	//The function draw all bullets on the map.
	void DrawBullets();
	//The function draw all gravity generators on the map.
	void DrawGravityGenerators();
	//The function draw all deceleration areas on the map.
	void DrawDecelerationAreas();
	//The function draw all dynamic particles on the map.
	void DrawDynamicParticles();
	//The function draw all knifes on the map.
	void DrawKnifes();
	//The function draw all lasers on the map.
	void DrawLasers();
	//The function draw all particles on the map.
	void DrawParticles();
	//The function draw all portals on the map.
	void DrawPortals();
	//The function draw all pilots on the map.
	void DrawPilots();
	//The function draw all ships on the map.
	void DrawShips();
	//The function draw all turrets on the map.
	void DrawTurrets();

	//The function draww current map.
	void DrawCurrentMap();
	//The function draw current menu.
	void DrawCurrentMenu();
	//The function draww indicated map.
	void DrawIndicatedMap(const Map::MapData* map);
	//The function draw indicated menu.
	void DrawIndicatedMenu(const Menu* menu);

	//Multydraw functions

	//Get data functions

	bool CanDrawFrame(GLFWwindow* window);
	float GetScale();

	//Get data functions

	//Initialisations functions

	GLFWwindow* CreateWindows(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share, GLFWframebuffersizefun Function);

	void InitBuffers();
	void InitGlad();
	void InitMemory();
	void InitOpenGL();
	void InitShaders();
	void InitTextures();

	//Initialisations functions

	//destructor and memory free

	void Free();
	void FreeBuffers();
	void FreeMemory();
	void FreeShaders();
	void FreeTextures();
	~OpenGL();

	//destructor and memory free

	//game pointers

	bool* game_p__play_match;
	bool* game_p__play_round;
	const bool* game_p__flag_all_entities_initialisate;
	const bool* game_p__flag_round_results;

	const GameTypes::tic_t* game_p__global_timer;
	const GameTypes::tic_t* game_p__stuning_timer;

	//Count of annihilation area generators on the map.
	const GameTypes::entities_count_t* game_p__annih_area_gens_count;
	//Count of asteroids on the map.
	const GameTypes::entities_count_t* game_p__asteroids_count;
	//Count of bombs on the map.
	const GameTypes::entities_count_t* game_p__bombs_count;
	//Count of bonuses on the map.
	const GameTypes::entities_count_t* game_p__bonuses_count;
	//Count of bullets on the map.
	const GameTypes::entities_count_t* game_p__bullets_count;
	//Count of deceleration areas on the map.
	const GameTypes::map_elements_count_t* game_p__deceler_areas_count;
	//Count of particles on the map.
	const GameTypes::entities_count_t* game_p__dynamic_particles_count;
	//Count of gravity generators on the map.
	const GameTypes::map_elements_count_t* game_p__grav_gens_count;
	//Count of knifes on the map.
	const GameTypes::entities_count_t* game_p__knifes_count;
	//Count of lazers on the map.
	const GameTypes::entities_count_t* game_p__lasers_count;
	//Count of map's lazers on the map.
	const GameTypes::map_elements_count_t* game_p__mega_lasers_count;
	//Count of particles on the map.
	const GameTypes::entities_count_t* game_p__particles_count;
	//Count of portals on the map.
	const GameTypes::map_elements_count_t* game_p__portals_count;
	//Count of pilots on the map.
	const GameTypes::players_count_t* game_p__pilots_count;
	//Count of players on the map.
	const GameTypes::players_count_t* game_p__players_count;
	//Count of ships on the map.
	const GameTypes::players_count_t* game_p__ships_count;
	//Count of turrets on the map.
	const GameTypes::map_elements_count_t* game_p__turrets_count;

	//Array of annihilation area generators.
	const std::vector<AnnihAreaGen>* game_p__annih_area_gens;
	//Array of asteroids.
	const std::vector<Asteroid>* game_p__asteroids;
	//Array of bombs.
	const std::vector<Bomb>* game_p__bombs;
	//Array of bonuses.
	const std::vector<Bonus>* game_p__bonuses;
	//Array of bullets.
	const std::vector<Bullet>* game_p__bullets;
	//Array of particles.
	const std::vector<DynamicParticle>* game_p__dynamic_particles;
	//Array of knifes.
	const std::vector<Knife>* game_p__knifes;
	//Array of lazers.
	const std::vector<Laser>* game_p__lasers;
	//Array of pilots.
	const std::vector<Pilot>* game_p__pilots;
	//Array of particles.
	const std::vector<Particle>* game_p__particles;
	//Array of ships
	const std::vector<Ship>* game_p__ships;

	//Object stores data about map on current level.
	const Map::MapData* game_p__map;
	//Array of deceleration_areas.
	const std::vector<DecelerationArea>* game_p__deceler_areas;
	//Array of gravity generators.
	const std::vector<GravGen>* game_p__grav_gens;
	//Array of portals
	const std::vector<Portal>* game_p__portals;
	//Array of turtets.
	const std::vector<Turret>* game_p__turrets;

	Menu** game_p__current_active_menu;

	const Menu* game_p__bonus_pull_menu;
	const Menu* game_p__main_menu;
	const Menu* game_p__map_pull_select_menu;
	const Menu* game_p__option_menu;
	const Menu* game_p__pause_menu;
	const Menu* game_p__ships_select_menu;
	const Menu* game_p__ships_control_menu;
	const Menu* game_p__spawning_objects_select_menu;

	Camera* game_p__camera;

	const bool* game_p__rotation_inverse;
	std::vector<GameTypes::tic_t>* game_p__double_clk_timers;
	GameTypes::control_flags_t* game_p__control_flags;

	std::shared_mutex* game_p__annih_area_gens_array_mtx;
	std::shared_mutex* game_p__asteroids_array_mtx;
	std::shared_mutex* game_p__bombs_array_mtx;
	std::shared_mutex* game_p__bonuses_array_mtx;
	std::shared_mutex* game_p__bullets_array_mtx;
	std::shared_mutex* game_p__camera_data_mtx;
	std::shared_mutex* game_p__deceler_areas_array_mtx;
	std::shared_mutex* game_p__dynamic_particles_array_mtx;
	std::shared_mutex* game_p__grav_gens_array_mtx;
	std::shared_mutex* game_p__input_values_mtx;
	std::shared_mutex* game_p__knifes_array_mtx;
	std::shared_mutex* game_p__lasers_array_mtx;
	std::shared_mutex* game_p__log_data_mtx;
	std::shared_mutex* game_p__map_data_mtx;
	std::shared_mutex* game_p__particles_array_mtx;
	std::shared_mutex* game_p__portals_array_mtx;
	std::shared_mutex* game_p__pilots_array_mtx;
	std::shared_mutex* game_p__ships_array_mtx;
	std::shared_mutex* game_p__turrets_array_mtx;

	//menu functions pointers

	MenuFunctions* object_p__menu_functions;

};