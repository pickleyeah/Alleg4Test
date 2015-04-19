#pragma once

#include <allegro.h>
#include "Vec2.h"
#include "Components.h"

enum DIR {
	DIR_NORTH,
	DIR_EAST,
	DIR_SOUTH,
	DIR_WEST,
};

class Entity
{
public:
	static Entity* MakeTestEntity(Area *area);

	Entity(void);
	virtual ~Entity(void);

	virtual void Init(Area *area);
	virtual void Remove() { m_remove = true; }

	virtual void ProcessInput(double dt) { m_input->ProcessInput(this, dt); }
	virtual void Update(double dt);
	virtual void Render(BITMAP *buffer, Vec2 offset) { m_render->Render(this, buffer, offset); }

	bool CanMoveTo(int x, int y);

	// Getters/setters
	bool Alive() { return m_alive; }
	void SetAlive(bool alive) { m_alive = alive; }

	bool ShouldRemove() { return m_remove; }

	int GridX() { return m_gridX; }
	void SetGridX(int x)
	{
		m_gridX = x;
		Pos.x = x * 64;
	}
	int GridY() { return m_gridY; }
	void SetGridY(int y)
	{
		m_gridY = y;
		Pos.y = y * 64;
	}
	void SetGridXY(int x, int y)
	{
		SetGridX(x);
		SetGridY(y);
	}

	Vec2 Pos;
	Vec2 Vel;
	Vec2 Size;
	DIR Dir;
	
	Area *GetArea() { return m_area; }

protected:
	int m_gridX, m_gridY;
	Area *m_area;

private:
	float m_timeAlive;
	bool m_alive, m_remove;

	InputComponent *m_input;
	MoveComponent *m_move;
	RenderComponent *m_render;
};


