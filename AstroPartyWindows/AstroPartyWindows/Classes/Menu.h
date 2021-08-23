#pragma once

#include "Vec.h"
#include <iostream>
#include "../Types/ClassesTypes.h"
#include "../Constants/ClassesConstants.h"


class Button;
class Menu;

class Button
{
protected:
	Vec2F* position;
	Vec2F* size;
	ClassTypes::Button::button_text_t* text;
	uint16_t text_length;
	ClassTypes::Button::button_id_t id;
public:
	uint8_t text_size;
	ClassTypes::Button::button_status_t status;

	Button();
	Button(ClassTypes::Button::button_id_t id, Vec2F* position, Vec2F* size, const char* text, uint8_t text_size);

	ClassTypes::Button::button_id_t GetId();
	Vec2F GetPosition();
	Vec2F GetSize();
	bool GetStatus(ClassTypes::Button::button_status_t status_mask);
	ClassTypes::Button::button_text_t* GetText();
	uint16_t GetTextLength();
	bool HavePoint(Vec2F* point);
	void Move(Vec2F* move_vector);
	void Set(Button* button);
	void Set(ClassTypes::Button::button_id_t id, Vec2F* position, Vec2F* size, const char* text, uint8_t text_size);
	void Set(ClassTypes::Button::button_id_t id, Vec2F* position, Vec2F* size, const char* text, uint8_t text_size, ClassTypes::Button::button_status_t status);
	void SetId(ClassTypes::Button::button_id_t id);
	void SetPosition(Vec2F* position);
	void SetSize(Vec2F* size);
	void SetStatus(ClassTypes::Button::button_status_t status_mask, bool value);
	void SetText(const char* text);
	void SetText(const char* text, uint8_t text_size);
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
	Menu(Vec2F* position, Vec2F* size, Button* buttons, ClassTypes::Menu::buttons_count_t buttons_count);

	ClassTypes::Menu::buttons_count_t GetButtonsCount();
	Vec2F GetPosition();
	Vec2F GetSize();
	void Move(Vec2F* move_vector);
	void Set(Menu* menu);
	void SetPosition(Vec2F* position);
	void Recalculate();

	~Menu();
};