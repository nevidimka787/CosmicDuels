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
	Mat2F hight_limits;
	Vec2F low_limits;
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
	//Scale of the camera.
	//size.x / size.y
	float scale;
	//Size of the view area of the camera.
	Vec2F view_area_size;

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
		Mat2F* hight_limits = nullptr,
		Vec2F* low_limits = nullptr);

	Mat2F GetLimits();
	Vec2F GetPosition();
	Vec2F GetSize();
	float GetScale();
	//All ships and pilots will be show in the camera.
	void Focus(
		Ship* ships_array,
		Pilot* pilots_array,
		GameTypes::players_count_t players_count = 0);

	void Set(Camera* camera);
	void SetCoefficients(
		float margin = CAMERA_DEFAULT_MARGIN,
		float move_velocity_coefficient = CAMERA_DEFAULT_MOVE_VELOCITY,
		float resize_velocity_coefficient = CAMERA_DEFAULT_RESIZE_VELOCITY);
	void SetHightLimits(
		float min_x = -1.0f,
		float min_y = -1.0f,
		float max_x = 1.0f,
		float max_y = 1.0f);
	void SetLowLimits(
		float size_x = 0.1f, 
		float size_y = 0.1f);
	void SetPosition(Vec2F* position = nullptr);
	void SetScale(
		float scale = 16.0f / 9.0f,
		bool x_is_const = false);
	void SetSize(float size = 1.0f);

	~Camera();
};