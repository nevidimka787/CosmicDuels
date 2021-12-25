#include "Menu.h"
#pragma warning(disable : 6011)
#pragma warning(disable : 6308)
#pragma warning(disable : 6386)
#pragma warning(disable : 6385)
#pragma warning(disable : 26451)//All integer operations can be overflow. It is absolutly useless warnintg.
#pragma warning(disable : 28182)

#define HORISONTAL_VECTOR	Vec2F(1.0f, 0.0f)

Area::Area() :
	points(nullptr),
	points_count(0)
{
}

Area::Area(const Area& area) :
	points(area.points_count > 0 ? new Vec2F[area.points_count] : nullptr),
	points_count(area.points_count)
{
	for (EngineTypes::Area::points_count_t point = 0; point < points_count; point++)
	{
		points[point] = area.points[point];
	}
}

Area::Area(const Vec2F* points, EngineTypes::Area::points_count_t points_count) :
	points((points_count > 0) ? new Vec2F[points_count] : nullptr),
	points_count(points_count)
{
	for (EngineTypes::Area::points_count_t point = 0; point < points_count; point++)
	{
		this->points[point] = points[point];
	}
}

const Vec2F* Area::GetPointsArrayP() const
{
	return points;
}

EngineTypes::Area::points_count_t Area::GetPointsCount() const
{
	return points_count;
}

bool Area::HavePointInside(Vec2F point) const
{
	if (points_count < 3)
	{
		return false;
	}
	EngineTypes::Area::points_count_t intersections_count = 0;
	Beam horisontal_beam = Beam(point, HORISONTAL_VECTOR);
	Segment temp__segment = Segment(points[points_count - 1], points[0], true);
	if (temp__segment.IsIntersection(&horisontal_beam))
	{
		intersections_count++;
	}
	for (EngineTypes::Area::points_count_t segment = 1; segment < points_count; segment++)
	{
		temp__segment.Set(&points[segment - 1], &points[segment], true);
		if (horisontal_beam.IsIntersection(&temp__segment))
		{
			intersections_count++;
		}
	}
	return (intersections_count % 2);
}

bool Area::HavePointInside(const Vec2F* point) const
{
	if (points_count < 3)
	{
		return false;
	}
	EngineTypes::Area::points_count_t intersections_count = 0;
	Beam horisontal_beam = Beam(*point, HORISONTAL_VECTOR);
	Segment temp__segment = Segment(points[points_count - 1], points[0], true);
	if (temp__segment.IsIntersection(&horisontal_beam))
	{
		intersections_count++;
	}
	for (EngineTypes::Area::points_count_t segment = 1; segment < points_count; segment++)
	{
		temp__segment.Set(&points[segment - 1], &points[segment], true);
		if (horisontal_beam.IsIntersection(&temp__segment))
		{
			intersections_count++;
		}
	}
	return (intersections_count % 2);
}

void Area::Set(const Area* area)
{
	if (points_count > 0)
	{
		delete[] points;
	}
	points_count = area->points_count;
	if (points_count > 0)
	{
		points = new Vec2F[area->points_count];
		for (EngineTypes::Area::points_count_t point = 0; point < points_count; point++)
		{
			points[point] = area->points[point];
		}
	}
	else
	{
		points = nullptr;
	}
}

void Area::Set(const Vec2F* points, EngineTypes::Area::points_count_t points_count)
{
	if (this->points_count > 0)
	{
		delete[] this->points;
	}
	this->points_count = points_count;
	if (this->points_count > 0)
	{
		this->points = new Vec2F[this->points_count];
		for (EngineTypes::Area::points_count_t point = 0; point < points_count; point++)
		{
			this->points[point] = points[point];
		}
	}
	else
	{
		this->points = nullptr;
	}
}

void Area::operator=(Area area)
{
	if (points_count > 0)
	{
		delete[] points;
	}
	points_count = area.points_count;
	if (points_count > 0)
	{
		points = new Vec2F[area.points_count];
		for (EngineTypes::Area::points_count_t point = 0; point < points_count; point++)
		{
			points[point] = area.points[point];
		}
	}
	else
	{
		points = nullptr;
	}
}

Area::~Area()
{
	if (points_count > 0)
	{
		delete[] points;
	}
}



