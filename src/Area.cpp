#include "Area.h"

#include <string.h>
#include "Game.h"
#include "Sprite.h"

Area::Area(Vec2 size, WorldGameState *world) :
	m_size(size),
	m_showGrid(false),
	m_worldGameState(world),
	m_startPos(Vec2(0,0)),
	m_startDir(DIR_SOUTH)
{
	m_blocks = std::unique_ptr<BLOCK_T[]>(new BLOCK_T[(int)(size.x * size.y)]);
	m_sprites.resize(COL_ALL+1, nullptr);

	m_sprites[0] = Sprite::GetSprite("Data/Tiles/Grass.bmp", 1, 1);
	m_sprites[COL_NORTH] = Sprite::GetSprite("Data/Tiles/Grass_BlockN.bmp", 1, 1);
	m_sprites[COL_SOUTH] = Sprite::GetSprite("Data/Tiles/Grass_BlockS.bmp", 1, 1);
	m_sprites[COL_WEST] = Sprite::GetSprite("Data/Tiles/Grass_BlockW.bmp", 1, 1);
	m_sprites[COL_EAST] = Sprite::GetSprite("Data/Tiles/Grass_BlockE.bmp", 1, 1);

	m_sprites[COL_NORTH | COL_WEST] = Sprite::GetSprite("Data/Tiles/Grass_BlockNW.bmp", 1, 1);
	m_sprites[COL_NORTH | COL_EAST] = Sprite::GetSprite("Data/Tiles/Grass_BlockNE.bmp", 1, 1);
	m_sprites[COL_SOUTH | COL_WEST] = Sprite::GetSprite("Data/Tiles/Grass_BlockSW.bmp", 1, 1);
	m_sprites[COL_SOUTH | COL_EAST] = Sprite::GetSprite("Data/Tiles/Grass_BlockSE.bmp", 1, 1);

	m_sprites[COL_ALL] = Sprite::GetSprite("Data/Tiles/Water.bmp", 4, 4);
}


Area::~Area(void)
{
	delete m_camera;
	//delete[] m_blocks;
	for (size_t i = 0; i < m_entities.size(); i++)
		delete m_entities[i];
}

struct MAP_HEADER_T
{
	char header[8];
	short width;
	short height;
	short entityCount;
	short spare;
};

const char Area::MAGIC_NUM[] = { 'M', 'A', 'P', 'T', 'E', 'S', 'T', '\0' };

Area* Area::LoadArea(const char* filename, Entity *player, WorldGameState *world)
{
	FILE *fp = fopen(filename, "rb");
	if (!fp)
		return false;
	MAP_HEADER_T header;
	fread(&header, sizeof(MAP_HEADER_T), 1, fp);
	if (memcmp(header.header, MAGIC_NUM, sizeof(MAGIC_NUM)) != 0)
		return nullptr;	// The header doesn't match, file must be wrong
	Area* result = new Area(Vec2(header.width, header.height), world);
	// Read blocks
	int blockCount = header.width * header.height;
	for (int i = 0; i < blockCount; i++)
	{
		fread(&result->m_blocks[i].colMask, sizeof(result->m_blocks[i].colMask), 1, fp);
		fread(&result->m_blocks[i].spriteName, sizeof(result->m_blocks[i].spriteName), 1, fp);
		fread(&result->m_blocks[i].warp, sizeof(result->m_blocks[i].warp), 1, fp);
		if (result->m_blocks[i].warp)
		{
			result->m_blocks[i].warpDetails = new WARP_DETAILS_T{ "" };
			fread(result->m_blocks[i].warpDetails, sizeof(*result->m_blocks[i].warpDetails), 1, fp);
		}
	}
	fclose(fp);
	// Load entities
	/*m_entities = new ENTITY[m_entityCount];
	if (fread(m_entities, sizeof(ENTITY), m_entityCount, fp) != m_entityCount)
		return false;*/
	// Set player
	if (player == nullptr)
	{
		player = Entity::CreatePlayerEntity(result);
		result->SetPlayer(player);
	}
	
	return result;
}

