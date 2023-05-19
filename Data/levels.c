#define TILE_SIZE 3
#define MAX_ENEMY 9



// Punkt
typedef struct point{
  int x;
  int y;
} point;



// Parametry poziomu
struct levels{

  point start_pos;      /* Pozycja startowa gracza */
  point finish_pos;     /* Pozycja portalu         */

  short enemies;        /* Ilość przeciwników      */

  struct canvas layout; /* Konstrukcja poziomu     */
};


// struktura płytki 2D
typedef struct tile{

  int exits; /* Ilość wyjść */
  int size;  /* Rozmiar     */

  char fill[TILE_SIZE][TILE_SIZE];
} tile;





// Porównanie, czy 2 punkty są takie same
bool pointcmp(point *point1, point *point2)
{
  return (point1 -> x == point2 -> x && point1 -> y == point2 -> y);
}




// Przypisuje losową pozycję, zwraca 1, jeśli się udało
void randompos(struct levels *lvl, point *pos, char wall)
{
  point temppos;

  do
  {
    temppos.x = rand() % (lvl -> layout.sizeX - 3) + 1;
    temppos.y = rand() % (lvl -> layout.sizeY - 3) + 1;
    /* Jeśli wybrana pozycja to ściana to powtórz */
  } while (lvl -> layout.fill[temppos.x][temppos.y] == wall || lvl -> layout.fill[temppos.x + 1][temppos.y] == wall);

  /* Przypisanie */
  pos -> x = temppos.x;
  pos -> y = temppos.y;

  return;
}




// Wybiera losowe miejsca z płytki i wypełnia tłem
bool cherrypick(tile *t, int times, char bg, char border)
{
  int i = 0,  s = 0;

  do
  {
      int rx = rand() % t -> size;
      int ry = rand() % t -> size;

      if (t -> fill[rx][ry] == border)
      {
        t -> fill[rx][ry] = bg;
        i++;
      }

      s++;  //zabepieczenie przed niesk pętlą
      if (s > 10)
        return false;

  } while(i < times);

  return true;
}



// Wypełnia płytke jednym kolorem
void filltile(tile *t, char fill)
{
  for(int i = 0; i < t -> size; i++)
    for (int j = 0; j < t -> size; j++)
      t -> fill[i][j] = fill;
}



// Tworzy płytkę o <exits> wyjściach, zwraca 1, jeśli niemożliwe
bool generatetile(tile *t, char bg, char wall)
{
  if (t == NULL || t -> exits < 0)
    return false;

  switch (t -> exits)
  {
    case 0:
        filltile(t, wall);
        break;

    case 1:
        filltile(t, wall);

        if (rand() % 2)
          t -> fill[1][1] = bg;

        // Wycinanie pustego pola z jednej strony
        switch (rand() % 3)
        {
          case 0:
              t -> fill[0][1] = bg;
              break;
          case 1:
              t -> fill[1][0] = bg;
              break;
          case 2:
              t -> fill[1][2] = bg;
              break;
          case 3:
              t -> fill[2][1] = bg;
              break;
        }
        break;

    case 2:
        filltile(t, wall);

        t -> fill[1][1] = bg;

        cherrypick(t, t -> exits, bg, wall);
        break;

    case 3:
        filltile(t, bg);

        cherrypick(t, t -> exits, wall, bg);
        break;

    case 4:
        filltile(t, bg);

        cherrypick(t, t -> exits - 2, wall, bg);
        break;

    default:
        filltile(t, bg);
        break;
  }

  return true;
}



// Generowanie poziomu
bool generatelevel(struct levels *level, int tile_size, char bg, char wall)
{
  int size = (WINDOW_WIDTH / TILE_SIZE) * (WINDOW_HEIGHT / TILE_SIZE) + (2 * TILE_SIZE); // ilosć płytek w poziomie
  tile tiles[size];


  createCanva(&level -> layout, WINDOW_WIDTH, WINDOW_HEIGHT, bg, wall);


  // Wygenerowanie płytek
  for(int i = 0; i < size; i++)
  {
    int randomopen = (rand() % 10) + 6;

    tiles[i].exits = rand() % randomopen;
    tiles[i].size = tile_size;

    if (generatetile(&tiles[i], bg, wall) == false)
      return false;
  }


  // Stworzenie pustego poziomu (tylko obramowka)
  FillEmptyCanva(&level -> layout, bg, BORDER);


  // Wypełnianie poziomu wygenerowanymi płytkami
  int count = 0;

  for (int j = 1; j < (level -> layout.sizeY) - 1; j += tile_size)
  {
    for(int i = 1; i < (level -> layout.sizeX) - 1; i += tile_size)
    {
      for (int tile_y = 0; tile_y < tile_size; tile_y++)
        for(int tile_x = 0; tile_x < tile_size; tile_x++)
          level -> layout.fill[i + tile_x][j + tile_y] = tiles[count].fill[tile_x][tile_y];

      count++;
    }
    count++;
  }



  // Generowanie Pozycji Startowej
  randompos(level, &level -> start_pos, wall);

  // Generowanie Pozycji Końcowej (Portalu)
  do
    randompos(level, &level -> finish_pos, wall);
  while (pointcmp(&level -> start_pos, &level -> finish_pos) == true || level -> layout.fill[level -> finish_pos.x][level -> finish_pos.y] == wall);



  level -> layout.fill[level -> finish_pos.x][level -> finish_pos.y] = FINISHPORTAL;
  level -> enemies = rand() % (MAX_ENEMY - 2) + 2;

  return true;
}
