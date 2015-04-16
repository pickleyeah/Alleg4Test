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

	void ProcessInput(Entity *entity);

private:
	TE_STATE m_state;
	Vec2 m_oldPos;
	int m_newGridX, m_newGridY;
};

