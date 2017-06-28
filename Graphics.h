#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <string>
using namespace std;

struct Rect
{
	int w, h, x, y;
};
class Texture;

void setDrawColor(int r, int g, int b, int a);
void clearScreen();
void getWindowSize(int *w, int *h);
void display();
void startDrawingPlayer(int index);
int getScreenWidth();
int getScreenHeight();
void drawTexture(Texture *texture, Rect *rect);
void drawTexture(Texture *texture, Rect *source, Rect *dest);
void drawTexture(Texture *texture, Rect *source, Rect *dest, int alpha);
Texture *createTexture(int width, int height);
void fillRect(Rect *r);
Texture *loadTexture(string filename);
void getTextureSize(Texture *texture, int *w, int *h);
void destroyTexture(Texture *texture);
void drawCharacter(char c, int x, int y, int r, int g, int b);
#endif