#include "Line.h"

#pragma warning (disable : 26495)

Line::Line()
{
}

Line::Line(const Line& line) :
	point(line.point),
	vector(line.vector)
{
}

Line::Line(const Vec2F&point, const Vec2F&point_vector, bool second_argument_is_point) :
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

Line::Line(const Vec2F* point, const Vec2F* point_vector, bool second_argument_is_point) :
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

Line::Line(const Beam& beam) :
	point(beam.point),
	vector(beam.vector)
{
}

Line::Line(const Beam* beam) :
	point(beam->point),
	vector(beam->vector)
{
}

Line::Line(const Segment& segment) :
	point(segment.point),
	vector(segment.vector)
{
}

Line::Line(const Segment* segment) :
	point(segment->point),
	vector(segment->vector)
{
}

void Line::Intersection(const Line* intersection_line, Vec2F* output_intersection_point) const
{
	Vec2F temp_vector1 = point - intersection_line->point;
	*output_intersection_point = temp_vector1 + vector;

	Mat2F temp_matrix1 = Mat2F(&temp_vector1, output_intersection_point);
	temp_vector1 = intersection_line->vector * temp_matrix1.Inverse();

	*output_intersection_point = temp_vector1 / (temp_vector1.x + temp_vector1.y) * temp_matrix1 + intersection_line->point;
}

bool Line::Intersection(const Beam* intersection_beam, Vec2F* output_intersection_point) const
{
	Vec2F temp_vector1 = point - intersection_beam->point;
	*output_intersection_point = temp_vector1 + vector;

	Mat2F temp_matrix1 = Mat2F(&temp_vector1, output_intersection_point);
	temp_vector1 = intersection_beam->vector * temp_matrix1.Inverse();

	if (temp_vector1.x + temp_vector1.y <= 0.0f)
	{
		return false;
	}

	*output_intersection_point = temp_vector1 / (temp_vector1.x + temp_vector1.y) * temp_matrix1 + intersection_beam->point;

	return true;
}

bool Line::Intersection(const Segment* intersection_segment, Vec2F* output_intersection_point) const
{

	Vec2F temp_vector1 = point - intersection_segment->point;
	*output_intersection_point = temp_vector1 + vector;

	Mat2F temp_matrix1 = Mat2F(&temp_vector1, output_intersection_point);
	Mat2F temp_matrix2 = temp_matrix1.InverseNotNormalize();
	temp_vector1 = intersection_segment->vector * temp_matrix2 / temp_matrix1.Determinant();

	if (temp_vector1.x + temp_vector1.y < 1.0f)
	{
		return false;
	}

	*output_intersection_point = temp_vector1 / (temp_vector1.x + temp_vector1.y) * temp_matrix1 + intersection_segment->point;

	return true;
}

bool Line::IsIntersection(const Beam* intersection_beam) const
{
	Vec2F temp_vector1 = point - intersection_beam->point;
	temp_vector1 = intersection_beam->vector * Mat2F(temp_vector1, temp_vector1 + vector).Inverse();

	return temp_vector1.x + temp_vector1.y > 0.0f;
}

bool Line::IsIntersection(const Segment* intersection_segment) const
{
	Vec2F temp_vector1 = point - intersection_segment->point;
	temp_vector1 = intersection_segment->vector * Mat2F(temp_vector1, temp_vector1 + vector).Inverse();

	return temp_vector1.x + temp_vector1.y >= 1.0f;
}

float Line::Distance(const Vec2F& target) const
{
	Vec2F temp_vector2 = Vec2F(vector.y, -vector.x);
	Line temp_line1 = Line(target, temp_vector2);
	Intersection(&temp_line1, &temp_vector2);

	return temp_vector2.Distance(target);
}

float Line::Distance(const Vec2F* target) const
{
	Vec2F temp_vector2 = Vec2F(vector.y, -vector.x);
	Line temp_line1 = Line(target, &temp_vector2);
	Intersection(&temp_line1, &temp_vector2);

	return temp_vector2.Distance(target);
}

float Line::Distance(const Vec2F& target, Vec2F* nearest_point) const
{
	Vec2F temp_vector2 = Vec2F(vector.y, -vector.x);
	Line temp_line1 = Line(&target, &temp_vector2);
	Intersection(&temp_line1, nearest_point);

	return nearest_point->Distance(target);
}

float Line::Distance(const Vec2F* target, Vec2F* nearest_point) const
{
	Vec2F temp_vector2 = Vec2F(vector.y, -vector.x);
	Line temp_line1 = Line(target, &temp_vector2);
	Intersection(&temp_line1, nearest_point);

	return temp_vector2.Distance(target);
}

float Line::Distance(const Line* target) const
{
	if (target->vector == vector || target->vector == -vector)
	{
		return Distance(&target->point);
	}
	return 0.0f;
}

float Line::Distance(const Beam* target) const
{
	if (IsIntersection(target))
	{
		return 0.0;
	}

	return Distance(&target->point);
}

float Line::Distance(const Beam* target, Vec2F* nearest_point) const
{
	if (Intersection(target, nearest_point))
	{
		return 0.0;
	}

	return Distance(&target->point, nearest_point);
}

float Line::Distance(const Segment* target) const
{
	if (IsIntersection(target))
	{
		return 0.0;
	}

	float temp_float1 = Distance(&target->point);
	float temp_float2 = Distance(target->point + target->vector);
	if (temp_float1 > temp_float2)
	{
		return temp_float2;
	}
	return temp_float1;
}

