#include "Camera.h"

#define min_x a11
#define min_y a12
#define max_x a21
#define max_y a22

Camera::Camera(const Camera& camera) :
	low_limits(camera.low_limits),
	hight_limits(camera.hight_limits),
	margin(camera.margin),
	move_velocity_coefficient(camera.move_velocity_coefficient),
	position(camera.position),
	resize_velocity_coefficient(camera.resize_velocity_coefficient),
	scale(camera.scale),
	view_area_size_x(camera.view_area_size_x)
{
}

Camera::Camera(Vec2F* position, float size, float scale, float margin, float move_velocity_coefficient, float resize_velocity_coefficient, Mat2F* hight_limits, Vec2F* low_limits) :
	margin(margin),
	move_velocity_coefficient(move_velocity_coefficient),
	resize_velocity_coefficient(resize_velocity_coefficient),
	scale(scale)
{
	if (low_limits != nullptr)
	{
		this->low_limits = *low_limits;
	}
	if (hight_limits != nullptr)
	{
		this->hight_limits = *hight_limits;
	}
	if (position != nullptr)
	{
		this->position = *position;
	}
	this->view_area_size_x = size;
	Limit();
}

Mat2F Camera::GetLimits()
{
	return hight_limits;
}

Vec2F Camera::GetPosition()
{
	return position;
}

float Camera::GetSize()
{
	return view_area_size_x;
}

float Camera::GetScale()
{
	return scale;
}

void Camera::Focus(Ship* ships_array, Pilot* pilots_array, GameTypes::players_count_t players_count)
{

	temp_flag = true;
	for (GameTypes::players_count_t ship = 0; ship < players_count; ship++)
	{
		if (ships_array[ship].exist == false)
		{
			goto EndOfFirstCycle;
		}
		temp_position = ships_array[ship].GetPosition();
		if (temp_flag)
		{
			if (ships_array[ship].exist)
			{
				temp_limits.Set(&temp_position, &temp_position);
				temp_flag = false;
			}
		}
		else
		{
			if (temp_position.x < temp_limits.min_x)
			{
				temp_limits.min_x = temp_position.x;
			}
			else if (temp_position.x > temp_limits.max_x)
			{
				temp_limits.max_x = temp_position.x;
			}
			if (temp_position.y < temp_limits.min_y)
			{
				temp_limits.min_y = temp_position.y;
			}
			else if (temp_position.y > temp_limits.max_y)
			{
				temp_limits.max_y = temp_position.y;
			}
		}
	EndOfFirstCycle:;
	}
	for (GameTypes::players_count_t pilot = 0; pilot < players_count; pilot++)
	{
		if (pilots_array[pilot].exist == false)
		{
			goto EndOfSecondCycle;
		}
		temp_position = pilots_array[pilot].GetPosition();
		if (temp_flag)
		{
			if (pilots_array[pilot].exist)
			{
				temp_limits.Set(&temp_position, &temp_position);
			}
		}
		else if (pilots_array[pilot].exist)
		{
			if (temp_position.x < temp_limits.min_x)
			{
				temp_limits.min_x = temp_position.x;
			}
			else if (temp_position.x > temp_limits.max_x)
			{
				temp_limits.max_x = temp_position.x;
			}
			if (temp_position.y < temp_limits.min_y)
			{
				temp_limits.min_y = temp_position.y;
			}
			else if (temp_position.y > temp_limits.max_y)
			{
				temp_limits.max_y = temp_position.y;
			}
		}
	EndOfSecondCycle:;
	}

	Limit();

	position.Set((temp_limits.max_x + temp_limits.min_x) / 2.0f, (temp_limits.max_y + temp_limits.min_y) / 2.0f);
	view_area_size_x = (temp_limits.max_x - temp_limits.min_x) * margin;
	if (view_area_size_x < low_limits.x)
	{
		view_area_size_x = low_limits.x;
	}
	if (view_area_size_x / scale < low_limits.y)
	{
		view_area_size_x = low_limits.y * scale;
	}
}

void Camera::Limit()
{

	if (temp_limits.max_x > hight_limits.max_x * margin)
	{
		temp_limits.max_x = hight_limits.max_x * margin;
	}
	if (temp_limits.max_y > hight_limits.max_y * margin)
	{
		temp_limits.max_y = hight_limits.max_y * margin;
	}
	if (temp_limits.min_x < hight_limits.min_x * margin)
	{
		temp_limits.min_x = hight_limits.min_x * margin;
	}
	if (temp_limits.min_y < hight_limits.min_y * margin)
	{
		temp_limits.min_y = hight_limits.min_y * margin;
	}
}

void Camera::Set(Camera* camera)
{
	hight_limits = camera->hight_limits;
	low_limits = camera->low_limits;
	margin = camera->margin;
	move_velocity_coefficient = camera->move_velocity_coefficient;
	position = camera->position;
	resize_velocity_coefficient = camera->resize_velocity_coefficient;
	view_area_size_x = camera->view_area_size_x;
}

void Camera::SetCoefficients(float margin, float move_velocity_coefficient, float resize_velocity_coefficient)
{
	this->margin = margin;
	this->move_velocity_coefficient = move_velocity_coefficient;
	this->resize_velocity_coefficient = resize_velocity_coefficient;
}

void Camera::SetHightLimits(float _min_x, float _min_y, float _max_x, float _max_y)
{
	hight_limits.Set(_min_x, _min_y, _max_x, _max_y);
}

void Camera::SetLowLimits(float size_x, float size_y)
{
	low_limits.Set(size_x, size_y);
}

void Camera::SetPosition(Vec2F* position)
{
	if (position != nullptr)
	{
		this->position = *position;
		return;
	}
	this->position.Set(0.0f, 0.0f);
}

void Camera::SetScale(float scale)
{
	this->scale = scale;
}

void Camera::SetSize(float size)
{
	this->view_area_size_x = size;
}

Camera::~Camera()
{
}