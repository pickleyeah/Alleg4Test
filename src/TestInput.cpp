#include "TestInput.h"
#include "Entity.h"
#include "IntroGameState.h"
#include <allegro.h>


TestInput::TestInput(void)
{
	m_state = TE_IDLE;
}


TestInput::~TestInput(void)
{
}

static int MOVESPEED = 192;

void TestInput::ProcessInput(Entity *entity)
{
	switch (m_state)
	{
	case TE_IDLE:
		m_oldPos = entity->Pos;
		m_newGridX = entity->GridX();
		m_newGridY = entity->GridY();
		if (key[KEY_LEFT])
		{
			entity->Vel.x = -MOVESPEED;
			entity->Dir = DIR_WEST;
			m_state = TE_MOVING;
			m_newGridX--;
		}
		else if (key[KEY_RIGHT])
		{
			entity->Vel.x = MOVESPEED;
			entity->Dir = DIR_EAST;
			m_state = TE_MOVING;
			m_newGridX++;
		}
		else if (key[KEY_UP])
		{
			entity->Vel.y = -MOVESPEED;
			entity->Dir = DIR_NORTH;
			m_state = TE_MOVING;
			m_newGridY--;
		}
		else if (key[KEY_DOWN])
		{
			entity->Vel.y = MOVESPEED;
			entity->Dir = DIR_SOUTH;
			m_state = TE_MOVING;
			m_newGridY++;
		}
		// Find out if we should cancel the move
		if (m_state == TE_MOVING)
		{
			Area *area = entity->GetArea();
			bool reset = false;
			// Grid pos out of bounds, reset
			if (m_newGridX < 0 || m_newGridY < 0 || m_newGridX >= area->Size().x || m_newGridY >= area->Size().y )
			{
				reset = true;
			}
			else
			{
				// Can't pass thru block
				BLOCK_T *block = area->GetBlock(m_newGridX, m_newGridY);
				switch (entity->Dir)
				{
				case DIR_NORTH:
					reset = reset || block->colMask & COL_SOUTH;
					break;
				case DIR_EAST:
					reset = reset || block->colMask & COL_WEST;
					break;
				case DIR_SOUTH:
					reset = reset || block->colMask & COL_NORTH;
					break;
				case DIR_WEST:
					reset = reset || block->colMask & COL_EAST;
					break;
				}
			}
			if (reset)
			{
				entity->Vel = Vec2(0,0);
				m_state = TE_IDLE;
			}
		}
		break;
	case TE_MOVING:
		// Once we've moved a whole grid space, clamp to the grid
		Vec2 moved = Vec2::Sub(entity->Pos, m_oldPos);
		if (moved.Length() >= IntroGameState::BLOCK_SIZE)
		{
			entity->SetGridXY(m_newGridX, m_newGridY);
			entity->Vel = Vec2(0,0);
			m_state = TE_IDLE;
		}
		break;
	}
}
