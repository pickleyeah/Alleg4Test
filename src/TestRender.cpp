#include "TestRender.h"
#include "Vec2.h"
#include "Entity.h"


TestRender::TestRender(void)
{
}


TestRender::~TestRender(void)
{
}

void TestRender::Render(Entity *entity, BITMAP *buffer, Vec2 offset)
{
	Vec2 Pos = entity->Pos;
	Vec2 Size = entity->Size;

	int x1 = offset.x + Pos.x;
	int y1 = offset.y + Pos.y;
	int x2 = offset.x + Pos.x + Size.x;
	int y2 = offset.y + Pos.y + Size.y;
	int cx = (x1 + x2) / 2;
	int cy = (y1 + y2) / 2;
	
	//rectfill(buffer, x1, y1, x2, y2, makecol(255,0,0));
	circlefill(buffer, cx, cy, Size.x / 2, makecol(0,255,0));

	int x3 = cx, y3 = cy;
	switch (entity->Dir)
	{
	case DIR_NORTH:
		y3 -= Size.y / 2;
		break;
	case DIR_EAST:
		x3 += Size.x / 2;
		break;
	case DIR_SOUTH:
		y3 += Size.y / 2;
		break;
	case DIR_WEST:
		x3 -= Size.x / 2;
		break;
	}
	line(buffer, cx, cy, x3, y3, makecol(0,0,0));
}
