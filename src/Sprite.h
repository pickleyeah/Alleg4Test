#pragma once

class Sprite
{
public:
	static const int TILESIZE = 64;
	static void PreloadSpriteList(const char* filename);
	static Sprite* GetSprite(const char* filename, int numFrames = 1, int fps = 1);
	static void ReleaseSprite(std::string filename) { s_spriteMap.erase(filename); }
	static void ReleaseAllSprites() { s_spriteMap.clear(); }

	~Sprite();

	void Render(double time, int x, int y);

private:
	static std::unordered_map<std::string, std::unique_ptr<Sprite>> s_spriteMap;
	Sprite() {}

	ALLEGRO_BITMAP* m_srcBitmap;
	int m_fps;
	int m_numFrames;
	std::vector<ALLEGRO_BITMAP*> m_frames;
};