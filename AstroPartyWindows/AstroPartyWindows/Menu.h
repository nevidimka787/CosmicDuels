#ifndef MENU_H
#define MENU_H

#include "Vec.h"
#include <iostream>


class Button;
class Menu;

class Button
{
protected:
	Vec2F* position;
	Vec2F* size;
	char* text;
	uint8_t text_size;
	void*(*ActivateFunction)(void*);
public:
	Button();
	Button(Vec2F* position, Vec2F* size, void* ActivateFunction(void*), const char* text, uint8_t text_size);

	void Set(Button* button);
	void* ButtonActivate(void*);
	Vec2F GetPosition();
	Vec2F GetSize();
	void Move(Vec2F* move_vector);
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
public:
	Menu();
	Menu(Vec2F* position, Button* buttons, uint8_t buttons_count);

	void Move(Vec2F* move_vector);
	void Set(Menu* menu);
	void SetPosition(Vec2F* position);
	void Recalculate();

	~Menu();
};

#endif // !NEMU_H
