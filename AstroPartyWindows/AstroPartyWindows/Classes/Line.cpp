#include "Line.h"

#pragma warning (disable : 26495)

LightLine::LightLine()
{
}

LightLine::LightLine(const LightLine& line) :
	point(line.point),
	vector(line.vector)
{
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

void Line::Intersection(LightLine* intersection_line, Vec2F* output_intersection_point)
{

	temp_vector1 = point - intersection_line->point;
	*output_intersection_point = temp_vector1 + vector;

	temp_matrix1.Set(&temp_vector1, output_intersection_point);
	temp_matrix2 = temp_matrix1.InverseNotNormalize();
	temp_vector1 = intersection_line->vector * temp_matrix2 / temp_matrix1.Determinant();

	*output_intersection_point = temp_vector1 / (temp_vector1.x + temp_vector1.y) * temp_matrix1 + intersection_line->point;
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

float Line::GetDistance(Vec2F* target)
{
	temp_vector2;
	temp_vector2.Set(vector.y, -vector.x);
	temp_line1.Set(target, &temp_vector2);
	Intersection(&temp_line1, &temp_vector2);

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
	temp_vector1 = target->vector.Perpendicular();
	temp_line1.Set(&target->point, &temp_vector1);

	target->temp_vector1 = temp_line1.point - target->point;
	temp_matrix1.Set(&target->temp_vector1, &target->vector);
	temp_matrix2 = temp_matrix1.InverseNotNormalize();
	temp_vector2 = temp_line1.vector * temp_matrix2;
	temp_vector2.y = temp_vector2.y / -temp_vector2.x / temp_matrix1.Determinant();

	target->temp_vector1 = target->point + target->vector;
	temp_line1.Set(&target->temp_vector1, &temp_vector1);
	target->temp_vector1 = temp_line1.point - target->point;
	temp_matrix2.Set(&target->temp_vector1, &target->vector);
	target->temp_matrix1 = temp_matrix2.InverseNotNormalize();
	temp_vector2 = temp_line1.vector * target->temp_matrix1;
	temp_vector2.y = temp_vector2.y / -temp_vector2.x / temp_matrix2.Determinant();

	if (abs(temp_vector2.y) < abs(target->temp_vector1.y))
	{
		temp_vector2.x = 0.0;
		return (temp_vector2 * temp_matrix1 + target->point).GetDistance(&point);
	}

	target->temp_vector1.x = 0.0;
	return (target->temp_vector1 * temp_matrix1 + target->point).GetDistance(&point);
}

void Line::Set(Line* line)
{
	point = line->point;
	vector = line->vector;
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

bool Beam::Intersection(LightLine* intersection_line, Vec2F* output_intersection_point)
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

float Beam::GetDistance(Vec2F* target)
{
	temp_vector2 = vector.Perpendicular();
	temp_line1.Set(target, &temp_vector2);
	if (Intersection(&temp_line1, &temp_vector2))
	{
		return temp_vector2.GetDistance(target);
	}

	return point.GetDistance(target);
}

float Beam::GetDistance(Line* target)
{
	return target->GetDistance(this);
}

float Beam::GetDistance(Beam* target)
{
	if (IsIntersection(target))
	{
		return 0.0;
	}

	temp_float1 = target->GetDistance(&point);
	temp_bool1 = false;

	temp_vector1 = vector.Perpendicular();
	temp_line1.Set(&target->point, &temp_vector1);
	if (Intersection(&temp_line1, &temp_vector2))
	{
		 temp_float2 = temp_vector2.GetDistance(&target->point);
		 temp_bool1 = true;
	}

	target->temp_float1 =  point.GetDistance(&target->point);

	if (temp_bool1 == true && temp_float2 < temp_float1 && temp_float2 < target->temp_float1)
	{
		return temp_float2;
	}

	if (temp_float1 < target->temp_float1)
	{
		return temp_float1;
	}

	return target->temp_float1;
}

float Beam::GetDistance(Segment* target)
{
	if (IsIntersection(target))
	{
		return 0.0;
	}
	temp_vector1.Set(target->vector.y, -target->vector.x);
	temp_line1.Set(&point, &temp_vector1);
	temp_bool1 = false;
	if (target->Intersection(&temp_line1, &temp_vector2))
	{
		temp_bool1 = true;
		temp_float1 = temp_vector2.GetDistance(&point);
	}
	target->temp_vector1 = vector.Perpendicular();
	temp_line1.Set(&target->point, &target->temp_vector1);
	if (Intersection(&temp_line1, &temp_vector2))
	{
		temp_float2 = temp_vector2.GetDistance(&point);
		if (temp_bool1 == false)
		{
			temp_bool1 = true;
			temp_float1 = temp_float2;
		}
		else if (temp_float2 < temp_float1)
		{
			temp_float1 = temp_float2;
		}
	}
	target->temp_vector2 = target->point + target->vector;
	temp_line1.Set(&target->temp_vector2, &target->temp_vector1);
	if (Intersection(&temp_line1, &temp_vector2))
	{
		temp_float2 = temp_vector2.GetDistance(&point);
		if (temp_bool1 == false)
		{
			return temp_float2;
		}
		if (temp_float2 < temp_float1)
		{
			return temp_float2;
		}
		return temp_float1;
	}
	if (temp_bool1)
	{
		return temp_float1;
	}
	temp_float1 = GetDistance(&target->point);
	temp_float2 = GetDistance(&target->temp_vector2);
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

bool Segment::Intersection(LightLine* intersection_line, Vec2F* output_intersection_point)
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
	temp_vector1 = intersection_line->point - point;

	temp_matrix1.Set(temp_vector1.x, temp_vector1.x + intersection_line->vector.x, temp_vector1.y, temp_vector1.y + intersection_line->vector.y);
	temp_matrix2 = temp_matrix1.InverseNotNormalize();
	temp_vector1 = vector * temp_matrix2 / temp_matrix1.Determinant();

	return (temp_vector1.x + temp_vector1.y >= 1.0f);
}

bool Segment::IsIntersection(Beam* intersection_beam)
{
	temp_vector1 = intersection_beam->point - point;

	temp_matrix1.Set(temp_vector1.x, temp_vector1.x + intersection_beam->vector.x, temp_vector1.y, temp_vector1.y + intersection_beam->vector.y);
	temp_matrix2 = temp_matrix1.InverseNotNormalize();
	temp_vector1 = vector * temp_matrix2 / temp_matrix1.Determinant();

	return (temp_vector1.x >= 0 && temp_vector1.x + temp_vector1.y >= 1.0f);
}

bool Segment::IsIntersection(Segment* intersection_segment)
{
	temp_vector1 = point - intersection_segment->point;

	temp_matrix1.Set(temp_vector1.x, temp_vector1.y, temp_vector1.x + vector.x, temp_vector1.y + vector.y);
	temp_matrix2 = temp_matrix1.InverseNotNormalize();
	temp_vector1 = intersection_segment->vector * temp_matrix2 / temp_matrix1.Determinant();

	return (temp_vector1.x >= 0.0f && temp_vector1.y >= 0.0f && temp_vector1.x + temp_vector1.y >= 1.0f);
}

float Segment::GetDistance(Vec2F* target)
{
	temp_vector1 = vector.Perpendicular();
	temp_line1.Set(target, &temp_vector1);
	if (Intersection(&temp_line1, &temp_vector2))
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

float Segment::GetDistance(Line* target)
{
	return target->GetDistance(this);
}

float Segment::GetDistance(Beam* target)
{
	return target->GetDistance(this);
}

float Segment::GetDistance(Segment* target)
{
	if (IsIntersection(target))
	{
		return 0.0;
	}

	temp_vector1 = point + vector;
	temp_vector2 = target->point + target->vector;

	temp_float1 = GetDistance(&target->point);
	temp_float2 = GetDistance(&temp_vector2);

	target->temp_vector1 = target->vector.Perpendicular();
	temp_line1.Set(&point, &target->temp_vector1);
	if (Intersection(&temp_line1, &temp_vector2))
	{
		target->temp_float1 = temp_vector2.GetDistance(&point);
	}

	temp_line1.Set(&temp_vector1, &target->temp_vector1);
	if (Intersection(&temp_line1, &temp_vector2))
	{
		target->temp_float2 = temp_vector2.GetDistance(&temp_vector1);
	}

	if (temp_float1 < temp_float2)
	{
		if (target->temp_float1 < target->temp_float2)
		{
			if (temp_float1 < target->temp_float1)
			{
				return temp_float1;
			}
			return target->temp_float1;
		}
		if (temp_float1 < target->temp_float2)
		{
			return temp_float1;
		}
		return target->temp_float2;
	}
	if (target->temp_float1 < target->temp_float2)
	{
		if (temp_float2 < target->temp_float1)
		{
			return temp_float2;
		}
		return target->temp_float1;
	}
	if (temp_float2 < target->temp_float2)
	{
		return temp_float2;
	}
	return target->temp_float2;
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