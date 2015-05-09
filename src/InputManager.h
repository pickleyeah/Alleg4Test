#pragma once

#include <allegro5/allegro.h>

class Input
{
public:
	static void Init()
	{
		al_get_keyboard_state(&m_keyState);
		al_get_keyboard_state(&m_keyStateOld);
	}
	static void Update()
	{
		m_keyStateOld = m_keyState;
		al_get_keyboard_state(&m_keyState);
	}

	static bool KeyDown(int k) { return al_key_down(&m_keyState, k); }
	static bool KeyUp(int k) { return !al_key_down(&m_keyState, k); }
	static bool KeyPressed(int k) { return al_key_down(&m_keyState, k) && !al_key_down(&m_keyStateOld, k); }
private:
	static char m_keys[ALLEGRO_KEY_MAX];
	static char m_keysOld[ALLEGRO_KEY_MAX];
	static ALLEGRO_KEYBOARD_STATE m_keyState, m_keyStateOld;
};
