#include "Area.h"
#include "Game.h"
#include <allegro.h>
#include <string.h>


Area::Area(Vec2 size) :
	m_size(size),
	m_showGrid(false)
{
	m_blocks = new BLOCK_T[(int)(size.x * size.y)];
	m_sprites.resize(COL_ALL+1, nullptr);

	m_sprites[0] = new Sprite("Data/Tiles/Grass.bmp", 1, 1);
	m_sprites[COL_NORTH] = new Sprite("Data/Tiles/Grass_BlockN.bmp", 1, 1);
	m_sprites[COL_SOUTH] = new Sprite("Data/Tiles/Grass_BlockS.bmp", 1, 1);
	m_sprites[COL_WEST] = new Sprite("Data/Tiles/Grass_BlockW.bmp", 1, 1);
	m_sprites[COL_EAST] = new Sprite("Data/Tiles/Grass_BlockE.bmp", 1, 1);

	m_sprites[COL_NORTH | COL_WEST] = new Sprite("Data/Tiles/Grass_BlockNW.bmp", 1, 1);
	m_sprites[COL_NORTH | COL_EAST] = new Sprite("Data/Tiles/Grass_BlockNE.bmp", 1, 1);
	m_sprites[COL_SOUTH | COL_WEST] = new Sprite("Data/Tiles/Grass_BlockSW.bmp", 1, 1);
	m_sprites[COL_SOUTH | COL_EAST] = new Sprite("Data/Tiles/Grass_BlockSE.bmp", 1, 1);

	/*m_sprites[COL_ALL] = Sprite("Data/Tiles/Boulder.bmp", 1, 1);*/
	m_sprites[COL_ALL] = new Sprite("Data/Tiles/Water.bmp", 4, 4);
}


Area::~Area(void)
{
	delete m_camera;
	delete[] m_blocks;
	for (size_t i = 0; i < m_sprites.size(); i++)
	{
		if (m_sprites[i]) // TODO: clean this mess up. having to step over null pointers isn't real nice
			delete m_sprites[i];
	}
	for (size_t i = 0; i < m_entities.size(); i++)
		delete m_entities[i];
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

Area *Area::CreateTestArea(Entity *player)
{
	int m = 16, n = 16;
	Area *result = new Area(Vec2(m, n));
	memset(result->m_blocks, 0, sizeof(BLOCK_T)*m*n);

	// Water block
	for (int x = 4; x < 8; x++)
	{
		for (int y = 4; y < 8; y++)
		{
			BLOCK_T *block = result->GetBlock(x, y);
			block->colMask |= COL_ALL;
		}
	}

	// Player
	if (player == nullptr)
		player = Entity::MakeTestEntity(result);
	//result->m_entities.push_back(player);
	result->SetPlayer(player);
	player->Dir = DIR_SOUTH;
	player->SetGridXY(1, 1);

	result->Init();
	return result;
}

Area *Area::CreateTestArea2(Entity *player)
{
	int m = 4, n = 4; // small-ass area
	Area *result = new Area(Vec2(m, n));
	memset(result->m_blocks, 0, sizeof(BLOCK_T)*m*n);

	//result->m_entities.push_back(player);
	result->SetPlayer(player);
	player->SetGridXY(3, 1);
	player->Dir = DIR_EAST;

	result->Init();
	return result;
}

void Area::Init()
{
	m_elapsedTime = 0;
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
	m_elapsedTime += dt;
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
			int spriteIndex = GetBlock(i, j)->colMask;
			m_sprites[spriteIndex]->Render(buffer, m_elapsedTime, x, y);
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