float Line::Distance(const Segment* target, Vec2F* nearest_point) const
{
	if (Intersection(target, nearest_point))
	{
		return 0.0;
	}
	
	Vec2F near1;
	float temp_float1 = Distance(&target->point, &near1);
	float temp_float2 = Distance(target->point + target->vector, nearest_point);
	if (temp_float1 > temp_float2)
	{
		return temp_float2;
	}
	*nearest_point = near1;
	return temp_float1;
}

void Line::Set(const Line* line)
{
	point = line->point;
	vector = line->vector;
}

void Line::Set(const Vec2F& point, const Vec2F& point_vector, bool second_argument_is_point)
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

void Line::Set(const Vec2F* point, const Vec2F* point_vector, bool second_argument_is_point)
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

std::ostream& operator<<(std::ostream& stream, const Line& line)
{
	stream << "P: " << line.point << " V: " << line.vector;
	return stream;
}

Beam::Beam()
{

}

Beam::Beam(const Beam& beam) :
	point(beam.point),
	vector(beam.vector)
{
}

Beam::Beam(const Vec2F& point, const Vec2F& point_vector, bool second_argument_is_point) :
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

Beam::Beam(const Vec2F* point, const Vec2F* point_vector, bool second_argument_is_point) :
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

bool Beam::Intersection(const Line* intersection_line, Vec2F* output_intersection_point) const
{
	Vec2F temp_vector1 = intersection_line->point - point;
	*output_intersection_point = temp_vector1 + intersection_line->vector;

	Mat2F temp_matrix1 = Mat2F(&temp_vector1, output_intersection_point);
	temp_vector1 = vector * temp_matrix1.Inverse();

	if (temp_vector1.x + temp_vector1.y <= 0.0f)
	{
		return false;
	}

	*output_intersection_point = temp_vector1 / (temp_vector1.x + temp_vector1.y) * temp_matrix1 + point;

	return true;
}

bool Beam::Intersection(const Beam* intersection_beam, Vec2F* output_intersection_point) const
{
	Vec2F temp_vector1 = point - intersection_beam->point;
	*output_intersection_point = temp_vector1 + vector;

	Mat2F temp_matrix1 = Mat2F(&temp_vector1, output_intersection_point);
	temp_vector1 = intersection_beam->vector * temp_matrix1.Inverse();

	if (temp_vector1.y < 0.0f || temp_vector1.x + temp_vector1.y <= 0.0f)
	{
		return false;
	}

	*output_intersection_point = temp_vector1 / (temp_vector1.x + temp_vector1.y) * temp_matrix1 + intersection_beam->point;

	return true;
}

bool Beam::Intersection(const Segment* intersection_segment, Vec2F* output_intersection_point) const
{
	Vec2F temp_vector1 = point - intersection_segment->point;
	*output_intersection_point = temp_vector1 + vector;

	Mat2F temp_matrix1 = Mat2F(&temp_vector1, output_intersection_point);
	temp_vector1 = intersection_segment->vector * temp_matrix1.Inverse();

	if (temp_vector1.y < 0.0f || temp_vector1.x + temp_vector1.y < 1.0f)
	{
		return false;
	}

	*output_intersection_point = temp_vector1 / (temp_vector1.x + temp_vector1.y) * temp_matrix1 + intersection_segment->point;

	return true;
}

bool Beam::IsIntersection(const Line* intersection_line) const
{
	Vec2F temp_vector1 = intersection_line->point - point;
	temp_vector1 = vector * Mat2F(temp_vector1, temp_vector1 + intersection_line->vector).Inverse();

	return temp_vector1.x + temp_vector1.y > 0.0f;
}

bool Beam::IsIntersection(const Beam* intersection_beam) const
{
	Vec2F temp_vector1 = point - intersection_beam->point;
	temp_vector1 = intersection_beam->vector * Mat2F(temp_vector1, temp_vector1 + vector).Inverse();

	return (temp_vector1.y >= 0.0f && temp_vector1.x + temp_vector1.y > 0.0f);
}

bool Beam::IsIntersection(const Segment* intersection_segment) const
{
	Vec2F temp_vector1 = point - intersection_segment->point;
	temp_vector1 = intersection_segment->vector * Mat2F(temp_vector1, temp_vector1 + vector).Inverse();

	return (temp_vector1.y >= 0.0f && temp_vector1.x + temp_vector1.y >= 1.0f);
}

float Beam::Distance(const Vec2F& target) const
{
	Vec2F temp_vector2 = vector.PerpendicularClockwise();
	Line temp_line1 = Line(target, temp_vector2);
	if (Intersection(&temp_line1, &temp_vector2))
	{
		return temp_vector2.Distance(target);
	}

	return point.Distance(target);
}

float Beam::Distance(const Vec2F* target) const
{
	Vec2F temp_vector2 = vector.PerpendicularClockwise();
	Line temp_line1 = Line(target, &temp_vector2);
	if (Intersection(&temp_line1, &temp_vector2))
	{
		return temp_vector2.Distance(target);
	}

	return point.Distance(target);
}

float Beam::Distance(const Vec2F& target, Vec2F* nearest_point) const
{
	Vec2F temp_vector2 = vector.PerpendicularClockwise();
	Line temp_line1 = Line(&target, &temp_vector2);
	if (Intersection(&temp_line1, &temp_vector2))
	{
		*nearest_point = temp_vector2;
		return temp_vector2.Distance(target);
	}

	*nearest_point = point;
	return point.Distance(target);
}

float Beam::Distance(const Vec2F* target, Vec2F* nearest_point) const
{
	Vec2F temp_vector2 = vector.PerpendicularClockwise();
	Line temp_line1 = Line(target, &temp_vector2);
	if (Intersection(&temp_line1, &temp_vector2))
	{
		*nearest_point = temp_vector2;
		return temp_vector2.Distance(target);
	}

	*nearest_point = point;
	return point.Distance(target);
}

