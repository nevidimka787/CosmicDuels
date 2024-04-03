#pragma once

#include "tinyxml2.h"

#include "../../Classes/Menu.h"
#include "../../Classes/Map.h"

#include <string>

class XMLParser
{
public:
	static std::vector<Button> Buttons(const tinyxml2::XMLDocument& doc);
	static std::vector<Map::MapData> Maps(const tinyxml2::XMLDocument& doc);
};

