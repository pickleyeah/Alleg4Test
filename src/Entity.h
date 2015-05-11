#pragma once

#include "Vec2.h"
#include "Components.h"
#include "ComponentMsgBus.h"

enum DIR {
	DIR_NORTH,
	DIR_EAST,
	DIR_SOUTH,
	DIR_WEST,
};

class Entity
{
public:
	static Entity* CreateEntity(std::string type);

	~Entity(void);

	void Init(Area *area);
	void Remove() { m_remove = true; }

	void ReceiveMsg(COMPONENTMSG_T msg, Component *sender) { m_msgBus->Send(msg, sender); }

	void ProcessInput(double dt) { m_input->ProcessInput(this, dt); }
	void Update(double dt);
	void Render(Vec2 offset) { m_render->Render(this, offset); }

	bool CanMoveTo(int x, int y);

	// Getters/setters
	Area *GetArea() { return m_area; }
	void SetArea(Area *area) { m_area = area; } // Used on Area transitions

	bool Alive() { return m_alive; }
	void SetAlive(bool alive) { m_alive = alive; }

	bool ShouldRemove() { return m_remove; }
	bool Solid() { return true; }

	int GridX() { return m_gridX; }
	void SetGridX(int x) { m_gridX = x; Pos.x = x * 64; }
	int GridY() { return m_gridY; }
	void SetGridY(int y) { m_gridY = y; Pos.y = y * 64; }
	void SetGridXY(int x, int y) { SetGridX(x); SetGridY(y); }
	
	Vec2 Pos;
	Vec2 Vel;
	Vec2 Size;
	DIR Dir;

private:
	Entity(void);

	int m_gridX, m_gridY;
	Area *m_area;
	float m_timeAlive;
	bool m_alive, m_remove;

	std::unique_ptr<InputComponent> m_input;
	std::unique_ptr<MoveComponent> m_move;
	std::unique_ptr<RenderComponent> m_render;
	ComponentMsgBus *m_msgBus;
};


