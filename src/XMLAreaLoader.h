#pragma once

#include "tinyxml2/tinyxml2.h"
#include "Area.h"

class XMLAreaLoader
{
public:
	static Area *LoadAreaFromXMLFile(const char* filename, WorldGameState *world);

private:
	XMLAreaLoader();

	static Area *LoadArea(tinyxml2::XMLElement *element, WorldGameState *world);
	static void LoadBlock(BLOCK_T *result, tinyxml2::XMLElement *element);
	static WARPDETAILS_T LoadWarpDetails(tinyxml2::XMLElement *element);
	static Vec2 LoadVec2(tinyxml2::XMLElement *element);
	static Entity *LoadEntity(tinyxml2::XMLElement *element);

	static InputComponent *LoadInputComponent(tinyxml2::XMLElement *element, std::shared_ptr<ComponentMsgBus> bus);
	static MoveComponent *LoadMoveComponent(tinyxml2::XMLElement *element, std::shared_ptr<ComponentMsgBus> bus);
	static RenderComponent *LoadRenderComponent(tinyxml2::XMLElement *element, std::shared_ptr<ComponentMsgBus> bus);
};