Button::Button() :
	status(BUTTON_STATUS_FALSE),
	text((EngineTypes::Button::text_t*)malloc(sizeof(EngineTypes::Button::text_t))),
	text_size(0),
	id(0),
	text_length(0)
{
	text[0] = '\0';
}

Button::Button(const Button& button) :
	position(button.position),
	size(button.size),
	text((EngineTypes::Button::text_t*)malloc(sizeof(EngineTypes::Button::text_t) * (button.text_length + 1))),
	text_size(button.text_size),
	id(button.id),
	status(button.status),
	text_length(button.text_length)
{
	area.Set(&button.area);
	for (EngineTypes::Button::text_length_t i = 0; i < text_length; i++)
	{
		text[i] = button.text[i];
	}
}

Button::Button(
	EngineTypes::Button::id_t id,
	Vec2F position,
	Vec2F size,
	const Area* area,
	const char* text,
	EngineTypes::Button::text_size_t text_size,
	EngineTypes::Button::status_t status)
	:
	id(id),
	position(position),
	size(size),
	status(status),
	text_size(text_size)
{
	this->area.Set(area);

	for (text_length = 0; text[text_length] != '\0'; text_length++);

	this->text = (EngineTypes::Button::text_t*)malloc(sizeof(EngineTypes::Button::text_t) * (text_length + 1));
	for (EngineTypes::Button::text_t i = 0; i <= text_length; i++)
	{
		this->text[i] = text[i];
	}
}

