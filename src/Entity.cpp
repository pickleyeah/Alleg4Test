#include "Entity.h"
#include "InputComponents.h"
#include "MoveComponents.h"
#include "RenderComponents.h"
#include "WorldGameState.h"


Entity::Entity(void) :
	m_area(nullptr),
	m_msgBus(nullptr)
{
}


Entity::~Entity(void)
{
}

//-------------------------------------------------------------------------------
// CreateEntity
// To add a new entity type, add another block which checks for the entity name
// and initializes it accordingly
// e.g. if (type == "NewEntity") ...
//-------------------------------------------------------------------------------
Entity* Entity::CreateEntity(std::string type)
{
	auto msgBus = std::make_shared<ComponentMsgBus>();
	Entity* result = new Entity();
	result->m_msgBus = msgBus.get();	// Give the container Entity object a pointer to the message bus to enable cross-Entity communication
	if (type == "Player")
	{
		result->m_input = std::unique_ptr<InputComponent>(new PlayerInput(msgBus, result));
		result->m_move = std::unique_ptr<MoveComponent>(new DefaultMove(msgBus, result));
		result->m_render = std::unique_ptr<RenderComponent>(new PlayerRender(msgBus, result));
		result->Size = Vec2(WorldGameState::BLOCK_SIZE, WorldGameState::BLOCK_SIZE);
	}
	else if (type == "Sign")
	{
		result->m_input = std::unique_ptr<InputComponent>(new NPCTextInput(msgBus, result));
		result->m_move = std::unique_ptr<MoveComponent>(new DefaultMove(msgBus, result));
		auto render = new PropRender(msgBus, result);
		render->SetSprite(Sprite::GetSprite("Data/Sprites/Sign.png"));
		result->m_render = std::unique_ptr<RenderComponent>(render);
		result->Size = Vec2(WorldGameState::BLOCK_SIZE, WorldGameState::BLOCK_SIZE);
	}
	else
	{
		throw std::exception("Unrecognised player type");
	}
	result->Dir = DIR_NORTH;

	return result;
}

void Entity::Init(Area *area)
{
	SetArea(area);
	m_timeAlive = 0;
	m_alive = true;
	m_remove = false;
	m_area = area;
}

void Entity::Update(double dt)
{
	m_timeAlive += dt;
	m_move->Update(dt);
}

bool Entity::CanMoveTo(int x, int y)
{
	if (x < 0 || y < 0 || x >= m_area->Size().x || y >= m_area->Size().y) // Out of bounds
	{
		return false;
	}
	else
	{
		// See if there are any entities in the way
		auto otherEntity = m_area->GetEntityAt(x, y);
		if (otherEntity && otherEntity)
			return false;
		// See if we can pass thru block from this direction
		BLOCK_T *block = m_area->GetBlock(x, y);
		switch (Dir)
		{
		case DIR_NORTH:
			return (~block->colMask & COL_SOUTH) > 0;
			break;
		case DIR_EAST:
			return (~block->colMask & COL_WEST) > 0;
			break;
		case DIR_SOUTH:
			return (~block->colMask & COL_NORTH) > 0;
			break;
		case DIR_WEST:
			return (~block->colMask & COL_EAST) > 0;
			break;
		}
	}
	return false;
}
