#pragma once

#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../stb_image.h"

#include "Color.h"
#include "Entity.h"
#include "Line.h"
#include "Map.h"
#include "Mat.h"
#include "Menu.h"
#include "Shader.h"
#include "Texture.h"
#include "Vec.h"

#include "../Types/AllTypes.h"
#include "../Constants/AllConstants.h"

class Camera
{
private:
	bool temp_flag = false;
	//a11 - min.x
	//a12 - min.y
	//a21 - max.x
	//a22 - max.y
	Mat2F temp_limits;
	Vec2F temp_position;


	//Maximun and ninimun coordinates that can be show in the camera.
	Mat2F limits;
	//How mach space after limits camera can show.
	float margin;
	//0 - not move
	//1 - momental move
	float move_velocity_coefficient;
	//Position of the camera.
	Vec2F position;
	//0 - not resize
	//1 - momental resize
	float resize_velocity_coefficient;
	//Size of the camera.
	Vec2F size;

	void Limit();
public:

	Camera(const Camera& camera);
	Camera(
		Vec2F* position = nullptr,
		float size = 1.1f,
		float scale = 16.0f / 9.0f,
		float margin = CAMERA_DEFAULT_MARGIN,
		float move_velocity_coefficient = CAMERA_DEFAULT_MOVE_VELOCITY,
		float resize_velocity_coefficient = CAMERA_DEFAULT_RESIZE_VELOCITY,
		Mat2F* limits = nullptr);

	float GetScale();
	//All ships will be show in the camera.
	void Focus(
		Ship* ships_array,
		Pilot* pilots_array,
		GameTypes::players_count_t ships_array_length = 0,
		GameTypes::players_count_t pilots_array_length = 0);

	void Set(Camera* camera);
	void SetCoefficients(
		float margin = CAMERA_DEFAULT_MARGIN,
		float move_velocity_coefficient = CAMERA_DEFAULT_MOVE_VELOCITY,
		float resize_velocity_coefficient = CAMERA_DEFAULT_RESIZE_VELOCITY);
	void SetLimits(float min_x, float min_y, float max_x, float max_y);
	void SetPosition(Vec2F* position);
	void SetSize(float size);

	~Camera();
};