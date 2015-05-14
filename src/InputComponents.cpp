#include "InputComponents.h"
#include "Entity.h"
#include "Area.h"
#include "WorldGameState.h"

//-----------------------------------------------------------------------------
// PlayerInput
//-----------------------------------------------------------------------------
PlayerInput::PlayerInput(std::shared_ptr<ComponentMsgBus> bus) :
	InputComponent(bus),
	m_secsSinceStateChange(0),
	m_state(ENTSTATE_IDLE)
{
}


PlayerInput::~PlayerInput(void)
{
}

void PlayerInput::ReceiveMsg(COMPONENTMSG_T msg, Component *sender, Entity *source)
{
	if (sender == this)
		return;
}

void PlayerInput::SetState(ENTSTATE state)
{
	m_state = state;
	m_msgBus->Send(COMPONENTMSG_T{ MSG_STATECHANGE, std::shared_ptr<ENTSTATE>(new ENTSTATE(state)) }, this, m_entity);
}

static int MOVESPEED = 256;

void PlayerInput::ProcessInput(double dt)
{
	m_secsSinceStateChange += dt;
	switch (m_state)
	{
	case ENTSTATE_IDLE:
		if (Input::KeyPressed(ALLEGRO_KEY_SPACE))
		{
			// Get the grid block we're facing
			int x = m_entity->GridX();
			int y = m_entity->GridY();
			switch (m_entity->Dir)
			{
			case DIR_NORTH: --y; break;
			case DIR_SOUTH: ++y; break;
			case DIR_WEST: --x; break;
			case DIR_EAST: ++x; break;
			}
			auto other = m_entity->GetArea()->GetEntityAt(x, y);
			// If there's an object, try and use it
			if (other)
				other->ReceiveMsg(COMPONENTMSG_T{ MSG_USE, std::shared_ptr<void>(nullptr) }, this, m_entity);
			return;
		}

		m_oldPos = m_entity->Pos;
		m_newGridX = m_entity->GridX();
		m_newGridY = m_entity->GridY();
		if (Input::KeyDown(ALLEGRO_KEY_LEFT))
		{
			m_entity->Vel.x = -MOVESPEED;
			m_entity->Dir = DIR_WEST;
			SetState(ENTSTATE_MOVING);
			m_newGridX--;
		}
		else if (Input::KeyDown(ALLEGRO_KEY_RIGHT))
		{
			m_entity->Vel.x = MOVESPEED;
			m_entity->Dir = DIR_EAST;
			SetState(ENTSTATE_MOVING);
			m_newGridX++;
		}
		else if (Input::KeyDown(ALLEGRO_KEY_UP))
		{
			m_entity->Vel.y = -MOVESPEED;
			m_entity->Dir = DIR_NORTH;
			SetState(ENTSTATE_MOVING);
			m_newGridY--;
		}
		else if (Input::KeyDown(ALLEGRO_KEY_DOWN))
		{
			m_entity->Vel.y = MOVESPEED;
			m_entity->Dir = DIR_SOUTH;
			SetState(ENTSTATE_MOVING);
			m_newGridY++;
		}

		if (m_state == ENTSTATE_MOVING && !m_entity->CanMoveTo(m_newGridX, m_newGridY))
		{
			m_entity->Vel = Vec2(0, 0);
			SetState(ENTSTATE_IDLE);
		}
		break;
	case ENTSTATE_MOVING:
		Vec2 moved = m_entity->Pos - m_oldPos;
		if (moved.Length() >= WorldGameState::BLOCK_SIZE * 0.95f)	// If it's close enough, snap to the grid position a bit early to make it less jerky when we stop moving
		{
			// If we've hit a warp block, trigger the area transition on the next frame
			// Sort of hacky in that it creates a dependency on WorldGameState, but since Entities should only exist inside of an Area/WorldGameState it seems reasonable enough
			BLOCK_T *block = m_entity->GetArea()->GetBlock(m_newGridX, m_newGridY);
			if (block->flags & HAS_WARP)
			{
				WorldGameState *world = m_entity->GetArea()->GetWorldGameState();
				world->TriggerAreaTransition(block->warpDetails);
				m_entity->Vel = Vec2(0, 0);
				SetState(ENTSTATE_IDLE);
				return;
			}
			// Save the player position before the grid clamp and put it back afterward to smooth out continuous movement 
			Vec2 tempPos = m_entity->Pos;
			// Once we've moved a whole grid space, clamp to the grid
			m_entity->SetGridXY(m_newGridX, m_newGridY);
			bool keepMoving = Input::KeyDown(ALLEGRO_KEY_UP) && m_entity->Dir == DIR_NORTH;
			keepMoving |= Input::KeyDown(ALLEGRO_KEY_RIGHT) && m_entity->Dir == DIR_EAST;
			keepMoving |= Input::KeyDown(ALLEGRO_KEY_DOWN) && m_entity->Dir == DIR_SOUTH;
			keepMoving |= Input::KeyDown(ALLEGRO_KEY_LEFT) && m_entity->Dir == DIR_WEST;
			// Stay in moving state if button is held down and we can move to the next space
			if (keepMoving)
			{
				switch (m_entity->Dir)
				{
				case DIR_NORTH: m_newGridY--; break;
				case DIR_EAST: m_newGridX++; break;
				case DIR_SOUTH: m_newGridY++; break;
				case DIR_WEST: m_newGridX--; break;
				}
			}

			if (keepMoving && m_entity->CanMoveTo(m_newGridX, m_newGridY))
			{
				m_oldPos = m_entity->Pos;
				m_entity->Pos = tempPos;
			}
			else
			{
				// Stop here and reset to idle state
				m_entity->SetGridXY(m_entity->GridX(), m_entity->GridY());
				m_entity->Vel = Vec2(0, 0);
				SetState(ENTSTATE_IDLE);
			}
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// NPCTextInput
//-----------------------------------------------------------------------------
void NPCTextInput::ProcessInput(double dt)
{

}

void NPCTextInput::ReceiveMsg(COMPONENTMSG_T msg, Component *sender, Entity *source)
{
	if (sender == this)
		return;
	if (msg.type == MSG_USE)
	{
		// Player (presumably) interacted with the sign
		printf("Player is reading the sign\n"
			"The sign says:\n%s\n\n", m_text.get()->Strings[0].c_str());
		// TODO: trigger text overlay in WorldGameState
		source->GetArea()->GetWorldGameState()->TriggerNPCTextDisplay(m_text.get());
	}
}