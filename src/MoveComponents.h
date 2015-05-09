#pragma once

#include "Components.h"

class TestMove : public MoveComponent
{
public:
	TestMove(std::shared_ptr<ComponentMsgBus> bus);
	~TestMove(void);

	void ReceiveMsg(COMPONENTMSG_T msg, Component *sender);
	void Update(Entity *entity, Area *area, double dt);
};