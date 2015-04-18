#include "RenderComponents.h"
#include "InputComponents.h"

#include "Vec2.h"
#include "Entity.h"


TestRender::TestRender(TestInput* input)
{
	m_input = input;
	m_idleSprites.push_back(load_bitmap("Data/Sprites/Player_Idle_N.bmp", nullptr));
	m_idleSprites.push_back(load_bitmap("Data/Sprites/Player_Idle_E.bmp", nullptr));
	m_idleSprites.push_back(load_bitmap("Data/Sprites/Player_Idle_S.bmp", nullptr));
	m_idleSprites.push_back(load_bitmap("Data/Sprites/Player_Idle_W.bmp", nullptr));

	BITMAP *walkN = load_bitmap("Data/Sprites/Player_Walk_N.bmp", nullptr);
	BITMAP *walkE = load_bitmap("Data/Sprites/Player_Walk_E.bmp", nullptr);
	BITMAP *walkS = load_bitmap("Data/Sprites/Player_Walk_S.bmp", nullptr);
	BITMAP *walkW = load_bitmap("Data/Sprites/Player_Walk_W.bmp", nullptr);
	m_spritesToDelete.push_back(walkN);
	m_spritesToDelete.push_back(walkE);
	m_spritesToDelete.push_back(walkS);
	m_spritesToDelete.push_back(walkW);

	int size = 64;
	for (int i = 0; i < 4; i++)
	{
		m_walkSprites.push_back(create_sub_bitmap(walkN, size*i, 0, size, size));
		m_walkSprites.push_back(create_sub_bitmap(walkE, size*i, 0, size, size));
		m_walkSprites.push_back(create_sub_bitmap(walkS, size*i, 0, size, size));
		m_walkSprites.push_back(create_sub_bitmap(walkW, size*i, 0, size, size));
	}
}

TestRender::~TestRender(void)
{
	for (int i = 0; i < m_idleSprites.size(); i++)
		release_bitmap(m_idleSprites[i]);
	for (int i = 0; i < m_walkSprites.size(); i++)
		release_bitmap(m_walkSprites[i]);
	for (int i = 0; i < m_spritesToDelete.size(); i++)
		release_bitmap(m_spritesToDelete[i]);
}

void TestRender::Render(Entity *entity, BITMAP *buffer, Vec2 offset)
{
	Vec2 Pos = entity->Pos;
	Vec2 Size = entity->Size;

	int x1 = offset.x + Pos.x;
	int y1 = offset.y + Pos.y;
	int x2 = offset.x + Pos.x + Size.x;
	int y2 = offset.y + Pos.y + Size.y;
	int cx = (x1 + x2) / 2;
	int cy = (y1 + y2) / 2;

	BITMAP *sprite = nullptr;
	switch (m_input->GetState())
	{
	case TE_IDLE:
		sprite = m_idleSprites[entity->Dir];
		break;
	case TE_MOVING:
		int frame = ((int)(m_input->TimeSinceStateChange() * WALK_FRAMES_PER_SEC)) % 4;
		sprite = m_walkSprites[entity->Dir + frame*4];
		break;
	}
	draw_sprite(buffer, sprite, x1, y1);
}
