#include "Line.h"

#define AS_LINE		1
#define AS_BEAM		2
#define AS_SEGMENT	3

#pragma warning (disable : 26495)

LightLine::LightLine()
{
}

LightLine::LightLine(const LightLine& line) :
	point(line.point),
	vector(line.vector)
{
}

void LightLine::Set(Vec2F point, Vec2F vector)
{
	this->point = point;
	this->vector = vector;
}

void LightLine::Set(Vec2F* point, Vec2F* vector)
{
	this->point = *point;
	this->vector = *vector;
}

LightLine::~LightLine()
{
}



Line::Line()
{
}

Line::Line(const Line& line) :
	point(line.point),
	vector(line.vector)
{
}

Line::Line(Vec2F point, Vec2F point_vector, bool second_argument_is_point) :
	point(point)
{
	if (second_argument_is_point)
	{
		vector = point_vector - point;
	}
	else
	{
		vector = point_vector;
	}
}

Line::Line(Vec2F* point, Vec2F* point_vector, bool second_argument_is_point) :
	point(*point)
{
	if (second_argument_is_point)
	{
		vector = *point_vector - *point;
	}
	else
	{
		vector = *point_vector;
	}
}

void Line::Intersection(Line* intersection_line, Vec2F* output_intersection_point)
{
	temp_vector1 = point - intersection_line->point;
	*output_intersection_point = temp_vector1 + vector;

	temp_matrix1.Set(&temp_vector1, output_intersection_point);
	temp_matrix2 = temp_matrix1.InverseNotNormalize();
	temp_vector1 = intersection_line->vector * temp_matrix2 / temp_matrix1.Determinant();

	*output_intersection_point = temp_vector1 / (temp_vector1.x + temp_vector1.y) * temp_matrix1 + intersection_line->point;
}

bool Line::Intersection(LightLine* intersection_line, Vec2F* output_intersection_point, uint8_t intersection_type)
{

	temp_vector1 = point - intersection_line->point;
	*output_intersection_point = temp_vector1 + vector;

	temp_matrix1.Set(&temp_vector1, output_intersection_point);
	temp_matrix2 = temp_matrix1.InverseNotNormalize();
	temp_vector1 = intersection_line->vector * temp_matrix2 / temp_matrix1.Determinant();

	if (intersection_type == AS_BEAM && temp_vector1.x + temp_vector1.y <= 0.0f ||
		intersection_type == AS_SEGMENT && temp_vector1.x + temp_vector1.y < 1.0f)
	{
		return false;
	}

	*output_intersection_point = temp_vector1 / (temp_vector1.x + temp_vector1.y) * temp_matrix1 + intersection_line->point;
	return true;
}

bool Line::Intersection(Beam* intersection_beam, Vec2F* output_intersection_point)
{
	temp_vector1 = point - intersection_beam->point;
	*output_intersection_point = temp_vector1 + vector;

	temp_matrix1.Set(&temp_vector1, output_intersection_point);
	temp_matrix2 = temp_matrix1.InverseNotNormalize();
	temp_vector1 = intersection_beam->vector * temp_matrix2 / temp_matrix1.Determinant();

	if (temp_vector1.x + temp_vector1.y <= 0.0f)
	{
		return false;
	}

	*output_intersection_point = temp_vector1 / (temp_vector1.x + temp_vector1.y) * temp_matrix1 + intersection_beam->point;

	return true;
}

bool Line::Intersection(Segment* intersection_segment, Vec2F* output_intersection_point)
{

	temp_vector1 = point - intersection_segment->point;
	*output_intersection_point = temp_vector1 + vector;

	temp_matrix1.Set(&temp_vector1, output_intersection_point);
	temp_matrix2 = temp_matrix1.InverseNotNormalize();
	temp_vector1 = intersection_segment->vector * temp_matrix2 / temp_matrix1.Determinant();

	if (temp_vector1.x + temp_vector1.y < 1.0f)
	{
		return false;
	}

	*output_intersection_point = temp_vector1 / (temp_vector1.x + temp_vector1.y) * temp_matrix1 + intersection_segment->point;

	return true;
}

bool Line::IsIntersection(Beam* intersection_beam)
{
	temp_vector1 = point - intersection_beam->point;

	temp_matrix1.Set(temp_vector1.x,  temp_vector1.x + vector.x, temp_vector1.y, temp_vector1.y + vector.y);
	temp_matrix2 = temp_matrix1.InverseNotNormalize();
	temp_vector1 = intersection_beam->vector * temp_matrix2 / temp_matrix1.Determinant();

	return (temp_vector1.x + temp_vector1.y > 0.0f);
}

