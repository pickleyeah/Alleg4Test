#include "RenderComponents.h"
#include "InputComponents.h"

#include "Vec2.h"
#include "Entity.h"
#include "Sprite.h"


TestRender::TestRender(TestInput* input)
{
	m_input = input;

	m_idleSprites.push_back(Sprite::GetSprite("Data/Sprites/Player_Idle_N.png", 1, 1));
	m_idleSprites.push_back(Sprite::GetSprite("Data/Sprites/Player_Idle_E.png", 1, 1));
	m_idleSprites.push_back(Sprite::GetSprite("Data/Sprites/Player_Idle_S.png", 1, 1));
	m_idleSprites.push_back(Sprite::GetSprite("Data/Sprites/Player_Idle_W.png", 1, 1));

	m_walkSprites.push_back(Sprite::GetSprite("Data/Sprites/Player_Walk_N.png", 4, WALK_FRAMES_PER_SEC));
	m_walkSprites.push_back(Sprite::GetSprite("Data/Sprites/Player_Walk_E.png", 4, WALK_FRAMES_PER_SEC));
	m_walkSprites.push_back(Sprite::GetSprite("Data/Sprites/Player_Walk_S.png", 4, WALK_FRAMES_PER_SEC));
	m_walkSprites.push_back(Sprite::GetSprite("Data/Sprites/Player_Walk_W.png", 4, WALK_FRAMES_PER_SEC));
}

TestRender::~TestRender(void)
{
}

void TestRender::Render(Entity *entity, Vec2 offset)
{
	Vec2 Pos = entity->Pos;
	Vec2 Size = entity->Size;

	int x = offset.x + Pos.x;
	int y = offset.y + Pos.y;

	Sprite *sprite = nullptr;
	switch (m_input->GetState())
	{
	case TE_IDLE:
		sprite = m_idleSprites[entity->Dir];
		break;
	case TE_MOVING:
		int frame = ((int)(m_input->TimeSinceStateChange() * WALK_FRAMES_PER_SEC)) % 4;
		sprite = m_walkSprites[entity->Dir];
		break;
	}
	sprite->Render(m_input->TimeSinceStateChange(), x, y);
}
