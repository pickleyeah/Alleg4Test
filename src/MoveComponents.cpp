#include "MoveComponents.h"
#include "Vec2.h"
#include "Entity.h"


TestMove::TestMove(std::shared_ptr<ComponentMsgBus> bus) :
	MoveComponent(bus)
{
}


TestMove::~TestMove(void)
{
}

void TestMove::ReceiveMsg(COMPONENTMSG_T msg, Component *sender)
{
	if (sender == this)
		return;
}

void TestMove::Update(Entity *entity, Area *area, double dt)
{
	// Pos += dt*Vel
	Vec2 oldPos = entity->Pos;
	entity->Pos = Vec2::Add(entity->Pos, Vec2::Mul(entity->Vel, (float)dt));
}