bool Line::IsIntersection(Segment* intersection_segment)
{

	temp_vector1 = point - intersection_segment->point;

	temp_matrix1.Set(temp_vector1.x,  temp_vector1.x + vector.x, temp_vector1.y, temp_vector1.y + vector.y);
	temp_matrix2 = temp_matrix1.InverseNotNormalize();
	temp_vector1 = intersection_segment->vector * temp_matrix2 / temp_matrix1.Determinant();

	return (temp_vector1.x + temp_vector1.y >= 1.0f);
}

float Line::GetDistance(Vec2F target)
{
	temp_vector2;
	temp_vector2.Set(vector.y, -vector.x);
	temp_line1.Set(&target, &temp_vector2);
	Intersection(&temp_line1, &temp_vector2, AS_LINE);

	return temp_vector2.GetDistance(target);
}

float Line::GetDistance(Vec2F* target)
{
	temp_vector2;
	temp_vector2.Set(vector.y, -vector.x);
	temp_line1.Set(target, &temp_vector2);
	Intersection(&temp_line1, &temp_vector2, AS_LINE);

	return temp_vector2.GetDistance(target);
}

float Line::GetDistance(Vec2F target, Vec2F* nearest_point)
{
	temp_vector2;
	temp_vector2.Set(vector.y, -vector.x);
	temp_line1.Set(&target, &temp_vector2);
	Intersection(&temp_line1, nearest_point, AS_LINE);

	return temp_vector2.GetDistance(target);
}

float Line::GetDistance(Vec2F* target, Vec2F* nearest_point)
{
	temp_vector2;
	temp_vector2.Set(vector.y, -vector.x);
	temp_line1.Set(target, &temp_vector2);
	Intersection(&temp_line1, nearest_point, AS_LINE);

	return temp_vector2.GetDistance(target);
}

float Line::GetDistance(Line* target)
{
	if (target->vector == vector || target->vector == -vector)
	{
		return GetDistance(&target->point);
	}
	return 0.0f;
}

float Line::GetDistance(Beam* target)
{
	if (IsIntersection(target))
	{
		return 0.0;
	}

	return GetDistance(&target->point);
}

float Line::GetDistance(Segment* target)
{
	if (IsIntersection(target))
	{
		return 0.0;
	}
	
	temp_float1 = GetDistance(&target->point);
	temp_float2 = GetDistance(target->point + target->vector);
	if (temp_float1 > temp_float2)
	{
		return temp_float2;
	}
	return temp_float1;
}

void Line::Set(Line* line)
{
	point = line->point;
	vector = line->vector;
}

void Line::Set(Vec2F point, Vec2F point_vector, bool second_argument_is_point)
{
	this->point = point;
	if (second_argument_is_point == true)
	{
		this->vector = point_vector - point;
	}
	else
	{
		this->vector = point_vector;
	}
}

void Line::Set(Vec2F* point, Vec2F* point_vector, bool second_argument_is_point)
{
	this->point = *point;
	if (second_argument_is_point == true)
	{
		this->vector = *point_vector - *point;
	}
	else
	{
		this->vector = *point_vector;
	}
}

void Line::operator=(Line line)
{
	this->point = line.point;
	this->vector = line.vector;
}

void Line::operator=(Beam beam)
{
	this->point = beam.point;
	this->vector = beam.vector;
}

void Line::operator=(Segment segment)
{
	this->point = segment.point;
	this->vector = segment.vector;
}

Line::operator Beam()
{
	return Beam(&point, &vector);
}

Line::operator Segment()
{
	return Segment(&point, &vector);
}

Line::~Line()
{
}



Beam::Beam()
{

}

Beam::Beam(const Beam& beam) :
	point(beam.point),
	vector(beam.vector)
{
}

Beam::Beam(Vec2F point, Vec2F point_vector, bool second_argument_is_point) :
	point(point)
{
	if (second_argument_is_point)
	{
		vector = point_vector - point;
	}
	else
	{
		vector = point_vector;
	}
}

Beam::Beam(Vec2F* point, Vec2F* point_vector, bool second_argument_is_point) :
	point(*point)
{
	if (second_argument_is_point)
	{
		vector = *point_vector - *point;
	}
	else
	{
		vector = *point_vector;
	}
}

bool Beam::Intersection(Line* intersection_line, Vec2F* output_intersection_point)
{
	temp_vector1 = intersection_line->point - point;
	*output_intersection_point = temp_vector1 + intersection_line->vector;

	temp_matrix1.Set(&temp_vector1, output_intersection_point);
	temp_matrix2 = temp_matrix1.InverseNotNormalize();
	temp_vector1 = vector * temp_matrix2 / temp_matrix1.Determinant();

	if (temp_vector1.x + temp_vector1.y <= 0.0f)
	{
		return false;
	}

	*output_intersection_point = temp_vector1 / (temp_vector1.x + temp_vector1.y) * temp_matrix1 + point;

	return true;
}

