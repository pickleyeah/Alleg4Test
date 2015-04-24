#include "Sprite.h"

std::unordered_map<std::string, std::unique_ptr<Sprite>> Sprite::s_spriteMap;

Sprite* Sprite::GetSprite(const char* filename, int numFrames, int fps)
{
	if (s_spriteMap[std::string(filename)])
		return s_spriteMap[std::string(filename)].get();

	Sprite *result = new Sprite();
	result->m_srcBitmap = load_bitmap(filename, NULL);
	result->m_numFrames = numFrames;
	result->m_fps = fps;
	// Infer frame size based on bitmap width and number of frames
	int width = result->m_srcBitmap->w / numFrames;
	for (int x = 0; x < result->m_srcBitmap->w; x += width)
		result->m_frames.push_back(create_sub_bitmap(result->m_srcBitmap, x, 0, width, result->m_srcBitmap->h));
	s_spriteMap[filename] = std::unique_ptr<Sprite>(result);
	return result;
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