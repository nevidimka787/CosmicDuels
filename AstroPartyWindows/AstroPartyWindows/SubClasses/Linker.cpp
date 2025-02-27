#include "Linker.h"

Linker::Linker(Game* game_object, MenuFunctions* menu_functions_object, OpenGL* open_gl_object)
{
	//game object

	game_object->object_p__menu_functions = menu_functions_object;

	game_object->menu_p__ships_select_buttons = menu_functions_object->ships_select_buttons;
	game_object->menu_p__start_bonus = &menu_functions_object->start_bonus;

	game_object->object_p__open_gl_realisation = open_gl_object;

	//game object

	//menu functions object

	menu_functions_object->game_p__bonus_pull_array = &game_object->bonus_pull_array;
	menu_functions_object->game_p__bonus_pull_select_menu = &game_object->bonus_pull_select_menu;
	menu_functions_object->game_p__credits_menu = &game_object->credits_menu;
	menu_functions_object->game_p__current_active_menu = &game_object->current_active_menu;
	menu_functions_object->game_p__flag_end_match = &game_object->flag_end_match;
	menu_functions_object->game_p__game_rules = &game_object->game_rules;
	menu_functions_object->game_p__main_menu = &game_object->main_menu;
	menu_functions_object->game_p__map_pull_array = &game_object->map_pull_array;
	menu_functions_object->game_p__map_pull_select_menu = &game_object->map_pull_select_menu;
	menu_functions_object->game_p__object_pull_array = &game_object->object_pull_array;
	menu_functions_object->game_p__option_menu = &game_object->option_menu;
	menu_functions_object->game_p__pause_round = &game_object->pause_round;
	menu_functions_object->game_p__pause_menu = &game_object->pause_menu;
	menu_functions_object->game_p__ships_control_menu = &game_object->ships_control_menu;
	menu_functions_object->game_p__ships_select_menu = &game_object->ships_select_menu;
	menu_functions_object->game_p__objects_pull_menu = &game_object->objects_pull_menu;
	menu_functions_object->game_p__play_match = &game_object->play_match;
	menu_functions_object->game_p__play_round = &game_object->play_round;
	menu_functions_object->game_p__teams = &game_object->teams;

	//menu functions object

	//open gl object

	open_gl_object->object_p__menu_functions = menu_functions_object;

	open_gl_object->game_p__annih_area_gens = &game_object->annih_area_gens;
	open_gl_object->game_p__annih_area_gens_array_mtx = &game_object->annih_area_gens_array_mtx;
	open_gl_object->game_p__annih_area_gens_count = &game_object->annih_area_gens_count;
	open_gl_object->game_p__asteroids = &game_object->asteroids;
	open_gl_object->game_p__asteroids_array_mtx = &game_object->asteroids_array_mtx;
	open_gl_object->game_p__asteroids_count = &game_object->asteroids_count;
	open_gl_object->game_p__bombs = &game_object->bombs;
	open_gl_object->game_p__bombs_array_mtx = &game_object->bombs_array_mtx;
	open_gl_object->game_p__bombs_count = &game_object->bombs_count;
	open_gl_object->game_p__bonus_pull_menu = &game_object->bonus_pull_select_menu;
	open_gl_object->game_p__bonuses = &game_object->bonuses;
	open_gl_object->game_p__bonuses_array_mtx = &game_object->bonuses_array_mtx;
	open_gl_object->game_p__bonuses_count = &game_object->bonuses_count;
	open_gl_object->game_p__bullets = &game_object->bullets;
	open_gl_object->game_p__bullets_array_mtx = &game_object->bullets_array_mtx;
	open_gl_object->game_p__bullets_count = &game_object->bullets_count;
	open_gl_object->game_p__double_clk_timers = &game_object->double_clk_timers;
	open_gl_object->game_p__control_flags = &game_object->control_flags;
	open_gl_object->game_p__camera = &game_object->camera;
	open_gl_object->game_p__camera_data_mtx = &game_object->camera_data_mtx;
	open_gl_object->game_p__current_active_menu = &game_object->current_active_menu;
	open_gl_object->game_p__deceler_areas = &game_object->deceler_areas;
	open_gl_object->game_p__deceler_areas_array_mtx = &game_object->deceler_areas_array_mtx;
	open_gl_object->game_p__deceler_areas_count = &game_object->deceler_areas_count;
	open_gl_object->game_p__dynamic_particles = &game_object->dynamic_particles;
	open_gl_object->game_p__dynamic_particles_array_mtx = &game_object->dynamic_particles_array_mtx;
	open_gl_object->game_p__dynamic_particles_count = &game_object->dynamic_particles_count;
	open_gl_object->game_p__flag_all_entities_initialisate = &game_object->flag_all_entities_initialisate;
	open_gl_object->game_p__flag_round_results = &game_object->flag_round_results;
	open_gl_object->game_p__global_timer = &game_object->global_timer;
	open_gl_object->game_p__grav_gens = &game_object->grav_gens;
	open_gl_object->game_p__grav_gens_array_mtx = &game_object->grav_gens_array_mtx;
	open_gl_object->game_p__grav_gens_count = &game_object->grav_gens_count;
	open_gl_object->game_p__knifes = &game_object->knifes;
	open_gl_object->game_p__knifes_array_mtx = &game_object->knifes_array_mtx;
	open_gl_object->game_p__knifes_count = &game_object->knifes_count;
	open_gl_object->game_p__lasers = &game_object->lasers;
	open_gl_object->game_p__lasers_array_mtx = &game_object->lasers_array_mtx;
	open_gl_object->game_p__lasers_count = &game_object->lasers_count;
	open_gl_object->game_p__main_menu = &game_object->main_menu;
	open_gl_object->game_p__map = &game_object->map;
	open_gl_object->game_p__map_data_mtx = &game_object->map_data_mtx;
	open_gl_object->game_p__map_pull_select_menu = &game_object->map_pull_select_menu;
	open_gl_object->game_p__mega_lasers_count = &game_object->mega_lasers_count;
	open_gl_object->game_p__option_menu = &game_object->option_menu;
	open_gl_object->game_p__particles = &game_object->particles;
	open_gl_object->game_p__particles_array_mtx = &game_object->particles_array_mtx;
	open_gl_object->game_p__particles_count = &game_object->particles_count;
	open_gl_object->game_p__pause_menu = &game_object->pause_menu;
	open_gl_object->game_p__portals = &game_object->portals;
	open_gl_object->game_p__portals_array_mtx = &game_object->portals_array_mtx;
	open_gl_object->game_p__portals_count = &game_object->portals_count;
	open_gl_object->game_p__pilots = &game_object->pilots;
	open_gl_object->game_p__pilots_array_mtx = &game_object->pilots_array_mtx;
	open_gl_object->game_p__pilots_count = &game_object->pilots_count;
	open_gl_object->game_p__players_count = &game_object->players_count;
	open_gl_object->game_p__rotation_inverse = &game_object->rotation_inverse;
	open_gl_object->game_p__ships = &game_object->ships;
	open_gl_object->game_p__ships_array_mtx = &game_object->ships_array_mtx;
	open_gl_object->game_p__ships_control_menu = &game_object->ships_control_menu;
	open_gl_object->game_p__ships_count = &game_object->ships_count;
	open_gl_object->game_p__ships_select_menu = &game_object->ships_select_menu;
	open_gl_object->game_p__stuning_timer = &game_object->stuning_timer;
	open_gl_object->game_p__play_match = &game_object->play_match;
	open_gl_object->game_p__play_round = &game_object->play_round;
	open_gl_object->game_p__spawning_objects_select_menu = &game_object->objects_pull_menu;
	open_gl_object->game_p__turrets = &game_object->turrets;
	open_gl_object->game_p__turrets_array_mtx = &game_object->turrets_array_mtx;
	open_gl_object->game_p__turrets_count = &game_object->turrets_count;

	//open gl object
}

Linker::~Linker()
{

}