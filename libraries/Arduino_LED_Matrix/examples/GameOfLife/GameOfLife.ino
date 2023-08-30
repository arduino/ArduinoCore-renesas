/*
  Game Of Life

  The Game of Life, also known simply as Life, is a cellular automaton devised 
  by the British mathematician John Horton Conway in 1970. It is a zero-player game, 
  meaning that its evolution is determined by its initial state, requiring no further 
  input.

  Example developed starting from Toby Oxborrow's sketch
  https://github.com/tobyoxborrow/gameoflife-arduino/blob/master/GameOfLife.ino

  See the full documentation here:
  https://docs.arduino.cc/tutorials/uno-r4-wifi/led-matrix
*/

#include "Arduino_LED_Matrix.h" // Include the LED_Matrix library

// grid dimensions. should not be larger than 8x8
#define MAX_Y 8
#define MAX_X 12

// time to wait between turns
#define TURN_DELAY 200

// how many turns per game before starting a new game
// you can also use the reset button on the board
#define TURNS_MAX 60

// number of patterns in predefined list
#define MAX_PATTERNS 4

// how many turns to wait if there are no changes before starting a new game
#define NO_CHANGES_RESET 4

int turns = 0;       // counter for turns
int noChanges = 0;  // counter for turns without changes

// game state. 0 is dead cell, 1 is live cell
uint8_t grid[MAX_Y][MAX_X] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

int currentPattern = 0;

String patternNames[] = {
  "Glider",
  "Light-weight spaceship",
  "R-Pentomino",
  "Diehard"
};

// custom starting grid patterns
boolean cGrids[][MAX_Y][MAX_X] = {
    { /* Glider */
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    },
    { /* Light-weight spaceship */
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    },
    { /* R-Pentomino */
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    },
    { /* Die hard */
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    }
};


ArduinoLEDMatrix matrix;

void setup() {
  Serial.begin(9600);
  delay(1000);

  Serial.println("Conway's game of life on Arduino LED Matrix");
  matrix.begin();
  
  resetGrid();
  displayGrid();

}

void loop() {
  delay(TURN_DELAY);

  playGoL();

  turns++;

  // reset the grid if no changes have occured recently
  // for when the game enters a static stable state
  if (noChanges > NO_CHANGES_RESET) {
    resetGrid();

  }
  // reset the grid if the loop has been running a long time
  // for when the game cycles between a few stable states
  if (turns > TURNS_MAX) {
    resetGrid();
  }

  displayGrid();
}

// play game of life
void playGoL() {
  /*
    1. Any live cell with fewer than two neighbours dies, as if by loneliness.
    2. Any live cell with more than three neighbours dies, as if by
    overcrowding.
    3. Any live cell with two or three neighbours lives, unchanged, to the next
    generation.
    4. Any dead cell with exactly three neighbours comes to life.
    */

  boolean newGrid[MAX_Y][MAX_X] = {
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };

  for (int y = 0; y < MAX_Y; y++) {
    for (int x = 0; x < MAX_X; x++) {
      int neighboughs = countNeighbours(y, x);
      if (grid[y][x] == 1) {
        if ((neighboughs == 2) || (neighboughs == 3)) {
          newGrid[y][x] = 1;
        } else {
          newGrid[y][x] = 0;
        }
      } else {
        if (neighboughs == 3) {
          newGrid[y][x] = 1;
        } else {
          newGrid[y][x] = 0;
        }
      }
    }
  }

  // update the current grid from the new grid and count how many changes
  // occured
  int changes = 0;
  for (int y = 0; y < MAX_Y; y++) {
    for (int x = 0; x < MAX_X; x++) {
      if (newGrid[y][x] != grid[y][x]) {
        changes++;
      }
      grid[y][x] = newGrid[y][x];
    }
  }

  // update global counter when no changes occured
  if (changes == 0) {
    noChanges++;
  }
}

// count the number of neighbour live cells for a given cell
int countNeighbours(int y, int x) {
  int count = 0;

  // -- Row above us ---
  if (y > 0) {
    // above left
    if (x > 0) {
      count += grid[y - 1][x - 1];
    }
    // above
    count += grid[y - 1][x];
    // above right
    if ((x + 1) < 8) {
      count += grid[y - 1][x + 1];
    }
  }

  // -- Same row -------
  // left
  if (x > 0) {
    count += grid[y][x - 1];
  }
  // right
  if ((x + 1) < 8) {
    count += grid[y][x + 1];
  }

  // -- Row below us ---
  if ((y + 1) < 8) {
    // below left
    if (x > 0) {
      count += grid[y + 1][x - 1];
    }
    // below
    count += grid[y + 1][x];
    // below right
    if ((x + 1) < 8) {
      count += grid[y + 1][x + 1];
    }
  }

  return count;
}

// reset the grid
void resetGrid() {
  Serial.print("Current pattern: ");
  Serial.println(patternNames[currentPattern]);
  noChanges = 0;
  turns = 0;

  for (int y = 0; y < MAX_Y; y++) {
    for (int x = 0; x < MAX_X; x++) {
      grid[y][x] = cGrids[currentPattern][y][x];
    }
  }
  currentPattern++;
  if(currentPattern >= MAX_PATTERNS){
    currentPattern = 0;
  }
}

// display the current grid to the LED matrix
void displayGrid() {
  matrix.renderBitmap(grid, 8, 12);
}