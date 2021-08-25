#include "Menu.h"
#pragma warning(disable : 6011)
#pragma warning(disable : 6308)
#pragma warning(disable : 6386)
#pragma warning(disable : 6385)
#pragma warning(disable : 26451)//All integer operations can be overflow. It is absolutly useless warnintg.
#pragma warning(disable : 28182)

Button::Button() :
	position(new Vec2F()),
	status(BUTTON_STATUS_INACTIVE),
	size(new Vec2F()),
	text((ClassTypes::Button::button_text_t*)malloc(sizeof(ClassTypes::Button::button_text_t))),
	text_size(0),
	id(0),
	text_length(0)
{
	text[0] = '\0';
}

Button::Button(const Button& button) :
	position(new Vec2F(*button.position)),
	size(new Vec2F(*button.size)),
	text((ClassTypes::Button::button_text_t*)malloc(sizeof(ClassTypes::Button::button_text_t)* (text_length + 1))),
	text_size(button.text_size),
	id(button.id),
	status(button.status),
	text_length(button.text_length)
{
	for (ClassTypes::Button::text_length_t i = 0; i < text_length; i++)
	{
		text[i] = button.text[i];
	}
}

Button::Button(ClassTypes::Button::button_id_t id, Vec2F* position, Vec2F* size, const char* text, ClassTypes::Button::text_size_t text_size) :
	id(id), 
	position(new Vec2F(*position)), 
	size(new Vec2F(*size)),
	status(BUTTON_STATUS_INACTIVE),
	text_size(text_size)
{
	for (text_length = 0; text[text_length] != '\0'; text_length++);

	this->text = (ClassTypes::Button::button_text_t*)malloc(sizeof(ClassTypes::Button::button_text_t) * (text_length + 1));
	for (uint32_t i = 0; i <= text_length; i++)
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

bool Button::GetStatus(ClassTypes::Button::button_status_t status_mask)
{
	return status & status_mask;
}

ClassTypes::Button::button_text_t* Button::GetText()
{
	return text;
}

uint16_t Button::GetTextLength()
{
	return text_length;
}

void Button::Set(Button* button)
{
	id = button->id;
	*position = *button->position;
	*size = *button->size;
	status = button->status;
	text_length = button->text_length;
	text_size = button->text_size;

	for (text_length = 0; button->text[text_length] != '\0'; text_length++);

	text = (ClassTypes::Button::button_text_t*)realloc(text, sizeof(ClassTypes::Button::button_text_t) * (text_length + 1));
	for (uint32_t i = 0; i <= text_length; i++)
	{
		text[i] = button->text[i];
	}
}

void Button::Set(ClassTypes::Button::button_id_t id, Vec2F* position, Vec2F* size, const char* text, ClassTypes::Button::text_size_t text_size, ClassTypes::Button::button_status_t status)
{
	this->id = id;
	*this->position = *position;
	*this->size = *size;
	this->status = status;
	this->text_size = text_size;

	for (text_length = 0; text[text_length] != '\0'; text_length++);

	this->text = (ClassTypes::Button::button_text_t*)realloc(this->text, sizeof(ClassTypes::Button::button_text_t) * (text_length + 1));
	for (uint32_t i = 0; i <= text_length; i++)
	{
		this->text[i] = text[i];
	}
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

void Button::Move(Vec2F* move_vector)
{
	*position += *move_vector;
}

void Button::SetId(ClassTypes::Button::button_id_t id)
{
	this->id = id;
}

void Button::SetOnlyCustomStatus(ClassTypes::Button::button_status_t status_mask)
{
	status |= status_mask & (BUTTON_STATUSE_ALL - (BUTTON_STATUS_ACTIVE | BUTTON_STATUS_SELECT));
	status &= status_mask | (BUTTON_STATUS_ACTIVE | BUTTON_STATUS_SELECT);
}

void Button::SetPosition(Vec2F* position)
{
	*this->position = *position;
}

void Button::SetSize(Vec2F* size)
{
	*this->size = *size;
}

void Button::SetStatus(ClassTypes::Button::button_status_t status_mask, bool value)
{
	if (value)
	{
		status |= status_mask;
		return;
	}
	status &= BUTTON_STATUSE_ALL - status_mask;
}

void Button::SetText(const char* text)
{
	for (text_length = 0; text[text_length] != '\0'; text_length++);
	std::cout << text_length << ' ' << this->text << ' ' << sizeof(ClassTypes::Button::button_text_t) * (text_length + 1) << std::endl;
	free(this->text);
	std::cout << this->text << std::endl;
	this->text = (ClassTypes::Button::button_text_t*)malloc(sizeof(ClassTypes::Button::button_text_t) * (text_length + 1));
	for (uint32_t i = 0; i <= text_length; i++)
	{
		this->text[i] = text[i];
	}
}

void Button::SetTextSize(ClassTypes::Button::text_size_t text_size)
{
	this->text_size = text_size;
}

void Button::TakeData(Button* button)
{
	*position = *button->position;
	*size = *button->size;
	text_size = button->text_size;
	text = button->text;
}

void Button::operator=(Button button)
{
	*position = *button.position;
	*size = *button.size;
	text_size = button.text_size;
	id = button.id;

	for (text_length = 0; button.text[text_length] != '\0'; text_length++);

	text = (ClassTypes::Button::button_text_t*)realloc(text, sizeof(ClassTypes::Button::button_text_t) * (text_length + 1));
	for (uint32_t i = 0; i <= text_length; i++)
	{
		text[i] = button.text[i];
	}
	status = button.status;
}

Button::~Button()
{
	delete position;
	delete size;
	free(text);
}



Menu::Menu() :
	default_buttons(new Button[1]),
	current_buttons(new Button[1]),
	buttons_count(1),
	position(new Vec2F()),
	size(new Vec2F())
{
}

Menu::Menu(Vec2F* position, Vec2F* size, Button* buttons, ClassTypes::Menu::buttons_count_t buttons_count) :
	position(new Vec2F(*position)),
	size(new Vec2F(*size)),
	buttons_count(buttons_count)
{
	if (buttons_count == 0)
	{
		buttons_count = 1;
		current_buttons = new Button[1];
		default_buttons = new Button[1];
	}
	else
	{
		current_buttons = new Button[buttons_count];
		default_buttons = new Button[buttons_count];

		for (ClassTypes::Menu::buttons_count_t i = 0; i < buttons_count; i++)
		{
			default_buttons[i].Set(&buttons[i]);
			current_buttons[i].Set(&default_buttons[i]);
		}
	}
}

ClassTypes::Menu::buttons_count_t Menu::GetButtonsCount()
{
	return buttons_count;
}

Vec2F Menu::GetPosition()
{
	return *position;
}

Vec2F Menu::GetSize()
{
	return *size;
}

void Menu::Move(Vec2F* move_vector)
{
	*position += *move_vector;
	Recalculate();
}

void Menu::Set(Menu* menu)
{
	buttons_count = menu->buttons_count;
	*position = *menu->position;
	*size = *menu->size;

	delete[] default_buttons;
	delete[] current_buttons;

	default_buttons = new Button[buttons_count];
	current_buttons = new Button[buttons_count];
	for (ClassTypes::Menu::buttons_count_t i = 0; i < buttons_count; i++)
	{
		default_buttons[i].Set(&menu->default_buttons[i]);
		current_buttons[i].Set(&menu->current_buttons[i]);
	}
}

void Menu::Set(Vec2F* position, Vec2F* size, Button* buttons, ClassTypes::Menu::buttons_count_t buttons_count)
{
	*this->position = *position;
	*this->size = *size;
	
	delete[] this->current_buttons;
	delete[] this->default_buttons;
	if (buttons_count == 0)
	{
		this->buttons_count = 1;
		current_buttons = new Button[1];
		default_buttons = new Button[1];
	}
	else
	{
		this->buttons_count = buttons_count;
		current_buttons = new Button[buttons_count];
		default_buttons = new Button[buttons_count];

		for (ClassTypes::Menu::buttons_count_t i = 0; i < buttons_count; i++)
		{
			default_buttons[i] = buttons[i];
		}
	}
	Recalculate();
}

void Menu::SetPosition(Vec2F* position)
{
	*this->position = *position;
	Recalculate();
}

void Menu::Recalculate()
{
	for (ClassTypes::Menu::buttons_count_t i = 0; i < buttons_count; i++)
	{
		current_buttons[i].TakeData(&default_buttons[i]);
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