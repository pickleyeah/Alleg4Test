#include "XMLAreaLoader.h"
#include "InputComponents.h"
#include "MoveComponents.h"
#include "RenderComponents.h"

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
	
	// Get player start position/direction
	auto startPosElement = element->FirstChildElement("startPos");
	auto startDirElement = element->FirstChildElement("startDir");
	if (startPosElement && startDirElement)
	{
		Vec2 startPos = LoadVec2(startPosElement);
		DIR startDir = (DIR)atoi(startDirElement->GetText());
		result->SetStartPosAndDir(startPos, startDir);
	}
		

	tinyxml2::XMLElement *blockElement = element->FirstChildElement("Blocks")->FirstChildElement("Block");
	int iBlock = 0;
	while (blockElement != NULL)
	{
		if (iBlock >= width*height)
			throw std::exception();
		LoadBlock(result->GetBlock(iBlock % width, iBlock / width), blockElement);

		blockElement = blockElement->NextSiblingElement("Block");
		iBlock++;
	}

	tinyxml2::XMLElement *entityElement = element->FirstChildElement("Entities")->FirstChildElement("Entity");
	while (entityElement)
	{
		result->AddEntity(LoadEntity(entityElement));
		entityElement = entityElement->NextSiblingElement("Entity");
	}

	return result;
}

void XMLAreaLoader::LoadBlock(BLOCK_T *result, tinyxml2::XMLElement *element)
{
	result->flags = (BLOCKFLAGS)atoi(element->Attribute("flags"));
	result->SetSprite(element->Attribute("sprite"));

	tinyxml2::XMLElement *overlayElement = element->FirstChildElement("Overlay");
	if (overlayElement)
		result->SetOverlay(overlayElement->Attribute("sprite"));

	tinyxml2::XMLElement *warpElement = element->FirstChildElement("Warp");
	if (warpElement)
	{
		//result->warp = true;
		result->warpDetails = new WARPDETAILS_T(LoadWarpDetails(warpElement));
	}
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
	auto bus = std::make_shared<ComponentMsgBus>();
	InputComponent *input = LoadInputComponent(element->FirstChildElement("InputComponent"), bus);
	MoveComponent *move = LoadMoveComponent(element->FirstChildElement("MoveComponent"), bus);
	RenderComponent *render = LoadRenderComponent(element->FirstChildElement("RenderComponent"), bus);
	Entity *result = new Entity(input, move, render);
	result->Dir = (DIR)atoi(element->FirstChildElement("Dir")->GetText());
	result->Size = LoadVec2(element->FirstChildElement("Size")) * 64;
	Vec2 gridPos = LoadVec2(element->FirstChildElement("Pos"));
	result->SetGridXY((int)gridPos.x, (int)gridPos.y);
	result->SetMsgBus(bus.get());
	return result;
}

InputComponent *XMLAreaLoader::LoadInputComponent(tinyxml2::XMLElement *element, std::shared_ptr<ComponentMsgBus> bus)
{
	std::string type = element->Attribute("type");
	InputComponent *result = nullptr;
	if (type == "NPCTextInput")
	{
		result = new NPCTextInput(bus);
		NPCTextInput *npcInput = dynamic_cast<NPCTextInput*>(result);
		tinyxml2::XMLElement *textElement = element->FirstChildElement("NPCText")->FirstChildElement("NPCTextLine");
		NPCText *strings = new NPCText();
		while (textElement)
		{
			strings->Strings.push_back(textElement->GetText());
			textElement = textElement->NextSiblingElement("NPCTextLine");
		}
		npcInput->SetText(strings);
	}
	// TODO: Add handlers for more InputComponent types here

	return result;
}

MoveComponent *XMLAreaLoader::LoadMoveComponent(tinyxml2::XMLElement *element, std::shared_ptr<ComponentMsgBus> bus)
{
	std::string type = element->Attribute("type");
	MoveComponent *result = nullptr;
	if (type == "DefaultMove")
	{
		result = new DefaultMove(bus);
	}
	return result;
}

RenderComponent *XMLAreaLoader::LoadRenderComponent(tinyxml2::XMLElement *element, std::shared_ptr<ComponentMsgBus> bus)
{
	std::string type = element->Attribute("type");
	RenderComponent *result = nullptr;
	if (type == "PropRender")
	{
		result = new PropRender(bus);
		((PropRender*)result)->SetSprite(element->FirstChildElement("Sprite")->GetText());
	}
	return result;
}
