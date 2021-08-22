#pragma once

#include <iostream>
#include <glad/glad.h>
#include "Vec.h"
#include "Color.h"
#include "../Types/ClassesTypes.h"
#include "../stb_image.h"

class Texture2D
{
private:
	GLuint id;
public:
	Texture2D(const char* texture_name);

	void Use();

	~Texture2D();
};