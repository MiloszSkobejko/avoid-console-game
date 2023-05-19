#include "levels.c"

#ifndef GAMELEVELS
#define GAMELEVELS

bool pointcmp(point *point1, point *point2);
void randompos(struct levels *lvl, point *pos, char wall);
void filltile(tile *t, char fill);
bool cherrypick(tile *t, int times, char bg, char border);
bool generatetile(tile *t, char bg, char wall);
bool generatelevel(struct levels *level, int tile_size, char bg, char wall);

#endif
