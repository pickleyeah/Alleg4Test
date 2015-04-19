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

struct WARP_DETAILS_T
{
	std::string areaName;
	Vec2 startPos;
	DIR startDir;
};

class BLOCK_T
{
public:
	BLOCK_T() {}
	~BLOCK_T()
	{
		if (warp)
			delete warpDetails;
	}
	char colMask;
	bool warp;
	WARP_DETAILS_T *warpDetails;
};

class WorldGameState;

class Area
{
public:
	Area(Vec2 size, WorldGameState *world);
	~Area(void);

	static Area *CreateTestArea(Entity *player, WorldGameState *world);
	static Area *CreateTestArea2(Entity *player, WorldGameState *world);

	BLOCK_T *GetBlock(int x, int y) { return &m_blocks[y*(int)m_size.x + x]; }
	Vec2 Size() { return m_size; }

	Entity *GetPlayer() { return m_player; }
	void SetPlayer(Entity *player);

	WorldGameState *GetWorldGameState() { return m_worldGameState; }

	void SetStartPosAndDir(Vec2 pos, DIR dir) { m_startPos = pos; m_startDir = dir; }
	
	void Init();

	void ProcessInput(double dt);
	void Update(double dt);
	void Render(BITMAP *buffer, Vec2 offset);
	void DrawGrid(BITMAP *buffer, Vec2 offset);

private:
	WorldGameState *m_worldGameState;
	Vec2 m_size;
	std::vector<Entity*> m_entities;

	Entity* m_player;
	Vec2 m_startPos;
	DIR m_startDir;

	double m_elapsedTime;

	bool m_showGrid;

	BLOCK_T *m_blocks;
	std::vector<Sprite*> m_sprites;
	Camera* m_camera;
};

