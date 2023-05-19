#include "entities.c"

#ifndef GAME_ENTITIES
#define GAME_ENTITIES

void createEnemies(struct levels *lvl ,struct entity *e);
void createEntity(struct entity *e, point position, point direction, int speed, char icon);
bool worldCollisions(struct canvas *canva, struct entity *e, char wall, point *destroy, int *ldt);
bool isOnCanva(struct canvas *canva, struct entity *e);
void updateEntityMovement(struct entity *e);

#endif
