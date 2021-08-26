#include "Camera.h"

#define min_x a11
#define min_y a12
#define max_x a21
#define max_y a22

Camera::Camera(const Camera& camera) :
	limits(camera.limits),
	margin(camera.margin),
	move_velocity_coefficient(camera.move_velocity_coefficient),
	position(camera.position),
	resize_velocity_coefficient(camera.resize_velocity_coefficient),
	size(camera.size)
{
}

Camera::Camera(Vec2F* position, float size, float scale, float margin, float move_velocity_coefficient, float resize_velocity_coefficient, Mat2F* limits) :
	margin(margin),
	move_velocity_coefficient(move_velocity_coefficient),
	resize_velocity_coefficient(resize_velocity_coefficient)
{
	if (limits != nullptr)
	{
		this->limits = *limits;
	}
	if (position != nullptr)
	{
		this->position = *position;
	}
	this->size.Set(size * scale, size);
	Limit();
}

float Camera::GetScale()
{
	return size.x / size.y;
}

void Camera::Focus(Ship* ships_array, Pilot* pilots_array, GameTypes::players_count_t ships_array_length, GameTypes::players_count_t pilots_array_length)
{

	temp_flag = true;
	for (GameTypes::players_count_t ship = 0; ship < ships_array_length; ship++)
	{
		if (temp_flag)
		{
			if (ships_array[ship].exist)
			{
				temp_position = ships_array[ship].GetPosition();
				temp_limits.Set(&temp_position, &temp_position);
			}
		}
		else if (ships_array[ship].exist)
		{
			temp_position = ships_array[ship].GetPosition();
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
	}
	for (GameTypes::players_count_t pilot = 0; pilot < ships_array_length; pilot++)
	{
		if (temp_flag)
		{
			if (pilots_array[pilot].exist)
			{
				temp_position = pilots_array[pilot].GetPosition();
				temp_limits.Set(&temp_position, &temp_position);
			}
		}
		else if (pilots_array[pilot].exist)
		{
			temp_position = pilots_array[pilot].GetPosition();
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
	}
}

void Camera::Limit()
{

	if (temp_limits.max_x > limits.max_x * margin)
	{
		temp_limits.max_x = limits.max_x * margin;
	}
	if (temp_limits.max_y > limits.max_y * margin)
	{
		temp_limits.max_y = limits.max_y * margin;
	}
	if (temp_limits.min_x > limits.min_x * margin)
	{
		temp_limits.min_x = limits.min_x * margin;
	}
	if (temp_limits.min_y > limits.min_y * margin)
	{
		temp_limits.min_y = limits.min_y * margin;
	}
	position.Set((temp_limits.max_x + temp_limits.min_x) / 2.0f, (temp_limits.max_y + temp_limits.min_y) / 2.0f);
	size.x = (temp_limits.max_y - position.y) * GetScale();
	size.y = temp_limits.max_y - position.y;
}

void Camera::Set(Camera* camera)
{
	limits = camera->limits;
	margin = camera->margin;
	move_velocity_coefficient = camera->move_velocity_coefficient;
	position = camera->position;
	resize_velocity_coefficient = camera->resize_velocity_coefficient;
	size = camera->size;
}

void Camera::SetCoefficients(float margin, float move_velocity_coefficient, float resize_velocity_coefficient)
{
	this->margin = margin;
	this->move_velocity_coefficient = move_velocity_coefficient;
	this->resize_velocity_coefficient = resize_velocity_coefficient;
}

void Camera::SetLimits(float _min_x, float _min_y, float _max_x, float _max_y)
{
	limits.Set(_min_x, _min_y, _max_x, _max_y);
}

void Camera::SetPosition(Vec2F* position)
{
	this->position = *position;
}

void Camera::SetSize(float size)
{
	this->size *= size / this->size.y;
}

Camera::~Camera()
{
}