#ifndef EVENT_H
#define EVENT_H

#include "Player.h"

enum EventType { EVENT_QUIT, EVENT_KEY_DOWN, EVENT_KEY_UP, EVENT_UNKNOWN };
enum EventKey {
	KEY_FULLSCREEN, KEY_ESCAPE, KEY_A, KEY_B, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_UNKNOWN
};

struct Event
{
	EventType type;
	union {
		struct { EventKey key; bool prevState; } keys;
	};
};

bool getKey(Player *p, EventKey key);
bool getEvent(Event *e);

#endif