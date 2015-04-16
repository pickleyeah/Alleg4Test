#pragma once

#include "Entity.h"

// The camera centers the screen around a certain entity (ie: player)
class Camera
{
public:
	Camera(Entity *entity);

	void ProcessInput();
	void Update(double dt);

	Vec2 GetOffset() { return Vec2::Mul(m_pos, -1); }
	Vec2 Pos() { return m_pos;  }

private:
	Vec2 m_pos;
	Entity *m_entity;
};