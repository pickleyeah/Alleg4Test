#include "Entity.h"
#include "TestInput.h"
#include "TestMove.h"
#include "TestRender.h"
#include "IntroGameState.h"


Entity::Entity(void)
{
}


Entity::~Entity(void)
{
}

Entity* Entity::MakeTestEntity(Area *area)
{
	Entity* result = new Entity();
	result->m_input = new TestInput();
	result->m_move = new TestMove();
	result->m_render = new TestRender();

	result->Size = Vec2(IntroGameState::BLOCK_SIZE,IntroGameState::BLOCK_SIZE);
	result->Dir = DIR_NORTH;
	return result;
}

void Entity::Init(Area *area)
{
	m_timeAlive = 0;
	m_alive = true;
	m_remove = false;
	m_area = area;
}

void Entity::Update(double dt)
{
	m_timeAlive += dt;

	m_move->Update(this, NULL, dt);
	// Pos += dt*Vel
	//Pos = Vec2::Add(Pos, Vec2::Mul(Vel, (float)dt));
}
