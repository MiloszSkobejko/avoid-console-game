
// struktura danych jednostki (np. gracza albo przeciwnika)
struct entity{

  point pos;      /*    Pozycja  */
  point dir;      /*   Kierunek  */

  int speed;      /*  Predkosc   */
  char icon;      /*    Ikona    */
};


// struktura danych gracza
struct player_t{

  short health;   /*  Zdrowie    */
  bool is_moving; /* czy.porusza */
  point destroy;  /* p(x,y) sciany do znieszczenia */
  int ldt;        /* Last distance traveled */

  struct entity ent;
};



// Tworzy jednostke
void createEntity(struct entity *e, point position, point direction, int speed, char icon)
{
   e -> pos = position;
   e -> dir = direction;
   e -> speed = speed;
   e -> icon = icon;
}


// Tworzy przeciwników w danym poziomie
void createEnemies(struct levels *lvl ,struct entity *e)
{
  for (int i = 0; i < lvl -> enemies; i++)
  {
    point enemypos;
    point enemydir;

    randompos(lvl, &enemypos, WALL);

    do
    {
      enemydir.x = (rand() % 3) - 1;
      enemydir.y = (enemydir.x != 0 ? 0 : (rand() % 3) - 1);
    } while(enemydir.x == 0 && enemydir.y == 0);

    createEntity(e + i, enemypos, enemydir, 1, ENEMY_C);
  }
}


// Sprawdza kolizję z podanym znakiem
bool worldCollisions(struct canvas *canva, struct entity *e, char wall, point *destroy, int *ldt)
{
  if (canva -> fill[e -> pos.x + e -> dir.x][e -> pos.y + e -> dir.y] == wall)
  {
    if (destroy != NULL)
    {
      if (ldt != NULL && *ldt > 0)
      {
        destroy -> x = e -> pos.x + e -> dir.x;
        destroy -> y = e -> pos.y + e -> dir.y;
      }
    }
    return true;
  }


  return false;
}



// Sprawdza, czy gracz nie dotyka przeciwnika
bool enemycollision(struct player_t *player, struct entity *enemies, int n, int *damage_cooldown)
{
  for (int i = 0; i < n; i++)
    if (pointcmp(&player -> ent.pos, &enemies[i].pos) && *damage_cooldown == 0)
      return true;

  return false;
}



// Sprawdza, czy jednostka mieści się na ekranie
bool isOnCanva(struct canvas *canva, struct entity *e)
{
    /* Jeśli (x,y) są większe niż aktualne płótno to jednostka nie może być wyświetlona */
    if (e -> pos.x > 0 && e -> pos.x < WINDOW_WIDTH - 1 && e -> pos.y < WINDOW_HEIGHT - 1 && e -> pos.y > 0)
      return true;

    return false;
}



// Aktualizowanie pozycji jednostki
void updateEntityMovement(struct entity *e)
{
  e -> pos.x   +=  e -> dir.x * e -> speed;
  e -> pos.y   +=  e -> dir.y * e -> speed;
}
