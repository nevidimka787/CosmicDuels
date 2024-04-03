#include "XMLParser.h"

using namespace tinyxml2;

#define GRID_SCALE	400.0f

static const bool isNumber(const char* str)
{
	bool dot_found = false;
	for (; *str != '\0'; ++str)
	{
		if (*str == '.')
		{
			if (dot_found) return false;
			dot_found = true;
			continue;
		}
		if (*str < '0' || *str > '9') return false;
	}
	return true;
}

static const bool parsButtonPosition(const XMLElement* point, Vec2F& position) {
	for (; point != nullptr; point = point->NextSiblingElement("mxPoint"))
	{
		auto as_str = point->Attribute("as");
		if (as_str == nullptr ||
			strcmp(as_str, "targetPoint") != 0) continue;

		auto x_str = point->Attribute("x");
		auto y_str = point->Attribute("y");
		if (x_str == nullptr || y_str == nullptr ||
			!isNumber(x_str) || !isNumber(y_str)) continue;

		position = Vec2F(atof(x_str), atof(y_str));
		return true;
	}

	return false;
}

static const bool parsPointsArray(const XMLElement* points, std::vector<Vec2F>& points_array)
{
	auto point = points->FirstChildElement("mxPoint");
	if (point == nullptr) return false;

	points_array.clear();
	for (; point != nullptr; point = point->NextSiblingElement("mxPoint"))
	{
		auto x_str = point->Attribute("x");
		auto y_str = point->Attribute("y");

		if (x_str == nullptr || y_str == nullptr ||
			!isNumber(x_str) || !isNumber(y_str)) continue;

		points_array.push_back(Vec2F(atof(x_str), atof(y_str)));
	}

	return true;
}

static const bool parsCellAsButton(const XMLElement* cell, Button& button) {
	if (cell == nullptr) return false;

	auto id_str = cell->Attribute("id");
	if (id_str == nullptr) return false;
	if (strcmp(id_str, "0") == 0 ||
		strcmp(id_str, "1") == 0) return false;

	auto geometry = cell->FirstChildElement("mxGeometry");
	if (geometry == nullptr) return false;

	auto point = geometry->FirstChildElement("mxPoint");
	Vec2F position;
	if (point == nullptr ||
		!parsButtonPosition(point, position)) return false;

	auto points = geometry->FirstChildElement("Array");
	std::vector<Vec2F> area_points(0);
	if (points == nullptr ||
		!parsPointsArray(points, area_points)) return false;

	auto text = cell->Attribute("value");
	if (text != nullptr) {
		button.SetText(std::string(text));
	}

	for (auto& area_point : area_points)
	{
		area_point -= position;
		area_point /= GRID_SCALE;
	}
	button.SetArea(Area(area_points));

	position /= GRID_SCALE;
	button.SetPosition(position);

	return true;
}

static const XMLElement* toRoot(const XMLDocument& doc)
{
	const auto& nodes_names = std::vector<std::string>({
		"diagram",
		"mxGraphModel",
		"root"
	});

	const XMLElement* root = doc.FirstChildElement("mxfile");
	for (const auto& node_name : nodes_names)
	{
		if (root == nullptr) return nullptr;
		root = root->FirstChildElement(node_name.c_str());
	}
	
	return root;
}

std::vector<Button> XMLParser::Buttons(const XMLDocument& doc)
{
	if (doc.ErrorID() != XML_SUCCESS)
	{
		return std::vector<Button>(0);
	}

	const XMLElement* root = toRoot(doc);
	if (root == nullptr) return std::vector<Button>(0);

	auto buttons = std::vector<Button>(0);

	const XMLElement* cell = root->FirstChildElement("mxCell");
	for (size_t element = 0; element < root->ChildElementCount(); ++element)
	{
		if (cell == nullptr) break;

		Button button;
		if (!parsCellAsButton(cell, button)) continue;

		buttons.push_back(std::move(button));

		cell = cell->NextSiblingElement("mxCell");
	}

	return buttons;
}

std::vector<Map::MapData> XMLParser::Maps(const XMLDocument& doc)
{
	return std::vector<Map::MapData>(0);
}