bool Beam::Intersection(LightLine* intersection_line, Vec2F* output_intersection_point, uint8_t intersection_type)
{
	temp_vector1 = intersection_line->point - point;
	*output_intersection_point = temp_vector1 + intersection_line->vector;

	temp_matrix1.Set(&temp_vector1, output_intersection_point);
	temp_matrix2 = temp_matrix1.InverseNotNormalize();
	temp_vector1 = vector * temp_matrix2 / temp_matrix1.Determinant();

	if (intersection_type == AS_LINE && (temp_vector1.x + temp_vector1.y <= 0.0f) ||
		intersection_type == AS_BEAM && (temp_vector1.y < 0.0f || temp_vector1.x + temp_vector1.y <= 0.0f) ||
		intersection_type == AS_SEGMENT && (temp_vector1.y < 0.0f || temp_vector1.x + temp_vector1.y < 1.0f))
	{
		return false;
	}

	*output_intersection_point = temp_vector1 / (temp_vector1.x + temp_vector1.y) * temp_matrix1 + point;

	return true;
}

bool Beam::Intersection(Beam* intersection_beam, Vec2F* output_intersection_point)
{
	temp_vector1 = point - intersection_beam->point;
	*output_intersection_point = temp_vector1 + vector;

	temp_matrix1.Set(&temp_vector1, output_intersection_point);
	temp_matrix2 = temp_matrix1.InverseNotNormalize();
	temp_vector1 = intersection_beam->vector * temp_matrix2 / temp_matrix1.Determinant();

	if (temp_vector1.y < 0.0f || temp_vector1.x + temp_vector1.y <= 0.0f)
	{
		return false;
	}

	*output_intersection_point = temp_vector1 / (temp_vector1.x + temp_vector1.y) * temp_matrix1 + intersection_beam->point;

	return true;
}

bool Beam::Intersection(Segment* intersection_segment, Vec2F* output_intersection_point)
{
	temp_vector1 = point - intersection_segment->point;
	*output_intersection_point = temp_vector1 + vector;

	temp_matrix1.Set(&temp_vector1, output_intersection_point);
	temp_matrix2 = temp_matrix1.InverseNotNormalize();
	temp_vector1 = intersection_segment->vector * temp_matrix2 / temp_matrix1.Determinant();

	if (temp_vector1.y < 0.0f || temp_vector1.x + temp_vector1.y < 1.0f)
	{
		return false;
	}

	*output_intersection_point = temp_vector1 / (temp_vector1.x + temp_vector1.y) * temp_matrix1 + intersection_segment->point;

	return true;
}

bool Beam::IsIntersection(Line* intersection_line)
{
	temp_vector1 = intersection_line->point - point;

	temp_matrix1.Set(temp_vector1.x,  temp_vector1.x + intersection_line->vector.x, temp_vector1.y, temp_vector1.y + intersection_line->vector.y);
	temp_matrix2 = temp_matrix1.InverseNotNormalize();
	temp_vector1 = vector * temp_matrix2 / temp_matrix1.Determinant();

	return (temp_vector1.x + temp_vector1.y > 0.0f);
}

bool Beam::IsIntersection(Beam* intersection_beam)
{
	temp_vector1 = point - intersection_beam->point;

	temp_matrix1.Set(temp_vector1.x,  temp_vector1.x + vector.x, temp_vector1.y, temp_vector1.y + vector.y);
	temp_matrix2 = temp_matrix1.InverseNotNormalize();
	temp_vector1 = intersection_beam->vector * temp_matrix2 / temp_matrix1.Determinant();

	return (temp_vector1.y >= 0.0f && temp_vector1.x + temp_vector1.y > 0.0f);
}

bool Beam::IsIntersection(Segment* intersection_segment)
{
	temp_vector1 = point - intersection_segment->point;

	temp_matrix1.Set(temp_vector1.x, temp_vector1.x + vector.x, temp_vector1.y, temp_vector1.y + vector.y);
	temp_matrix2 = temp_matrix1.InverseNotNormalize();
	temp_vector1 = intersection_segment->vector * temp_matrix2 / temp_matrix1.Determinant();

	return (temp_vector1.y >= 0.0f && temp_vector1.x + temp_vector1.y >= 1.0f);
}

float Beam::GetDistance(Vec2F target)
{
	temp_vector2 = vector.Perpendicular();
	temp_line1.Set(&target, &temp_vector2);
	if (Intersection(&temp_line1, &temp_vector2, AS_LINE))
	{
		return temp_vector2.GetDistance(target);
	}

	return point.GetDistance(target);
}

float Beam::GetDistance(Vec2F* target)
{
	temp_vector2 = vector.Perpendicular();
	temp_line1.Set(target, &temp_vector2);
	if (Intersection(&temp_line1, &temp_vector2, AS_LINE))
	{
		return temp_vector2.GetDistance(target);
	}

	return point.GetDistance(target);
}

