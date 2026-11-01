#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

#define MAX_LEVEL 3

#define MAX_GHOSTS 10
#define MS_PER_GHOST_TICK 500
#define MS_PER_TICK 20
int currentLevel = 0;
typedef struct {
  int row;
  int col;
} Coord;

typedef enum { Up, Left, Idle, Right, Down } Direction;

Direction oppositeDirection(Direction dir) { return (Direction)(4 - dir); }

Coord moveOneStep(Coord from, Direction dir) {
  static const int rowDelta[] = {-1, 0, 0, 0, 1};
  static const int colDelta[] = {0, -1, 0, 1, 0};
  return (Coord){from.row + rowDelta[dir], from.col + colDelta[dir]};
}

typedef struct {
  Coord pos;
  Direction direction;
  char itemBelow;
} Ghost;

typedef enum { GS_Preparing, GS_Running, GS_Win, GS_Lose } GameStatus;

typedef struct {
  GameStatus status;
  int level;
  int score;

  int numRows;
  int numCols;
  int ghostCnt;
  int foodsCnt;

  Coord pacmanPos;

  Ghost ghosts[MAX_GHOSTS];

  char **grid;
} Game;

bool isWall(char c) { return c == 'B' || c == 'Y' || c == 'R' || c == 'G'; }

bool isFood(char c) { return c == '.'; }

bool isPacman(char c) { return c == 'C'; }

bool isGhost(char c) { return c == '@'; }

bool isoutofmap (int x, int y, int row, int col) {
  if (x < 0 || x >= row || y < 0 || y >= col)
    return true;
  return false;

}
/**
 * @brief Create a Game object. Level information including the map is read from
 * the file @c mapFileName. Return the Game object created.
 *
 * @param level Level ID.
 * @param mapFileName Path to the map file.
 * @return Game The created Game object.
 */
Game createGame(int level, const char *mapFileName) {
  Game game;
  game.status = GS_Preparing;
  game.level = level;
  game.score = 0;
  game.foodsCnt = 0;
  // Read the map file, which is in the folder maps, with the form like level0.txt, level1.txt, etc.
  FILE *fp = fopen(mapFileName, "r");
  assert(fp != NULL);
  fscanf(fp, "%d%d%d", &game.numRows, &game.numCols, &game.ghostCnt);
  game.grid = malloc(game.numRows * sizeof(char *));
  for (int i = 0; i < game.numRows; i++)
    game.grid[i] = malloc(game.numCols * sizeof(char));
  for (int i = 0; i < game.numRows; i++) {
    for (int j = 0; j < game.numCols; j++) {
      char c;
      fscanf(fp, "%c", &c);
      if (c == '\n') {
        j--;
        continue;
      }
      if (isPacman(c)) {
        game.pacmanPos = (Coord){i, j};
      }
      if (c >= '0' && c <= '9') {
        game.ghosts[c-'0'].pos = (Coord){i, j};
        game.ghosts[c-'0'].itemBelow = ' ';
        game.ghosts[c-'0'].direction = Idle;
        c = '@';
      }
      if (isFood(c))
        game.foodsCnt++;
      game.grid[i][j] = c;
    }
  }
  char dir[1000];
  int i = 0;
  fgets(dir, sizeof(dir) - 1, fp);   
  while(!feof(fp))  
  {  
      memset(dir, 0, sizeof(dir));  
      fgets(dir, sizeof(dir) - 1, fp);   
      if (dir[0] == 'u')
        game.ghosts[i].direction = Up;
      else if (dir[0] == 'd')
        game.ghosts[i].direction = Down;
      else if (dir[0] == 'l')
        game.ghosts[i].direction = Left;
      else if (dir[0] == 'r')
        game.ghosts[i].direction = Right;
      i++;
  }  
  fclose(fp);
  return game;
}

/**
 * @brief Destroy a Game object. Resources are released.
 *
 * @param pGame
 */
void destroyGame(Game *pGame) {
  for (int i = pGame->numRows - 1; i >= 0; --i)
    free(pGame->grid[i]);
  free(pGame->grid);
}

void printInitialGame(const Game *pGame) {
  clear_screen();
  for (int i = 0; i != pGame->numRows; ++i)
  {
    for (int j = 0; j != pGame->numCols; ++j)
    {
      switch (pGame->grid[i][j])
      {
      case 'C':
        printf(BRIGHT_YELLOW_TEXT("C"));
        break;
      case 'B':
        printf(BLUE_TEXT("#"));
        break;
      case 'Y':
        printf(YELLOW_TEXT("#"));
        break;
      case 'R':
        printf(RED_TEXT("#"));
        break;
      case 'G':
        printf(GREEN_TEXT("#"));
        break;
      default:
        putchar(pGame->grid[i][j]);
        break;
      }
    }
    putchar('\n');
  }
  putchar('\n');
  printf("Level %d\n", pGame->level);
  assert(pGame->score == 0);
  printf("Score: 0\n");
  printf("Food remaining: %d\n", pGame->foodsCnt);
  printf("Pacman wants food! (control by w/a/s/d)\n");
}

void printScoreUpdate(const Game *pGame) {
  move_cursor(pGame->numRows + 2, 7);
  printf("%d          ", pGame->score);
}

void printFoodUpdate(const Game *pGame) {
  move_cursor(pGame->numRows + 3, 16);
  printf("%d          ", pGame->foodsCnt);
}

/**
 * @brief Move all the ghosts by one step.
 *
 * @param pGame Pointer to the current Game object.
 */
