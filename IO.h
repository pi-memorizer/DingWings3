#ifndef IO_H
#define IO_H
#include "Revengine.h"

#include <string>
using namespace std;

void save();
void load();

class File;
File * openFile(string s, bool writing);
void closeFile(File *file);

unsigned char readByte(File*file);
unsigned short readUShort(File * file);
unsigned int readUInt(File * file);
unsigned long long readULong(File * file);
char readChar(File * file);
short readShort(File * file);
int readInt(File * file);
long long readLong(File * file);
string readString(File * file);
void writeByte(File *file, unsigned char v);
void writeChar(File * file, char v);
void writeShort(File * file, short v);
void writeInt(File * file, int v);
void writeLong(File * file, long long v);
void writeString(File * file, string v);

#endif