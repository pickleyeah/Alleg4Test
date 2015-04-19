#include "Entity.h"
#include "InputComponents.h"
#include "MoveComponents.h"
#include "RenderComponents.h"
#include "WorldGameState.h"


Entity::Entity(void)
{
}


Entity::~Entity(void)
{
}

Entity* Entity::MakeTestEntity(Area *area)
{
	Entity* result = new Entity();
	result->m_input = new TestInput();
	result->m_move = new TestMove();
	result->m_render = new TestRender((TestInput*)result->m_input);

	result->Size = Vec2(WorldGameState::BLOCK_SIZE,WorldGameState::BLOCK_SIZE);
	result->Dir = DIR_NORTH;
	return result;
}

void Entity::Init(Area *area)
{
	m_timeAlive = 0;
	m_alive = true;
	m_remove = false;
	m_area = area;
}

void Entity::Update(double dt)
{
	m_timeAlive += dt;

	m_move->Update(this, NULL, dt);
	// Pos += dt*Vel
	//Pos = Vec2::Add(Pos, Vec2::Mul(Vel, (float)dt));
}

bool Entity::CanMoveTo(int x, int y)
{
	if (x < 0 || y < 0 || x >= m_area->Size().x || y >= m_area->Size().y) // Out of bounds
	{
		return false;
	}
	else
	{
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
