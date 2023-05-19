// +------------------------------  Biblioteki i moduły --------------------------------+
#include "consolegfx.h"
#include "visuals.h"
#include "levels.h"
#include "entities.h"
#include "userinput.h"



// +---------------------------------   Deklaracje   -----------------------------------+
#define HEALTH 5
#define DAMAGE_COOLDOWN 3
#define RESTART_COOLDOWN 30
#define LVLRESTARTTIME 2.0


// deklaracje struktur
struct canvas canva;
struct levels level;
struct levels level_copy;
struct player_t player;

struct canvas startgamescreen;  /* Ekran startowego menu  */
struct canvas restartscreen;    /* Ekran restartu poziomu */
struct canvas endgamescreen;    /* Ekran restartu gry     */
// Zarzadza grą
bool gameover = false;


// Punkt o współrzędnych 0, 0
point nullpoint;

// Wynik
int score;int MaxScore = 0;
int damage_cooldown; // <- Ticki

// Przeciwnicy
struct entity enemies[MAX_ENEMY];



// Deklaracje funkcji
int Initialize();
void Refresh();
void nextlevel(struct player_t *p, struct entity *e, struct levels *lvl);
void showgameui(struct player_t *p);
void showgamemenu();
void restartlevel(struct player_t *p, point *startpos);
void cooldown(int * c);
bool gamerestart();


// +-------------------------   Rozpoczynanie gry od nowa   --------------------------------+
bool gamerestart()
{
    ClearConsoleScreen(CURSOR_POSITION_X, CURSOR_POSITION_Y);

    Text(&endgamescreen, 18, 10, "KONIEC GRY!");
    Text(&endgamescreen, 12, 12, "ROZPOCZYNANIE NOWEJ GRY ZA:");

    for (int timer = 4; timer > 0; timer--)
    {
      showgameui(&player);
      endgamescreen.fill[42][12] = timer +'0';

      printCanva(&endgamescreen);

      /* Sprawdzenie, czy gracz nie chce zakończyć */
      if (input(&player) == close)
        return false;

      wait(1.0);
      ClearConsoleScreen(CURSOR_POSITION_X, CURSOR_POSITION_Y);
    }
    return true;
}



// +----------------------   Wczytywanie poziomu od nowa   --------------------------------+
void restartlevel(struct player_t *p, point *startpos)
{
  showgameui(&player);

  Text(&restartscreen, 18, 10, "POZIOM OD NOWA");
  Text(&restartscreen, 20, 12,  "-1 Zdrowie");

  printCanva(&restartscreen);

  level = level_copy;

  p -> health -= 1;
  p -> ent.pos = *startpos;
  p -> ent.dir = nullpoint;
  p -> destroy = nullpoint;
  p -> is_moving = false;
  p -> ldt = 0;

  wait(LVLRESTARTTIME);
  restart_cooldown = RESTART_COOLDOWN;
  ClearConsoleScreen(CURSOR_POSITION_X, CURSOR_POSITION_Y);
}







// +-------------------   Wyświetlanie Ekranu startowego gry   -----------------------------+
void showgamemenu()
{
  Text(&startgamescreen, 16, 5, "Witaj w grze avoid!");
  Text(&startgamescreen, 14, 7, "Milosz Skobejko 187224");
  Text(&startgamescreen, 14, 12, "TECHNIKI PROGRAMOWANIA");
  Text(&startgamescreen, 12, 14, "POLITECHNIKA GDANSKA 2022");

  printCanva(&startgamescreen);

  wait(4.0);
  ClearConsoleScreen(CURSOR_POSITION_X, CURSOR_POSITION_Y);
}







// +------------------------------   Wyświetlanie UI   ---------------------------------+
void showgameui(struct player_t *p)
{
  printf("HEALTH:\t");

  for (int i = 0; i < p -> health; i++)
    printf("%c ", HEALTH_C);

  if (p -> health > 0)
  {
    for (int j = 0; j < HEALTH - p -> health; j++)
      printf("  ");
  }
  else
    printf("          ");

  if (score < 10)
    printf("      SCORE: 00%d  MAX SCORE: %d  ", score, MaxScore);
  else if (score < 100 && score > 9)
    printf("     SCORE: 0%d   MAX SCORE: %d  ", score, MaxScore);
  else if (score < 1000 && score > 99)
    printf("    SCORE: %d   MAX SCORE: %d  ", score, MaxScore);
  printf("\n");
}


