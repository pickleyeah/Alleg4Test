#include "XMLAreaLoader.h"

Area *XMLAreaLoader::LoadAreaFromXMLFile(const char* filename, WorldGameState *world)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(filename);
	auto *areaElement = doc.FirstChildElement("Area");
	return LoadArea(areaElement, world);
}

Area *XMLAreaLoader::LoadArea(tinyxml2::XMLElement *element, WorldGameState *world)
{
	int width = atoi(element->Attribute("width"));
	int height = atoi(element->Attribute("height"));
	Area *result = new Area(Vec2(width, height), world);
	tinyxml2::XMLElement *blockNode = element->FirstChildElement("Blocks")->FirstChildElement("Block");
	int iBlock = 0;
	while (blockNode != NULL)
	{
		if (iBlock >= width*height)
			throw std::exception();
		result->m_blocks[iBlock] = LoadBlock(blockNode);
		printf("block - flags: %d sprite: %s\n", result->m_blocks[iBlock].colMask, result->m_blocks[iBlock].spriteName);

		blockNode = blockNode->NextSiblingElement("Block");
		iBlock++;
	}
	return result;
}

BLOCK_T XMLAreaLoader::LoadBlock(tinyxml2::XMLElement *element)
{
	BLOCK_T result;
	result.colMask = atoi(element->Attribute("flags"));
	sprintf(result.spriteName, "%s", element->Attribute("sprite"));
	tinyxml2::XMLElement *warpElement = element->FirstChildElement("Warp");
	if (warpElement)
	{
		result.warpDetails = new WARPDETAILS_T(LoadWarpDetails(warpElement));
		printf("warp - area: %s dir: %d pos.x: %d pos.y: %d\n", result.warpDetails->area.c_str(), result.warpDetails->dir, result.warpDetails->pos.x, result.warpDetails->pos.y);
	}
	return result;
}

WARPDETAILS_T XMLAreaLoader::LoadWarpDetails(tinyxml2::XMLElement *element)
{
	WARPDETAILS_T result;
	result.area = element->FirstChildElement("Area")->GetText();
	result.dir = (DIR)atoi(element->FirstChildElement("Dir")->GetText());
	result.pos = LoadVec2(element->FirstChildElement("Pos"));
	return result;
}

Vec2 XMLAreaLoader::LoadVec2(tinyxml2::XMLElement *element)
{
	Vec2 result;
	result.x = atof(element->FirstChildElement("x")->GetText());
	result.y = atof(element->FirstChildElement("y")->GetText());
	return result;
}

Entity *XMLAreaLoader::LoadEntity(tinyxml2::XMLElement *element)
{
	throw std::exception("lel");
}

InputComponent *XMLAreaLoader::LoadInputComponent(tinyxml2::XMLElement *element)
{
	throw std::exception("lel");
}

MoveComponent *XMLAreaLoader::LoadMoveComponent(tinyxml2::XMLElement *element)
{
	throw std::exception("lel");
}

RenderComponent *XMLAreaLoader::LoadRenderComponent(tinyxml2::XMLElement *element)
{
	throw std::exception("lel");
}
