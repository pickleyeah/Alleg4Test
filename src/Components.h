#pragma once

#include "Vec2.h"

class Entity;
class Area;

class InputComponent
{
public:
	virtual ~InputComponent(void) {};
	virtual void ProcessInput(Entity *entity, double dt) = 0;
};

class RenderComponent
{
public:
	virtual ~RenderComponent(void) {};
	virtual void Render(Entity *entity, ALLEGRO_BITMAP *buffer, Vec2 offset) = 0;
};

class MoveComponent
{
public:
	virtual ~MoveComponent(void) {};
	virtual void Update(Entity *entity, Area *area, double dt) = 0;
};