float Beam::GetDistance(Vec2F target, Vec2F* nearest_point)
{
	temp_vector2 = vector.Perpendicular();
	temp_line1.Set(&target, &temp_vector2);
	if (Intersection(&temp_line1, &temp_vector2, AS_LINE))
	{
		*nearest_point = temp_vector2;
		return temp_vector2.GetDistance(target);
	}

	*nearest_point = point;
	return point.GetDistance(target);
}

float Beam::GetDistance(Vec2F* target, Vec2F* nearest_point)
{
	temp_vector2 = vector.Perpendicular();
	temp_line1.Set(target, &temp_vector2);
	if (Intersection(&temp_line1, &temp_vector2, AS_LINE))
	{
		*nearest_point = temp_vector2;
		return temp_vector2.GetDistance(target);
	}

	*nearest_point = point;
	return point.GetDistance(target);
}

float Beam::GetDistance(Line* target)
{
	if (IsIntersection(target))
	{
		return 0.0;
	}

	return target->GetDistance(point);
}

float Beam::GetDistance(Beam* target)
{
	if (IsIntersection(target))
	{
		return 0.0;
	}

	temp_vector2 = target->vector.Perpendicular();
	temp_line1.Set(&target->point, &temp_vector2);
	if (Intersection(&temp_line1, &temp_vector2, AS_LINE))
	{
		temp_float1 = target->point.GetDistance(&temp_vector2);
		
		temp_vector2 = vector.Perpendicular();
		temp_line1.Set(&point, &temp_vector2);
		if (target->Intersection(&temp_line1, &temp_vector2, AS_LINE))
		{
			temp_float2 = point.GetDistance(&temp_vector2);
		}

		if (temp_float1 > temp_float2)
		{
			return temp_float2;
		}
		return temp_float1;
	}

	temp_vector2 = vector.Perpendicular();
	temp_line1.Set(&point, &temp_vector2);
	if (target->Intersection(&temp_line1, &temp_vector2, AS_LINE))
	{
		temp_float2 = point.GetDistance(&temp_vector2);
	}

	return point.GetDistance(target->point);
}

float Beam::GetDistance(Segment* target)
{
	if (IsIntersection(target))
	{
		return 0.0;
	}

	//Get distance between line and point

	temp_vector2 = target->vector.Perpendicular();
	temp_line1.Set(&target->point, &temp_vector2);
	//dist from target->point to this 
	if (Intersection(&temp_line1, &temp_vector2, AS_LINE))
	{
		temp_float1 = target->point.GetDistance(temp_vector2);
		
		temp_vector2 = target->vector.Perpendicular();
		temp_line1.Set(target->point + target->vector, temp_vector2);
		//dist from target->point2 to this 
		if (Intersection(&temp_line1, &temp_vector2, AS_LINE))
		{
			temp_float2 = target->point.GetDistance(temp_vector2);

			temp_vector2 = vector.Perpendicular();
			temp_line1.Set(&point, &temp_vector2);
			//dist from this->point to target
			if (target->Intersection(&temp_line1, &temp_vector2, AS_LINE))
			{
				if (temp_float1 > temp_float2)
				{
					temp_float1 = point.GetDistance(temp_vector2);
				}
				else
				{
					temp_float2 = point.GetDistance(temp_vector2);
				}

				if (temp_float1 > temp_float2)
				{
					return temp_float2;
				}
				return temp_float1;
			}
			return temp_float2;
		}
		temp_vector2 = vector.Perpendicular();
		temp_line1.Set(&point, &temp_vector2);
		//dist from this->point to target
		if (target->Intersection(&temp_line1, &temp_vector2, AS_LINE))
		{
			temp_float2 = point.GetDistance(temp_vector2);

			if (temp_float1 > temp_float2)
			{
				return temp_float2;
			}
			return temp_float1;
		}
		return temp_float1;
	}

	temp_vector2 = target->vector.Perpendicular();
	temp_line1.Set(target->point + target->vector, temp_vector2);
	//dist from target->point2 to this 
	if (Intersection(&temp_line1, &temp_vector2, AS_LINE))
	{
		temp_float1 = target->point.GetDistance(temp_vector2);

		temp_vector2 = vector.Perpendicular();
		temp_line1.Set(&point, &temp_vector2);
		//dist from this->point to target
		if (target->Intersection(&temp_line1, &temp_vector2, AS_LINE))
		{
			temp_float2 = point.GetDistance(temp_vector2);

			if (temp_float1 > temp_float2)
			{
				return temp_float2;
			}
			return temp_float1;
		}
		return temp_float1;
	}

	temp_vector2 = vector.Perpendicular();
	temp_line1.Set(&point, &temp_vector2);
	//dist from this->point to target
	if (target->Intersection(&temp_line1, &temp_vector2, AS_LINE))
	{
		return point.GetDistance(temp_vector2);
	}
	
	//Get distance between points

	temp_float1 = target->point.GetDistance(&point);
	temp_float2 = (target->point + target->vector).GetDistance(&point);

	if (temp_float1 > temp_float2)
	{
		return temp_float2;
	}
	return temp_float1;
}

