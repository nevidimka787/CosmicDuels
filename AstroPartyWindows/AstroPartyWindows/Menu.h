#ifndef MENU_H
#define MENU_H

#include "Vec.h"
#include <iostream>


class Button;
class Menu;

class Button
{
#define BUTTOM_STATUS_ACTIVE	0x00
#define BUTTOM_STATUS_INACTIVE	0x01

#define BUTTON_STATUS_CUSTOM_0	0x02
#define BUTTON_STATUS_CUSTOM_1	0x04
#define BUTTON_STATUS_CUSTOM_2	0x08
#define BUTTON_STATUS_CUSTOM_3	0x10
#define BUTTON_STATUS_CUSTOM_4	0x20
#define BUTTON_STATUS_CUSTOM_5	0x40
#define BUTTON_STATUS_CUSTOM_6	0x80

protected:
	Vec2F* position;
	Vec2F* size;
	char* text;
	uint8_t text_size;
	uint32_t id;
	uint8_t status;
public:
	Button();
	Button(uint32_t id, Vec2F* position, Vec2F* size, const char* text, uint8_t text_size);

	uint32_t GetId();
	Vec2F GetPosition();
	Vec2F GetSize();
	bool HavePoint(Vec2F* point);
	void Move(Vec2F* move_vector);
	void Set(Button* button);
	void SetId(uint32_t id);
	void SetPosition(Vec2F* position);
	void SetSize(Vec2F* size);
	void SetText(const char* text);
	void SetText(const char* text, uint8_t text_size);

	~Button();
};

class Menu
{
protected:
	Vec2F* position;
	Button* default_buttons;
	Button* current_buttons;
	uint8_t buttons_count;
	void (*ProcessInputFunction)(Vec2F*);
public:
	Menu();
	Menu(Vec2F* position, Button* buttons, uint8_t buttons_count, void ProcessInputFunction(Vec2F*));

	void Move(Vec2F* move_vector);
	void Set(Menu* menu);
	void SetPosition(Vec2F* position);
	void ProcessInput(Vec2F* clk_pos);
	void Recalculate();

	~Menu();
};

#endif // !NEMU_H
