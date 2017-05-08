#include "Revengine.h"

List<Sprite*> sprites;
SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
Hashmap<int, bool> keys;
Player **players = new Player*[0];
int numPlayers = 0;
Hashmap<string, int> flags;

int getOnscreenX(Player *p, int x, int xOffset)
{
	return WIDTH / 2 - 16 - p->xOffset + xOffset + 32 * (x - p->x);
}

int getOnscreenY(Player *p, int y, int yOffset)
{
	return HEIGHT / 2 - 16 - p->yOffset + yOffset + 32 * (y - p->y);
}

int getFlag(string s)
{
	if (flags.contains(s))
	{
		return flags[s];
	}
	else return 0;
}

void setFlag(string s, int v)
{
	if (flags.contains(s))
	{
		flags[s] = v;
	}
	else {
		flags.add(s, v);
	}
}

void quitSDL()
{
	if (window != nullptr) SDL_DestroyWindow(window);
	if (renderer != nullptr) SDL_DestroyRenderer(renderer);
	for (int i = 0; i < sprites.length(); i++)
	{
		delete sprites[i];
	}
	for (int i = 0; i < numPlayers; i++)
	{
		delete players[i];
	}
	IMG_Quit();
	Mix_Quit();
	SDL_Quit();
}

Player *getPlayer(int id)
{
	int index = -1;
	for (int i = 0; i < numPlayers; i++)
	{
		if (players[i]->id == id)
		{
			index = i;
			break;
		}
	}
	if (index == -1)
		return nullptr;
	else
		return players[index];
}

void addPlayer(int id, int keyRight, int keyUp, int keyLeft, int keyDown, int keyA, int keyB)
{
	int index = -1;
	for (int i = 0; i < numPlayers; i++)
	{
		if (players[i]->id == id)
		{
			index = i;
			break;
		}
	}
	if (index == -1)
	{
		Player **buffer = new Player*[numPlayers + 1];
		for (int i = 0; i < numPlayers; i++)
		{
			buffer[i] = players[i];
		}
		buffer[numPlayers] = new Player(id,keyRight,keyUp,keyLeft,keyDown,keyA,keyB);
		delete[] players;
		players = buffer;
		numPlayers++;
	}
}

void removePlayer(int id)
{
	int index = -1;
	for (int i = 0; i < numPlayers; i++)
	{
		if (players[i]->id == id)
		{
			index = i;
			break;
		}
	}
	if (index != -1)
	{
		delete players[index];
		Player **buffer = new Player*[numPlayers - 1];
		for (int i = 0, j = 0; i < numPlayers; i++)
		{
			if (i != index)
			{
				buffer[j] = players[i];
				j++;
			}
		}
		delete[] players;
		players = buffer;
		numPlayers--;
	}
}

void savePlayer(SDL_RWops * file, Player *p)
{
	p->dir = readByte(file);
	p->worldID = readShort(file);
	p->x = readInt(file);
	p->y = readInt(file);
	p->xOffset = readChar(file);
	p->yOffset = readChar(file);
}

void loadPlayer(SDL_RWops *file, Player *p)
{
	writeByte(file, (char)p->dir);
	writeShort(file, (short)p->worldID);
	writeInt(file, p->x);
	writeInt(file, p->y);
	writeChar(file, (char)p->xOffset);
	writeChar(file, (char)p->yOffset);
}

void save()
{
	SDL_RWops * file = SDL_RWFromFile("save.bin", "w+b");
	if (file == nullptr)
	{
		debug("Unable to write file");
		return;
	}
	for (int i = 0; i < worlds.length(); i++)
	{
		List<Entity*> &entities = worlds[i]->entities;
		for (int j = 0; j < entities.length(); j++)
		{
			entities[j]->save();
		}
	}
	List<string> flagKeys = flags.keys();
	writeInt(file, flagKeys.length());
	for (int i = 0; i < flagKeys.length(); i++)
	{
		writeString(file, flagKeys[i]);
		writeInt(file, flags[flagKeys[i]]);
	}
	Player *p = getPlayer(0);
	assert(p != nullptr);
	savePlayer(file, p);
	SDL_RWclose(file);
}

void load()
{
	SDL_RWops * file = SDL_RWFromFile("save.bin", "r+b");
	if (file == nullptr)
	{
		debug("Unable to read file");
		return;
	}
	for (int i = 0; i < worlds.length(); i++)
	{
		List<Entity*> &entities = worlds[i]->entities;
		for (int j = 0; j < entities.length(); j++)
		{
			entities[j]->load();
		}
	}
	int numFlags = readInt(file);
	for (int i = 0; i < numFlags; i++)
	{
		flags.add(readString(file), readInt(file));
	}
	Player *p = getPlayer(0);
	assert(p != nullptr);
	loadPlayer(file, p);
	SDL_RWclose(file);
}

unsigned char readByte(SDL_RWops * file)
{
	unsigned char c;
	SDL_RWread(file, &c, sizeof(unsigned char), 1);
	return c;
}
char readChar(SDL_RWops * file)
{
	char c;
	SDL_RWread(file, &c, sizeof(char), 1);
	return c;
}
short readShort(SDL_RWops * file)
{
	return SDL_ReadBE16(file);
}
int readInt(SDL_RWops * file)
{
	return SDL_ReadBE32(file);
}
long long readLong(SDL_RWops * file)
{
	return SDL_ReadBE64(file);
}
unsigned short readUShort(SDL_RWops * file)
{
	return SDL_ReadBE16(file);
}
unsigned int readUInt(SDL_RWops * file)
{
	return SDL_ReadBE32(file);
}
unsigned long long readULong(SDL_RWops * file)
{
	return SDL_ReadBE64(file);
}
string readString(SDL_RWops * file)
{
	int length = readShort(file);
	char * c = new char[length+1];
	c[length] = 0;
	SDL_RWread(file, c, length, 1);
	string s(c);
	delete[] c;
	return s;
}
void writeByte(SDL_RWops * file, unsigned char v)
{
	SDL_RWwrite(file, &v, sizeof(unsigned char), 1);
}
void writeChar(SDL_RWops * file, char v)
{
	SDL_RWwrite(file, &v, sizeof(char), 1);
}
void writeShort(SDL_RWops * file, short v)
{
	SDL_WriteBE16(file, v);
}
void writeInt(SDL_RWops * file, int v)
{
	SDL_WriteBE32(file, v);
}
void writeLong(SDL_RWops * file, long long v)
{
	SDL_WriteBE64(file, v);
}
void writeString(SDL_RWops * file, string v)
{
	writeShort(file, (short)v.length());
	SDL_RWwrite(file, v.c_str(), v.length(), 1);
}