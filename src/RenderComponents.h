#pragma once

#include "Components.h"

class TestInput;

#define WALK_FRAMES_PER_SEC 8

class TestRender : public RenderComponent
{
public:
	TestRender(TestInput* input);
	~TestRender(void);

	void Render(Entity *entity, BITMAP *buffer, Vec2 offset);

private:
	TestInput *m_input;
	std::vector<BITMAP*> m_idleSprites;
	std::vector<BITMAP*> m_walkSprites;
	std::vector<BITMAP*> m_spritesToDelete;
};