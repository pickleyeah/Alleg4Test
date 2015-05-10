#pragma once

#include "Components.h"

enum TE_STATE
{
	TE_IDLE,
	TE_MOVING,
};

class PlayerInput : public InputComponent
{
public:
	PlayerInput(std::shared_ptr<ComponentMsgBus> bus);
	~PlayerInput(void);

	void ReceiveMsg(COMPONENTMSG_T msg, Component *sender);
	void SetState(TE_STATE state);
	void ProcessInput(Entity *entity, double dt);

private:
	TE_STATE m_state;
	Vec2 m_oldPos;
	int m_newGridX, m_newGridY;
	double m_secsSinceStateChange;
};

class NPCTextInput : public InputComponent
{
public:
	NPCTextInput(std::shared_ptr<ComponentMsgBus> bus) : InputComponent(bus) { }
	~NPCTextInput(void) { };

	void ReceiveMsg(COMPONENTMSG_T msg, Component *sender);
	void ProcessInput(Entity *entity, double dt);
};

