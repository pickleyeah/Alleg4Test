#include "InputComponents.h"
#include "Entity.h"
#include "Area.h"
#include "IntroGameState.h"

TestInput::TestInput(void) :
	m_secsSinceStateChange(0),
	m_state(TE_IDLE)
{
}


TestInput::~TestInput(void)
{
}

static int MOVESPEED = 256;

void TestInput::ProcessInput(Entity *entity, double dt)
{
	m_secsSinceStateChange += dt;
	switch (m_state)
	{
	case TE_IDLE:
		m_oldPos = entity->Pos;
		m_newGridX = entity->GridX();
		m_newGridY = entity->GridY();
		if (Input::KeyDown(KEY_LEFT))
		{
			entity->Vel.x = -MOVESPEED;
			entity->Dir = DIR_WEST;
			m_state = TE_MOVING;
			m_newGridX--;
		}
		else if (Input::KeyDown(KEY_RIGHT))
		{
			entity->Vel.x = MOVESPEED;
			entity->Dir = DIR_EAST;
			m_state = TE_MOVING;
			m_newGridX++;
		}
		else if (Input::KeyDown(KEY_UP))
		{
			entity->Vel.y = -MOVESPEED;
			entity->Dir = DIR_NORTH;
			m_state = TE_MOVING;
			m_newGridY--;
		}
		else if (Input::KeyDown(KEY_DOWN))
		{
			entity->Vel.y = MOVESPEED;
			entity->Dir = DIR_SOUTH;
			m_state = TE_MOVING;
			m_newGridY++;
		}

		if (m_state == TE_MOVING && !CanMoveTo(entity, entity->GetArea(), m_newGridX, m_newGridY))
		{
			entity->Vel = Vec2(0, 0);
			m_state = TE_IDLE;
		}
		// Find out if we should cancel the move
		//if (m_state == TE_MOVING)
		//{
		//	Area *area = entity->GetArea();
		//	bool reset = false;
		//	// Grid pos out of bounds, reset
		//	if (m_newGridX < 0 || m_newGridY < 0 || m_newGridX >= area->Size().x || m_newGridY >= area->Size().y)
		//	{
		//		reset = true;
		//	}
		//	else
		//	{
		//		// Can't pass thru block
		//		BLOCK_T *block = area->GetBlock(m_newGridX, m_newGridY);
		//		switch (entity->Dir)
		//		{
		//		case DIR_NORTH:
		//			reset = reset || block->colMask & COL_SOUTH;
		//			break;
		//		case DIR_EAST:
		//			reset = reset || block->colMask & COL_WEST;
		//			break;
		//		case DIR_SOUTH:
		//			reset = reset || block->colMask & COL_NORTH;
		//			break;
		//		case DIR_WEST:
		//			reset = reset || block->colMask & COL_EAST;
		//			break;
		//		}
		//	}
		//	if (reset)
		//	{
		//		entity->Vel = Vec2(0, 0);
		//		m_state = TE_IDLE;
		//	}
		//	else
		//	{
		//		m_secsSinceStateChange = 0;
		//	}
		//}
		break;
	case TE_MOVING:
		Vec2 moved = Vec2::Sub(entity->Pos, m_oldPos);
		if (moved.Length() >= IntroGameState::BLOCK_SIZE)
		{
			// Save the player position before the grid clamp and put it back afterward to smooth out continuous movement 
			Vec2 tempPos = entity->Pos;
			// Once we've moved a whole grid space, clamp to the grid
			entity->SetGridXY(m_newGridX, m_newGridY);
			bool keepMoving = Input::KeyDown(KEY_UP) && entity->Dir == DIR_NORTH;
			keepMoving |= Input::KeyDown(KEY_RIGHT) && entity->Dir == DIR_EAST;
			keepMoving |= Input::KeyDown(KEY_DOWN) && entity->Dir == DIR_SOUTH;
			keepMoving |= Input::KeyDown(KEY_LEFT) && entity->Dir == DIR_WEST;
			// Stay in moving state if button is held down and we can move to the next space
			if (keepMoving)
			{
				switch (entity->Dir)
				{
				case DIR_NORTH: m_newGridY--; break;
				case DIR_EAST: m_newGridX++; break;
				case DIR_SOUTH: m_newGridY++; break;
				case DIR_WEST: m_newGridX--; break;
				}
			}

			if (keepMoving && CanMoveTo(entity, entity->GetArea(), m_newGridX, m_newGridY))
			{
				m_oldPos = entity->Pos;
				entity->Pos = tempPos;
			}
			else
			{
				// Stop here and reset to idle state
				entity->SetGridXY(entity->GridX(), entity->GridY());
				entity->Vel = Vec2(0, 0);
				m_state = TE_IDLE;
			}
		}
		break;
	}
}

bool TestInput::CanMoveTo(Entity *entity, Area *area, int x, int y)
{
	bool reset = false;
	// Grid pos out of bounds, reset
	if (m_newGridX < 0 || m_newGridY < 0 || m_newGridX >= area->Size().x || m_newGridY >= area->Size().y)
	{
		return false;
	}
	else
	{
		// Can't pass thru block
		BLOCK_T *block = area->GetBlock(m_newGridX, m_newGridY);
		switch (entity->Dir)
		{
		case DIR_NORTH:
			return ~block->colMask & COL_SOUTH;
			break;
		case DIR_EAST:
			return ~block->colMask & COL_WEST;
			break;
		case DIR_SOUTH:
			return ~block->colMask & COL_NORTH;
			break;
		case DIR_WEST:
			return ~block->colMask & COL_EAST;
			break;
		}
	}
	return true;
}
