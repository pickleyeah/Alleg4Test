#pragma once

#include "Components.h"

enum TE_STATE
{
	TE_IDLE,
	TE_MOVING,
};

class TestInput :
	public InputComponent
{
public:
	TestInput(std::shared_ptr<ComponentMsgBus> bus);
	~TestInput(void);

	void ReceiveMsg(COMPONENTMSG_T msg, Component *sender);

	TE_STATE GetState() { return m_state; }
	void SetState(TE_STATE state)
	{
		m_state = state;
		GetMsgBus()->Send(COMPONENTMSG_T{ MSG_STATECHANGE, std::shared_ptr<TE_STATE>(new TE_STATE(state)) }, this);
	}
	double TimeSinceStateChange() { return m_secsSinceStateChange; }

	void ProcessInput(Entity *entity, double dt);

private:
	TE_STATE m_state;
	Vec2 m_oldPos;
	int m_newGridX, m_newGridY;
	double m_secsSinceStateChange;
};

