#include "Area.h"
#include "Game.h"
#include <allegro.h>
#include <string.h>


Area::Area(Vec2 size) :
	m_size(size)
{
	m_blocks = new BLOCK_T[(int)(size.x * size.y)];
	// 0
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
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
}

Area *Area::CreateTestArea()
{
	int m = 8, n = 8;
	Area *result = new Area(Vec2(m, n));
	memset(result->m_blocks, 0, sizeof(BLOCK_T)*m*n);

	BLOCK_T *block = result->GetBlock(5, 5);
	block->color = makecol(255, 255, 0);
	block->colMask |= COL_ALL;

	// Player
	Entity* player = Entity::MakeTestEntity(result);
	player->SetGridXY(1,1);
	result->m_entities.push_back(player);

	result->Init();
	return result;
}

void Area::Init()
{
	for (int i = 0; i < m_entities.size(); i++)
		m_entities[i]->Init(this);
}

void Area::ProcessInput()
{
	for (int i = 0; i < m_entities.size(); i++)
		m_entities[i]->ProcessInput();
}

void Area::Update(double dt)
{
	for (int i = 0; i < m_entities.size(); i++)
		m_entities[i]->Update(dt);
}

void Area::Render(BITMAP *buffer, Vec2 offset)
{
	// Render blocks
	for (int i = 0; i < (int)m_size.x; i++)
	{
		for (int j = 0; j < (int)m_size.y; j++)
		{
			int color = GetBlock(i,j)->color;
			int x = offset.x + i * 64;
			int y = offset.y + j * 64;
			blit(m_bitmaps[GetBlock(i, j)->colMask], buffer, 0, 0, x, y, x + 64, y + 64);
			//rectfill(buffer, x,y, x+64, y+64, color);
		}
	}
	DrawGrid(buffer, offset);

	for (int i = 0; i < m_entities.size(); i++)
		m_entities[i]->Render(buffer, offset);
}

static const int BLOCK_SIZE = 64;
void Area::DrawGrid(BITMAP *buffer, Vec2 offset)
{
	for (int i = 0; i < Game::SCREEN_X; i += BLOCK_SIZE)
		vline(buffer, i, offset.y, Game::SCREEN_Y, makecol(255,255,255));
	for (int i = 0; i < Game::SCREEN_Y; i += BLOCK_SIZE)
		hline(buffer, offset.x, i, Game::SCREEN_X, makecol(255,255,255));;
}