void Beam::Set(Beam* beam)
{
	point = beam->point;
	vector = beam->vector;
}

void Beam::Set(Vec2F point, Vec2F point_vector, bool second_argument_is_point)
{
	this->point = point;
	if (second_argument_is_point == true)
	{
		this->vector = point_vector - point;
	}
	else
	{
		this->vector = point_vector;
	}
}

void Beam::Set(Vec2F* point, Vec2F* point_vector, bool second_argument_is_point)
{
	this->point = *point;
	if (second_argument_is_point == true)
	{
		this->vector = *point_vector - *point;
	}
	else
	{
		this->vector = *point_vector;
	}
}

void Beam::operator=(Line line)
{
	this->point = line.point;
	this->vector = line.vector;
}

void Beam::operator=(Beam beam)
{
	this->point = beam.point;
	this->vector = beam.vector;
}

void Beam::operator=(Segment segment)
{
	this->point = segment.point;
	this->vector = segment.vector;
}

Beam::operator Line()
{
	return Line(&point, &vector);
}

Beam::operator Segment()
{
	return Segment(&point, &vector);
}

Beam::~Beam()
{
}



Segment::Segment()
{

}

Segment::Segment(const Segment& segment) :
	point(segment.point),
	vector(segment.vector)
{
}

Segment::Segment(Vec2F point, Vec2F point_vector, bool second_argument_is_point) :
	point(point)
{
	if (second_argument_is_point)
	{
		vector = point_vector - point;
	}
	else
	{
		vector = point_vector;
	}
}

Segment::Segment(Vec2F* point, Vec2F* point_vector, bool second_argument_is_point) : 
	point(*point)
{
	if (second_argument_is_point)
	{
		vector = *point_vector - *point;
	}
	else
	{
		vector = *point_vector;
	}
}

bool Segment::Intersection(Line* intersection_line, Vec2F* output_intersection_point)
{
	temp_vector1 = intersection_line->point - point;
	*output_intersection_point = temp_vector1 + intersection_line->vector;

	temp_matrix1.Set(&temp_vector1, output_intersection_point);
	temp_matrix2 = temp_matrix1.InverseNotNormalize();
	temp_vector1 = vector * temp_matrix2 / temp_matrix1.Determinant();

	if (temp_vector1.x + temp_vector1.y < 1.0f)
	{
		return false;
	}

	*output_intersection_point = temp_vector1 / (temp_vector1.x + temp_vector1.y) * temp_matrix1 + point;

	return true;
}

bool Segment::Intersection(LightLine* intersection_line, Vec2F* output_intersection_point, uint8_t intersection_type)
{
	temp_vector1 = intersection_line->point - point;
	*output_intersection_point = temp_vector1 + intersection_line->vector;

	temp_matrix1.Set(&temp_vector1, output_intersection_point);
	temp_matrix2 = temp_matrix1.InverseNotNormalize();
	temp_vector1 = vector * temp_matrix2 / temp_matrix1.Determinant();

	if (intersection_type == AS_LINE && (temp_vector1.x + temp_vector1.y < 1.0f) ||
		intersection_type == AS_BEAM && (temp_vector1.y < 0.0f || temp_vector1.x + temp_vector1.y < 1.0f) ||
		intersection_type == AS_SEGMENT && (temp_vector1.x < 0.0f || temp_vector1.y < 0.0f || temp_vector1.x + temp_vector1.y < 1.0f))
	{
		return false;
	}

	*output_intersection_point = temp_vector1 / (temp_vector1.x + temp_vector1.y) * temp_matrix1 + point;

	return true;
}

bool Segment::Intersection(Beam* intersection_beam, Vec2F* output_intersection_point)
{
	temp_vector1 = intersection_beam->point - point;
	*output_intersection_point = temp_vector1 + intersection_beam->vector;

	temp_matrix1.Set(&temp_vector1, output_intersection_point);
	temp_matrix2 = temp_matrix1.InverseNotNormalize();
	temp_vector1 = vector * temp_matrix2 / temp_matrix1.Determinant();

	if (temp_vector1.y < 0.0f || temp_vector1.x + temp_vector1.y < 1.0f)
	{
		return false;
	}

	*output_intersection_point = temp_vector1 / (temp_vector1.x + temp_vector1.y) * temp_matrix1 + point;

	return true;
}

bool Segment::Intersection(Segment* intersection_segment, Vec2F* output_intersection_point)
{
	temp_vector1 = point - intersection_segment->point;
	*output_intersection_point = temp_vector1 + vector;

	temp_matrix1.Set(&temp_vector1, output_intersection_point);
	temp_matrix2 = temp_matrix1.InverseNotNormalize();
	temp_vector1 = intersection_segment->vector * temp_matrix2 / temp_matrix1.Determinant();

	if (temp_vector1.x < 0.0f || temp_vector1.y < 0.0f || temp_vector1.x + temp_vector1.y < 1.0f)
	{
		return false;
	}

	*output_intersection_point = temp_vector1 / (temp_vector1.x + temp_vector1.y) * temp_matrix1 + intersection_segment->point;

	return true;
}

