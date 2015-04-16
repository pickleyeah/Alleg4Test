#pragma once

#include <vector>
#include "Vec2.h"
#include "Entity.h"

enum COL_MASK
{
	COL_NORTH = 0x01,
	COL_WEST = 0x02,
	COL_SOUTH = 0x04,
	COL_EAST = 0x08,
	COL_ALL = 0x0f,
};

struct BLOCK_T
{
	int color;
	char colMask;
};

class Area
{
public:
	Area(Vec2 size);
	~Area(void);

	static Area *CreateTestArea();

	BLOCK_T *GetBlock(int x, int y) { return &m_blocks[y*(int)m_size.x + x]; }
	Vec2 Size() { return m_size; }

	void Init();

	void ProcessInput();
	void Update(double dt);
	void Render(BITMAP *buffer, Vec2 offset);
	void DrawGrid(BITMAP *buffer, Vec2 offset);

private:
	Vec2 m_size;
	std::vector<Entity*> m_entities;
	BLOCK_T *m_blocks;
};