float Beam::Distance(const Line* target) const
{
	if (IsIntersection(target))
	{
		return 0.0;
	}

	return target->Distance(point);
}

float Beam::Distance(const Line* target, Vec2F* nearest_point) const
{
	if (Intersection(target, nearest_point))
	{
		return 0.0;
	}

	return target->Distance(point, nearest_point);
}

float Beam::Distance(const Beam* target) const
{
	if (IsIntersection(target))
	{
		return 0.0;
	}

	float temp_float1;
	float temp_float2;

	Vec2F temp_vector2 = target->vector.PerpendicularClockwise();
	Line temp_line1 = Line(&target->point, &temp_vector2);
	//dist from target->point to this
	if (Intersection(&temp_line1, &temp_vector2))
	{
		temp_float1 = target->point.Distance(&temp_vector2);
		
		temp_line1.Set(point, target->vector.PerpendicularClockwise());
		//dist from point to target
		if (target->Intersection(&temp_line1, &temp_vector2))
		{
			temp_float2 = point.Distance(&temp_vector2);
			if (temp_float1 > temp_float2)
			{
				return temp_float2;
			}
			return temp_float1;
		}
		return temp_float1;
	}

	temp_line1.Set(point, target->vector.PerpendicularClockwise());
	//dist from point to target
	if (target->Intersection(&temp_line1, &temp_vector2))
	{
		return point.Distance(&temp_vector2);
	}

	return point.Distance(target->point);
}

float Beam::Distance(const Beam* target, Vec2F* nearest_point) const
{
	if (IsIntersection(target))
	{
		return 0.0;
	}

	float temp_float1;
	float temp_float2;

	Vec2F temp_vector2 = target->vector.PerpendicularClockwise();
	Line temp_line1 = Line(&target->point, &temp_vector2);
	if (Intersection(&temp_line1, &temp_vector2))
	{
		temp_float1 = target->point.Distance(&temp_vector2);
		*nearest_point = temp_vector2;

		temp_vector2 = vector.PerpendicularClockwise();
		temp_line1.Set(&point, &temp_vector2);
		if (target->Intersection(&temp_line1, &temp_vector2))
		{
			temp_float2 = point.Distance(&temp_vector2);
			if (temp_float1 > temp_float2)
			{
				*nearest_point = point;
				return temp_float2;
			}
			return temp_float1;
		}
		return temp_float1;
	}
	*nearest_point = point;

	temp_vector2 = vector.PerpendicularClockwise();
	temp_line1.Set(&point, &temp_vector2);
	if (target->Intersection(&temp_line1, &temp_vector2))
	{
		return point.Distance(&temp_vector2);
	}

	return point.Distance(target->point);
}

float Beam::Distance(const Segment* target) const
{
	if (IsIntersection(target))
	{
		return 0.0;
	}

	float temp_float1;
	float temp_float2;

	//Get distance between line and point

	Vec2F intersect_point;
	Line temp_line1 = Line(target->point, vector.PerpendicularClockwise());
	//dist from target->point to this 
	if (Intersection(&temp_line1, &intersect_point))
	{
		temp_float1 = target->point.Distance(intersect_point);
		
		temp_line1.Set(target->point + target->vector, vector.PerpendicularClockwise());
		//dist from target->point2 to this 
		if (Intersection(&temp_line1, &intersect_point))
		{
			temp_float2 = target->point.Distance(intersect_point);

			temp_line1.Set(point, target->vector.PerpendicularClockwise());
			//dist from this->point to target
			if (target->Intersection(&temp_line1, &intersect_point))
			{
				if (temp_float1 > temp_float2)
				{
					temp_float1 = point.Distance(intersect_point);
				}
				else
				{
					temp_float2 = point.Distance(intersect_point);
				}

				if (temp_float1 > temp_float2)
				{
					return temp_float2;
				}
				return temp_float1;
			}
			if (temp_float1 > temp_float2)
			{
				return temp_float2;
			}
			return temp_float1;
		}
		temp_line1.Set(point, target->vector.PerpendicularClockwise());
		//dist from this->point to target
		if (target->Intersection(&temp_line1, &intersect_point))
		{
			temp_float2 = point.Distance(intersect_point);

			if (temp_float1 > temp_float2)
			{
				return temp_float2;
			}
			return temp_float1;
		}
		return temp_float1;
	}

	temp_line1.Set(target->point + target->vector, vector.PerpendicularClockwise());
	//dist from target->point2 to this 
	if (Intersection(&temp_line1, &intersect_point))
	{
		temp_float1 = target->point.Distance(intersect_point);

		temp_line1.Set(point, target->vector.PerpendicularClockwise());
		//dist from this->point to target
		if (target->Intersection(&temp_line1, &intersect_point))
		{
			temp_float2 = point.Distance(intersect_point);

			if (temp_float1 > temp_float2)
			{
				return temp_float2;
			}
			return temp_float1;
		}
		return temp_float1;
	}

	temp_line1.Set(point, target->vector.PerpendicularClockwise());
	//dist from this->point to target
	if (target->Intersection(&temp_line1, &intersect_point))
	{
		return point.Distance(intersect_point);
	}
	
	//Get distance between points

	temp_float1 = target->point.Distance(&point);
	temp_float2 = (target->point + target->vector).Distance(&point);

	if (temp_float1 > temp_float2)
	{
		return temp_float2;
	}
	return temp_float1;
}