bool Segment::IsIntersection(Line* intersection_line)
{
	temp_vector1 = point - intersection_line->point;

	temp_matrix1.Set(temp_vector1, temp_vector1 + vector);
	temp_matrix2 = temp_matrix1.InverseNotNormalize();
	temp_vector1 = intersection_line->vector * temp_matrix2 / temp_matrix1.Determinant();

	return (temp_vector1.x + temp_vector1.y >= 1.0f);
}

bool Segment::IsIntersection(Beam* intersection_beam)
{
	temp_vector1 = point - intersection_beam->point;

	temp_matrix1.Set(temp_vector1, temp_vector1 + vector);
	temp_matrix2 = temp_matrix1.InverseNotNormalize();
	temp_vector1 = intersection_beam->vector * temp_matrix2 / temp_matrix1.Determinant();

	return (temp_vector1.x >= 0 && temp_vector1.x + temp_vector1.y >= 1.0f);
}

bool Segment::IsIntersection(Segment* intersection_segment)
{
	temp_vector1 = point - intersection_segment->point;

	temp_matrix1.Set(temp_vector1, temp_vector1 + vector);
	temp_matrix2 = temp_matrix1.InverseNotNormalize();
	temp_vector1 = intersection_segment->vector * temp_matrix2 / temp_matrix1.Determinant();

	return temp_vector1.x >= 0.0f && temp_vector1.y >= 0.0f && temp_vector1.x + temp_vector1.y >= 1.0f;
}

float Segment::GetDistance(Vec2F target)
{
	temp_vector1 = vector.Perpendicular();
	temp_line1.Set(&target, &temp_vector1);
	if (Intersection(&temp_line1, &temp_vector2, AS_LINE))
	{
		return temp_vector2.GetDistance(target);
	}

	temp_float1 = point.GetDistance(target);
	temp_float2 = (point + vector).GetDistance(target);

	if (temp_float1 > temp_float2)
	{
		return temp_float2;
	}
	
	return temp_float1;
}

float Segment::GetDistance(Vec2F* target)
{
	temp_vector1 = vector.Perpendicular();
	temp_line1.Set(target, &temp_vector1);
	if (Intersection(&temp_line1, &temp_vector2, AS_LINE))
	{
		return temp_vector2.GetDistance(target);
	}

	temp_float1 = point.GetDistance(target);
	temp_float2 = (point + vector).GetDistance(target);

	if (temp_float1 > temp_float2)
	{
		return temp_float2;
	}

	return temp_float1;
}

float Segment::GetDistance(Vec2F target, Vec2F* nearest_point)
{
	temp_vector1 = vector.Perpendicular();
	temp_line1.Set(&target, &temp_vector1);
	if (Intersection(&temp_line1, &temp_vector2, AS_LINE))
	{
		*nearest_point = temp_vector2;
		return temp_vector2.GetDistance(target);
	}

	temp_float1 = point.GetDistance(target);
	temp_float2 = (point + vector).GetDistance(target);

	if (temp_float1 > temp_float2)
	{
		*nearest_point = point + vector;
		return temp_float2;
	}

	*nearest_point = point;
	return temp_float1;
}

float Segment::GetDistance(Vec2F* target, Vec2F* nearest_point)
{
	temp_vector1 = vector.Perpendicular();
	temp_line1.Set(target, &temp_vector1);
	if (Intersection(&temp_line1, &temp_vector2, AS_LINE))
	{
		*nearest_point = temp_vector2;
		return temp_vector2.GetDistance(target);
	}

	temp_float1 = point.GetDistance(target);
	temp_float2 = (point + vector).GetDistance(target);

	if (temp_float1 > temp_float2)
	{
		*nearest_point = point + vector;
		return temp_float2;
	}

	*nearest_point = point;
	return temp_float1;
}

float Segment::GetDistance(Line* target)
{
	if (IsIntersection(target))
	{
		return 0.0;
	}

	temp_float1 = target->GetDistance(point);
	temp_float2 = target->GetDistance(point + vector);

	if (target->temp_float1 > target->temp_float2)
	{
		return target->temp_float2;
	}
	return target->temp_float1;
}

