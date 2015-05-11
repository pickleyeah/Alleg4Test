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
	Vec2 centre = m_entity->Pos + m_entity->Size * 0.5f;
	m_pos = centre - Vec2(Game::SCREEN_X * 0.5f, Game::SCREEN_Y * 0.5f);
}