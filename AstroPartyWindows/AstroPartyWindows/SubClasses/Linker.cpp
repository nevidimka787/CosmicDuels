#include "Linker.h"

Linker::Linker(Game* game_object, MenuFunctions* menu_functions_object, OpenGL* open_gl_object)
{
	//game object

	game_object->object_p__menu_functions = menu_functions_object;

	game_object->menu_p__ships_select_buttons = menu_functions_object->ships_select_buttons;
	game_object->menu_p__start_bonus = &menu_functions_object->start_bonus;

	//game object

	//menu functions object

	menu_functions_object->game_p__current_active_menu = &game_object->current_active_menu;
	menu_functions_object->game_p__game_rules = &game_object->game_rules;
	menu_functions_object->game_p__main_menu = &game_object->main_menu;
	menu_functions_object->game_p__map_pull_array = &game_object->map_pull_array;
	menu_functions_object->game_p__map_pull_select_menu = &game_object->map_pull_select_menu;
	menu_functions_object->game_p__object_pull_array = &game_object->object_pull_array;
	menu_functions_object->game_p__option_menu = &game_object->option_menu;
	menu_functions_object->game_p__pause_game = &game_object->pause_game;
	menu_functions_object->game_p__pause_menu = &game_object->pause_menu;
	menu_functions_object->game_p__ships_control_menu = &game_object->ships_control_menu;
	menu_functions_object->game_p__ships_select_menu = &game_object->ships_select_menu;
	menu_functions_object->game_p__spawning_objects_select_menu = &game_object->spawning_objects_select_menu;
	menu_functions_object->game_p__start_game = &game_object->start_game;
	menu_functions_object->game_p__teams = &game_object->teams;

	//menu functions object

	//open gl object

	open_gl_object->object_p__menu_functions = menu_functions_object;

	open_gl_object->game_p__asteroids = &game_object->asteroids;
	open_gl_object->game_p__asteroids_count = &game_object->asteroids_count;
	open_gl_object->game_p__bombs = &game_object->bombs;
	open_gl_object->game_p__bombs_count = &game_object->bombs_count;
	open_gl_object->game_p__bonuses = &game_object->bonuses;
	open_gl_object->game_p__bonuses_count = &game_object->bonuses_count;
	open_gl_object->game_p__bullets = &game_object->bullets;
	open_gl_object->game_p__bullets_count = &game_object->bullets_count;
	open_gl_object->game_p__camera = &game_object->camera;
	open_gl_object->game_p__current_active_menu = &game_object->current_active_menu;
	open_gl_object->game_p__flag_all_entities_initialisate = &game_object->flag_all_entities_initialisate;
	open_gl_object->game_p__grav_gens = &game_object->grav_gens;
	open_gl_object->game_p__grav_gens_count = &game_object->grav_gens_count;
	open_gl_object->game_p__knifes = &game_object->knifes;
	open_gl_object->game_p__knifes_count = &game_object->knifes_count;
	open_gl_object->game_p__lasers = &game_object->lasers;
	open_gl_object->game_p__lasers_count = &game_object->lasers_count;
	open_gl_object->game_p__main_menu = &game_object->main_menu;
	open_gl_object->game_p__map = &game_object->map;
	open_gl_object->game_p__map_pull_select_menu = &game_object->map_pull_select_menu;
	open_gl_object->game_p__mega_lasers = &game_object->mega_lasers;
	open_gl_object->game_p__mega_lasers_count = &game_object->mega_lasers_count;
	open_gl_object->game_p__option_menu = &game_object->option_menu;
	open_gl_object->game_p__particles = &game_object->particles;
	open_gl_object->game_p__particles_count = &game_object->particles_count;
	open_gl_object->game_p__pause_menu = &game_object->pause_menu;
	open_gl_object->game_p__pilots = &game_object->pilots;
	open_gl_object->game_p__pilots_count = &game_object->pilots_count;
	open_gl_object->game_p__players_count = &game_object->players_count;
	open_gl_object->game_p__rectangles = &game_object->rectangles;
	open_gl_object->game_p__rectangles_count = &game_object->rectangles_count;
	open_gl_object->game_p__ships = &game_object->ships;
	open_gl_object->game_p__ships_control_menu = &game_object->ships_control_menu;
	open_gl_object->game_p__ships_count = &game_object->ships_count;
	open_gl_object->game_p__ships_select_menu = &game_object->ships_select_menu;
	open_gl_object->game_p__start_game = &game_object->start_game;
	open_gl_object->game_p__spawning_objects_select_menu = &game_object->spawning_objects_select_menu;
	open_gl_object->game_p__turels = &game_object->turels;
	open_gl_object->game_p__turels_count = &game_object->turels_count;

	//open gl object
}

Linker::~Linker()
{

}