void Area::Write(const char* filename)
{
	FILE *fp = fopen(filename, "wb");
	if (!fp)
		return;
	// Write header
	MAP_HEADER_T header;
	memcpy(header.header, MAGIC_NUM, sizeof(MAGIC_NUM));
	header.width = (short)m_size.x;
	header.height = (short)m_size.y;
	header.entityCount = 0;
	header.spare = 0;
	fwrite(&header, sizeof(MAP_HEADER_T), 1, fp);
	// Write blocks
	for (int i = 0; i < (int)(m_size.x * m_size.y); i++)
	{
		fwrite(&m_blocks[i].colMask, sizeof(m_blocks[i].colMask), 1, fp);
		fwrite(&m_blocks[i].spriteName, sizeof(m_blocks[i].spriteName), 1, fp);
		fwrite(&m_blocks[i].warp, sizeof(m_blocks[i].warp), 1, fp);
		if (m_blocks[i].warp)
			fwrite(m_blocks[i].warpDetails, sizeof(*m_blocks[i].warpDetails), 1, fp);
	}

	fclose(fp);
}

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

Area *Area::CreateTestArea(Entity *player, WorldGameState *world)
{
	int m = 16, n = 16;
	Area *result = new Area(Vec2(m, n), world);
	memset(result->m_blocks.get(), 0, sizeof(BLOCK_T)*m*n);

	// Water block
	for (int x = 4; x < 8; x++)
	{
		for (int y = 4; y < 8; y++)
		{
			BLOCK_T *block = result->GetBlock(x, y);
			block->colMask |= COL_ALL;
		}
	}

	// Warp block
	result->GetBlock(6, 0)->warp = true;
	result->GetBlock(6, 0)->warpDetails = new WARP_DETAILS_T{ "Data/Areas/Area2.lvl", Vec2(2, 3), DIR_NORTH };
	// Player
	if (player == nullptr)
	{
		player = Entity::CreatePlayerEntity(result);
		result->SetPlayer(player);
	}
	return result;
}

Area *Area::CreateTestArea2(Entity *player, WorldGameState *world)
{
	int m = 4, n = 4; // small-ass area
	Area *result = new Area(Vec2(m, n), world);
	memset(result->m_blocks.get(), 0, sizeof(BLOCK_T)*m*n);

	// Warp block
	result->GetBlock(2, 3)->warp = true;
	result->GetBlock(2, 3)->warpDetails = new WARP_DETAILS_T{ "Data/Areas/Area1.lvl", Vec2(6, 0), DIR_SOUTH };

	return result;
}

void Area::Init()
{
	m_elapsedTime = 0;
	for (size_t i = 0; i < m_entities.size(); i++)
		m_entities[i]->Init(this);
	m_player->SetGridXY(m_startPos.x, m_startPos.y);
	m_player->Dir = m_startDir;
	m_camera = new Camera(m_player);
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
}

void Area::Render(ALLEGRO_BITMAP *buffer, Vec2 offset)
{
	offset = m_camera->GetOffset();
	// Render blocks
	for (int i = 0; i < (int)m_size.x; i++)
	{
		for (int j = 0; j < (int)m_size.y; j++)
		{
			int x = (int)offset.x + i * 64;
			int y = (int)offset.y + j * 64;
			int spriteIndex = GetBlock(i, j)->colMask;
			if (!GetBlock(i, j)->warp)
				m_sprites[spriteIndex]->Render(buffer, m_elapsedTime, x, y);
		}
	}

	if (m_showGrid)
		DrawGrid(buffer, offset);

	for (size_t i = 0; i < m_entities.size(); i++)
		m_entities[i]->Render(buffer, offset);

	// Borders
	/*rectfill(buffer, 0, 0, 32, Game::SCREEN_Y, makecol(32, 32, 32));
	rectfill(buffer, Game::SCREEN_X - 32, 0, Game::SCREEN_X, Game::SCREEN_Y, makecol(32, 32, 32));

	rectfill(buffer, 0, 0, Game::SCREEN_X, 16, makecol(32, 32, 32));
	rectfill(buffer, 0, Game::SCREEN_Y - 16, Game::SCREEN_X, Game::SCREEN_Y, makecol(32, 32, 32));*/
}

static const int BLOCK_SIZE = 64;
void Area::DrawGrid(ALLEGRO_BITMAP *buffer, Vec2 offset)
{
	/*int sizeX = m_size.x * BLOCK_SIZE;
	int sizeY = m_size.y * BLOCK_SIZE;
	for (int i = 0; i <= sizeX; i += BLOCK_SIZE)
		vline(buffer, i + (int)offset.x, 0 + (int)offset.y, sizeY + (int)offset.y, makecol(255, 255, 255));
	for (int i = 0; i <= sizeY; i += BLOCK_SIZE)
		hline(buffer, 0 + (int)offset.x, i + (int)offset.y, sizeX + (int)offset.x, makecol(255, 255, 255));*/
}
