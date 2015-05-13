#pragma once

#include <vector>
#include "Vec2.h"
#include "Sprite.h"
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

struct WARPDETAILS_T
{
	std::string area;
	Vec2 pos;
	DIR dir;
};

class BLOCK_T
{
public:
	BLOCK_T() : colMask(0), warp(false), warpDetails(nullptr)
	{
		sprintf(spriteName, "none");
	}
	~BLOCK_T()
	{
		if (warpDetails != nullptr)
			delete warpDetails;
	}
	char colMask;
	char spriteName[64];
	bool warp;
	WARPDETAILS_T *warpDetails;
};

class WorldGameState;

class Area
{
public:
	friend class XMLAreaLoader;
	static const char Area::MAGIC_NUM[];
	static Area* LoadArea(const char* filename, Entity *player, WorldGameState *world);
	void Write(const char* filename);

	Area(Vec2 size, WorldGameState *world);
	~Area(void);

	Entity *GetEntityAt(int x, int y);
	BLOCK_T *GetBlock(int x, int y) { return &m_blocks[y*(int)m_size.x + x]; }
	Vec2 Size() { return m_size; }

	Entity *GetPlayer() { return m_player; }
	void SetPlayer(Entity *player);

	WorldGameState *GetWorldGameState() { return m_worldGameState; }

	void SetStartPosAndDir(Vec2 pos, DIR dir) { m_startPos = pos; m_startDir = dir; }
	
	void Init();

	void ProcessInput(double dt);
	void Update(double dt);
	void Render(Vec2 offset);
	void DrawGrid(Vec2 offset);
	void BroadcastMsgToEntities(COMPONENTMSG_T msg, Component *sender, Entity *source);

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
	std::unique_ptr<Camera> m_camera;
};