Button::Button(
	EngineTypes::Button::id_t id,
	const Vec2F* position,
	const Vec2F* size,
	const Area* area,
	const char* text,
	EngineTypes::Button::text_size_t text_size,
	EngineTypes::Button::status_t status) 
	:
	id(id), 
	position(*position), 
	size(*size),
	status(status),
	text_size(text_size)
{
	this->area.Set(area);

	for (text_length = 0; text[text_length] != '\0'; text_length++);

	this->text = (EngineTypes::Button::text_t*)malloc(sizeof(EngineTypes::Button::text_t) * (text_length + 1));
	for (EngineTypes::Button::text_t i = 0; i <= text_length; i++)
	{
		this->text[i] = text[i];
	}
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

bool Button::GetStatus(EngineTypes::Button::status_t status_mask) const
{
	return status & status_mask;
}

EngineTypes::Button::text_t* Button::GetText() const
{
	return text;
}

char* Button::GetTextC() const
{
	char* return_text = new char[text_length + 1];
	for (EngineTypes::Button::text_length_t i = 0; i <= text_length; i++)
	{
		return_text[i] = text[i];
	}
	return return_text;
}

EngineTypes::Button::text_t* Button::GetTextU() const
{
	EngineTypes::Button::text_t* return_text = new EngineTypes::Button::text_t[text_length + 1];
	for (EngineTypes::Button::text_length_t i = 0; i <= text_length; i++)
	{
		return_text[i] = text[i];
	}
	return return_text;
}

uint16_t Button::GetTextLength() const
{
	return text_length;
}

bool Button::HavePoint(Vec2F point) const
{
	Vec2F current_point = point - position;
	current_point.ScaleThis(Vec2F(1.0f / size.x, 1.0f / size.y));
	return area.HavePointInside(current_point);
}

bool Button::HavePoint(const Vec2F* point) const
{
	Vec2F current_point = *point - position;
	current_point.ScaleThis(Vec2F(1.0f / size.x, 1.0f / size.y));
	return area.HavePointInside(current_point);
}

void Button::Move(Vec2F move_vector)
{
	position += move_vector;
}

void Button::Move(const Vec2F* move_vector)
{
	position += *move_vector;
}

void Button::Set(const Button* button)
{
	area.Set(&button->area);
	id = button->id;
	position = button->position;
	size = button->size;
	status = button->status;
	text_length = button->text_length;
	text_size = button->text_size;

	SetText(button->text, button->text_length);
}

void Button::Set(EngineTypes::Button::id_t id, Vec2F position, Vec2F size, const Area* area, const char* text, EngineTypes::Button::text_size_t text_size, EngineTypes::Button::status_t status)
{
	this->area.Set(area);
	this->id = id;
	this->position = position;
	this->size = size;
	this->status = status;
	this->text_size = text_size;

	SetText(text);
}

void Button::Set(EngineTypes::Button::id_t id, const Vec2F* position, const Vec2F* size, const Area* area, const char* text, EngineTypes::Button::text_size_t text_size, EngineTypes::Button::status_t status)
{
	this->area.Set(area);
	this->id = id;
	this->position = *position;
	this->size = *size;
	this->status = status;
	this->text_size = text_size;

	SetText(text);
}

void Button::SetArea(const Area* area)
{
	this->area.Set(area);
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

void Button::SetPosition(Vec2F* position)
{
	this->position = *position;
}

void Button::SetSize(Vec2F* size)
{
	this->size = *size;
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

void Button::SetText(const char* text)
{
	for (text_length = 0; text[text_length] != '\0'; text_length++);
	this->text = (EngineTypes::Button::text_t*)realloc(this->text, sizeof(EngineTypes::Button::text_t) * (text_length + 1));
	for (EngineTypes::Button::text_t i = 0; i <= text_length; i++)
	{
		this->text[i] = text[i];
	}
}

void Button::SetText(const char* text, EngineTypes::Button::text_length_t text_length)
{
	this->text_length = text_length;
	this->text = (EngineTypes::Button::text_t*)realloc(this->text, sizeof(EngineTypes::Button::text_t) * (text_length + 1));
	for (EngineTypes::Button::text_t i = 0; i <= text_length; i++)
	{
		this->text[i] = text[i];
	}
}

void Button::SetText(const EngineTypes::Button::text_t* text)
{
	for (text_length = 0; text[text_length] != '\0'; text_length++);
	this->text = (EngineTypes::Button::text_t*)realloc(this->text, sizeof(EngineTypes::Button::text_t) * (text_length + 1));
	for (EngineTypes::Button::text_t i = 0; i <= text_length; i++)
	{
		this->text[i] = text[i];
	}
}

void Button::SetText(const EngineTypes::Button::text_t* text, EngineTypes::Button::text_length_t text_length)
{
	this->text_length = text_length;
	this->text = (EngineTypes::Button::text_t*)realloc(this->text, sizeof(EngineTypes::Button::text_t) * (text_length + 1));
	for (EngineTypes::Button::text_t i = 0; i <= text_length; i++)
	{
		this->text[i] = text[i];
	}
}

void Button::TakeData(Button* button)
{
	position = button->position;
	size = button->size;
	text_size = button->text_size;
	id = button->id;
	//SetText(button->text, button->text_length);
}

void Button::operator=(Button button)
{
	id = button.id;
	position = button.position;
	size = button.size;
	status = button.status;
	text_size = button.text_size;

	//SetText(button.text, button.text_length);
}

Button::~Button()
{
	free(text);
}



Menu::Menu() :
	default_buttons(nullptr),
	current_buttons(nullptr),
	buttons_count(0)
{
}

Menu::Menu(const Menu& menu):
	buttons_count(menu.buttons_count),
	current_buttons(menu.buttons_count > 0 ? new Button[menu.buttons_count] : nullptr),
	default_buttons(menu.buttons_count > 0 ? new Button[menu.buttons_count] : nullptr),
	position(menu.position),
	size(menu.size)
{
	for (EngineTypes::Menu::buttons_count_t i = 0; i < buttons_count; i++)
	{
		default_buttons[i].Set(&default_buttons[i]);
		current_buttons[i].Set(&current_buttons[i]);
	}
	HardRecalculate();
}

Menu::Menu(
	const Vec2F* position,
	const Vec2F* size,
	const Button* buttons,
	EngineTypes::Menu::buttons_count_t buttons_count)
	:
	position(*position),
	size(*size),
	buttons_count(buttons_count),
	current_buttons(buttons_count > 0 ? new Button[buttons_count] : nullptr),
	default_buttons(buttons_count > 0 ? new Button[buttons_count] : nullptr)
{
	for (EngineTypes::Menu::buttons_count_t i = 0; i < buttons_count; i++)
	{
		default_buttons[i].Set(&buttons[i]);
	}
	HardRecalculate();
}

void Menu::AddButton(EngineTypes::Menu::buttons_count_t button_number, const Button* button)
{
	Button* new_buttons = new Button[++buttons_count];
	if (button_number >= buttons_count)
	{
		button_number = buttons_count - 1;
	}

	for (EngineTypes::Menu::buttons_count_t i = 0; i < button_number; i++)
	{
		new_buttons[i].Set(&default_buttons[i]);
	}
	new_buttons[button_number].Set(button);
	for (EngineTypes::Menu::buttons_count_t i = button_number + 1; i < buttons_count; i++)
	{
		new_buttons[i].Set(&default_buttons[i - 1]);
	}

	if (buttons_count > 1)
	{
		delete[] default_buttons;
		delete[] current_buttons;
	}
	default_buttons = new_buttons;
	current_buttons = new Button[buttons_count];
	HardRecalculate();
}

void Menu::Clear()
{
	if (buttons_count > 0)
	{
		buttons_count = 0;
		delete[] current_buttons;
		delete[] default_buttons;
	}

	current_buttons = nullptr;
	default_buttons = nullptr;
}

void Menu::DeleteButton(EngineTypes::Menu::buttons_count_t button_number)
{
	if (button_number >= buttons_count)
	{
		return;
	}

	Button* new_buttons = new Button[--buttons_count];

	for (EngineTypes::Menu::buttons_count_t i = 0; i < button_number; i++)
	{
		new_buttons->Set(&default_buttons[i]);
	}
	for (EngineTypes::Menu::buttons_count_t i = button_number; i < buttons_count; i++)
	{
		new_buttons[i].Set(&default_buttons[i + 1]);
	}

	delete[] default_buttons;
	default_buttons = new_buttons;
	HardRecalculate();
}

EngineTypes::Menu::buttons_count_t Menu::GetButtonsCount() const
{
	return buttons_count;
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
	for (EngineTypes::Menu::buttons_count_t i = 0; i < buttons_count; i++)
	{
		current_buttons[i].Set(&default_buttons[i]);
		current_buttons[i].Move(&position);
	}
}

void Menu::Move(Vec2F move_vector)
{
	position += move_vector;
	Recalculate();
}

void Menu::Move(const Vec2F* move_vector)
{
	position += *move_vector;
	Recalculate();
}

void Menu::Recalculate()
{
	for (EngineTypes::Menu::buttons_count_t i = 0; i < buttons_count; i++)
	{
		current_buttons[i].TakeData(&default_buttons[i]);
		current_buttons[i].Move(&position);
	}
}

void Menu::Set(const Menu* menu)
{
	position = menu->position;
	size = menu->size;

	if (buttons_count > 0)
	{
		delete[] default_buttons;
		delete[] current_buttons;
	}

	buttons_count = menu->buttons_count;

	if (buttons_count > 0)
	{
		default_buttons = new Button[buttons_count];
		current_buttons = new Button[buttons_count];
		for (EngineTypes::Menu::buttons_count_t i = 0; i < buttons_count; i++)
		{
			default_buttons[i].Set(&menu->default_buttons[i]);
		}
	}
	else
	{
		default_buttons = nullptr;
		current_buttons = nullptr;
	}
	HardRecalculate();
}

void Menu::Set(const Vec2F* position, const Vec2F* size, const Button* buttons, EngineTypes::Menu::buttons_count_t buttons_count)
{
	this->position = *position;
	this->size = *size;
	if (this->buttons_count > 0)
	{
		delete[] this->current_buttons;
		delete[] this->default_buttons;
	}

	this->buttons_count = buttons_count;

	if (buttons_count > 0)
	{
		current_buttons = new Button[buttons_count];
		default_buttons = new Button[buttons_count];

		for (EngineTypes::Menu::buttons_count_t i = 0; i < buttons_count; i++)
		{
			default_buttons[i].Set(&buttons[i]);
		}
	}
	else
	{
		default_buttons = nullptr;
		current_buttons = nullptr;
	}
	HardRecalculate();
}

void Menu::SetPosition(Vec2F position)
{
	this->position = position;
	Recalculate();
}

void Menu::SetPosition(const Vec2F* position)
{
	this->position = *position;
	Recalculate();
}

void Menu::UpdateDefaultButtons()
{
	for (EngineTypes::Menu::buttons_count_t i = 0; i < buttons_count; i++)
	{
		default_buttons[i].SetStatus(current_buttons[i].status, true);
		default_buttons[i].SetText(current_buttons[i].GetText(), current_buttons[i].GetTextLength());
		default_buttons[i].text_size = current_buttons[i].text_size;
	}
}

Menu::~Menu()
{
	if (buttons_count > 0)
	{
		delete[] default_buttons;
		delete[] current_buttons;
	}
}