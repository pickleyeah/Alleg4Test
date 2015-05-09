#include "InputManager.h"

char Input::m_keys[ALLEGRO_KEY_MAX];
char Input::m_keysOld[ALLEGRO_KEY_MAX];

ALLEGRO_KEYBOARD_STATE Input::m_keyState;
ALLEGRO_KEYBOARD_STATE Input::m_keyStateOld;