float Beam::Distance(const Segment* target, Vec2F* nearest_point) const
{
	if (IsIntersection(target))
	{
		return 0.0;
	}

	float temp_float1;
	float temp_float2;

	//Get distance between line and point

	Vec2F temp_vector2 = vector.PerpendicularClockwise();
	Line temp_line1 = Line(&target->point, &temp_vector2);
	//dist from target->point to this 
	if (Intersection(&temp_line1, &temp_vector2))
	{
		temp_float1 = target->point.Distance(temp_vector2);
		Vec2F near1 = temp_vector2;

		temp_line1.Set(target->point + target->vector, vector.PerpendicularClockwise());
		//dist from target->point2 to this 
		if (Intersection(&temp_line1, &temp_vector2))
		{
			temp_float2 = target->point.Distance(temp_vector2);
			*nearest_point = temp_vector2;

			temp_line1.Set(point, target->vector.PerpendicularClockwise());
			//dist from this->point to target
			if (target->Intersection(&temp_line1, &temp_vector2))
			{
				if (temp_float1 > temp_float2)
				{
					temp_float1 = point.Distance(temp_vector2);
					near1 = point;
				}
				else
				{
					temp_float2 = point.Distance(temp_vector2);
					*nearest_point = point;
				}

				if (temp_float1 > temp_float2)
				{
					return temp_float2;
				}
				*nearest_point = near1;
				return temp_float1;
			}
			if (temp_float1 > temp_float2)
			{
				return temp_float2;
			}
			*nearest_point = near1;
			return temp_float1;
		}

		temp_line1.Set(point, target->vector.PerpendicularClockwise());
		//dist from this->point to target
		if (target->Intersection(&temp_line1, &temp_vector2))
		{
			temp_float2 = point.Distance(temp_vector2);
			*nearest_point = point;

			if (temp_float1 > temp_float2)
			{
				return temp_float2;
			}
			*nearest_point = near1;
			return temp_float1;
		}
		*nearest_point = near1;
		return temp_float1;
	}

	temp_line1.Set(target->point + target->vector, vector.PerpendicularClockwise());
	//dist from target->point2 to this 
	if (Intersection(&temp_line1, &temp_vector2))
	{
		temp_float1 = target->point.Distance(temp_vector2);
		*nearest_point = temp_vector2;

		temp_line1.Set(point, target->vector.PerpendicularClockwise());
		//dist from this->point to target
		if (target->Intersection(&temp_line1, &temp_vector2))
		{
			temp_float2 = point.Distance(temp_vector2);

			if (temp_float1 > temp_float2)
			{
				*nearest_point = point;
				return temp_float2;
			}
			return temp_float1;
		}
		return temp_float1;
	}

	*nearest_point = point;

	temp_line1.Set(point, target->vector.PerpendicularClockwise());
	//dist from this->point to target
	if (target->Intersection(&temp_line1, &temp_vector2))
	{
		return point.Distance(temp_vector2);
	}

	//Get distance between points

	temp_float1 = target->point.Distance(&point);
	temp_float2 = (target->point + target->vector).Distance(&point);

	if (temp_float1 > temp_float2)
	{
		return temp_float2;
	}
	return temp_float1;
}

void Beam::Rotate(float angle)
{
	vector.RotateThis(angle);
}

void Beam::Set(const Beam* beam)
{
	point = beam->point;
	vector = beam->vector;
}

void Beam::Set(const Vec2F& point, const Vec2F& point_vector, bool second_argument_is_point)
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

void Beam::Set(const Vec2F* point, const Vec2F* point_vector, bool second_argument_is_point)
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

std::ostream& operator<<(std::ostream& stream, const Beam& beam)
{
	stream << "P: " << beam.point << " V: " << beam.vector;
	return stream;
}

Segment::Segment()
{

}

Segment::Segment(const Segment& segment) :
	point(segment.point),
	vector(segment.vector)
{
}

Segment::Segment(const Vec2F& point, const Vec2F& point_vector, bool second_argument_is_point) :
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

Segment::Segment(const Vec2F* point, const Vec2F* point_vector, bool second_argument_is_point) :
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

bool Segment::Intersection(const Line* intersection_line, Vec2F* output_intersection_point) const
{
	Vec2F temp_vector1 = intersection_line->point - point;
	*output_intersection_point = temp_vector1 + intersection_line->vector;

	Mat2F temp_matrix1 = Mat2F(&temp_vector1, output_intersection_point);
	temp_vector1 = vector * temp_matrix1.Inverse();

	if (temp_vector1.x + temp_vector1.y < 1.0f || !(temp_vector1 == temp_vector1))
	{
		return false;
	}

	*output_intersection_point = temp_vector1 / (temp_vector1.x + temp_vector1.y) * temp_matrix1 + point;

	return true;
}

bool Segment::Intersection(const Beam* intersection_beam, Vec2F* output_intersection_point) const
{
	Vec2F temp_vector1 = intersection_beam->point - point;
	*output_intersection_point = temp_vector1 + intersection_beam->vector;

	Mat2F temp_matrix1 = Mat2F(&temp_vector1, output_intersection_point);
	temp_vector1 = vector * temp_matrix1.Inverse();

	if (temp_vector1.y < 0.0f || temp_vector1.x + temp_vector1.y < 1.0f)
	{
		return false;
	}

	*output_intersection_point = temp_vector1 / (temp_vector1.x + temp_vector1.y) * temp_matrix1 + point;

	return true;
}

