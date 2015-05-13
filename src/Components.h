#pragma once

#include "ComponentMsgBus.h"
#include "Vec2.h"

class Entity;
class Area;

class Component
{
public:
	Component(std::shared_ptr<ComponentMsgBus> bus) :
		m_msgBus(bus),
		m_entity(nullptr)
	{
		bus.get()->AddComponent(this);
	};
	virtual ~Component() = 0;
	virtual void ReceiveMsg(COMPONENTMSG_T msg, Component *sender, Entity *source) = 0;
	void SetEntity(Entity *entity) { m_entity = entity; }
protected:
	std::shared_ptr<ComponentMsgBus> m_msgBus;
	Entity *m_entity;
};

class InputComponent : public Component
{
public:
	InputComponent(std::shared_ptr<ComponentMsgBus> bus) :
		Component(bus)
	{}
	virtual ~InputComponent() { }
	virtual void ProcessInput(double dt) = 0;
};

class RenderComponent : public Component
{
public:
	RenderComponent(std::shared_ptr<ComponentMsgBus> bus) :
		Component(bus)
	{}
	virtual ~RenderComponent() {};
	virtual void Render(Vec2 offset) = 0;
};

class MoveComponent : public Component
{
public:
	MoveComponent(std::shared_ptr<ComponentMsgBus> bus) :
		Component(bus)
	{}
	virtual ~MoveComponent() {};
	virtual void Update(double dt) = 0;
};