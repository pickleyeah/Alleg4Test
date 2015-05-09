#pragma once

// Static class to provide central point for objects to get elapsed time from
class GameTime
{
public:
	static double FrameTime() { return m_frameTime; }
	static double TotalElapsedTime() { return m_totalElapsedTime; }
	friend class Game;
private:
	GameTime() {};
	static void Update(double dt);
	static double m_frameTime;
	static double m_totalElapsedTime;
};