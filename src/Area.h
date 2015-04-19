#pragma once

#include <vector>
#include "Vec2.h"
#include "Sprite.h"
#include "Entity.h"
#include "Camera.h"

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
	char colMask;
};

class Area
{
public:
	Area(Vec2 size);
	~Area(void);

	static Area *CreateTestArea(Entity *player);
	static Area *CreateTestArea2(Entity *player);

	BLOCK_T *GetBlock(int x, int y) { return &m_blocks[y*(int)m_size.x + x]; }
	Vec2 Size() { return m_size; }

	Entity *GetPlayer() { return m_player; }
	void SetPlayer(Entity *player);
	
	void Init();

	void ProcessInput(double dt);
	void Update(double dt);
	void Render(BITMAP *buffer, Vec2 offset);
	void DrawGrid(BITMAP *buffer, Vec2 offset);

private:
	Vec2 m_size;
	std::vector<Entity*> m_entities;
	Entity* m_player;

	double m_elapsedTime;

	bool m_showGrid;

	BLOCK_T *m_blocks;
	std::vector<Sprite> m_sprites;
	Camera* m_camera;
};

