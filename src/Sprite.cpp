#include "Sprite.h"
#include "CSVFile.h"

std::unordered_map<std::string, std::unique_ptr<Sprite>> Sprite::s_spriteMap;

void Sprite::PreloadSpriteList(const char* filename)
{
	CSVFile csvFile(filename);
	while (csvFile.HasRemainingEntries())
	{
		std::vector<std::string> entry = csvFile.GetNextEntry();
		// Load the sprite
		const char *spriteFileName = entry[0].c_str();
		int numFrames = atoi(entry[1].c_str());
		int fps = atoi(entry[2].c_str());
		Sprite::GetSprite(spriteFileName, numFrames, fps);
	}
}

Sprite* Sprite::GetSprite(const char* filename, int numFrames, int fps)
{
	if (s_spriteMap[std::string(filename)])
		return s_spriteMap[std::string(filename)].get();

	Sprite *result = new Sprite();
	result->m_srcBitmap = al_load_bitmap(filename);
	result->m_numFrames = numFrames;
	result->m_fps = fps;
	// Infer frame size based on bitmap width and number of frames

	int frameWidth = al_get_bitmap_width(result->m_srcBitmap) / numFrames;
	for (int x = 0; x < al_get_bitmap_width(result->m_srcBitmap); x += frameWidth)
		result->m_frames.push_back(al_create_sub_bitmap(result->m_srcBitmap, x, 0, frameWidth, al_get_bitmap_height(result->m_srcBitmap)));
	s_spriteMap[filename] = std::unique_ptr<Sprite>(result);
	return result;
}

Sprite::~Sprite()
{
	for (size_t i = 0; i < m_frames.size(); i++)
	{
		al_destroy_bitmap(m_frames[i]);
	}
	al_destroy_bitmap(m_srcBitmap);
}

void Sprite::Render(ALLEGRO_BITMAP *buffer, double time, int x, int y)
{
	int frame = (int)(time * m_fps) % m_numFrames;
	al_set_target_bitmap(buffer);
	al_draw_bitmap(m_frames[frame], x, y, 0);
	al_set_target_backbuffer(al_get_current_display());
	//draw_sprite(buffer, m_frames[frame], x, y);
}