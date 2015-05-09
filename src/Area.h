#pragma once

#include <vector>
#include "Vec2.h"
//#include "Sprite.h"
#include "Entity.h"
#include "Camera.h"

enum BLOCKFLAGS
{
	COLLIDE_NORTH = 0x01,
	COLLIDE_WEST = 0x02,
	COLLIDE_SOUTH = 0x04,
	COLLIDE_EAST = 0x08,
	COLLIDE_ALL = 0x0f,
	HAS_WARP = 0x10,

};

// TODO: Retire these in favour of BLOCKFLAGS
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
	const char areaName[32];
	Vec2 startPos;
	DIR startDir;
};

class Sprite;

class BLOCK_T
{
public:
	BLOCK_T() : colMask(0), warp(false)
	{
		sprintf(spriteName, "none");
	}
	~BLOCK_T()
	{
		if (warp)
			delete warpDetails;
	}
	char colMask;
	char spriteName[64];
	bool warp;
	WARP_DETAILS_T *warpDetails;
};

class WorldGameState;

class Area
{
public:
	static const char Area::MAGIC_NUM[];
	static Area* LoadArea(const char* filename, Entity *player, WorldGameState *world);
	void Write(const char* filename);

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
	void Render(ALLEGRO_BITMAP *buffer, Vec2 offset);
	void DrawGrid(ALLEGRO_BITMAP *buffer, Vec2 offset);

private:
	WorldGameState *m_worldGameState;
	Vec2 m_size;
	std::vector<Entity*> m_entities;

	Entity* m_player;
	Vec2 m_startPos;
	DIR m_startDir;

	double m_elapsedTime;

	bool m_showGrid;

	std::unique_ptr<BLOCK_T[]> m_blocks;
	std::vector<Sprite*> m_sprites;
	Camera* m_camera;
};

