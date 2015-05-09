#include "ComponentMsgBus.h"
#include "Components.h"

ComponentMsgBus::ComponentMsgBus()
{

}

ComponentMsgBus::~ComponentMsgBus()
{
	m_components.clear();
}

void ComponentMsgBus::Send(COMPONENTMSG_T msg, Component *sender)
{
	for (size_t i = 0; i < m_components.size(); i++)
		m_components[i]->ReceiveMsg(msg, sender);
}