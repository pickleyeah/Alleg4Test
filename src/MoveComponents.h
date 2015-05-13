#pragma once

#include "Components.h"

class DefaultMove : public MoveComponent
{
public:
	DefaultMove(std::shared_ptr<ComponentMsgBus> bus);
	~DefaultMove(void);

	void ReceiveMsg(COMPONENTMSG_T msg, Component *sender, Entity *source);
	void Update(double dt);
};