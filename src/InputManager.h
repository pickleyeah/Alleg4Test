#pragma once

class Input
{
public:
	static void Init()
	{
		memcpy(m_keys, (void*)key, sizeof(char)*KEY_MAX);
		memcpy(m_keysOld, (void*)m_keys, sizeof(char)*KEY_MAX);
	}
	static void Update()
	{
		memcpy(m_keysOld, (void*)m_keys, sizeof(char)*KEY_MAX);
		memcpy(m_keys, (void*)key, sizeof(char)*KEY_MAX);
	}

	static bool KeyDown(char k) { return m_keys[k]; }
	static bool KeyUp(char k) { return !m_keys[k]; }
	static bool KeyPressed(char k) { return m_keys[k] && !m_keysOld[k]; }
private:
	static char m_keys[KEY_MAX];
	static char m_keysOld[KEY_MAX];
};