void moveGhosts(Game *pGame) {
  int cnt = pGame->ghostCnt;
  for (int i = 0; i<cnt; i++) {
    Coord prev = pGame->ghosts[i].pos;
    Direction dir = pGame->ghosts[i].direction;
    Coord tmp = moveOneStep(prev, dir);
    if (isWall(pGame->grid[tmp.row][tmp.col]) || isoutofmap(tmp.row, tmp.col, pGame->numRows, pGame->numCols)) {
      dir = oppositeDirection(dir);
    }
    pGame->ghosts[i].direction = dir;
    pGame->ghosts[i].pos = moveOneStep(pGame->ghosts[i].pos, dir);
    if (pGame->ghosts[i].itemBelow == '@') {
      for (int j = cnt-1; j >= 0; j--) {
        if (pGame->ghosts[j].pos.col == prev.col && pGame->ghosts[j].pos.row == prev.row && pGame->ghosts[j].direction != Idle && i != j) 
          pGame->grid[prev.row][prev.col] = pGame->ghosts[j].itemBelow;
      }
    }
    else  pGame->grid[prev.row][prev.col] = pGame->ghosts[i].itemBelow;
    pGame->ghosts[i].itemBelow = pGame->grid[pGame->ghosts[i].pos.row][pGame->ghosts[i].pos.col];
    pGame->grid[pGame->ghosts[i].pos.row][pGame->ghosts[i].pos.col] = '@';
    move_cursor(prev.row, prev.col);
    printf("%c", pGame->grid[prev.row][prev.col]);
    move_cursor(pGame->ghosts[i].pos.row, pGame->ghosts[i].pos.col);
    printf("%c", pGame->grid[pGame->ghosts[i].pos.row][pGame->ghosts[i].pos.col]);
  }
  return;
}

Direction getPacmanMovement(void) {
  if (kbhit()) {
    switch (getch()) {
    case 'w': return Up;
    case 's': return Down;
    case 'a': return Left;
    case 'd': return Right;
    }
  }
  return Idle;
}

/**
 * @brief Move the Pacman by one step, following the user's control obtained by
 * @c getPacmanMovement().
 *
 * @param pGame Pointer to the current Game object.
 */
void movePacman(Game *pGame) {
  Direction dir = getPacmanMovement();
  Coord prev = pGame->pacmanPos;
  Coord tmp = moveOneStep(pGame->pacmanPos, dir);
  if (isWall(pGame->grid[tmp.row][tmp.col]) || isoutofmap(tmp.row, tmp.col, pGame->numRows, pGame->numCols)) {
    return;
  }
  pGame->pacmanPos = tmp;
  if (isFood(pGame->grid[tmp.row][tmp.col])) {
    pGame->foodsCnt--;
    pGame->score+=10;
    printFoodUpdate(pGame);
  }
  pGame->grid[prev.row][prev.col] = ' ';
  if (!isGhost(pGame->grid[pGame->pacmanPos.row][pGame->pacmanPos.col])) //To avoid pacman run into a ghost but doesn't die
    pGame->grid[pGame->pacmanPos.row][pGame->pacmanPos.col] = 'C';

  move_cursor(prev.row, prev.col);
  putchar(' ');
  move_cursor(pGame->pacmanPos.row, pGame->pacmanPos.col);
  if (!isGhost(pGame->grid[pGame->pacmanPos.row][pGame->pacmanPos.col]))
    printf(BRIGHT_YELLOW_TEXT("C"));
  return;
}

/**
 * @brief Test if Pacman has died.
 */
bool pacmanDies(const Game *pGame) {
  if (isGhost(pGame->grid[pGame->pacmanPos.row][pGame->pacmanPos.col]))
    return true;
  return false;
}

/**
 * @brief Test if the game has ended. If so, update the @c status and return
 * true; otherwise return false.
 *
 * @param pGame Pointer to the current Game object.
 */
bool gameEnds(Game *pGame) {
  if (pacmanDies(pGame)) {
    pGame->status = GS_Lose;
    return true;
  }
  if (pGame->foodsCnt == 0) {
    pGame->status = GS_Win;
    return true;
  }
  return false;
}

void runGame(Game *pGame) {
  printInitialGame(pGame);
  pGame->status = GS_Running;
  int ticks = 0;
  while (1) {
    sleep_ms(MS_PER_TICK);
    ++ticks;
    movePacman(pGame);
    if (gameEnds(pGame))
      break;
    if (ticks == MS_PER_GHOST_TICK / MS_PER_TICK) {
      ticks = 0;
      --pGame->score;
      printScoreUpdate(pGame);
      moveGhosts(pGame);
      if (gameEnds(pGame))
        break;
    }
  }
}

void runLevel(int level) {
  char path[100];
  sprintf(path, "maps/level%d.txt", level);
  Game game = createGame(level, path);
  runGame(&game);
  assert(game.status == GS_Lose || game.status == GS_Win);
  move_cursor(game.numRows + 4, 0);
  if (game.status == GS_Lose)
    printf("Pacman dies!");
  else {
    printf("You win!");
    currentLevel = level + 1;
  }
  printf("                                       \n");
  destroyGame(&game);
}

int main(void) {
  prepare_game();
  while (currentLevel <= MAX_LEVEL) {
    int tmp = currentLevel;
    runLevel(currentLevel);
    if (currentLevel == tmp) {
      printf("Press Enter to restart, or press q to quit the game.\n");
      char c;
      while ((c = _getch()) != '\r') {
        if (c == 'q') {
          return 0;
        }
      }
    }
    else {
      if (currentLevel > MAX_LEVEL) {
        system("pause");
        return 0;
      }
      printf("Press Enter to continue to the next level, or press q to quit the game.\n");
      char c;
      while ((c = _getch()) != '\r') {
        if (c == 'q') {
          return 0;
        }
      }
    }
  }
  return 0;
}