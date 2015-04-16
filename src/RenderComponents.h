#pragma once

#include "Components.h"

class TestRender : public RenderComponent
{
public:
	TestRender(void);
	~TestRender(void);

	void Render(Entity *entity, BITMAP *buffer, Vec2 offset);
};