// +--------------------------   Ustawienia cooldownu   --------------------------------+
void cooldown(int * c)
{
  if (*c > 0)
    *c -= 1;
  return;
}



// +----------------------   Ustawienia następnego poziomu   ---------------------------+
void nextlevel(struct player_t *p, struct entity *e, struct levels *lvl)
{
  score += p -> health;

  if (score > MaxScore)
    MaxScore = score;

  generatelevel(lvl, TILE_SIZE, BACKGROUND, WALL);

  p -> health = HEALTH;
  p -> ent.pos = lvl -> start_pos;
  p -> ent.dir = nullpoint;
  p -> destroy = nullpoint;
  p -> is_moving = false;
  p -> ldt = 0;

  level_copy = level;

  createEnemies(&level ,enemies);
}







// +----------------     Inicjalizacja, dzieje się raz na początku ---------------------+
int Initialize()
{

  score = 0;
  damage_cooldown  = 0;
  restart_cooldown = 0;

  srand(time(NULL)); // <- ziarno dla generatora
  nullpoint.x = 0;
  nullpoint.y = 0;

  // Generowanie poziomu
  generatelevel(&level, TILE_SIZE, BACKGROUND, WALL);
  level_copy = level;   /* Tworzenie kopii poziomu, aby użyć w razie restartu */


  // Tworzenie gracza, dane gracza
  player.health = HEALTH;
  player.is_moving = false;
  player.destroy = nullpoint;
  player.ldt = 0;

              /* Entity , start_pos,   speed,  icon*/
  createEntity(&player.ent, level.start_pos, nullpoint, 1, PLAYER_C);


  // Tworzenie przeciwników
  createEnemies(&level ,enemies);

  // Tworzenie innych płótn
  createCanva(&startgamescreen, WINDOW_WIDTH, WINDOW_HEIGHT, BACKGROUND, BORDER);
  createCanva(&restartscreen,   WINDOW_WIDTH, WINDOW_HEIGHT, BACKGROUND, BORDER);
  createCanva(&endgamescreen,   WINDOW_WIDTH, WINDOW_HEIGHT, BACKGROUND, BORDER);

  // Wypełianie ramką
  FillEmptyCanva(&startgamescreen,BACKGROUND, BORDER);
  FillEmptyCanva(&restartscreen,  BACKGROUND, BORDER);
  FillEmptyCanva(&endgamescreen,  BACKGROUND, BORDER);

  // Jeśli nie można stworzyć płótna, zwraca 1
  return createCanva(&canva, WINDOW_WIDTH, WINDOW_HEIGHT, BACKGROUND, BORDER);
}








