#pragma once

#include "Components.h"

enum ENTSTATE
{
	ENTSTATE_IDLE,
	ENTSTATE_MOVING,
};

class PlayerInput : public InputComponent
{
public:
	PlayerInput(std::shared_ptr<ComponentMsgBus> bus);
	~PlayerInput(void);

	void ReceiveMsg(COMPONENTMSG_T msg, Component *sender, Entity *source);
	void SetState(ENTSTATE state);
	void ProcessInput(double dt);

private:
	ENTSTATE m_state;
	Vec2 m_oldPos;
	int m_newGridX, m_newGridY;
	double m_secsSinceStateChange;
};

class NPCText
{
public:
	std::vector<std::string> Strings;
};

class NPCTextInput : public InputComponent
{
public:
	NPCTextInput(std::shared_ptr<ComponentMsgBus> bus) : InputComponent(bus) { }
	~NPCTextInput(void) { };

	void ReceiveMsg(COMPONENTMSG_T msg, Component *sender, Entity *source);
	void ProcessInput(double dt);
	void SetText(NPCText *strings) { m_text = std::unique_ptr<NPCText>(strings); }
private:
	std::unique_ptr<NPCText> m_text;
};