bool Segment::Intersection(const Segment* intersection_segment, Vec2F* output_intersection_point) const
{
	Vec2F temp_vector1 = point - intersection_segment->point;
	*output_intersection_point = temp_vector1 + vector;

	Mat2F temp_matrix1 = Mat2F(&temp_vector1, output_intersection_point);
	temp_vector1 = intersection_segment->vector * temp_matrix1.Inverse();

	if (temp_vector1.x < 0.0f || temp_vector1.y < 0.0f || temp_vector1.x + temp_vector1.y < 1.0f)
	{
		return false;
	}

	*output_intersection_point = temp_vector1 / (temp_vector1.x + temp_vector1.y) * temp_matrix1 + intersection_segment->point;

	return true;
}

bool Segment::IsIntersection(const Line* intersection_line) const
{
	Vec2F temp_vector1 = point - intersection_line->point;

	Mat2F temp_matrix1 = Mat2F(temp_vector1, temp_vector1 + vector);
	temp_vector1 = intersection_line->vector * Mat2F(temp_vector1, temp_vector1 + vector).Inverse();

	return temp_vector1.x + temp_vector1.y >= 1.0f;
}

bool Segment::IsIntersection(const Beam* intersection_beam) const
{
	Vec2F temp_vector1 = point - intersection_beam->point;
	temp_vector1 = intersection_beam->vector * Mat2F(temp_vector1, temp_vector1 + vector).Inverse();

	return temp_vector1.x >= 0 && temp_vector1.y >= 0.0f;
}

bool Segment::IsIntersection(const Segment* intersection_segment) const
{
	Vec2F temp_vector1 = point - intersection_segment->point;
	temp_vector1 = intersection_segment->vector * Mat2F(temp_vector1, temp_vector1 + vector).Inverse();

	return temp_vector1.x >= 0.0f && temp_vector1.y >= 0.0f && temp_vector1.x + temp_vector1.y >= 1.0f;
}

float Segment::Distance(const Vec2F& target) const
{
	Vec2F temp_vector1 = vector.PerpendicularClockwise();
	Line temp_line1 = Line(&target, &temp_vector1);
	if (Intersection(&temp_line1, &temp_vector1))
	{
		return temp_vector1.Distance(target);
	}

	float temp_float1 = point.Distance(target);
	float temp_float2 = (point + vector).Distance(target);

	if (temp_float1 > temp_float2)
	{
		return temp_float2;
	}
	
	return temp_float1;
}

float Segment::Distance(const Vec2F* target) const
{
	Vec2F temp_vector1 = vector.PerpendicularClockwise();
	Line temp_line1 = Line(target, &temp_vector1);
	if (Intersection(&temp_line1, &temp_vector1))
	{
		return temp_vector1.Distance(target);
	}

	float temp_float1 = point.Distance(target);
	float temp_float2 = (point + vector).Distance(target);

	if (temp_float1 > temp_float2)
	{
		return temp_float2;
	}

	return temp_float1;
}

float Segment::Distance(const Vec2F& target, Vec2F* nearest_point) const
{
	Vec2F temp_vector1 = vector.PerpendicularClockwise();
	Line temp_line1 = Line(&target, &temp_vector1);
	if (Intersection(&temp_line1, &temp_vector1))
	{
		*nearest_point = temp_vector1;
		return temp_vector1.Distance(target);
	}

	float temp_float1 = point.Distance(target);
	float temp_float2 = (point + vector).Distance(target);

	if (temp_float1 > temp_float2)
	{
		*nearest_point = point + vector;
		return temp_float2;
	}

	*nearest_point = point;
	return temp_float1;
}

float Segment::Distance(const Vec2F* target, Vec2F* nearest_point) const
{
	Vec2F temp_vector1 = vector.PerpendicularClockwise();
	Line temp_line1 = Line(target, &temp_vector1);
	if (Intersection(&temp_line1, &temp_vector1))
	{
		*nearest_point = temp_vector1;
		return temp_vector1.Distance(target);
	}

	float temp_float1 = point.Distance(target);
	float temp_float2 = (point + vector).Distance(target);

	if (temp_float1 > temp_float2)
	{
		*nearest_point = point + vector;
		return temp_float2;
	}

	*nearest_point = point;
	return temp_float1;
}

float Segment::Distance(const Line* target) const
{
	if (IsIntersection(target))
	{
		return 0.0;
	}

	float temp_float1 = target->Distance(point);
	float temp_float2 = target->Distance(point + vector);

	if (temp_float1 > temp_float2)
	{
		return temp_float2;
	}
	return temp_float1;
}

float Segment::Distance(const Line* target, Vec2F* nearest_point) const
{
	if (Intersection(target, nearest_point))
	{
		return 0.0;
	}

	float temp_float1 = target->Distance(point);
	float temp_float2 = target->Distance(point + vector);

	if (temp_float1 > temp_float2)
	{
		*nearest_point = point + vector;
		return temp_float2;
	}
	*nearest_point = point;
	return temp_float1;
}

