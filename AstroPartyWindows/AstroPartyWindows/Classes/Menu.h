#pragma once

#include <iostream>

#include "Math.h"

#include "../Types/AllTypes.h"
#include "../Constants/AllConstants.h"

class Button;
class ControledButton;
class Menu;

class Button
{
#define BUTTON_DEFAULT_TEXT_SIZE 6
protected:
	Vec2F* position;
	Vec2F* size;
	ClassTypes::Button::button_text_t* text;
	ClassTypes::Button::text_length_t text_length;
	ClassTypes::Button::button_id_t id;
public:
	uint8_t text_size;
	ClassTypes::Button::button_status_t status;

	Button();
	Button(const Button& button);
	Button(
		ClassTypes::Button::button_id_t id,
		Vec2F* position,
		Vec2F* size,
		const char* text = "",
		ClassTypes::Button::text_size_t text_size = BUTTON_DEFAULT_TEXT_SIZE);

	ClassTypes::Button::button_id_t GetId();
	Vec2F GetPosition();
	Vec2F GetSize();
	bool GetStatus(ClassTypes::Button::button_status_t status_mask);
	ClassTypes::Button::button_text_t* GetText();
	ClassTypes::Button::text_length_t GetTextLength();
	bool HavePoint(Vec2F* point);
	void Move(Vec2F* move_vector);
	void Set(Button* button);
	void Set(
		ClassTypes::Button::button_id_t id, 
		Vec2F* position, 
		Vec2F* size, 
		const char* text = "",
		ClassTypes::Button::text_size_t text_size = BUTTON_DEFAULT_TEXT_SIZE,
		ClassTypes::Button::button_status_t status = BUTTON_STATUS_INACTIVE);
	void SetId(ClassTypes::Button::button_id_t id);
	void SetOnlyCustomStatus(ClassTypes::Button::button_status_t status_mask);
	void SetPosition(Vec2F* position);
	void SetSize(Vec2F* size);
	void SetStatus(ClassTypes::Button::button_status_t status_mask, bool value);
	void SetText(const char* text);
	void SetTextSize(ClassTypes::Button::text_size_t text_size);
	//not change id and statuse
	void TakeData(Button* button);

	void operator=(Button button);

	~Button();
};

class Menu
{
protected:
	Vec2F* position;
	Vec2F* size;
	Button* default_buttons;
	ClassTypes::Menu::buttons_count_t buttons_count;
public:
	Button* current_buttons;
	Menu();
	Menu(
		Vec2F* position, 
		Vec2F* size,
		Button* buttons = nullptr,
		ClassTypes::Menu::buttons_count_t buttons_count = 0);

	ClassTypes::Menu::buttons_count_t GetButtonsCount();
	Vec2F GetPosition();
	Vec2F GetSize();
	void Move(Vec2F* move_vector);
	void Set(Menu* menu);
	void Set(
		Vec2F* position,
		Vec2F* size,
		Button* buttons = nullptr,
		ClassTypes::Menu::buttons_count_t buttons_count = 0);
	void SetPosition(Vec2F* position);
	void Recalculate();

	~Menu();
};