#include "Line.h"

Line::Line() :
	point(new Vec2F()),
	vector(new Vec2F())
{
}

Line::Line(const Line& line) :
	point(new Vec2F(*line.point)),
	vector(new Vec2F(*line.vector))
{
}

Line::Line(Vec2F* point, Vec2F* point_vector, bool second_argument_is_point) :
	point(new Vec2F(*point))
{
	if (second_argument_is_point)
	{
		vector = new Vec2F(*point_vector - *point);
	}
	else
	{
		vector = new Vec2F(*point_vector);
	}
}

bool Line::Intersection(Line* intersection_line, Vec2F* output_intersection_point)
{
	if (*vector == *intersection_line->vector || -*vector == *intersection_line->vector)
	{
		return false;
	}

	Vec2F temp = *intersection_line->point - *point;
	Mat2F mat = Mat2F(&temp, vector);
	Mat2F invers_mat = mat.InverseNotNormalize();
	*output_intersection_point = invers_mat * *intersection_line->vector;
	output_intersection_point->y = output_intersection_point->y / -output_intersection_point->x / mat.Determinant();
	output_intersection_point->x = 0.0;
	*output_intersection_point = mat * *output_intersection_point + *point;
	
	return true;
}

bool Line::Intersection(Beam* intersection_beam, Vec2F* output_intersection_point)
{
	if (*vector == *intersection_beam->vector || -*vector == *intersection_beam->vector)
	{
		return false;
	}

	Vec2F temp = *intersection_beam->point - *point;
	Mat2F mat = Mat2F(&temp, vector);
	Mat2F invers_mat = mat.InverseNotNormalize();
	*output_intersection_point = invers_mat * *intersection_beam->vector;
	if (output_intersection_point->x > 0.0)
	{
		return false;
	}
	output_intersection_point->y = output_intersection_point->y / -output_intersection_point->x / mat.Determinant();
	output_intersection_point->x = 0.0;
	*output_intersection_point = mat * *output_intersection_point + *point;

	return true;
}

bool Line::Intersection(Segment* intersection_segment, Vec2F* output_intersection_point)
{
	if (*vector == *intersection_segment->vector || -*vector == *intersection_segment->vector)
	{
		return false;
	}

	Vec2F temp = *intersection_segment->point - *point;
	Mat2F mat = Mat2F(&temp, vector);
	Mat2F invers_mat = mat.InverseNotNormalize();
	*output_intersection_point = invers_mat * *intersection_segment->vector / mat.Determinant();
	if (output_intersection_point->x > -1.0)
	{
		return false;
	}
	output_intersection_point->y = output_intersection_point->y / -output_intersection_point->x;
	output_intersection_point->x = 0.0;
	*output_intersection_point = mat * *output_intersection_point + *point;

	return true;
}

bool Line::IsIntersection(Line* intersection_line)
{
	return *vector != *intersection_line->vector && -*vector != *intersection_line->vector;
}

bool Line::IsIntersection(Beam* intersection_beam)
{
	if (*vector == *intersection_beam->vector || -*vector == *intersection_beam->vector)
	{
		return false;
	}

	Vec2F temp = *intersection_beam->point - *point;
	Mat2F mat = Mat2F(&temp, vector);
	Mat2F invers_mat = mat.InverseNotNormalize();
	Vec2F vec = invers_mat * *intersection_beam->vector;

	return vec.x <= 0.0;
}

bool Line::IsIntersection(Segment* intersection_segment)
{
	if (*vector == *intersection_segment->vector || -*vector == *intersection_segment->vector)
	{
		return false;
	}
	Vec2F temp = *intersection_segment->point - *point;
	Mat2F mat = Mat2F(&temp, vector);
	Mat2F invers_mat = mat.InverseNotNormalize();
	Vec2F vec = invers_mat * *intersection_segment->vector / mat.Determinant();

	return vec.x <= -1.0;
}

float Line::GetDistance(Vec2F* target)
{
	Vec2F temp = Vec2F(vector->y, -vector->x);
	Line line = Line(target, &temp, false);
	Vec2F* intersect_point = new Vec2F();
	Intersection(&line, intersect_point);

	return intersect_point->GetDistance(target);
}

float Line::GetDistance(Line* target)
{
	if (IsIntersection(target))
	{
		return 0.0;
	}

	return GetDistance(target->point);
}

float Line::GetDistance(Beam* target)
{
	if (IsIntersection(target))
	{
		return 0.0;
	}

	return GetDistance(target->point);
}

