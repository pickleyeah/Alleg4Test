#include "Camera.h"
#include "Game.h"

Camera::Camera(Entity *entity)
{
	m_entity = entity;
}

void Camera::ProcessInput()
{

}

void Camera::Update(double dt)
{
	// Put center of entity at center of screen
	Vec2 centre = Vec2::Add(m_entity->Pos, Vec2::Mul(m_entity->Size, 0.5f));
	m_pos = Vec2(centre.x - Game::SCREEN_X / 2, centre.y - Game::SCREEN_Y / 2);
}