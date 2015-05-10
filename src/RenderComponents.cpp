#include "RenderComponents.h"
#include "InputComponents.h"

#include "Vec2.h"
#include "Entity.h"
#include "Sprite.h"
#include "GameTime.h"


TestRender::TestRender(std::shared_ptr<ComponentMsgBus> bus) :
	RenderComponent(bus),
	m_state(TE_IDLE),
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

TestRender::~TestRender(void)
{
}

void TestRender::ReceiveMsg(COMPONENTMSG_T msg, Component *sender)
{
	if (sender == this)
		return;
	switch (msg.type)
	{
	case MSG_STATECHANGE:
		m_state = *((TE_STATE*)msg.data.get());
		m_lastStateChangeTime = GameTime::TotalElapsedTime();
		printf("State change: %d\n", m_state);
		break;
	}
}

void TestRender::Render(Entity *entity, Vec2 offset)
{
	double secsSinceStateChange = GameTime::TotalElapsedTime() - m_lastStateChangeTime;

	Vec2 Pos = entity->Pos;
	Vec2 Size = entity->Size;

	int x = offset.x + Pos.x;
	int y = offset.y + Pos.y;

	Sprite *sprite = nullptr;
	switch (m_state)
	{
	case TE_IDLE:
		sprite = m_idleSprites[entity->Dir];
		break;
	case TE_MOVING:
		sprite = m_walkSprites[entity->Dir];
		break;
	}
	sprite->Render(secsSinceStateChange, x, y);
}