float Segment::Distance(const Beam* target) const
{
	if (IsIntersection(target))
	{
		return 0.0;
	}

	float temp_float1;
	float temp_float2;

	//Get distance between line and point

	Vec2F temp_vector2 = target->vector.PerpendicularClockwise();
	Line temp_line1 = Line(&point, &temp_vector2);
	//dist from this->point to target 
	if (target->Intersection(&temp_line1, &temp_vector2))
	{
		temp_float1 = point.Distance(temp_vector2);

		temp_line1.Set(point + vector, target->vector.PerpendicularClockwise());
		//dist from this->point2 to target 
		if (target->Intersection(&temp_line1, &temp_vector2))
		{
			temp_float2 = point.Distance(temp_vector2);

			temp_line1.Set(target->point, vector.PerpendicularClockwise());
			//dist from target->point to this
			if (Intersection(&temp_line1, &temp_vector2))
			{
				if (temp_float1 > temp_float2)
				{
					temp_float1 = target->point.Distance(temp_vector2);
				}
				else
				{
					temp_float2 = target->point.Distance(temp_vector2);
				}

				if (temp_float1 > temp_float2)
				{
					return temp_float2;
				}
				return temp_float1;
			}
			return temp_float2;
		}

		temp_line1.Set(target->point, vector.PerpendicularClockwise());
		//dist from target->point to this
		if (Intersection(&temp_line1, &temp_vector2))
		{
			temp_float2 = target->point.Distance(temp_vector2);

			if (temp_float1 > temp_float2)
			{
				return temp_float2;
			}
			return temp_float1;
		}
		return temp_float1;
	}

	temp_line1.Set(point + vector, target->vector.PerpendicularClockwise());
	//dist from this->point2 to target 
	if (target->Intersection(&temp_line1, &temp_vector2))
	{
		temp_float1 = point.Distance(temp_vector2);

		temp_line1.Set(target->point, vector.PerpendicularClockwise());
		//dist from target->point to this
		if (Intersection(&temp_line1, &temp_vector2))
		{
			temp_float2 = target->point.Distance(temp_vector2);

			if (temp_float1 > temp_float2)
			{
				return temp_float2;
			}
			return temp_float1;
		}
		return temp_float1;
	}
	
	temp_line1.Set(target->point, vector.PerpendicularClockwise());
	//dist from target->point to this
	if (Intersection(&temp_line1, &temp_vector2))
	{
		return target->point.Distance(temp_vector2);
	}

	temp_float1 = point.Distance(target->point);
	temp_float2 = (point + vector).Distance(target->point);

	if (temp_float1 > temp_float2)
	{
		return temp_float2;
	}
	return temp_float1;
}

float Segment::Distance(const Beam* target, Vec2F* nearest_point) const
{
	if (Intersection(target, nearest_point))
	{
		return 0.0;
	}

	float temp_float1;
	float temp_float2;

	//Get distance between line and point

	Vec2F temp_vector2 = target->vector.PerpendicularClockwise();
	Line temp_line1 = Line(&point, &temp_vector2);
	//dist from this->point to target 
	if (target->Intersection(&temp_line1, &temp_vector2))
	{
		temp_float1 = point.Distance(temp_vector2);
		Vec2F near1 = point;

		temp_line1.Set(point + vector, target->vector.PerpendicularClockwise());
		//dist from this->point2 to target 
		if (target->Intersection(&temp_line1, &temp_vector2))
		{
			temp_float2 = point.Distance(temp_vector2);
			*nearest_point = point;

			temp_line1.Set(target->point, vector.PerpendicularClockwise());
			//dist from target->point to this
			if (Intersection(&temp_line1, &temp_vector2))
			{
				if (temp_float1 > temp_float2)
				{
					temp_float1 = target->point.Distance(temp_vector2);
					near1 = temp_vector2;
				}
				else
				{
					temp_float2 = target->point.Distance(temp_vector2);
					*nearest_point = temp_vector2;
				}

				if (temp_float1 > temp_float2)
				{
					return temp_float2;
				}
				*nearest_point = near1;
				return temp_float1;
			}
			return temp_float2;
		}
		temp_line1.Set(target->point, vector.PerpendicularClockwise());
		//dist from target->point to this
		if (Intersection(&temp_line1, &temp_vector2))
		{
			temp_float2 = target->point.Distance(temp_vector2);
			*nearest_point = temp_vector2;

			if (temp_float1 > temp_float2)
			{
				return temp_float2;
			}
			*nearest_point = near1;
			return temp_float1;
		}
		*nearest_point = near1;
		return temp_float1;
	}

	temp_line1.Set(point + vector, target->vector.PerpendicularClockwise());
	//dist from this->point2 to target 
	if (target->Intersection(&temp_line1, &temp_vector2))
	{
		temp_float1 = point.Distance(temp_vector2);
		*nearest_point = point;

		temp_line1.Set(target->point, vector.PerpendicularClockwise());
		//dist from target->point to this
		if (Intersection(&temp_line1, &temp_vector2))
		{
			temp_float2 = target->point.Distance(temp_vector2);

			if (temp_float1 > temp_float2)
			{
				*nearest_point = temp_vector2;
				return temp_float2;
			}
			return temp_float1;
		}
		return temp_float1;
	}

	temp_line1.Set(target->point, vector.PerpendicularClockwise());
	//dist from target->point to this
	if (Intersection(&temp_line1, &temp_vector2))
	{
		*nearest_point = temp_vector2;
		return target->point.Distance(temp_vector2);
	}

	temp_float1 = point.Distance(target->point);
	temp_float2 = (point + vector).Distance(target->point);

	if (temp_float1 > temp_float2)
	{
		*nearest_point = point + vector;
		return temp_float2;
	}
	*nearest_point = point;
	return temp_float1;
}