float Line::GetDistance(Segment* target)
{
	if (IsIntersection(target))
	{
		return 0.0;
	}
	Vec2F temp1 = target->vector->Perpendicular();
	Line line = Line(target->point, &temp1, false);
	Vec2F intersect_point1, intersect_point2;

	Vec2F temp2 = *line.point - *target->point;
	Mat2F mat1 = Mat2F(&temp2, target->vector);
	Mat2F invers_mat = mat1.InverseNotNormalize();
	intersect_point1 = invers_mat * *line.vector;
	intersect_point1.y = intersect_point1.y / -intersect_point1.x / mat1.Determinant();

	temp2 = *target->point + *target->vector;
	line = Line(&temp2, &temp1, false);
	temp2 = *line.point - *target->point;
	Mat2F mat2 = Mat2F(&temp2, target->vector);
	invers_mat = mat2.InverseNotNormalize();
	intersect_point1 = invers_mat * *line.vector;
	intersect_point1.y = intersect_point1.y / -intersect_point1.x / mat2.Determinant();

	if (abs(intersect_point1.y) < abs(intersect_point2.y))
	{
		intersect_point1.x = 0.0;
		return (mat1 * intersect_point1 + *target->point).GetDistance(point);
	}

	intersect_point2.x = 0.0;
	return (mat1 * intersect_point2 + *target->point).GetDistance(point);
}

void Line::Set(Line* line)
{
	*point = *line->point;
	*vector = *line->vector;
}

void Line::Set(Vec2F* point, Vec2F* point_vector, bool second_argument_is_point)
{
	*this->point = *point;
	if (second_argument_is_point == true)
	{
		*this->vector = *point_vector - *point;
	}
	else
	{
		*this->vector = *point_vector;
	}
}

void Line::operator=(Line line)
{
	*this->point = *line.point;
	*this->vector = *line.vector;
}

void Line::operator=(Beam beam)
{
	*this->point = *beam.point;
	*this->vector = *beam.vector;
}

void Line::operator=(Segment segment)
{
	*this->point = *segment.point;
	*this->vector = *segment.vector;
}

Line::operator Beam()
{
	return Beam(point, vector);
}

Line::operator Segment()
{
	return Segment(point, vector);
}

Line::~Line()
{
	delete point;
	delete vector;
}



Beam::Beam() :
	point(new Vec2F()),
	vector(new Vec2F())
{

}

Beam::Beam(const Beam& beam) :
	point(new Vec2F(*beam.point)),
	vector(new Vec2F(*beam.vector))
{
}

Beam::Beam(Vec2F* point, Vec2F* point_vector, bool second_argument_is_point) :
	point(new Vec2F(*point))
{
	if (second_argument_is_point)
	{
		vector = new Vec2F(*point_vector - *point);
	}
	else
	{
		vector = new Vec2F(*point_vector);
	}
}

bool Beam::Intersection(Line* intersection_line, Vec2F* output_intersection_point)
{
	if (*vector == *intersection_line->vector || -*vector == *intersection_line->vector)
	{
		return false;
	}

	Vec2F temp = *intersection_line->point - *point;
	Mat2F mat = Mat2F(&temp, vector);
	Mat2F invers_mat = mat.InverseNotNormalize();
	*output_intersection_point = invers_mat * *intersection_line->vector;
	if ((((*(uint64_t*)&output_intersection_point->x) & 0x8000000000000000) ^ ((*(uint64_t*)&output_intersection_point->y) & 0x8000000000000000)) == false)//x and y have equilar sign
	{
		return false;
	}
	output_intersection_point->y = output_intersection_point->y / -output_intersection_point->x / mat.Determinant();
	output_intersection_point->x = 0.0;
	*output_intersection_point = mat * *output_intersection_point + *point;

	return true;
}

bool Beam::Intersection(Beam* intersection_beam, Vec2F* output_intersection_point)
{
	if (*vector == *intersection_beam->vector || -*vector == *intersection_beam->vector)
	{
		return false;
	}

	Vec2F temp = *intersection_beam->point - *point;
	Mat2F mat = Mat2F(&temp, vector);
	Mat2F invers_mat = mat.InverseNotNormalize();
	*output_intersection_point = invers_mat * *intersection_beam->vector;
	if (output_intersection_point->x > 0.0 || output_intersection_point->y < 0.0)
	{
		return false;
	}
	output_intersection_point->y = output_intersection_point->y / -output_intersection_point->x / mat.Determinant();
	output_intersection_point->x = 0.0;
	*output_intersection_point = mat * *output_intersection_point + *point;

	return true;
}

