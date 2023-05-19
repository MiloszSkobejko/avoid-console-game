#include "consolegfx.c"

#ifndef CONSOLEGRAPHICS
#define CONSOLEGRAPHICS

//Function declarations

void wait (float seconds);
void ClearConsoleScreen(int CursorPosX, int CursorPosY);
void FillEmptyCanva(struct canvas *c, char background, char border);
bool createCanva(struct canvas *c, int sizeX, int sizeY, char bg, char outline);
void printCanva(struct canvas *c);
void Text(struct canvas *c, int start_x, int start_y, char text[]);

#endif
