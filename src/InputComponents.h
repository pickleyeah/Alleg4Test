#pragma once

#include "Components.h"

enum TE_STATE
{
	TE_IDLE,
	TE_MOVING,
};

class TestInput :
	public InputComponent
{
public:
	TestInput(void);
	~TestInput(void);

	TE_STATE GetState() { return m_state; }
	double TimeSinceStateChange() { return m_secsSinceStateChange; }

	void ProcessInput(Entity *entity, double dt);

private:
	bool CanMoveTo(Entity *entity, Area *area, int x, int y);
	TE_STATE m_state;
	Vec2 m_oldPos;
	int m_newGridX, m_newGridY;
	double m_secsSinceStateChange;
};

