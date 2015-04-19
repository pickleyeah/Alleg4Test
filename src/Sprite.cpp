#include "Sprite.h"

Sprite::Sprite(const char* filename, int numFrames, int fps)
{
	m_fps = fps;
	m_numFrames = numFrames;
	m_srcBitmap = load_bitmap(filename, nullptr);
	// Infer frame size based on bitmap width and number of frames
	int width = m_srcBitmap->w / numFrames;
	for (int x = 0; x < m_srcBitmap->w; x += width)
		m_frames.push_back(create_sub_bitmap(m_srcBitmap, x, 0, width, m_srcBitmap->h));
}

Sprite::~Sprite()
{
	for (size_t i = 0; i < m_frames.size(); i++)
	{
		destroy_bitmap(m_frames[i]);
	}
	destroy_bitmap(m_srcBitmap);
}

void Sprite::Render(BITMAP *buffer, double time, int x, int y)
{
	int frame = (int)(time * m_fps) % m_numFrames;
	draw_sprite(buffer, m_frames[frame], x, y);
}