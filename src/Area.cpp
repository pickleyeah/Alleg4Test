#include "Area.h"
#include "Game.h"
#include <allegro.h>
#include <string.h>


Area::Area(Vec2 size) :
	m_size(size)
{
	m_blocks = new BLOCK_T[(size.x * size.y)];
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
	// Top row - red
	for (int i = 0; i < m; i++)
	{
		BLOCK_T *block = result->GetBlock(i, 0);
		block->color = makecol(255,0,0);
		block->colMask |= COL_SOUTH;
	}
	// Left column - blue
	for (int i = 0; i < n; i++)
	{
		BLOCK_T *block = result->GetBlock(0, i);
		block->color = makecol(0,255,0);
		block->colMask |= COL_EAST;
	}
	// Bottom row - green
	for (int i = 0; i < m; i++)
	{
		BLOCK_T *block = result->GetBlock(i, n-1);
		block->color = makecol(0,0,255);
		block->colMask |= COL_NORTH;
	}
	// Right column - yellow
	for (int i = 0; i < n; i++)
	{
		BLOCK_T *block = result->GetBlock(m-1, i);
		block->color = makecol(255,255,0);
		block->colMask |= COL_WEST;
	}
	// Two small columns - white
	for (int i = 3; i < 6; i+=2)
	{
		for (int j = 1; j < 7; j++)
		{
			BLOCK_T *block = result->GetBlock(i, j);
			block->color = makecol(255,255,255);
			block->colMask |= COL_EAST;
		}
	}
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
			rectfill(buffer, x,y, x+64, y+64, color);
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
