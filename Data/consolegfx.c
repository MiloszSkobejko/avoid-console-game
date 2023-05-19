// Dołączanie niezbędnych bibliotek
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdbool.h>

#define WINDOW_WIDTH 50
#define WINDOW_HEIGHT 23

#define CURSOR_POSITION_X 0
#define CURSOR_POSITION_Y 0

#define MAX_WAIT_TIME 30.0

// Parametry płótna
struct canvas{

  int sizeX;
  int sizeY;

  char background;
  char outline;

  char fill[WINDOW_WIDTH][WINDOW_HEIGHT];
};


// Sprawia, że kod czeka podaną ilość sekund
void wait (float seconds)
{
    if (seconds > MAX_WAIT_TIME || seconds < 0)
      return;

    clock_t start = clock();
    float passed = 0;

    do
    {
      clock_t end = clock();
      passed = (float)(end - start) / CLOCKS_PER_SEC;
    } while (passed < seconds);

    return;
}


// Czyści ekran konsoli
void ClearConsoleScreen(int CursorPosX, int CursorPosY)
{
	HANDLE Out;
	COORD Position;

	Out = GetStdHandle(STD_OUTPUT_HANDLE);

	Position.X = CursorPosX;
	Position.Y = CursorPosY;
	SetConsoleCursorPosition(Out, Position);
}



// Wypełnia płótno wartościami domyśnymi
void FillEmptyCanva(struct canvas *c, char background, char outline)
{

	for (int y = 0; y < c -> sizeY; y++) {
		for (int x = 0; x < c -> sizeX; x++)
		 {
			 // Inny znak na ramkę
			if (x == 0 || x == c -> sizeX - 1 || y == 0 || y == c -> sizeY - 1)
        c -> fill[x][y] = outline;
			else
				c -> fill[x][y] = background;
		}
	}
}


// Tworzy nowe płótno
bool createCanva(struct canvas *c, int sizeX, int sizeY, char background, char outline)
{
  if (sizeX > WINDOW_WIDTH || sizeY > WINDOW_HEIGHT || c == NULL || sizeX < 0 || sizeY < 0)
    return false;

  c -> sizeX = sizeX;
  c -> sizeY = sizeY;
  c -> background = background;
  c -> outline = outline;

  FillEmptyCanva(c, background, outline);

  return true;
}



// Wyświetla płótno
void printCanva(struct canvas *c)
{
	for (int i = 0; i < c -> sizeY; i++) {
		for(int j = 0; j < c -> sizeX; j++)
			putchar(c -> fill[j][i]);
		putchar('\n');
	}
}



// Wyświetla tekst na płótnie rozpoczynając od [start_x, start_y]
void Text(struct canvas *c, int start_x, int start_y, char text[])
{
	if (start_y > 0 && start_y < c -> sizeY)
	{
		int i = 0;

		while (text[i] != '\0')
		{
			if (start_x > 0 && start_x < c -> sizeX)
				c -> fill[start_x][start_y] = text[i];

			start_x++;
			i++;
		}
	}
}
