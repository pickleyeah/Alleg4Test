#ifndef COMPONENTMSGBUS_H
#define COMPONENTMSGBUS_H

#include <vector>

enum MSGTYPE
{
	MSG_STATECHANGE,
};

struct COMPONENTMSG_T
{
	MSGTYPE type;
	std::shared_ptr<void> data;
};

class Component;

class ComponentMsgBus
{
public:
	ComponentMsgBus();
	~ComponentMsgBus();
	void AddComponent(Component *c) { m_components.push_back(c); }
	void Send(COMPONENTMSG_T msg, Component *sender);
private:
	std::vector<Component*> m_components;
};

#endif