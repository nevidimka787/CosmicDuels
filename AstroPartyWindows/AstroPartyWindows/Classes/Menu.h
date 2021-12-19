#pragma once

#include <iostream>

#include "Math.h"

#include "../Types/AllTypes.h"
#include "../Constants/AllConstants.h"

class Button;
class Menu;
class Area;

class Area
{
private:
	Vec2F* points;
	EngineTypes::Area::points_count_t points_count;

	Vec2F horisontal_vector;
	EngineTypes::Area::points_count_t intersections_count;
public:
	Area();
	Area(const Area& area);
	Area(Vec2F* points, EngineTypes::Area::points_count_t points_count);

	bool HavePointInside(Vec2F* point) const;
	void Set(const Area* area);
	void Set(const Vec2F* points, EngineTypes::Area::points_count_t points_count);

	~Area();
};

class Button
{
#define BUTTON_DEFAULT_TEXT_SIZE 6
protected:
	Vec2F position;
	Vec2F size;
	Area area;
	EngineTypes::Button::text_t* text;
	EngineTypes::Button::text_length_t text_length;
	EngineTypes::Button::id_t id;
public:
	uint8_t text_size;
	EngineTypes::Button::status_t status;

	Button();
	Button(const Button& button);
	Button(
		EngineTypes::Button::id_t id,
		const Vec2F* position,
		const Vec2F* size,
		const Area* area,
		const char* text = "",
		EngineTypes::Button::text_size_t text_size = BUTTON_DEFAULT_TEXT_SIZE,
		EngineTypes::Button::status_t status = BUTTON_STATUS_FALSE);

	EngineTypes::Button::id_t GetId() const;
	Vec2F GetPosition() const;
	Vec2F GetSize() const;
	bool GetStatus(EngineTypes::Button::status_t status_mask) const;
	//Return pointer to memory space this text.
	EngineTypes::Button::text_t* GetText() const;
	//Create a new memory space and return pointer to it.
	char* GetTextC() const;
	//Create a new memory space and return pointer to it.
	EngineTypes::Button::text_t* GetTextU() const;
	EngineTypes::Button::text_length_t GetTextLength() const;
	bool HavePoint(const Vec2F* point) const;
	void Move(const Vec2F* move_vector);
	void Set(const Button* button);
	void Set(
		EngineTypes::Button::id_t id, 
		Vec2F* position, 
		Vec2F* size,
		Area* area,
		const char* text = "",
		EngineTypes::Button::text_size_t text_size = BUTTON_DEFAULT_TEXT_SIZE,
		EngineTypes::Button::status_t status = BUTTON_STATUS_FALSE);
	void SetArea(Area* area);
	void SetId(EngineTypes::Button::id_t id);
	void SetOnlyCustomStatus(EngineTypes::Button::status_t status_mask);
	void SetPosition(Vec2F* position);
	void SetSize(Vec2F* size);
	void SetStatus(EngineTypes::Button::status_t status_mask, bool value);
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
	EngineTypes::Menu::buttons_count_t GetButtonsCount() const;
	Vec2F GetPosition() const;
	Vec2F GetSize() const;
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