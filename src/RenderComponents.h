#pragma once

#include "Components.h"
#include "InputComponents.h"

class TestInput;
class Sprite;

#define WALK_FRAMES_PER_SEC 8

class TestRender : public RenderComponent
{
public:
	TestRender(std::shared_ptr<ComponentMsgBus> bus);
	~TestRender(void);

	void ReceiveMsg(COMPONENTMSG_T msg, Component *sender);
	void Render(Entity *entity, Vec2 offset);

private:
	TE_STATE m_state;
	double m_secsSinceStateChange;

	TestInput *m_input;
	std::vector<Sprite*> m_idleSprites;
	std::vector<Sprite*> m_walkSprites;
};