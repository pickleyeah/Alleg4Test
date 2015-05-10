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

void DefaultMove::ReceiveMsg(COMPONENTMSG_T msg, Component *sender)
{
	if (sender == this)
		return;
}

void DefaultMove::Update(Entity *entity, Area *area, double dt)
{
	// Pos += dt*Vel
	Vec2 oldPos = entity->Pos;
	entity->Pos = Vec2::Add(entity->Pos, Vec2::Mul(entity->Vel, (float)dt));
}