#include <jni.h>
#include <string>
#include "Graphics.h"
#include "Event.h"
#include "Player.h"
using namespace std;

extern "C"
JNIEXPORT jstring JNICALL
Java_studiodingwing_revengine_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

string to_string(int value)
{
    return "";
}

class File
{
public:
};

class Texture
{
public:
};

void setDrawColor(int r, int g, int b, int a)
{
}

void clearScreen()
{
}

void getWindowSize(int *w, int *h)
{
}

void display()
{
}

void startDrawingPlayer(int index)
{
}

int getScreenWidth()
{
    return 0;
}

int getScreenHeight()
{
    return 0;
}

void drawTexture(Texture *texture, Rect *rect)
{
}

void drawTexture(Texture *texture, Rect *source, Rect *dest)
{
}

Texture *createTexture(int width, int height)
{
    Texture * t = new Texture();
    return t;
}

Texture *loadTexture(string filename)
{
    Texture * t = new Texture();
    return t;
}

void getTextureSize(Texture *t, int *w, int *h)
{
}

void destroyTexture(Texture *texture)
{
    delete texture;
}
void fillRect(Rect *rect)
{
}

void delay(int millis)
{
}

int startGame()
{
    return 0;
}

void exitGame()
{
}

File * openFile(string s, bool writing)
{
    return nullptr;
}

void closeFile(File *file)
{
    delete file;
}

unsigned char readByte(File * file)
{
    return 0;
}
char readChar(File * file)
{
    return 0;
}
short readShort(File * file)
{
    return 0;
}
int readInt(File * file)
{
    return 0;
}
long long readLong(File * file)
{
    return 0;
}
unsigned short readUShort(File * file)
{
    return 0;
}
unsigned int readUInt(File * file)
{
    return 0;
}
unsigned long long readULong(File * file)
{
    return 0;
}
string readString(File * file)
{
    return "";
}
void writeByte(File * file, unsigned char v)
{
}
void writeChar(File * file, char v)
{
}
void writeShort(File * file, short v)
{
}
void writeInt(File * file, int v)
{
}
void writeLong(File * file, long long v)
{
}
void writeString(File * file, string v)
{
}

bool getEvent(Event *e)
{
    e->type = EVENT_UNKNOWN;
    return false;
}

bool getKey(Player *p, EventKey key)
{
    return false;
}

void createSound(string name)
{
}

void setBackgroundMusic(string name)
{
}

void playSound(string name)
{
}