bool Beam::Intersection(Segment* intersection_segment, Vec2F* output_intersection_point)
{
	if (*vector == *intersection_segment->vector || -*vector == *intersection_segment->vector)
	{
		return false;
	}

	Vec2F temp = *intersection_segment->point - *point;
	Mat2F mat = Mat2F(&temp, vector);
	Mat2F invers_mat = mat.InverseNotNormalize();
	*output_intersection_point = invers_mat * *intersection_segment->vector / mat.Determinant();
	if (output_intersection_point->x > -1.0 || output_intersection_point->y < 0.0)
	{
		return false;
	}
	output_intersection_point->y = output_intersection_point->y / -output_intersection_point->x;
	output_intersection_point->x = 0.0;
	*output_intersection_point = mat * *output_intersection_point + *point;

	return true;
}

bool Beam::IsIntersection(Line* intersection_line)
{
	if (*vector == *intersection_line->vector || -*vector == *intersection_line->vector)
	{
		return false;
	}

	Vec2F temp = *intersection_line->point - *point;
	Mat2F mat = Mat2F(&temp, vector);
	Mat2F invers_mat = mat.InverseNotNormalize();
	Vec2F vec = invers_mat * *intersection_line->vector;

	return *(uint64_t*)&vec.x & 0x8000000000000000 ^ *(uint64_t*)&vec.y & 0x8000000000000000;//x and y have equilar sign
}

bool Beam::IsIntersection(Beam* intersection_beam)
{
	if (*vector == *intersection_beam->vector || -*vector == *intersection_beam->vector)
	{
		return false;
	}

	Vec2F temp = *intersection_beam->point - *point;
	Mat2F mat = Mat2F(&temp, vector);
	Mat2F invers_mat = mat.InverseNotNormalize();
	Vec2F vec = invers_mat * *intersection_beam->vector;
	
	return vec.x <= -1.0 && vec.y >= 0.0;
}

bool Beam::IsIntersection(Segment* intersection_segment)
{
	if (*vector == *intersection_segment->vector || -*vector == *intersection_segment->vector)
	{
		return false;
	}

	Vec2F temp = *intersection_segment->point - *point;
	Mat2F mat = Mat2F(&temp, vector);
	Mat2F invers_mat = mat.InverseNotNormalize();
	Vec2F vec = invers_mat * *intersection_segment->vector / mat.Determinant();

	return vec.x <= -1.0 && vec.y >= 0.0;
}

float Beam::GetDistance(Vec2F* target)
{
	Vec2F temp = vector->Perpendicular();
	Line line = Line(target, &temp, false);
	Vec2F* intersect_point = new Vec2F();
	if (Intersection(&line, intersect_point))
	{
		return intersect_point->GetDistance(target);
	}

	return point->GetDistance(target);
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

	float dist1 = target->GetDistance(point);
	float dist2;
	bool set_dist2 = false;

	Vec2F temp = vector->Perpendicular();
	Line line = Line(target->point, &temp, false);
	Vec2F* intersect_point = new Vec2F();
	if (Intersection(&line, intersect_point))
	{
		 dist2 = intersect_point->GetDistance(target->point);
		 set_dist2 = true;
	}

	float dist3 =  point->GetDistance(target->point);

	if (set_dist2 == true && dist2 < dist1 && dist2 < dist3)
	{
		return dist2;
	}

	if (dist1 < dist3)
	{
		return dist1;
	}

	return dist3;
}

float Beam::GetDistance(Segment* target)
{
	if (IsIntersection(target))
	{
		return 0.0;
	}
	Vec2F line_vector;
	line_vector.Set(target->vector->y, -target->vector->x);
	Line line = Line(point, &line_vector, false);
	Vec2F* intersect_point = new Vec2F();
	float min, dist;
	bool line_intersect = false;
	if (line.Intersection(target, intersect_point))
	{
		line_intersect = true;
		min = intersect_point->GetDistance(point);
	}
	Vec2F temp = vector->Perpendicular();
	line = Line(target->point, &temp, false);
	if (line.Intersection(this, intersect_point))
	{
		dist = intersect_point->GetDistance(point);
		if (line_intersect == false)
		{
			line_intersect = true;
			min = dist;
		}
		else if (dist < min)
		{
			min = dist;
		}
	}
	Vec2F point2 = *target->point + *target->vector;
	line = Line(&point2, &temp, false);
	if (line.Intersection(this, intersect_point))
	{
		dist = intersect_point->GetDistance(point);
		if (line_intersect == false)
		{
			return dist;
		}
		if (dist < min)
		{
			return dist;
		}
		return min;
	}
	if (line_intersect)
	{
		return min;
	}
	float dist1 = GetDistance(target->point);
	float dist2 = GetDistance(&point2);
	if (dist1 > dist2)
	{
		return dist2;
	}
	return dist1;
}

