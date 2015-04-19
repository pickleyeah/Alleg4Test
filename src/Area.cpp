#include "Area.h"
#include "Game.h"
#include <allegro.h>
#include <string.h>


Area::Area(Vec2 size) :
	m_size(size),
	m_showGrid(false)
{
	m_blocks = new BLOCK_T[(int)(size.x * size.y)];
	m_bitmaps.resize(COL_ALL+1, load_bitmap("Data/Tiles/Grass.bmp", NULL));

	m_bitmaps[COL_NORTH] = load_bitmap("Data/Tiles/Grass_BlockN.bmp", NULL);
	m_bitmaps[COL_SOUTH] = load_bitmap("Data/Tiles/Grass_BlockS.bmp", NULL);
	m_bitmaps[COL_WEST] = load_bitmap("Data/Tiles/Grass_BlockW.bmp", NULL);
	m_bitmaps[COL_EAST] = load_bitmap("Data/Tiles/Grass_BlockE.bmp", NULL);

	m_bitmaps[COL_NORTH | COL_WEST] = load_bitmap("Data/Tiles/Grass_BlockNW.bmp", NULL);
	m_bitmaps[COL_NORTH | COL_EAST] = load_bitmap("Data/Tiles/Grass_BlockNE.bmp", NULL);
	m_bitmaps[COL_SOUTH | COL_WEST] = load_bitmap("Data/Tiles/Grass_BlockSW.bmp", NULL);
	m_bitmaps[COL_SOUTH | COL_EAST] = load_bitmap("Data/Tiles/Grass_BlockSE.bmp", NULL);

	m_bitmaps[COL_ALL] = load_bitmap("Data/Tiles/Boulder.bmp", NULL);
}


Area::~Area(void)
{
	delete[] m_blocks;
	for (size_t i = 0; i < m_bitmaps.size(); i++)
	{
		if (m_bitmaps[i] != nullptr)
			release_bitmap(m_bitmaps[i]);
	}
}

Area *Area::CreateTestArea()
{
	int m = 16, n = 16;
	Area *result = new Area(Vec2(m, n));
	memset(result->m_blocks, 0, sizeof(BLOCK_T)*m*n);

	// 10 random rocks
	srand(time(NULL));
	for (int i = 0; i < 10; i++)
	{
		int x = rand() % 8 + 2, y = rand() % 8 + 2;
		BLOCK_T *block = result->GetBlock(x, y);
		block->colMask |= COL_ALL;
	}

	// Player
	Entity* player = Entity::MakeTestEntity(result);
	player->SetGridXY(1,1);
	result->m_entities.push_back(player);
	result->m_player = player;

	result->Init();
	return result;
}

void Area::Init()
{
	m_camera = new Camera(m_player);
	for (size_t i = 0; i < m_entities.size(); i++)
		m_entities[i]->Init(this);
}

void Area::ProcessInput(double dt)
{
	if (Input::KeyPressed(KEY_G))
		m_showGrid = !m_showGrid;
	for (size_t i = 0; i < m_entities.size(); i++)
		m_entities[i]->ProcessInput(dt);
	m_camera->ProcessInput();
}

void Area::Update(double dt)
{
	for (size_t i = 0; i < m_entities.size(); i++)
		m_entities[i]->Update(dt);
	m_camera->Update(dt);
}

void Area::Render(BITMAP *buffer, Vec2 offset)
{
	offset = m_camera->GetOffset();
	// Render blocks
	for (int i = 0; i < (int)m_size.x; i++)
	{
		for (int j = 0; j < (int)m_size.y; j++)
		{
			int x = (int)offset.x + i * 64;
			int y = (int)offset.y + j * 64;
			draw_sprite(buffer, m_bitmaps[GetBlock(i, j)->colMask], x, y);
			blit(m_bitmaps[GetBlock(i, j)->colMask], buffer, 0, 0, x, y, x + 64, y + 64);
		}
	}

	if (m_showGrid)
		DrawGrid(buffer, offset);

	for (size_t i = 0; i < m_entities.size(); i++)
		m_entities[i]->Render(buffer, offset);

	// Borders
	rectfill(buffer, 0, 0, 32, Game::SCREEN_Y, makecol(32, 32, 32));
	rectfill(buffer, Game::SCREEN_X - 32, 0, Game::SCREEN_X, Game::SCREEN_Y, makecol(32, 32, 32));

	rectfill(buffer, 0, 0, Game::SCREEN_X, 16, makecol(32, 32, 32));
	rectfill(buffer, 0, Game::SCREEN_Y - 16, Game::SCREEN_X, Game::SCREEN_Y, makecol(32, 32, 32));
}

static const int BLOCK_SIZE = 64;
void Area::DrawGrid(BITMAP *buffer, Vec2 offset)
{
	int sizeX = m_size.x * BLOCK_SIZE;
	int sizeY = m_size.y * BLOCK_SIZE;
	for (int i = 0; i <= sizeX; i += BLOCK_SIZE)
		vline(buffer, i + (int)offset.x, 0 + (int)offset.y, sizeY + (int)offset.y, makecol(255, 255, 255));
	for (int i = 0; i <= sizeY; i += BLOCK_SIZE)
		hline(buffer, 0 + (int)offset.x, i + (int)offset.y, sizeX + (int)offset.x, makecol(255, 255, 255));;
}
