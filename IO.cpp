#include "IO.h"
#include "Revengine.h"
#include "Entity.h"
#include "Player.h"
#include "World.h"

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
	char * c = new char[length + 1];
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