void Beam::Set(Beam* beam)
{
	*point = *beam->point;
	*vector = *beam->vector;
}

void Beam::Set(Vec2F* point, Vec2F* point_vector, bool second_argument_is_point)
{
	*this->point = *point;
	if (second_argument_is_point == true)
	{
		*this->vector = *point_vector - *point;
	}
	else
	{
		*this->vector = *point_vector;
	}
}

void Beam::operator=(Line line)
{
	*this->point = *line.point;
	*this->vector = *line.vector;
}

void Beam::operator=(Beam beam)
{
	*this->point = *beam.point;
	*this->vector = *beam.vector;
}

void Beam::operator=(Segment segment)
{
	*this->point = *segment.point;
	*this->vector = *segment.vector;
}

Beam::operator Line()
{
	return Line(point, vector);
}

Beam::operator Segment()
{
	return Segment(point, vector);
}

Beam::~Beam()
{
	delete point;
	delete vector;
}



Segment::Segment() : point(new Vec2F()), vector(new Vec2F())
{

}

Segment::Segment(const Segment& segment) :
	point(new Vec2F(*segment.point)),
	vector(new Vec2F(*segment.vector))
{
}

Segment::Segment(Vec2F* point, Vec2F* point_vector, bool second_argument_is_point) : point(new Vec2F(*point))
{
	if (second_argument_is_point)
	{
		vector = new Vec2F(*point_vector - *point);
	}
	else
	{
		vector = new Vec2F(*point_vector);
	}
}

bool Segment::Intersection(Line* intersection_line, Vec2F* output_intersection_point)
{
	if (*vector == *intersection_line->vector || -*vector == *intersection_line->vector)
	{
		return false;
	}

	Vec2F temp = *intersection_line->point - *point;
	Mat2F mat = Mat2F(&temp, vector);
	Mat2F invers_mat = mat.InverseNotNormalize();
	*output_intersection_point = invers_mat * *intersection_line->vector;
	if (((*(uint64_t*)&output_intersection_point->x & 0x8000000000000000) ^ (*(uint64_t*)&output_intersection_point->y & 0x8000000000000000)) == false)//x and y have equilar sign
	{
		return false;
	}
	output_intersection_point->y = output_intersection_point->y / -output_intersection_point->x / mat.Determinant();
	if (output_intersection_point->y > 1.0)
	{
		return false;
	}
	output_intersection_point->x = 0.0;
	*output_intersection_point = mat * *output_intersection_point + *point;

	return true;
}

bool Segment::Intersection(Beam* intersection_beam, Vec2F* output_intersection_point)
{
	if (*vector == *intersection_beam->vector || -*vector == *intersection_beam->vector)
	{
		return false;
	}

	Vec2F temp = *intersection_beam->point - *point;
	Mat2F mat = Mat2F(&temp, vector);
	Mat2F invers_mat = mat.InverseNotNormalize();
	*output_intersection_point = invers_mat * *intersection_beam->vector;
	if (output_intersection_point->x > 0.0 || output_intersection_point->y < 0.0)
	{
		return false;
	}
	output_intersection_point->y = output_intersection_point->y / -output_intersection_point->x / mat.Determinant();
	if (output_intersection_point->y > 1.0)
	{
		return false;
	}
	output_intersection_point->x = 0.0;
	*output_intersection_point = mat * *output_intersection_point + *point;

	return true;
}

bool Segment::Intersection(Segment* intersection_segment, Vec2F* output_intersection_point)
{
	if (*vector == *intersection_segment->vector || -*vector == *intersection_segment->vector)
	{
		return false;
	}

	Vec2F temp = *intersection_segment->point - *point;
	Mat2F mat = Mat2F(&temp, vector);
	Mat2F invers_mat = mat.InverseNotNormalize();
	*output_intersection_point = invers_mat * *intersection_segment->vector / mat.Determinant();
	if (output_intersection_point->x > -1.0 || output_intersection_point->y < 0.0)
	{
		return false;
	}
	output_intersection_point->y = output_intersection_point->y / -output_intersection_point->x;
	if (output_intersection_point->y > 1.0)
	{
		return false;
	}
	output_intersection_point->x = 0.0;
	*output_intersection_point = mat * *output_intersection_point + *point;

	return true;
}