float Segment::Distance(const Segment* target) const
{
	if (IsIntersection(target))
	{
		return 0.0;
	}

	float temp_float1;
	float temp_float2;

	//Get distance between point and line.

	Vec2F temp_vector2 = vector.PerpendicularClockwise();
	Line temp_line1 = Line(&target->point, &temp_vector2);
	//dist from target->point to this
	if (Intersection(&temp_line1, &temp_vector2))
	{
		temp_float1 = target->point.Distance(&temp_vector2);

		temp_line1.Set(target->point + target->vector, vector.PerpendicularClockwise());
		//dist from target->point2 to this
		if (Intersection(&temp_line1, &temp_vector2))
		{
			temp_float2 = (target->point + target->vector).Distance(&temp_vector2);

			temp_line1.Set(point, target->vector.PerpendicularClockwise());
			//dist from this->point to target
			if (target->Intersection(&temp_line1, &temp_vector2))
			{
				if (temp_float1 > temp_float2)
				{
					temp_float1 = point.Distance(&temp_vector2);
				}
				else
				{
					temp_float2 = point.Distance(&temp_vector2);
				}
			}

			temp_line1.Set(point + vector, target->vector.PerpendicularClockwise());
			//dist from this->point2 to target
			if (target->Intersection(&temp_line1, &temp_vector2))
			{
				if (temp_float1 > temp_float2)
				{
					temp_float1 = (point + vector).Distance(&temp_vector2);
				}
				else
				{
					temp_float2 = (point + vector).Distance(&temp_vector2);
				}
			}
			
			if (temp_float1 > temp_float2)
			{
				return temp_float2;
			}
			return temp_float1;
		}

		temp_line1.Set(point, target->vector.PerpendicularClockwise());
		//dist from this->point to target
		if (target->Intersection(&temp_line1, &temp_vector2))
		{
			temp_float2 = point.Distance(&temp_vector2);

			temp_line1.Set(point + vector, target->vector.PerpendicularClockwise());
			//dist from this->point2 to target
			if (target->Intersection(&temp_line1, &temp_vector2))
			{
				if (temp_float1 > temp_float2)
				{
					temp_float1 = (point + vector).Distance(&temp_vector2);
				}
				else
				{
					temp_float2 = (point + vector).Distance(&temp_vector2);
				}
			}

			if (temp_float1 > temp_float2)
			{
				return temp_float2;
			}
			return temp_float1;
		}

		temp_line1.Set(point + vector, target->vector.PerpendicularClockwise());
		//dist from this->point2 to target
		if (target->Intersection(&temp_line1, &temp_vector2))
		{
			temp_float2 = (point + vector).Distance(&temp_vector2);

			if (temp_float1 > temp_float2)
			{
				return temp_float2;
			}
			return temp_float1;
		}

		return temp_float1;
	}

	temp_line1.Set(target->point + target->vector, vector.PerpendicularClockwise());
	//dist from target->point2 to this
	if (Intersection(&temp_line1, &temp_vector2))
	{
		temp_float1 = (target->point + target->vector).Distance(&temp_vector2);

		temp_line1.Set(point, target->vector.PerpendicularClockwise());
		//dist from this->point to target
		if (target->Intersection(&temp_line1, &temp_vector2))
		{
			temp_float2 = point.Distance(&temp_vector2);

			temp_line1.Set(point + vector, target->vector.PerpendicularClockwise());
			//dist from this->point2 to target
			if (target->Intersection(&temp_line1, &temp_vector2))
			{
				if (temp_float1 > temp_float2)
				{
					temp_float1 = (point + vector).Distance(&temp_vector2);
				}
				else
				{
					temp_float2 = (point + vector).Distance(&temp_vector2);
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

	temp_line1.Set(point, target->vector.PerpendicularClockwise());
	//dist from this->point to target
	if (target->Intersection(&temp_line1, &temp_vector2))
	{
		temp_float1 = point.Distance(&temp_vector2);

		temp_line1.Set(point + vector, target->vector.PerpendicularClockwise());
		//dist from this->point2 to target
		if (target->Intersection(&temp_line1, &temp_vector2))
		{
			temp_float2 = (point + vector).Distance(&temp_vector2);
			
			if (temp_float1 > temp_float2)
			{
				return temp_float2;
			}
			return temp_float1;
		}

		return temp_float1;
	}

	temp_line1.Set(point + vector, target->vector.PerpendicularClockwise());
	//dist from this->point2 to target
	if (target->Intersection(&temp_line1, &temp_vector2))
	{
		return (point + vector).Distance(&temp_vector2);
	}

	//Get distance between points.

	temp_float1 = point.Distance(target->point);
	temp_float2 = point.Distance(target->point + target->vector);

	if (temp_float1 > temp_float2)
	{
		temp_float1 = (point + vector).Distance(target->point);
	}
	else
	{
		temp_float2 = (point + vector).Distance(target->point);
	}

	if (temp_float1 > temp_float2)
	{
		temp_float1 = (point + vector).Distance(target->point + target->vector);
	}
	else
	{
		temp_float2 = (point + vector).Distance(target->point + target->vector);
	}

	if (temp_float1 > temp_float2)
	{
		return temp_float2;
	}
	return temp_float1;
}

float Segment::Distance(const Segment* target, Vec2F* nearest_point) const
{
	if (Intersection(target, nearest_point))
	{
		return 0.0;
	}

	float temp_float1;
	float temp_float2;

	//Get distance between point and line.

	Vec2F temp_vector2 = vector.PerpendicularClockwise();
	Line temp_line1 = Line(&target->point, &temp_vector2);
	//dist from target->point to this
	if (Intersection(&temp_line1, &temp_vector2))
	{
		temp_float1 = target->point.Distance(&temp_vector2);
		Vec2F near1 = temp_vector2;

		temp_line1.Set(target->point + target->vector, vector.PerpendicularClockwise());
		//dist from target->point2 to this
		if (Intersection(&temp_line1, &temp_vector2))
		{
			temp_float2 = (target->point + target->vector).Distance(&temp_vector2);
			*nearest_point = temp_vector2;

			temp_line1.Set(point, target->vector.PerpendicularClockwise());
			//dist from this->point to target
			if (target->Intersection(&temp_line1, &temp_vector2))
			{
				if (temp_float1 > temp_float2)
				{
					temp_float1 = point.Distance(&temp_vector2);
					near1 = point;
				}
				else
				{
					temp_float2 = point.Distance(&temp_vector2);
					*nearest_point = point;
				}
			}

			temp_line1.Set(point + vector, target->vector.PerpendicularClockwise());
			//dist from this->point2 to target
			if (target->Intersection(&temp_line1, &temp_vector2))
			{
				if (temp_float1 > temp_float2)
				{
					temp_float1 = (point + vector).Distance(&temp_vector2);
					near1 = point + vector;
				}
				else
				{
					temp_float2 = (point + vector).Distance(&temp_vector2);
					*nearest_point = point + vector;
				}
			}

			if (temp_float1 > temp_float2)
			{
				return temp_float2;
			}
			*nearest_point = near1;
			return temp_float1;
		}

		temp_line1.Set(point, target->vector.PerpendicularClockwise());
		//dist from this->point to target
		if (target->Intersection(&temp_line1, &temp_vector2))
		{
			temp_float2 = point.Distance(&temp_vector2);
			*nearest_point = point;

			temp_line1.Set(point + vector, target->vector.PerpendicularClockwise());
			//dist from this->point2 to target
			if (target->Intersection(&temp_line1, &temp_vector2))
			{
				if (temp_float1 > temp_float2)
				{
					temp_float1 = (point + vector).Distance(&temp_vector2);
					near1 = point + vector;
				}
				else
				{
					temp_float2 = (point + vector).Distance(&temp_vector2);
					*nearest_point = point + vector;
				}
			}

			if (temp_float1 > temp_float2)
			{
				return temp_float2;
			}
			*nearest_point = near1;
			return temp_float1;
		}

		temp_line1.Set(point + vector, target->vector.PerpendicularClockwise());
		//dist from this->point2 to target
		if (target->Intersection(&temp_line1, &temp_vector2))
		{
			temp_float2 = (point + vector).Distance(&temp_vector2);
			*nearest_point = point + vector;

			if (temp_float1 > temp_float2)
			{
				return temp_float2;
			}
			*nearest_point = near1;
			return temp_float1;
		}

		*nearest_point = near1;
		return temp_float1;
	}

	temp_line1.Set(target->point + target->vector, vector.PerpendicularClockwise());
	//dist from target->point2 to this
	if (Intersection(&temp_line1, &temp_vector2))
	{
		temp_float1 = (target->point + target->vector).Distance(&temp_vector2);
		Vec2F near1 = temp_vector2;

		temp_line1.Set(point, target->vector.PerpendicularClockwise());
		//dist from this->point to target
		if (target->Intersection(&temp_line1, &temp_vector2))
		{
			temp_float2 = point.Distance(&temp_vector2);
			*nearest_point = point;

			temp_line1.Set(point + vector, target->vector.PerpendicularClockwise());
			//dist from this->point2 to target
			if (target->Intersection(&temp_line1, &temp_vector2))
			{
				if (temp_float1 > temp_float2)
				{
					temp_float1 = (point + vector).Distance(&temp_vector2);
					near1 = point + vector;
				}
				else
				{
					temp_float2 = (point + vector).Distance(&temp_vector2);
					*nearest_point = point + vector;
				}
			}

			if (temp_float1 > temp_float2)
			{
				return temp_float2;
			}
			*nearest_point = near1;
			return temp_float1;
		}
		*nearest_point = near1;
		return temp_float1;
	}

	temp_line1.Set(point, target->vector.PerpendicularClockwise());
	//dist from this->point to target
	if (target->Intersection(&temp_line1, &temp_vector2))
	{
		temp_float1 = point.Distance(&temp_vector2);
		*nearest_point = point;

		temp_line1.Set(point + vector, target->vector.PerpendicularClockwise());
		//dist from this->point2 to target
		if (target->Intersection(&temp_line1, &temp_vector2))
		{
			temp_float2 = (point + vector).Distance(&temp_vector2);

			if (temp_float1 > temp_float2)
			{
				*nearest_point = point + vector;
				return temp_float2;
			}
			return temp_float1;
		}

		return temp_float1;
	}


	temp_line1.Set(point + vector, target->vector.PerpendicularClockwise());
	//dist from this->point2 to target
	if (target->Intersection(&temp_line1, &temp_vector2))
	{
		*nearest_point = point + vector;
		return (point + vector).Distance(&temp_vector2);
	}

	//Get distance between points.

	temp_float1 = point.Distance(target->point);
	temp_float2 = point.Distance(target->point + target->vector);

	Vec2F near1 = point;
	*nearest_point = point;

	if (temp_float1 > temp_float2)
	{
		temp_float1 = (point + vector).Distance(target->point);
		near1 = point + vector;
	}
	else
	{
		temp_float2 = (point + vector).Distance(target->point);
		*nearest_point = point + vector;
	}

	if (temp_float1 > temp_float2)
	{
		temp_float1 = (point + vector).Distance(target->point + target->vector);
		near1 = point + vector;
	}
	else
	{
		temp_float2 = (point + vector).Distance(target->point + target->vector);
		*nearest_point = point + vector;
	}

	if (temp_float1 > temp_float2)
	{
		return temp_float2;
	}
	*nearest_point = near1;
	return temp_float1;
}

Vec2F Segment::SecondPoint() const
{
	return point + vector;
}

void Segment::Set(const Segment* segment)
{
	point = segment->point;
	vector = segment->vector;
}

void Segment::Set(const Vec2F& point, const Vec2F& point_vector, bool second_argument_is_point)
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

void Segment::Set(const Vec2F* point, const Vec2F* point_vector, bool second_argument_is_point)
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

std::ostream& operator<<(std::ostream& stream, const Segment& segment)
{
	stream << "P: " << segment.point << " V: " << segment.vector;
	return stream;
}
