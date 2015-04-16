#pragma once
#include "Entity.h"

class TestEntity :
	public Entity
{
public:
	TestEntity(void);
	~TestEntity(void);

	virtual void Init();
	virtual void Update(double dt);
	//virtual void Draw(BITMAP *buffer, Vec2 offset);

private:
	//TE_STATE m_state;
	Vec2 m_oldPos;
};
