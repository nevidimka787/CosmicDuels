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
	new_view_area_size_x(camera.new_view_area_size_x),
	view_area_size_x(camera.view_area_size_x)
{
}

Camera::Camera(
	const Vec2F* position,
	float size,
	float scale,
	float margin, 
	float move_velocity_coefficient,
	float resize_velocity_coefficient,
	const Mat2F* hight_limits,
	const Vec2F* low_limits)
	:
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
	this->new_view_area_size_x = size;
	this->view_area_size_x = size;
}

Mat2F Camera::GetLimits() const
{
	return hight_limits;
}

Vec2F Camera::GetPosition() const
{
	return position;
}

float Camera::GetSize() const
{
	return view_area_size_x;
}

float Camera::GetScale() const
{
	return scale;
}

void Camera::Focus(
	const Ship* ships_array,
	const Pilot* pilots_array,
	GameTypes::players_count_t ships_count,
	GameTypes::players_count_t pilots_count)
{
#define MAX_X		0x01
#define MAX_Y		0x02
#define MIN_X		0x04
#define MIN_Y		0x08
#define UPDATE_F	0xFF
#define UPDATE_N	0x00

	Mat2F current_limits;
	Vec2F pos_buff;
	uint8_t first_update = MAX_X | MAX_Y | MIN_X | MIN_Y;

	const Ship* temp__ship_p = ships_array;
	const Pilot* temp__pilot_p = pilots_array;

	for (GameTypes::players_count_t found_ships = 0; found_ships < ships_count; temp__ship_p++)
	{
		if (temp__ship_p->exist)
		{
			pos_buff = temp__ship_p->GetPosition();
			if (first_update & MAX_X)
			{
				first_update &= UPDATE_F - MAX_X;
				current_limits.max_x = pos_buff.x;
			}
			else if (pos_buff.x > current_limits.max_x)
			{
				current_limits.max_x = pos_buff.x;
			}

			if (first_update & MAX_Y)
			{
				first_update &= UPDATE_F - MAX_Y;
				current_limits.max_y = pos_buff.y;
			}
			else if (pos_buff.y > current_limits.max_y)
			{
				current_limits.max_y = pos_buff.y;
			}

			if (first_update & MIN_X)
			{
				first_update &= UPDATE_F - MIN_X;
				current_limits.min_x = pos_buff.x;
			}
			else if (pos_buff.x < current_limits.min_x)
			{
				current_limits.min_x = pos_buff.x;
			}

			if (first_update & MIN_Y)
			{
				first_update &= UPDATE_F - MIN_Y;
				current_limits.min_y = pos_buff.y;
			}
			else if (pos_buff.y < current_limits.min_y)
			{
				current_limits.min_y = pos_buff.y;
			}

			found_ships++;
		}
	}

	for (GameTypes::players_count_t found_pilots = 0; found_pilots < pilots_count; temp__pilot_p++)
	{
		if (temp__pilot_p->exist)
		{
			pos_buff = temp__pilot_p->GetPosition();
			if (first_update & MAX_X)
			{
				first_update &= UPDATE_F - MAX_X;
				current_limits.max_x = pos_buff.x;
			}
			else if (pos_buff.x > current_limits.max_x)
			{
				current_limits.max_x = pos_buff.x;
			}

			if (first_update & MAX_Y)
			{
				first_update &= UPDATE_F - MAX_Y;
				current_limits.max_y = pos_buff.y;
			}
			else if (pos_buff.y > current_limits.max_y)
			{
				current_limits.max_y = pos_buff.y;
			}

			if (first_update & MIN_X)
			{
				first_update &= UPDATE_F - MIN_X;
				current_limits.min_x = pos_buff.x;
			}
			else if (pos_buff.x < current_limits.min_x)
			{
				current_limits.min_x = pos_buff.x;
			}

			if (first_update & MIN_Y)
			{
				first_update &= UPDATE_F - MIN_Y;
				current_limits.min_y = pos_buff.y;
			}
			else if (pos_buff.y < current_limits.min_y)
			{
				current_limits.min_y = pos_buff.y;
			}

			found_pilots++;
		}
	}

	if (first_update)
	{
		return;
	}

	if (current_limits.max_x > hight_limits.max_x)
	{
		current_limits.max_x = hight_limits.max_x;
	}
	if (current_limits.max_y > hight_limits.max_y)
	{
		current_limits.max_y = hight_limits.max_y;
	}
	if (current_limits.min_x < hight_limits.min_x)
	{
		current_limits.min_x = hight_limits.min_x;
	}
	if (current_limits.min_y < hight_limits.min_y)
	{
		current_limits.min_y = hight_limits.min_y;
	}

	pos_buff.Set((current_limits.max_x + current_limits.min_x) / 2.0f, (current_limits.max_y + current_limits.min_y) / 2.0f);

	new_view_area_size_x = fmaxf(current_limits.max_x - current_limits.min_x, (current_limits.max_y - current_limits.min_y) * scale) / 2.0f;
	new_view_area_size_x += margin;
	if (new_view_area_size_x < low_limits.x || new_view_area_size_x < low_limits.y * scale)
	{
		new_view_area_size_x = fmaxf(low_limits.x, low_limits.y * scale);
	}

	position = (position + pos_buff * move_velocity_coefficient) / (1.0f + move_velocity_coefficient);
	view_area_size_x = (view_area_size_x + new_view_area_size_x * move_velocity_coefficient) / (1.0f + move_velocity_coefficient);
}

void Camera::Set(Camera* camera)
{
	hight_limits = camera->hight_limits;
	low_limits = camera->low_limits;
	margin = camera->margin;
	move_velocity_coefficient = camera->move_velocity_coefficient;
	position = camera->position;
	resize_velocity_coefficient = camera->resize_velocity_coefficient;
	new_view_area_size_x = camera->new_view_area_size_x;
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

void Camera::SetPosition(Vec2F position)
{
	this->position = position;
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
	this->new_view_area_size_x = size;
	this->view_area_size_x = size;
}

void Camera::UpdateLimits()
{
	if (temp_flag)
	{
		temp_limits.max_x = temp_position.x;
		temp_limits.min_x = temp_position.x;
		temp_limits.max_y = temp_position.y;
		temp_limits.min_y = temp_position.y;
		temp_flag = false;
	}
	else
	{
		if (temp_position.x > temp_limits.max_x)
		{
			temp_limits.max_x = temp_position.x;
		}
		else if (temp_position.x < temp_limits.min_x)
		{
			temp_limits.min_x = temp_position.x;
		}
		if (temp_position.y > temp_limits.max_y)
		{
			temp_limits.max_y = temp_position.y;
		}
		else if (temp_position.y < temp_limits.min_y)
		{
			temp_limits.min_y = temp_position.y;
		}
	}
}

Camera::~Camera()
{
}