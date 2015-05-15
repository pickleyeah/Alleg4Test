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
	BLOCK_T() :
		flags((BLOCKFLAGS)0),
		warpDetails(nullptr),
		sprite(nullptr),
		overlay(nullptr)
	{
	}
	~BLOCK_T()
	{
		if (warpDetails != nullptr)
			delete warpDetails;
	}
	BLOCKFLAGS flags;
	WARPDETAILS_T *warpDetails;

	Sprite *GetSprite() { return sprite; }
	void SetSprite(const char* filename) { sprite = Sprite::GetSprite(filename); }
	Sprite *GetOverlay() { return overlay; }
	void SetOverlay(const char* filename) { overlay = Sprite::GetSprite(filename); }

private:
	Sprite *sprite;
	Sprite *overlay;
};

class WorldGameState;

class Area
{
public:
	Area(Vec2 size, WorldGameState *world);
	~Area(void);

	Entity *GetEntityAt(int x, int y);
	void AddEntity(Entity *entity) { m_entities.push_back(entity); }

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

