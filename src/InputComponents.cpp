#include "InputComponents.h"
#include "Entity.h"
#include "Area.h"
#include "WorldGameState.h"

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
		if (Input::KeyPressed(ALLEGRO_KEY_SPACE))
		{
			// TODO: Perform use/talk action here
			return;
		}

		m_oldPos = entity->Pos;
		m_newGridX = entity->GridX();
		m_newGridY = entity->GridY();
		if (Input::KeyDown(ALLEGRO_KEY_LEFT))
		{
			entity->Vel.x = -MOVESPEED;
			entity->Dir = DIR_WEST;
			m_state = TE_MOVING;
			m_newGridX--;
		}
		else if (Input::KeyDown(ALLEGRO_KEY_RIGHT))
		{
			entity->Vel.x = MOVESPEED;
			entity->Dir = DIR_EAST;
			m_state = TE_MOVING;
			m_newGridX++;
		}
		else if (Input::KeyDown(ALLEGRO_KEY_UP))
		{
			entity->Vel.y = -MOVESPEED;
			entity->Dir = DIR_NORTH;
			m_state = TE_MOVING;
			m_newGridY--;
		}
		else if (Input::KeyDown(ALLEGRO_KEY_DOWN))
		{
			entity->Vel.y = MOVESPEED;
			entity->Dir = DIR_SOUTH;
			m_state = TE_MOVING;
			m_newGridY++;
		}

		if (m_state == TE_MOVING && !entity->CanMoveTo(m_newGridX, m_newGridY))
		{
			entity->Vel = Vec2(0, 0);
			m_state = TE_IDLE;
		}
		break;
	case TE_MOVING:
		Vec2 moved = Vec2::Sub(entity->Pos, m_oldPos);
		if (moved.Length() >= WorldGameState::BLOCK_SIZE * 0.95f)	// If it's close enough, snap to the grid position a bit early to make it less jerky when we stop moving
		{
			// If we've hit a warp block, trigger the area transition on the next frame
			// Sort of hacky in that it creates a dependency on WorldGameState, but since Entities should only exist inside of an Area/WorldGameState it seems reasonable enough
			BLOCK_T *block = entity->GetArea()->GetBlock(m_newGridX, m_newGridY);
			if (block->warp)
			{
				WorldGameState *world = entity->GetArea()->GetWorldGameState();
				world->TransitionToArea(block->warpDetails);
				entity->Vel = Vec2(0, 0);
				m_state = TE_IDLE;
				return;
			}
			// Save the player position before the grid clamp and put it back afterward to smooth out continuous movement 
			Vec2 tempPos = entity->Pos;
			// Once we've moved a whole grid space, clamp to the grid
			entity->SetGridXY(m_newGridX, m_newGridY);
			bool keepMoving = Input::KeyDown(ALLEGRO_KEY_UP) && entity->Dir == DIR_NORTH;
			keepMoving |= Input::KeyDown(ALLEGRO_KEY_RIGHT) && entity->Dir == DIR_EAST;
			keepMoving |= Input::KeyDown(ALLEGRO_KEY_DOWN) && entity->Dir == DIR_SOUTH;
			keepMoving |= Input::KeyDown(ALLEGRO_KEY_LEFT) && entity->Dir == DIR_WEST;
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

			if (keepMoving && entity->CanMoveTo(m_newGridX, m_newGridY))
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
