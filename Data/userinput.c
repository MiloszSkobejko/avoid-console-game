#include <conio.h>

#define ARROW_UP 72
#define ARROW_DOWN 80
#define ARROW_LEFT 75
#define ARROW_RIGHT 77

int restart_cooldown;

// Nazwanie eventów
enum events {restart = 1, close = 2, play = 0};


/* ---------- Wejście z klawiatury ------------ */
int input(struct player_t *player)
{

  if (kbhit())
  {
      switch (_getch())
      {
          case 'w':
            if (player -> is_moving == false)
            {
              player -> ent.dir.x = 0;
              player -> ent.dir.y = -1;
              player -> is_moving = true;
            }
            break;
          case 's':
            if (player -> is_moving == false)
            {
              player -> ent.dir.x = 0;
              player -> ent.dir.y = 1;
              player -> is_moving = true;
            }
            break;
          case 'a':
            if (player -> is_moving == false)
            {
              player -> ent.dir.x = -1;
              player -> ent.dir.y = 0;
              player -> is_moving = true;
            }
            break;
          case 'd':
            if (player -> is_moving == false)
            {
              player -> ent.dir.x = 1;
              player -> ent.dir.y = 0;
              player -> is_moving = true;
            }
            break;
          case ARROW_UP:
            if (player -> is_moving == false)
            {
              player -> ent.dir.x = 0;
              player -> ent.dir.y = -1;
              player -> is_moving = true;
            }
            break;
          case ARROW_DOWN:
            if (player -> is_moving == false)
            {
              player -> ent.dir.x = 0;
              player -> ent.dir.y = 1;
              player -> is_moving = true;
            }
            break;
          case ARROW_LEFT:
            if (player -> is_moving == false)
            {
              player -> ent.dir.x = -1;
              player -> ent.dir.y = 0;
              player -> is_moving = true;
            }
            break;
          case ARROW_RIGHT:
            if (player -> is_moving == false)
            {
              player -> ent.dir.x = 1;
              player -> ent.dir.y = 0;
              player -> is_moving = true;
            }
            break;

          case 'x':
            return close;
            break;

          case'r':
            if (restart_cooldown == 0)
              return restart;
            break;
        }
    }

  return play;
}
