#pragma once

#include <vector>
#include <allegro.h>

class Sprite
{
public:
	Sprite(const char* filename, int numFrames, int fps);
	~Sprite();

	void Render(BITMAP *buffer, double time, int x, int y);

private:
	BITMAP* m_srcBitmap;
	int m_fps;
	int m_numFrames;
	std::vector<BITMAP*> m_frames;
};