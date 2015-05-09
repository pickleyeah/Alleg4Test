#pragma once

#include "Components.h"

class TestInput;
class Sprite;

#define WALK_FRAMES_PER_SEC 8

class TestRender : public RenderComponent
{
public:
	TestRender(TestInput* input);
	~TestRender(void);

	void Render(Entity *entity, ALLEGRO_BITMAP *buffer, Vec2 offset);

private:
	TestInput *m_input;
	std::vector<Sprite*> m_idleSprites;
	std::vector<Sprite*> m_walkSprites;
};