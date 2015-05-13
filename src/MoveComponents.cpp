#include "MoveComponents.h"
#include "Vec2.h"
#include "Entity.h"


DefaultMove::DefaultMove(std::shared_ptr<ComponentMsgBus> bus) :
	MoveComponent(bus)
{
}


DefaultMove::~DefaultMove(void)
{
}

void DefaultMove::ReceiveMsg(COMPONENTMSG_T msg, Component *sender, Entity *source)
{
	if (sender == this)
		return;
}

void DefaultMove::Update(double dt)
{
	// Pos += dt*Vel
	Vec2 oldPos = m_entity->Pos;
	m_entity->Pos = m_entity->Pos + m_entity->Vel * (float)dt;
}