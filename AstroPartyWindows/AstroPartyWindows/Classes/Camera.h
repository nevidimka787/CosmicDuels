#pragma once

#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../stb_image.h"

#include "Entity.h"
#include "Line.h"
#include "Mat.h"

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


	//Position of the camera.
	Vec2F position;
	//Maximun and ninimun coordinates that can be show in the camera.
	Mat2F hight_limits;
	Vec2F low_limits;
	//Scale of the camera.
	//size.x / size.y
	float scale;
	//Size of the view area of the camera.
	float new_view_area_size_x;
	float view_area_size_x;

	void Limit();
	//Use temp_position
	//Use and change temp flag.
	inline void UpdateLimits();
public:
	//How mach space after limits camera can show.
	float margin;
	//0 - not move
	//1 - momental move
	float move_velocity_coefficient;
	//0 - not resize
	//1 - momental resize
	float resize_velocity_coefficient;

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
	float GetSize();
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
		float min_x = -GAME_ENGINE_AREA_SIZE,
		float min_y = -GAME_ENGINE_AREA_SIZE,
		float max_x = GAME_ENGINE_AREA_SIZE,
		float max_y = GAME_ENGINE_AREA_SIZE);
	void SetLowLimits(
		float size_x = GAME_ENGINE_AREA_SIZE / 10.0f,
		float size_y = GAME_ENGINE_AREA_SIZE / 10.0f);
	void SetPosition(Vec2F position = Vec2F()); 
	void SetPosition(Vec2F* position = nullptr);
	void SetScale(float scale = 16.0f / 9.0f);
	void SetSize(float size = 1.0f);

	~Camera();
};