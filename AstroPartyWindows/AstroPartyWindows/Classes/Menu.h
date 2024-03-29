#pragma once

#include <iostream>

#include "Math.h"

#include "../Types/AllTypes.h"
#include "../Constants/AllConstants.h"

#include <vector>

class Button;
class Menu;
class Area;

class Area
{
private:
	std::vector<Vec2F> points;
public:
	Area();
	Area(const Area& area);
	Area(const std::vector<Vec2F>& points);

	EngineTypes::Area::points_count_t GetPointsCount() const;
	bool HavePointInside(const Vec2F& point) const;
	bool HavePointInside(const Vec2F* point) const;
	const std::vector<Vec2F>& GetArrayP() const;
	void Set(const Area* area);
	void Set(const std::vector<Vec2F>& points);

	void operator=(const Area& area);

	~Area();
};

class Button
{
#define BUTTON_DEFAULT_TEXT_SIZE 6
protected:
	Vec2F position;
	Vec2F size;
	Area area;
	std::string text;
	EngineTypes::Button::id_t id;
	uint8_t text_size;
	EngineTypes::Button::status_t status;
public:

	Button();
	Button(const Button& button);
	Button(
		EngineTypes::Button::id_t id,
		const Vec2F& position,
		const Vec2F& size,
		const Area& area,
		const std::string& text = "",
		EngineTypes::Button::text_size_t text_size = BUTTON_DEFAULT_TEXT_SIZE,
		EngineTypes::Button::status_t status = BUTTON_STATUS_FALSE);

	const Area* GetAreaP() const;
	EngineTypes::Button::id_t GetId() const;
	Vec2F GetPosition() const;
	Vec2F GetSize() const;
	EngineTypes::Button::status_t GetStatus() const;
	bool GetStatus(EngineTypes::Button::status_t status_mask) const;
	//Return pointer to memory space this text.
	std::string GetText() const;
	EngineTypes::Button::text_length_t GetTextLength() const;
	EngineTypes::Button::text_size_t GetTexSize() const;
	bool HavePoint(const Vec2F& point) const;
	void Move(const Vec2F& move_vector);
	void Set(
		EngineTypes::Button::id_t id,
		const Vec2F& position,
		const Vec2F& size,
		const Area& area,
		const std::string& text = "",
		EngineTypes::Button::text_size_t text_size = BUTTON_DEFAULT_TEXT_SIZE,
		EngineTypes::Button::status_t status = BUTTON_STATUS_FALSE);
	void SetArea(const Area& area);
	void SetId(EngineTypes::Button::id_t id);
	void SetOnlyCustomStatus(EngineTypes::Button::status_t status_mask);
	void SetPosition(const Vec2F& position);
	void SetSize(const Vec2F& size);
	void SetStatus(EngineTypes::Button::status_t status_mask, bool value);
	void SetText(const std::string& text);
	void SetTextSize(EngineTypes::Button::text_size_t text_size);
	//not change id and statuse
	void TakeData(const Button& button);

	void operator=(const Button& button);

	~Button();
};

class Menu
{
protected:
	Vec2F position;
	Vec2F size;
	std::vector<Button> default_buttons;
public:
	std::vector<Button> current_buttons;
	Menu();
	Menu(const Menu& menu);
	Menu(
		const Vec2F* position, 
		const Vec2F* size,
		const std::vector<Button>& buttons);

	void AddButton(EngineTypes::Menu::buttons_count_t button_number, const Button& button);
	//Delete all buttons from the menu.
	void Clear();
	void DeleteButton(EngineTypes::Menu::buttons_count_t button_number);
	EngineTypes::Menu::buttons_count_t GetButtonsCount() const;
	Vec2F GetPosition() const;
	Vec2F GetSize() const;
	void HardRecalculate();
	void Move(const Vec2F& move_vector);
	void Recalculate();
	void Set(
		const Vec2F& position,
		const Vec2F& size,
		const std::vector<Button>& buttons);
	void SetPosition(const Vec2F& position);
	void UpdateDefaultButtons();

	void operator=(const Menu& menu);

	~Menu();
};