// +--------------------  Wyświetlanie obrazu, funkcje logiczne -----------------------+
void Refresh()
{

  showgameui(&player);                          /*                  UI                */
  FillEmptyCanva(&canva, BACKGROUND, BORDER);   /*     Wypełnianie płótna ramką       */
  canva = level.layout;                         /*     Wypełnianie płótna poziomem    */
  cooldown(&damage_cooldown);                   /*        sprawdzanie cooldownu       */
  cooldown(&restart_cooldown);                  /*        sprawdzanie cooldownu       */


  // Sprawdzanie, czy gracz jest na ekranie i wyświetlanie
  if (isOnCanva(&canva, &player.ent))
    canva.fill[player.ent.pos.x][player.ent.pos.y] = player.ent.icon;



  // Sprawdzanie, czy przeciwnik jest na ekranie i wyświetlanie
  for (int i = 0; i < level.enemies; i++)
    if (isOnCanva(&canva, &enemies[i]))
      canva.fill[enemies[i].pos.x][enemies[i].pos.y] = enemies[i].icon;



  // Zamiana zniszczonej sciany na tło po oddaleniu się gracza
  if (player.ldt > 0 && worldCollisions(&canva, &player.ent, BROKEN_WALL, NULL, NULL) == false)
  {
    if (player.destroy.x != nullpoint.x && player.destroy.y != nullpoint.y)
      {
        canva.fill[player.destroy.x][player.destroy.y] = BACKGROUND;
        level.layout.fill[player.destroy.x][player.destroy.y] = BACKGROUND;
      }
    player.destroy = nullpoint;
  }



  // Aktualizowanie pozycji gracza
  if (worldCollisions(&canva, &player.ent, WALL, &player.destroy, &player.ldt) == false &&
      worldCollisions(&canva, &player.ent, BORDER       , NULL, NULL) == false &&
      worldCollisions(&canva, &player.ent, BROKEN_WALL  , NULL, NULL) == false)
  {
    /* Jeśli nie wykryto kolizji */
    updateEntityMovement(&player.ent);
    if (player.is_moving)
      player.ldt += 1;
  }
  else
  {
    /*   Jeśli wykryto kolizje   */
    if (player.destroy.x != nullpoint.x && player.destroy.y != nullpoint.y)
    {
      canva.fill[player.destroy.x][player.destroy.y] = BROKEN_WALL;
      level.layout.fill[player.destroy.x][player.destroy.y] = BROKEN_WALL;
    }

    player.is_moving = false;
    player.ldt = 0;
    player.ent.dir = nullpoint;
  }


  //Sprawdzenie, czy gracz dotarł do portalu
  if (pointcmp(&player.ent.pos, &level.finish_pos))
    nextlevel(&player, enemies, &level);


  //Sprawdzanie kolizji z przeciwnikiem 1
  if (enemycollision(&player, enemies, level.enemies, &damage_cooldown))
  {
    player.health--;
    damage_cooldown = DAMAGE_COOLDOWN;  /* Zapobiega oberwaniu 2 razy w tym samym miejscu */
  }


  // Aktualizowanie pozycji przeciwników
  for (int i = 0; i < level. enemies; i++)
  {
      if (worldCollisions(&canva, &enemies[i], WALL       , NULL, NULL) == false &&
          worldCollisions(&canva, &enemies[i], BORDER     , NULL, NULL) == false &&
          worldCollisions(&canva, &enemies[i], BROKEN_WALL, NULL, NULL) == false)
      {
        /* Nie wykryto kolizji */
        updateEntityMovement(&enemies[i]);
      }
      else
      {
        /* Jeśli wykryto kolizje  [tylko ze ścianą]           */
        /* Zmiana kierunku przeciwnika, gdy dojdzie do sciany */
        if (enemies[i].dir.x == 0)
          enemies[i].dir.y *= -1;
        else
          enemies[i].dir.x *= -1;
      }
  }


  //Sprawdzanie kolizji z przeciwnikiem 2
  if (enemycollision(&player, enemies, level.enemies, &damage_cooldown))
  {
    player.health--;
    damage_cooldown = DAMAGE_COOLDOWN;
  }



  // Sprawdzanie czy się przegrało, rozpoczynanie nowej gry
  if (player.health < 1)
  {
    if (gamerestart())
      Initialize();
    else
      {
        gameover = true;
        return;
      }
  }

  // Wyświetlanie płótna i od razu czyszczenie
  printCanva(&canva);
  ClearConsoleScreen(CURSOR_POSITION_X, CURSOR_POSITION_Y);
}







// +------------------------------------  Main  ---------------------------------------+
int main()
{


  /* Inizjalizja danych, płótn, wszystkich wartości domyśnych */
  if (Initialize() == false)
    return 1;

  /* Wyśietlenie  UI ---------------------------------------- */
  showgamemenu();



  /* Gra dopóki gracz nie wyjdzie  -------------------------- */
  while (!gameover)
  {
    Refresh();

    switch (input(&player))
    {
      case close:   //Gracz chce zakończyć gre
        gameover = true;
        break;

      case restart: //Gracz chce powórzyć poziom
        restartlevel(&player, &level.start_pos);
        break;

      default:
        break;
    }
  }


  // Napisy koncowe
  ClearConsoleScreen(CURSOR_POSITION_X, CURSOR_POSITION_Y);
  printf("Dziekuje za gre :)");
  wait(LVLRESTARTTIME);

  return 0;
}
