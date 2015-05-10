#pragma once

#include "Components.h"
#include "InputComponents.h"
#include "Sprite.h"

class PlayerInput;

#define WALK_FRAMES_PER_SEC 8

class PlayerRender : public RenderComponent
{
public:
	PlayerRender(std::shared_ptr<ComponentMsgBus> bus);

	void ReceiveMsg(COMPONENTMSG_T msg, Component *sender);
	void Render(Entity *entity, Vec2 offset);

private:
	TE_STATE m_state;
	double m_lastStateChangeTime;

	PlayerInput *m_input;
	std::vector<Sprite*> m_idleSprites;
	std::vector<Sprite*> m_walkSprites;
};

class PropRender : public RenderComponent
{
public:
	PropRender(std::shared_ptr<ComponentMsgBus> bus);
	void SetSprite(Sprite *sprite) { m_sprite = sprite; }
	void ReceiveMsg(COMPONENTMSG_T msg, Component *sender);
	void Render(Entity *entity, Vec2 offset);
private:
	double m_lastStateChangeTime;
	Sprite *m_sprite;
};