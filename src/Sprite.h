#pragma once

class Sprite
{
public:
	static const int TILESIZE = 64;
	static Sprite* GetSprite(const char* filename, int numFrames = 1, int fps = 1);
	static void ReleaseSprite(std::string filename) { s_spriteMap.erase(filename); }
	static void ReleaseAllSprites() { s_spriteMap.clear(); }

	~Sprite();

	void Render(BITMAP *buffer, double time, int x, int y);

private:
	static std::unordered_map<std::string, std::unique_ptr<Sprite>> s_spriteMap;
	Sprite() {}

	BITMAP* m_srcBitmap;
	int m_fps;
	int m_numFrames;
	std::vector<BITMAP*> m_frames;
};