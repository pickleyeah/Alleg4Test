#pragma once

#include "ComponentMsgBus.h"
#include "Vec2.h"

class Entity;
class Area;

class Component
{
public:
	Component(std::shared_ptr<ComponentMsgBus> bus) :
		m_msgBus(bus)
	{
		bus.get()->AddComponent(this);
	};
	virtual ~Component() = 0;
	virtual void ReceiveMsg(COMPONENTMSG_T msg, Component *sender) = 0;
	ComponentMsgBus *GetMsgBus() { return m_msgBus.get(); }
private:
	std::shared_ptr<ComponentMsgBus> m_msgBus;
};

class InputComponent : public Component
{
public:
	InputComponent(std::shared_ptr<ComponentMsgBus> bus) :
		Component(bus)
	{}
	virtual ~InputComponent() { }
	virtual void ProcessInput(Entity *entity, double dt) = 0;
};

class RenderComponent : public Component
{
public:
	RenderComponent(std::shared_ptr<ComponentMsgBus> bus) :
		Component(bus)
	{}
	virtual ~RenderComponent() {};
	virtual void Render(Entity *entity, Vec2 offset) = 0;
};

class MoveComponent : public Component
{
public:
	MoveComponent(std::shared_ptr<ComponentMsgBus> bus) :
		Component(bus)
	{}
	virtual ~MoveComponent() {};
	virtual void Update(Entity *entity, Area *area, double dt) = 0;
};