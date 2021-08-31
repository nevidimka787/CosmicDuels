#pragma once

#include <iostream>

#include "Math.h"

#include "../Types/AllTypes.h"
#include "../Constants/AllConstants.h"

class Button;
class Menu;

class Button
{
#define BUTTON_DEFAULT_TEXT_SIZE 6
protected:
	Vec2F position;
	Vec2F size;
	EngineTypes::Button::text_t* text;
	EngineTypes::Button::text_length_t text_length;
	EngineTypes::Button::button_id_t id;
public:
	uint8_t text_size;
	EngineTypes::Button::button_status_t status;

	Button();
	Button(const Button& button);
	Button(
		EngineTypes::Button::button_id_t id,
		Vec2F* position,
		Vec2F* size,
		const char* text = "",
		EngineTypes::Button::text_size_t text_size = BUTTON_DEFAULT_TEXT_SIZE);

	EngineTypes::Button::button_id_t GetId();
	Vec2F GetPosition();
	Vec2F GetSize();
	bool GetStatus(EngineTypes::Button::button_status_t status_mask);
	EngineTypes::Button::text_t* GetText();
	EngineTypes::Button::text_length_t GetTextLength();
	bool HavePoint(Vec2F* point);
	void Move(Vec2F* move_vector);
	void Set(Button* button);
	void Set(
		EngineTypes::Button::button_id_t id, 
		Vec2F* position, 
		Vec2F* size, 
		const char* text = "",
		EngineTypes::Button::text_size_t text_size = BUTTON_DEFAULT_TEXT_SIZE,
		EngineTypes::Button::button_status_t status = BUTTON_STATUS_INACTIVE);
	void SetId(EngineTypes::Button::button_id_t id);
	void SetOnlyCustomStatus(EngineTypes::Button::button_status_t status_mask);
	void SetPosition(Vec2F* position);
	void SetSize(Vec2F* size);
	void SetStatus(EngineTypes::Button::button_status_t status_mask, bool value);
	void SetText(const char* text);
	void SetText(char* text, EngineTypes::Button::text_length_t text_length);
	void SetText(EngineTypes::Button::text_t* text);
	void SetText(EngineTypes::Button::text_t* text, EngineTypes::Button::text_length_t text_length);
	//not change id and statuse
	void TakeData(Button* button);

	void operator=(Button button);

	~Button();
};

class Menu
{
protected:
	Vec2F position;
	Vec2F size;
	Button* default_buttons;
	EngineTypes::Menu::buttons_count_t buttons_count;
public:
	Button* current_buttons;
	Menu();
	Menu(const Menu& menu);
	Menu(
		Vec2F* position, 
		Vec2F* size,
		Button* buttons = nullptr,
		EngineTypes::Menu::buttons_count_t buttons_count = 0);

	void AddButton(EngineTypes::Menu::buttons_count_t button_number, Button* button);
	void DeleteButton(EngineTypes::Menu::buttons_count_t button_number);
	EngineTypes::Menu::buttons_count_t GetButtonsCount();
	Vec2F GetPosition();
	Vec2F GetSize();
	void HardRecalculate();
	void Move(Vec2F* move_vector);
	void Recalculate();
	void Set(Menu* menu);
	void Set(
		Vec2F* position,
		Vec2F* size,
		Button* buttons = nullptr,
		EngineTypes::Menu::buttons_count_t buttons_count = 0);
	void SetPosition(Vec2F* position);
	void UpdateDefaultButtons();

	~Menu();
};