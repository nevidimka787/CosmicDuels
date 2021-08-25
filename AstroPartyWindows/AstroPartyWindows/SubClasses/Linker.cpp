#include "Linker.h"

Linker::Linker(Game* game_object, MenuFunctions* menu_functions_object, OpenGL* open_gl_object)
{
	//game object

	game_object->object_p__menu_functions = menu_functions_object;

	game_object->menu_p__ships_select_buttons = menu_functions_object->ships_select_buttons;
	game_object->menu_p__start_bonus = &menu_functions_object->start_bonus;

	game_object->open_gl_p__basic_square = &open_gl_object->basic_square;
	game_object->open_gl_p__basic_square_vertex = &open_gl_object->basic_square;
	game_object->open_gl_p__basic_triangle = &open_gl_object->basic_triangle;
	game_object->open_gl_p__basic_triangle_vertex = &open_gl_object->basic_triangle_buffer;
	game_object->open_gl_p__long_triangle = &open_gl_object->long_triangle;
	game_object->open_gl_p__long_triangle_vertex = &open_gl_object->long_triangle_buffer;

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

	open_gl_object->game_p__current_active_menu = &game_object->current_active_menu;
	open_gl_object->game_p__flag_all_entities_initialisate = &game_object->flag_all_entities_initialisate;
	open_gl_object->game_p__main_menu = &game_object->main_menu;
	open_gl_object->game_p__map_pull_select_menu = &game_object->map_pull_select_menu;
	open_gl_object->game_p__option_menu = &game_object->option_menu;
	open_gl_object->game_p__pause_menu = &game_object->pause_menu;
	open_gl_object->game_p__start_game = &game_object->start_game;
	open_gl_object->game_p__ships = &game_object->ships;
	open_gl_object->game_p__ships_control_menu = &game_object->ships_control_menu;
	open_gl_object->game_p__ships_select_menu = &game_object->ships_select_menu;
	open_gl_object->game_p__spawning_objects_select_menu = &game_object->spawning_objects_select_menu;

	//open gl object
}

Linker::~Linker()
{

}