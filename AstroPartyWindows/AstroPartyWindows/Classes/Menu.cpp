#include "Menu.h"
#include <vector>
#include <string>

#pragma warning(disable : 6011)
#pragma warning(disable : 6308)
#pragma warning(disable : 6386)
#pragma warning(disable : 6385)
#pragma warning(disable : 26451)//All integer operations can be overflow. It is absolutly useless warnintg.
#pragma warning(disable : 28182)

#define HORISONTAL_VECTOR	Vec2F(1.0f, 0.0f)

Area::Area() : points(std::vector<Vec2F>(0)) {}

Area::Area(const Area& area) : points(area.points) {}

Area::Area(const std::vector<Vec2F>& points) : points(points) {}

const std::vector<Vec2F>& Area::GetArrayP() const
{
	return points;
}

EngineTypes::Area::points_count_t Area::GetPointsCount() const
{
	return points.size();
}

bool Area::HavePointInside(const Vec2F& point) const
{
	if (points.size() < 3)
	{
		return false;
	}
	EngineTypes::Area::points_count_t intersections_count = 0;
	const Beam& horisontal_beam = Beam(point, HORISONTAL_VECTOR);
	Segment temp_segment = Segment(points.back(), points.front(), true);
	if (temp_segment.IsIntersection(horisontal_beam))
	{
		++intersections_count;
	}
	for (size_t i = 1; i < points.size(); ++i)
	{
		temp_segment.Set(points[i - 1u], points[i], true);
		if (horisontal_beam.IsIntersection(temp_segment))
		{
			++intersections_count;
		}
	}
	return (intersections_count % 2);
}

bool Area::HavePointInside(const Vec2F* point) const
{
	return HavePointInside(*point);
}

void Area::Set(const Area* area)
{
	points = area->points;
}

void Area::Set(const std::vector<Vec2F>& points)
{
	this->points = points;
}

void Area::operator=(const Area& area)
{
	points = area.points;
}

Area::~Area()
{
}

Button::Button() :
	area(Area()),
	id(0),
	status(BUTTON_STATUS_FALSE),
	text(),
	text_size(0)
{
	text[0] = '\0';
}

Button::Button(const Button& button) :
	area(button.area),
	id(button.id),
	position(button.position),
	size(button.size),
	status(button.status),
	text(button.text),
	text_size(button.text_size)
{
}

Button::Button(
	EngineTypes::Button::id_t id,
	const Vec2F& position,
	const Vec2F& size,
	const Area& area,
	const std::string& text,
	EngineTypes::Button::text_size_t text_size,
	EngineTypes::Button::status_t status)
	:
	area(area),
	id(id),
	position(position),
	size(size),
	status(status),
	text(text),
	text_size(text_size)
{
}

const Area* Button::GetAreaP() const
{
	return &area;
}

EngineTypes::Button::id_t Button::GetId() const
{
	return id;
}

Vec2F Button::GetPosition() const
{
	return position;
}

Vec2F Button::GetSize() const
{
	return size;
}

EngineTypes::Button::status_t Button::GetStatus() const
{
	return status;
}

bool Button::GetStatus(EngineTypes::Button::status_t status_mask) const
{
	return status & status_mask;
}

std::string Button::GetText() const
{
	return text;
}

uint16_t Button::GetTextLength() const
{
	return text.size();
}

EngineTypes::Button::text_size_t Button::GetTexSize() const
{
	return text_size;
}

bool Button::HavePoint(const Vec2F& point) const
{
	Vec2F current_point = point - position;
	current_point.ScaleThis(Vec2F(1.0f / size.x, 1.0f / size.y));
	return area.HavePointInside(current_point);
}

void Button::Move(const Vec2F& move_vector)
{
	position += move_vector;
}

void Button::Set(
	EngineTypes::Button::id_t id,
	const Vec2F& position,
	const Vec2F& size,
	const Area& area,
	const std::string& text,
	EngineTypes::Button::text_size_t text_size,
	EngineTypes::Button::status_t status)
{
	this->area = area;
	this->id = id;
	this->position = position;
	this->size = size;
	this->status = status;
	this->text_size = text_size;

	SetText(text);
}

void Button::SetArea(const Area& area)
{
	this->area = area;
}

void Button::SetId(EngineTypes::Button::id_t id)
{
	this->id = id;
}

void Button::SetOnlyCustomStatus(EngineTypes::Button::status_t status_mask)
{
	status |= status_mask & (BUTTON_STATUS_ALL - (BUTTON_STATUS_TRUE | BUTTON_STATUS_SELECT));
	status &= status_mask | (BUTTON_STATUS_TRUE | BUTTON_STATUS_SELECT);
}

