#include "TestEntity.h"
#include "IntroGameState.h"
#include <allegro.h>


TestEntity::TestEntity(void)
{
}


TestEntity::~TestEntity(void)
{
}

void TestEntity::Init()
{
	//Entity::Init();
	Size = Vec2(IntroGameState::BLOCK_SIZE,IntroGameState::BLOCK_SIZE);
	Dir = DIR_NORTH;
	//m_state = TE_IDLE;
}

//static int MOVESPEED = 192;
void TestEntity::Update(double dt)
{
	Entity::Update(dt);

	//switch (m_state)
	//{
	//case TE_IDLE:
	//	m_oldPos = Pos;
	//	if (key[KEY_LEFT])
	//	{
	//		Vel.x = -MOVESPEED;
	//		Dir = DIR_WEST;
	//		m_state = TE_MOVING;
	//	}
	//	else if (key[KEY_RIGHT])
	//	{
	//		Vel.x = MOVESPEED;
	//		Dir = DIR_EAST;
	//		m_state = TE_MOVING;
	//	}
	//	if (key[KEY_UP])
	//	{
	//		Vel.y = -MOVESPEED;
	//		Dir = DIR_NORTH;
	//		m_state = TE_MOVING;
	//	}
	//	else if (key[KEY_DOWN])
	//	{
	//		Vel.y = MOVESPEED;
	//		Dir = DIR_SOUTH;
	//		m_state = TE_MOVING;
	//	}
	//	break;
	//case TE_MOVING:
	//	// Once we've moved a whole grid space, clamp to the grid
	//	Vec2 moved = Vec2::Sub(Pos, m_oldPos);
	//	if (moved.Length() >= IntroGameState::BLOCK_SIZE)
	//	{
	//		moved.Normalise();
	//		moved = Vec2::Mul(moved, IntroGameState::BLOCK_SIZE);
	//		Pos = Vec2::Add(m_oldPos, moved);
	//		// Mitigate accumulated floating precision error
	//		Pos.x = (int)Pos.x;
	//		Pos.y = (int)Pos.y;
	//		Vel = Vec2(0,0);
	//		m_state = TE_IDLE;
	//	}
	//	break;
	//}
}

//void TestEntity::Draw(BITMAP *buffer, Vec2 offset)
//{
//	int x1 = offset.x + Pos.x;
//	int y1 = offset.y + Pos.y;
//	int x2 = offset.x + Pos.x + Size.x;
//	int y2 = offset.y + Pos.y + Size.y;
//	int cx = (x1 + x2) / 2;
//	int cy = (y1 + y2) / 2;
//	
//	//rectfill(buffer, x1, y1, x2, y2, makecol(255,0,0));
//	circlefill(buffer, cx, cy, Size.x / 2, makecol(0,255,0));
//
//	int x3 = cx, y3 = cy;
//	switch (Dir)
//	{
//	case DIR_NORTH:
//		y3 -= Size.y / 2;
//		break;
//	case DIR_EAST:
//		x3 += Size.x / 2;
//		break;
//	case DIR_SOUTH:
//		y3 += Size.y / 2;
//		break;
//	case DIR_WEST:
//		x3 -= Size.x / 2;
//		break;
//	}
//	line(buffer, cx, cy, x3, y3, makecol(0,0,0));
//}