float Segment::GetDistance(Beam* target)
{
	if (IsIntersection(target))
	{
		return 0.0;
	}

	//Get distance between line and point

	temp_vector2 = vector.Perpendicular();
	temp_line1.Set(&point, &temp_vector2);
	//dist from this->point to target 
	if (target->Intersection(&temp_line1, &temp_vector2, AS_LINE))
	{
		temp_float1 = point.GetDistance(temp_vector2);

		temp_line1.Set(point + vector, temp_vector2);
		//dist from this->point2 to target 
		if (target->Intersection(&temp_line1, &temp_vector2, AS_LINE))
		{
			temp_float2 = point.GetDistance(temp_vector2);

			temp_vector2 = target->vector.Perpendicular();
			temp_line1.Set(&target->point, &temp_vector2);
			//dist from target->point to this
			if (Intersection(&temp_line1, &temp_vector2, AS_LINE))
			{
				if (temp_float1 > temp_float2)
				{
					temp_float1 = target->point.GetDistance(temp_vector2);
				}
				else
				{
					temp_float2 = target->point.GetDistance(temp_vector2);
				}

				if (temp_float1 > temp_float2)
				{
					return temp_float2;
				}
				return temp_float1;
			}
			return temp_float2;
		}
		temp_vector2 = target->vector.Perpendicular();
		temp_line1.Set(&target->point, &temp_vector2);
		//dist from target->point to this
		if (Intersection(&temp_line1, &temp_vector2, AS_LINE))
		{
			temp_float2 = target->point.GetDistance(temp_vector2);

			if (temp_float1 > temp_float2)
			{
				return temp_float2;
			}
			return temp_float1;
		}
		return temp_float1;
	}

	temp_vector2 = vector.Perpendicular();
	temp_line1.Set(point + vector, temp_vector2);
	//dist from this->point2 to target 
	if (target->Intersection(&temp_line1, &temp_vector2, AS_LINE))
	{
		temp_float1 = point.GetDistance(temp_vector2);

		temp_vector2 = target->vector.Perpendicular();
		temp_line1.Set(&target->point, &temp_vector2);
		//dist from target->point to this
		if (Intersection(&temp_line1, &temp_vector2, AS_LINE))
		{
			temp_float2 = target->point.GetDistance(temp_vector2);

			if (temp_float1 > temp_float2)
			{
				return temp_float2;
			}
			return temp_float1;
		}
		return temp_float1;
	}

	temp_vector2 = target->vector.Perpendicular();
	temp_line1.Set(&target->point, &temp_vector2);
	//dist from target->point to this
	if (Intersection(&temp_line1, &temp_vector2, AS_LINE))
	{
		return target->point.GetDistance(temp_vector2);
	}

	temp_float1 = point.GetDistance(target->point);
	temp_float2 = (point + vector).GetDistance(target->point);

	if (temp_float1 > temp_float2)
	{
		return temp_float2;
	}
	return temp_float1;
}