void Button::SetPosition(const Vec2F& position)
{
	this->position = position;
}

void Button::SetSize(const Vec2F& size)
{
	this->size = size;
}

void Button::SetStatus(EngineTypes::Button::status_t status_mask, bool value)
{
	if (value)
	{
		status |= status_mask;
		return;
	}
	status &= BUTTON_STATUS_ALL - status_mask;
}

void Button::SetText(const std::string& text)
{
	this->text = text;
}

void Button::SetTextSize(EngineTypes::Button::text_size_t text_size)
{
	this->text_size = text_size;
}

void Button::TakeData(const Button& button)
{
	position = button.position;
	size = button.size;
	text_size = button.text_size;
	id = button.id;
}

void Button::operator=(const Button& button)
{
	id = button.id;
	position = button.position;
	size = button.size;
	status = button.status;
	text_size = button.text_size;
	area = button.area;

	//SetText(button.text, button.text_length);
}

Button::~Button()
{
}



Menu::Menu() :
	default_buttons(),
	current_buttons()
{
}

Menu::Menu(const Menu& menu):
	current_buttons(menu.current_buttons),
	default_buttons(menu.default_buttons),
	position(menu.position),
	size(menu.size)
{
	HardRecalculate();
}

Menu::Menu(
	const Vec2F* position,
	const Vec2F* size,
	const std::vector<Button>& buttons)
	:
	position(*position),
	size(*size),
	current_buttons(buttons),
	default_buttons(buttons)
{
	HardRecalculate();
}

void Menu::AddButton(EngineTypes::Menu::buttons_count_t button_number, const Button& button)
{
	default_buttons.push_back(button);
	current_buttons.push_back(button);
	if (button_number >= default_buttons.size())
	{
		HardRecalculate();
		return;
	}

	for (EngineTypes::Menu::buttons_count_t i = button_number + 1; i < default_buttons.size(); ++i)
	{
		default_buttons[i] = default_buttons[i - 1];
		current_buttons[i] = current_buttons[i - 1];
	}
	default_buttons[button_number] = button;
	current_buttons[button_number] = button;

	HardRecalculate();
}

void Menu::Clear()
{
	default_buttons.clear();
	current_buttons.clear();
}

void Menu::DeleteButton(EngineTypes::Menu::buttons_count_t button_number)
{
	if (button_number >= default_buttons.size()) return;

	for (EngineTypes::Menu::buttons_count_t i = default_buttons.size() - 1; i > button_number; --i)
	{
		default_buttons[i - 1] = default_buttons[i];
		current_buttons[i - 1] = default_buttons[i];
	}

	default_buttons.pop_back();
	current_buttons.pop_back();

	HardRecalculate();
}

EngineTypes::Menu::buttons_count_t Menu::GetButtonsCount() const
{
	return default_buttons.size();
}

Vec2F Menu::GetPosition() const
{
	return position;
}

Vec2F Menu::GetSize() const
{
	return size;
}

void Menu::HardRecalculate()
{
	for (EngineTypes::Menu::buttons_count_t i = 0; i < default_buttons.size(); i++)
	{
		current_buttons[i] = default_buttons[i];
		current_buttons[i].Move(position);
	}
}

void Menu::Move(const Vec2F& move_vector)
{
	position += move_vector;
	Recalculate();
}

void Menu::Recalculate()
{
	for (EngineTypes::Menu::buttons_count_t i = 0; i < default_buttons.size(); i++)
	{
		current_buttons[i].TakeData(default_buttons[i]);
		current_buttons[i].Move(position);
	}
}

void Menu::Set(
	const Vec2F& position, 
	const Vec2F& size,
	const std::vector<Button>& buttons)
{
	this->position = position;
	this->size = size;

	this->default_buttons = buttons;
	this->current_buttons = buttons;
	HardRecalculate();
}

void Menu::SetPosition(const Vec2F& position)
{
	this->position = position;
	Recalculate();
}

void Menu::UpdateDefaultButtons()
{
	for (EngineTypes::Menu::buttons_count_t i = 0; i < default_buttons.size(); i++)
	{
		default_buttons[i].SetStatus(current_buttons[i].GetStatus(), true);
		default_buttons[i].SetText(std::move(current_buttons[i].GetText()));
		default_buttons[i].SetTextSize(current_buttons[i].GetTexSize());
	}
}

Menu::~Menu()
{
}