bool Segment::IsIntersection(Line* intersection_line)
{
	if (*vector == *intersection_line->vector || -*vector == *intersection_line->vector)
	{
		return false;
	}

	Vec2F temp = *intersection_line->point - *point;
	Mat2F mat = Mat2F(&temp, vector);
	Mat2F invers_mat = mat.InverseNotNormalize();
	Vec2F vec = invers_mat * *intersection_line->vector;
	if ((((*(uint64_t*)&vec.x) & 0x8000000000000000) ^ ((*(uint64_t*)&vec.y) & 0x8000000000000000)) == false)//x and y have equilar sign
	{
		return false;
	}
	vec.y = vec.y / -vec.x / mat.Determinant();

	return vec.y <= 1.0;
}

bool Segment::IsIntersection(Beam* intersection_beam)
{
	if (*vector == *intersection_beam->vector || -*vector == *intersection_beam->vector)
	{
		return false;
	}

	Vec2F temp = *intersection_beam->point - *point;
	Mat2F mat = Mat2F(&temp, vector);
	Mat2F invers_mat = mat.InverseNotNormalize();
	Vec2F vec = invers_mat * *intersection_beam->vector;
	if (vec.x > 0.0 || vec.y < 0.0)
	{
		return false;
	}
	vec.y = vec.y / -vec.x / mat.Determinant();
	return vec.y <= 1.0;
}

bool Segment::IsIntersection(Segment* intersection_segment)
{
	if (*vector == *intersection_segment->vector || -*vector == *intersection_segment->vector)
	{
		return false;
	}

	Vec2F temp = *intersection_segment->point - *point;
	Mat2F mat = Mat2F(&temp, vector);
	Mat2F invers_mat = mat.InverseNotNormalize();
	Vec2F vec = invers_mat * *intersection_segment->vector / mat.Determinant();
	if (vec.x > -1.0 || vec.y < 0.0)
	{
		return false;
	}
	vec.y = vec.y / -vec.x;
	return vec.y <= 1.0;
}

float Segment::GetDistance(Vec2F* target)
{
	Vec2F temp = vector->Perpendicular();
	Line line = Line(target, &temp, false);
	Vec2F* intersect_point = new Vec2F();
	if (Intersection(&line, intersect_point))
	{
		return intersect_point->GetDistance(target);
	}

	float dist1 = point->GetDistance(target);
	float dist2 = (*point + *vector).GetDistance(target);

	if (dist1 > dist2)
	{
		return dist2;
	}
	
	return dist1;
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

	Vec2F point2 = *point + *vector;
	Vec2F target_point2 = *target->point + *target->vector;

	float dist1 = GetDistance(target->point);
	float dist2 = GetDistance(&target_point2);
	float dist3 = dist1;
	float dist4 = dist1;

	Vec2F perpendicular = target->vector->Perpendicular();
	Line line = Line(point, &perpendicular, false);
	Vec2F* intersect_point = new Vec2F();
	if (Intersection(&line, intersect_point))
	{
		dist3 = intersect_point->GetDistance(point);
	}

	line = Line(&point2, &perpendicular, false);
	if (Intersection(&line, intersect_point))
	{
		dist4 = intersect_point->GetDistance(&point2);
	}

	if (dist1 < dist2)
	{
		if (dist3 < dist4)
		{
			if (dist1 < dist3)
			{
				return dist1;
			}
			return dist3;
		}
		if (dist1 < dist4)
		{
			return dist1;
		}
		return dist4;
	}
	if (dist3 < dist4)
	{
		if (dist2 < dist3)
		{
			return dist2;
		}
		return dist3;
	}
	if (dist2 < dist4)
	{
		return dist2;
	}
	return dist4;
}

Vec2F Segment::GetSecondPoint()
{
	return *point + *vector;
}

void Segment::Set(Segment* segment)
{
	*point = *segment->point;
	*vector = *segment->vector;
}

void Segment::Set(Vec2F* point, Vec2F* point_vector, bool second_argument_is_point)
{
	*this->point = *point;
	if (second_argument_is_point == true)
	{
		*this->vector = *point_vector - *point;
	}
	else
	{
		*this->vector = *point_vector;
	}
}

void Segment::operator=(Line line)
{
	*this->point = *line.point;
	*this->vector = *line.vector;
}

void Segment::operator=(Beam beam)
{
	*this->point = *beam.point;
	*this->vector = *beam.vector;
}

void Segment::operator=(Segment segment)
{
	*this->point = *segment.point;
	*this->vector = *segment.vector;
}

Segment::operator Line()
{
	return Line(point, vector);
}

Segment::operator Beam()
{
	return Beam(point, vector);
}

Segment::~Segment()
{
	delete point;
	delete vector;
}