float Segment::GetDistance(Segment* target)
{
	if (IsIntersection(target))
	{
		return 0.0;
	}

	//Get distance between point and line.

	temp_vector2 = target->vector.Perpendicular();
	temp_line1.Set(&target->point, &temp_vector2);
	//dist from target->point to this
	if (Intersection(&temp_line1, &temp_vector2, AS_LINE))
	{
		temp_float1 = target->point.GetDistance(&temp_vector2);

		temp_vector2 = target->vector.Perpendicular();
		temp_line1.Set(target->point + target->vector, temp_vector2);
		//dist from target->point2 to this
		if (Intersection(&temp_line1, &temp_vector2, AS_LINE))
		{
			temp_float2 = (target->point + target->vector).GetDistance(&temp_vector2);

			temp_vector2 = vector.Perpendicular();
			temp_line1.Set(&point, &temp_vector2);
			//dist from this->point to target
			if (target->Intersection(&temp_line1, &temp_vector2, AS_LINE))
			{
				if (temp_float1 > temp_float2)
				{
					temp_float1 = point.GetDistance(&temp_vector2);
				}
				else
				{
					temp_float2 = point.GetDistance(&temp_vector2);
				}
			}

			temp_vector2 = vector.Perpendicular();
			temp_line1.Set(point + vector, temp_vector2);
			//dist from this->point2 to target
			if (target->Intersection(&temp_line1, &temp_vector2, AS_LINE))
			{
				if (temp_float1 > temp_float2)
				{
					temp_float1 = (point + vector).GetDistance(&temp_vector2);
				}
				else
				{
					temp_float2 = (point + vector).GetDistance(&temp_vector2);
				}
			}
			
			if (temp_float1 > temp_float2)
			{
				return temp_float2;
			}
			return temp_float1;
		}

		temp_vector2 = vector.Perpendicular();
		temp_line1.Set(&point, &temp_vector2);
		//dist from this->point to target
		if (target->Intersection(&temp_line1, &temp_vector2, AS_LINE))
		{
			temp_float2 = point.GetDistance(&temp_vector2);

			temp_vector2 = vector.Perpendicular();
			temp_line1.Set(point + vector, temp_vector2);
			//dist from this->point2 to target
			if (target->Intersection(&temp_line1, &temp_vector2, AS_LINE))
			{
				if (temp_float1 > temp_float2)
				{
					temp_float1 = (point + vector).GetDistance(&temp_vector2);
				}
				else
				{
					temp_float2 = (point + vector).GetDistance(&temp_vector2);
				}
			}

			if (temp_float1 > temp_float2)
			{
				return temp_float2;
			}
			return temp_float1;
		}

		temp_vector2 = vector.Perpendicular();
		temp_line1.Set(point + vector, temp_vector2);
		//dist from this->point2 to target
		if (target->Intersection(&temp_line1, &temp_vector2, AS_LINE))
		{
			temp_float2 = (point + vector).GetDistance(&temp_vector2);

			if (temp_float1 > temp_float2)
			{
				return temp_float2;
			}
			return temp_float1;
		}

		return temp_float1;
	}

	temp_vector2 = target->vector.Perpendicular();
	temp_line1.Set(target->point + target->vector, temp_vector2);
	//dist from target->point2 to this
	if (Intersection(&temp_line1, &temp_vector2, AS_LINE))
	{
		temp_float1 = (target->point + target->vector).GetDistance(&temp_vector2);

		temp_vector2 = vector.Perpendicular();
		temp_line1.Set(&point, &temp_vector2);
		//dist from this->point to target
		if (target->Intersection(&temp_line1, &temp_vector2, AS_LINE))
		{
			temp_float2 = point.GetDistance(&temp_vector2);

			temp_vector2 = vector.Perpendicular();
			temp_line1.Set(point + vector, temp_vector2);
			//dist from this->point2 to target
			if (target->Intersection(&temp_line1, &temp_vector2, AS_LINE))
			{
				if (temp_float1 > temp_float2)
				{
					temp_float1 = (point + vector).GetDistance(&temp_vector2);
				}
				else
				{
					temp_float2 = (point + vector).GetDistance(&temp_vector2);
				}
			}

			if (temp_float1 > temp_float2)
			{
				return temp_float2;
			}
			return temp_float1;
		}
		return temp_float1;
	}

	temp_vector2 = vector.Perpendicular();
	temp_line1.Set(&point, &temp_vector2);
	//dist from this->point to target
	if (target->Intersection(&temp_line1, &temp_vector2, AS_LINE))
	{
		temp_float1 = point.GetDistance(&temp_vector2);

		temp_vector2 = vector.Perpendicular();
		temp_line1.Set(point + vector, temp_vector2);
		//dist from this->point2 to target
		if (target->Intersection(&temp_line1, &temp_vector2, AS_LINE))
		{
			temp_float2 = (point + vector).GetDistance(&temp_vector2);
			
			if (temp_float1 > temp_float2)
			{
				return temp_float2;
			}
			return temp_float1;
		}

		return temp_float1;
	}


	temp_vector2 = vector.Perpendicular();
	temp_line1.Set(point + vector, temp_vector2);
	//dist from this->point2 to target
	if (target->Intersection(&temp_line1, &temp_vector2, AS_LINE))
	{
		return (point + vector).GetDistance(&temp_vector2);
	}

	//Get distance between points.

	temp_float1 = point.GetDistance(target->point);
	temp_float2 = point.GetDistance(target->point + target->vector);

	if (temp_float1 > temp_float2)
	{
		temp_float1 = (point + vector).GetDistance(target->point);
	}
	else
	{
		temp_float2 = (point + vector).GetDistance(target->point);
	}

	if (temp_float1 > temp_float2)
	{
		temp_float1 = (point + vector).GetDistance(target->point + target->vector);
	}
	else
	{
		temp_float2 = (point + vector).GetDistance(target->point + target->vector);
	}

	if (temp_float1 > temp_float2)
	{
		return temp_float2;
	}
	return temp_float1;
}

Vec2F Segment::GetSecondPoint()
{
	return point + vector;
}

void Segment::Set(Segment* segment)
{
	point = segment->point;
	vector = segment->vector;
}

void Segment::Set(Vec2F point, Vec2F point_vector, bool second_argument_is_point)
{
	this->point = point;
	if (second_argument_is_point == true)
	{
		this->vector = point_vector - point;
	}
	else
	{
		this->vector = point_vector;
	}
}

void Segment::Set(Vec2F* point, Vec2F* point_vector, bool second_argument_is_point)
{
	this->point = *point;
	if (second_argument_is_point == true)
	{
		this->vector = *point_vector - *point;
	}
	else
	{
		this->vector = *point_vector;
	}
}

void Segment::operator=(Line line)
{
	this->point = line.point;
	this->vector = line.vector;
}

void Segment::operator=(Beam beam)
{
	this->point = beam.point;
	this->vector = beam.vector;
}

void Segment::operator=(Segment segment)
{
	this->point = segment.point;
	this->vector = segment.vector;
}

Segment::operator Line()
{
	return Line(&point, &vector);
}

Segment::operator Beam()
{
	return Beam(&point, &vector);
}

Segment::~Segment()
{
}