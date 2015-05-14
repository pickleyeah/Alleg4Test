#include "Entity.h"
#include "InputComponents.h"
#include "MoveComponents.h"
#include "RenderComponents.h"
#include "WorldGameState.h"


Entity::Entity(InputComponent *input, MoveComponent *move, RenderComponent *render) :
	m_input(std::unique_ptr<InputComponent>(input)),
	m_move(std::unique_ptr<MoveComponent>(move)),
	m_render(std::unique_ptr<RenderComponent>(render)),
	m_area(nullptr),
	m_msgBus(nullptr)
{
	m_input->SetEntity(this);
	m_move->SetEntity(this);
	m_render->SetEntity(this);
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
	std::shared_ptr<ComponentMsgBus> msgBus = std::make_shared<ComponentMsgBus>();
	InputComponent *input;
	MoveComponent *move;
	RenderComponent *render;
	Entity* result;
	if (type == "Player")
	{
		input = new PlayerInput(msgBus);
		move = new DefaultMove(msgBus);
		render = new PlayerRender(msgBus);
	}
	else if (type == "Sign")
	{
		input = new NPCTextInput(msgBus);
		NPCText *signText = new NPCText();
		signText->Strings.push_back("YOU ARE READING A SIGN. CONGRATULATIONS!");
		((NPCTextInput*)input)->SetText(signText);

		move = new DefaultMove(msgBus);

		render = new PropRender(msgBus);
		((PropRender*)render)->SetSprite("Data/Sprites/Sign.png");
	}
	else
	{
		throw std::exception("Unrecognised player type");
	}
	result = new Entity(input, move, render);
	result->Dir = DIR_NORTH;
	result->Size = Vec2(WorldGameState::BLOCK_SIZE, WorldGameState::BLOCK_SIZE);
	result->SetMsgBus(msgBus.get());	// Give the container Entity object a pointer to the message bus to enable cross-Entity communication

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
			return (~block->flags & COLLIDE_SOUTH) > 0;
			break;
		case DIR_EAST:
			return (~block->flags & COLLIDE_WEST) > 0;
			break;
		case DIR_SOUTH:
			return (~block->flags & COLLIDE_NORTH) > 0;
			break;
		case DIR_WEST:
			return (~block->flags & COLLIDE_EAST) > 0;
			break;
		}
	}
	return false;
}
