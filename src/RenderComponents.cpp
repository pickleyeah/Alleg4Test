#include "RenderComponents.h"
#include "InputComponents.h"

#include "Vec2.h"
#include "Entity.h"
#include "Sprite.h"
#include "GameTime.h"

//-----------------------------------------------------------------------------
// PlayerRender
//-----------------------------------------------------------------------------
PlayerRender::PlayerRender(std::shared_ptr<ComponentMsgBus> bus, Entity *entity) :
	RenderComponent(bus, entity),
	m_state(ENTSTATE_IDLE),
	m_lastStateChangeTime(0)
{
	m_idleSprites.push_back(Sprite::GetSprite("Data/Sprites/Player_Idle_N.png"));
	m_idleSprites.push_back(Sprite::GetSprite("Data/Sprites/Player_Idle_E.png"));
	m_idleSprites.push_back(Sprite::GetSprite("Data/Sprites/Player_Idle_S.png"));
	m_idleSprites.push_back(Sprite::GetSprite("Data/Sprites/Player_Idle_W.png"));

	m_walkSprites.push_back(Sprite::GetSprite("Data/Sprites/Player_Walk_N.png"));
	m_walkSprites.push_back(Sprite::GetSprite("Data/Sprites/Player_Walk_E.png"));
	m_walkSprites.push_back(Sprite::GetSprite("Data/Sprites/Player_Walk_S.png"));
	m_walkSprites.push_back(Sprite::GetSprite("Data/Sprites/Player_Walk_W.png"));
}

void PlayerRender::ReceiveMsg(COMPONENTMSG_T msg, Component *sender, Entity *source)
{
	if (sender == this)
		return;
	switch (msg.type)
	{
	case MSG_STATECHANGE:
		m_state = *((ENTSTATE*)msg.data.get());
		m_lastStateChangeTime = GameTime::TotalElapsedTime();
		break;
	}
}

void PlayerRender::Render(Vec2 offset)
{
	double secsSinceStateChange = GameTime::TotalElapsedTime() - m_lastStateChangeTime;

	Vec2 Pos = m_entity->Pos;
	Vec2 Size = m_entity->Size;

	int x = offset.x + Pos.x;
	int y = offset.y + Pos.y;

	Sprite *sprite = nullptr;
	switch (m_state)
	{
	case ENTSTATE_IDLE:
		sprite = m_idleSprites[m_entity->Dir];
		break;
	case ENTSTATE_MOVING:
		sprite = m_walkSprites[m_entity->Dir];
		break;
	}
	sprite->Render(secsSinceStateChange, x, y);
}

//-----------------------------------------------------------------------------
// PropRender
//-----------------------------------------------------------------------------
PropRender::PropRender(std::shared_ptr<ComponentMsgBus> bus, Entity *entity) :
	RenderComponent(bus, entity),
	m_lastStateChangeTime(0),
	m_sprite(0)
{
}

void PropRender::ReceiveMsg(COMPONENTMSG_T msg, Component *sender, Entity *source)
{
	if (sender == this)
		return;
}

void PropRender::Render(Vec2 offset)
{
	double secsSinceStateChange = GameTime::TotalElapsedTime() - m_lastStateChangeTime;
	int x = offset.x + m_entity->Pos.x;
	int y = offset.y + m_entity->Pos.y;
	m_sprite->Render(secsSinceStateChange, x, y);
}
