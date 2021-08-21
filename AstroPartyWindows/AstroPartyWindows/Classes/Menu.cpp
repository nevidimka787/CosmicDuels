#include "Menu.h"
#pragma warning(disable : 26451)//All integer operations can be overflow. It is absolutly useless warnintg.
#pragma warning(disable : 6386)
#pragma warning(disable : 6385)

Button::Button() : position(new Vec2F()), size(new Vec2F()), text(nullptr), text_size(0), id(0), status(BUTTOM_STATUS_INACTIVE)
{

}

Button::Button(uint32_t id, Vec2F* position, Vec2F* size, const char* text, uint8_t text_size) : id(id), position(new Vec2F(*position)), size(new Vec2F(*size)), text_size(text_size), status(BUTTOM_STATUS_INACTIVE)
{
	uint32_t length;
	for (length = 0; text[length] != '\0'; length++);

	this->text = new char[length + 1];
	for (uint32_t i = 0; i <= length; i++)
	{
		this->text[i] = text[i];
	}
}

ClassTypes::Button::button_id_t Button::GetId()
{
	return id;
}

Vec2F Button::GetPosition()
{
	return *position;
}

Vec2F Button::GetSize()
{
	return *size;
}

void Button::Set(Button* button)
{
	*position = *button->position;
	*size = *button->size;
	text_size = button->text_size;
	id = button->id;
	
	if (text != nullptr)
	{
		delete[] text;
	}

	uint32_t length;
	for (length = 0; button->text[length] != '\0'; length++);

	text = new char[length + 1];
	for (uint32_t i = 0; i <= length; i++)
	{
		text[i] = button->text[i];
	}
	status = button->status;
}

void Button::Set(uint32_t id, Vec2F* position, Vec2F* size, const char* text, uint8_t text_size)
{
	this->id = id;
	*this->position = *position;
	*this->size = *size;

	if (this->text != nullptr)
	{
		delete[] this->text;
	}

	uint32_t length;
	for (length = 0; text[length] != '\0'; length++);

	this->text = new char[length + 1];
	for (uint32_t i = 0; i <= length; i++)
	{
		this->text[i] = text[i];
	}

	this->text_size = text_size;
}

void Button::Move(Vec2F* move_vector)
{
	*position += *move_vector;
}

void Button::SetId(uint32_t id)
{
	this->id = id;
}

void Button::SetPosition(Vec2F* position)
{
	*this->position = *position;
}

void Button::SetSize(Vec2F* size)
{
	*this->size = *size;
}

bool Button::HavePoint(Vec2F* point)
{
	if (size->x > 0.0f)
	{
		if (size->y > 0.0f)
		{
			return point->x >= position->x && point->y >= position->y && point->x <= position->x + size->x && point->y <= position->y + size->y;
		}
		return point->x >= position->x && point->y <= position->y && point->x <= position->x + size->x && point->y >= position->y + size->y;
	}
	if (size->y > 0.0f)
	{
		return point->x <= position->x && point->y >= position->y && point->x >= position->x + size->x && point->y <= position->y + size->y;
	}
	return point->x <= position->x && point->y <= position->y && point->x >= position->x + size->x && point->y >= position->y + size->y;
}

void Button::SetText(const char* text)
{
	if (this->text != nullptr)
	{
		delete[] this->text;
	}

	uint32_t length;
	for (length = 0; text[length] != '\0'; length++);

	this->text = new char[length + 1];
	for (uint32_t i = 0; i <= length; i++)
	{
		this->text[i] = text[i];
	}
}

void Button::SetText(const char* text, uint8_t text_size)
{
	SetText(text);

	this->text_size = text_size;
}

void Button::operator=(Button button)
{
	if (position == nullptr)
	{
		position = new Vec2F();
	}
	if (size == nullptr)
	{
		size = new Vec2F();
	}
	*position = *button.position;
	*size = *button.size;
	text_size = button.text_size;
	id = button.id;

	if (text != nullptr)
	{
		delete[] text;
	}

	uint32_t length;
	for (length = 0; button.text[length] != '\0'; length++);

	text = new char[length + 1];
	for (uint32_t i = 0; i <= length; i++)
	{
		text[i] = button.text[i];
	}
	status = button.status;
}

Button::~Button()
{
	delete position;
	delete size;
	delete[] text;
}



Menu::Menu() : default_buttons(nullptr), current_buttons(nullptr), buttons_count(0), position(new Vec2F())
{
	
}

Menu::Menu(Vec2F* position, Button* buttons, ClassTypes::Menu::buttons_count_t buttons_count) : position(new Vec2F(*position)), default_buttons(new Button[buttons_count]), current_buttons(new Button[buttons_count]), buttons_count(buttons_count)
{
	for (ClassTypes::Menu::buttons_count_t i = 0; i < buttons_count; i++)
	{
		default_buttons[i].Set(&buttons[i]);
		current_buttons[i].Set(&default_buttons[i]);
	}
}

ClassTypes::Menu::buttons_count_t Menu::GetButtonsCount()
{
	return buttons_count;
}

void Menu::Move(Vec2F* move_vector)
{
	*position += *move_vector;
	Recalculate();
}

void Menu::Set(Menu* menu)
{
	if (default_buttons != nullptr)
	{
		delete[] default_buttons;
	}
	if (current_buttons != nullptr)
	{
		delete[] current_buttons;
	}
	default_buttons = new Button[menu->buttons_count];
	current_buttons = new Button[menu->buttons_count];
	for (uint8_t i = 0; i < buttons_count; i++)
	{
		default_buttons[i] = menu->default_buttons[i];
		current_buttons[i] = default_buttons[i];
		current_buttons[i].Move(position);
	}
}

void Menu::SetPosition(Vec2F* position)
{
	*this->position = *position;
	Recalculate();
}

void Menu::Recalculate()
{
	for (uint8_t i = 0; i < buttons_count; i++)
	{
		current_buttons[i] = default_buttons[i];
		current_buttons[i].Move(position);
	}
}

Menu::~Menu()
{
	if (default_buttons != nullptr)
	{
		delete[] default_buttons;
	}
}