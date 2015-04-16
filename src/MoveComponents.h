#pragma once

#include "Components.h"

class TestMove : public MoveComponent
{
public:
	TestMove(void);
	~TestMove(void);

	void Update(Entity *entity, Area *area, double dt);
};