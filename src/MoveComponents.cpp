#include "MoveComponents.h"
#include "Vec2.h"
#include "Entity.h"


TestMove::TestMove(void)
{
}


TestMove::~TestMove(void)
{
}

void TestMove::Update(Entity *entity, Area *area, double dt)
{
	// Pos += dt*Vel
	Vec2 oldPos = entity->Pos;
	entity->Pos = Vec2::Add(entity->Pos, Vec2::Mul(entity->Vel, (float)dt));
}