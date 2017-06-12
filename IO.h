#ifndef IO_H
#define IO_H
#include "Revengine.h"

#include <string>;
using namespace std;

void save();
void load();

unsigned char readByte(SDL_RWops*file);
unsigned short readUShort(SDL_RWops * file);
unsigned int readUInt(SDL_RWops * file);
unsigned long long readULong(SDL_RWops * file);
char readChar(SDL_RWops * file);
short readShort(SDL_RWops * file);
int readInt(SDL_RWops * file);
long long readLong(SDL_RWops * file);
string readString(SDL_RWops * file);
void writeByte(SDL_RWops *file, unsigned char v);
void writeChar(SDL_RWops * file, char v);
void writeShort(SDL_RWops * file, short v);
void writeInt(SDL_RWops * file, int v);
void writeLong(SDL_RWops * file, long long v);
void writeString(SDL_RWops * file, string v);

#endif