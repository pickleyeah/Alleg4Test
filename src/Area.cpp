#include "Area.h"

#include <allegro5/allegro_primitives.h>
#include <string.h>
#include "Game.h"
#include "WorldGameState.h"
#include "Sprite.h"

Area::Area(Vec2 size, WorldGameState *world) :
	m_size(size),
	m_showGrid(false),
	m_worldGameState(world),
	m_startPos(Vec2(0,0)),
	m_startDir(DIR_SOUTH),
	m_player(nullptr),
	m_elapsedTime(0)
{
	m_blocks = std::unique_ptr<BLOCK_T[]>(new BLOCK_T[(int)(size.x * size.y)]);
	m_sprites.resize(COLLIDE_ALL+1, nullptr);

	m_sprites[0] = Sprite::GetSprite("Data/Tiles/Grass.png");
	m_sprites[COLLIDE_NORTH] = Sprite::GetSprite("Data/Tiles/Grass_BlockN.png");
	m_sprites[COLLIDE_SOUTH] = Sprite::GetSprite("Data/Tiles/Grass_BlockS.png");
	m_sprites[COLLIDE_WEST] = Sprite::GetSprite("Data/Tiles/Grass_BlockW.png");
	m_sprites[COLLIDE_EAST] = Sprite::GetSprite("Data/Tiles/Grass_BlockE.png");

	m_sprites[COLLIDE_NORTH | COLLIDE_WEST] = Sprite::GetSprite("Data/Tiles/Grass_BlockNW.png");
	m_sprites[COLLIDE_NORTH | COLLIDE_EAST] = Sprite::GetSprite("Data/Tiles/Grass_BlockNE.png");
	m_sprites[COLLIDE_SOUTH | COLLIDE_WEST] = Sprite::GetSprite("Data/Tiles/Grass_BlockSW.png");
	m_sprites[COLLIDE_SOUTH | COLLIDE_EAST] = Sprite::GetSprite("Data/Tiles/Grass_BlockSE.png");

	m_sprites[COLLIDE_ALL] = Sprite::GetSprite("Data/Tiles/Water.png");
}


Area::~Area(void)
{
	for (size_t i = 0; i < m_entities.size(); i++)
		delete m_entities[i];
}

Entity *Area::GetEntityAt(int x, int y)
{
	for (size_t i = 0; i < m_entities.size(); i++)
	{
		Vec2 gridTopLeft = Vec2(m_entities[i]->GridX(), m_entities[i]->GridY());
		Vec2 gridBottomRight = gridTopLeft + Vec2((int)(m_entities[i]->Size.x / WorldGameState::BLOCK_SIZE), (int)(m_entities[i]->Size.y / WorldGameState::BLOCK_SIZE));
		if (x >= gridTopLeft.x && x < gridBottomRight.x && y >= gridTopLeft.y && y < gridBottomRight.y)
			return m_entities[i];
	}
	return nullptr;
}

struct MAP_HEADER_T
{
	char header[8];
	short width;
	short height;
	short entityCount;
	short spare;
};

void Area::SetPlayer(Entity *player)
{
	// Transfer object ownership - remove it from the other area's entity list
	// http://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom
	// well played, c++. well played.
	Area *oldArea = player->GetArea();
	if (oldArea != nullptr)
		oldArea->m_entities.erase(std::remove(oldArea->m_entities.begin(), oldArea->m_entities.end(), player), oldArea->m_entities.end());
	// Assign the player to the new area
	m_player = player;
	m_entities.push_back(player);
	m_player->SetArea(this);
}

void Area::Init()
{
	m_elapsedTime = 0;
	for (size_t i = 0; i < m_entities.size(); i++)
		m_entities[i]->Init(this);
	m_player->SetGridXY(m_startPos.x, m_startPos.y);
	m_player->Dir = m_startDir;
	m_camera = std::unique_ptr<Camera>(new Camera(m_player));
	m_camera->Update(0);
}

void Area::ProcessInput(double dt)
{
	if (Input::KeyPressed(ALLEGRO_KEY_G))
		m_showGrid = !m_showGrid;
	for (size_t i = 0; i < m_entities.size(); i++)
		m_entities[i]->ProcessInput(dt);
	m_camera->ProcessInput();
}

void Area::Update(double dt)
{
	m_elapsedTime += dt;
	for (size_t i = 0; i < m_entities.size(); i++)
		m_entities[i]->Update(dt);
	m_camera->Update(dt);

	// Remove entities awaiting removal
	for (size_t i = m_entities.size(); i > 0; i--)
	{
		if (m_entities[i-1]->ShouldRemove())
		{
			// TODO: turn Entity pointers into smart pointers
			delete m_entities[i - 1];
			m_entities.erase(m_entities.begin() + i - 1);
		}
	}
}

void Area::Render(Vec2 offset)
{
	offset = m_camera->GetOffset();
	// Render blocks
	for (int i = 0; i < (int)m_size.x; i++)
	{
		for (int j = 0; j < (int)m_size.y; j++)
		{
			int x = (int)offset.x + i * 64;
			int y = (int)offset.y + j * 64;
			int spriteIndex = GetBlock(i, j)->flags;
			if (GetBlock(i, j)->GetSprite())
				GetBlock(i, j)->GetSprite()->Render(m_elapsedTime, x, y);
			if (GetBlock(i, j)->GetOverlay())
				GetBlock(i, j)->GetOverlay()->Render(m_elapsedTime, x, y);
		}
	}

	if (m_showGrid)
		DrawGrid(offset);

	// Move the entity just a little bit higher to give the illusion of depth
	for (size_t i = 0; i < m_entities.size(); i++)
		m_entities[i]->Render(offset + Vec2(0,-12));

	// Borders
	/*al_draw_filled_rectangle(0, 0, 32, Game::SCREEN_Y, al_map_rgb(32, 32, 32));
	al_draw_filled_rectangle(Game::SCREEN_X - 32, 0, Game::SCREEN_X, Game::SCREEN_Y, al_map_rgb(32, 32, 32));

	al_draw_filled_rectangle(0, 0, Game::SCREEN_X, 16, al_map_rgb(32, 32, 32));
	al_draw_filled_rectangle(0, Game::SCREEN_Y - 16, Game::SCREEN_X, Game::SCREEN_Y, al_map_rgb(32, 32, 32));*/
}

static const int BLOCK_SIZE = 64;
void Area::DrawGrid(Vec2 offset)
{
	int sizeX = m_size.x * BLOCK_SIZE;
	int sizeY = m_size.y * BLOCK_SIZE;
	for (int i = 0; i <= sizeX; i += BLOCK_SIZE)
		al_draw_line(i + (int)offset.x, 0 + (int)offset.y, i + (int)offset.x, sizeY + (int)offset.y, al_map_rgb(255, 255, 255), 1);
	for (int i = 0; i <= sizeY; i += BLOCK_SIZE)
		al_draw_line(0 + (int)offset.x, i + (int)offset.y, sizeX + (int)offset.x, i + (int)offset.y, al_map_rgb(255, 255, 255), 1);
}

void Area::BroadcastMsgToEntities(COMPONENTMSG_T msg, Component *sender, Entity *source)
{
	for (size_t i = 0; i < m_entities.size(); i++)
		m_entities[i]->ReceiveMsg(msg, sender, source);
}
