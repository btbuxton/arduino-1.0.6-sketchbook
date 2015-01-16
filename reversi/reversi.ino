/*
 * Uses Seeedstudio TFT 2.8" Screen Version 1.0
 * 
 * Implement pass
 */
#include <TFT.h>
#include <TouchScreen.h>

#define TS_MINX 140
#define TS_MAXX 900
#define TS_MINY 120
#define TS_MAXY 940

#define SIZE 8
#define BLANK 0
#define BLACK_PIECE 1
#define WHITE_PIECE 2
#define ERROR_PIECE 3

const word SQUARE_SIZE = min(MAX_X,MAX_Y) / SIZE;
const short DIRS[8][2] = {
  { 0,-1 }
  ,{ 1,-1 }
  ,{ 1,0 }
  ,{ 1,1 }
  ,{ 0,1 }
  ,{ -1,1 }
  ,{ -1,0 }
  ,{ -1,-1 }
};
byte REVERSI[SIZE][SIZE];
byte CURRENT_PLAYER;
bool GAME_OVER;

TouchScreen TS = TouchScreen(17, A2, A1, 14, 300);

void init_board(byte board[SIZE][SIZE]) {
  for (byte x=0; x < SIZE; x++) {
    for (byte y=0; y < SIZE; y++) {
      board[x][y]=BLANK;
    }
  }
  board[3][3]=BLACK_PIECE;
  board[4][4]=BLACK_PIECE;
  board[3][4]=WHITE_PIECE;
  board[4][3]=WHITE_PIECE;
}

void get_score(byte *black_score, byte *white_score, byte board[SIZE][SIZE]) {
  *white_score = *black_score = 0;
  for (byte x=0; x < SIZE; x++) {
    for (byte y=0; y < SIZE; y++) {
      byte value = board[x][y];
      if (value == BLACK_PIECE) {
        (*black_score)++;
      } 
      else if (value == WHITE_PIECE) {
        (*white_score)++;
      }
    }
  }
}

void draw_board(byte board[SIZE][SIZE]) {
  Tft.fillRectangle(MIN_X,MIN_Y,(MAX_X-MIN_X),(MAX_Y-MIN_Y),GREEN);
  for (byte x = 0; x <= SIZE; x++) {
    Tft.drawLine(x * SQUARE_SIZE, 0, x * SQUARE_SIZE, SIZE * SQUARE_SIZE, BLACK);
  }
  for (byte y = 0; y <= SIZE; y++) {
    Tft.drawLine(0, y * SQUARE_SIZE, SIZE * SQUARE_SIZE, y * SQUARE_SIZE, BLACK);
  }
}

void update_status(byte turn, byte board[SIZE][SIZE]) {
  byte half = SQUARE_SIZE / 2;
  Tft.fillRectangle(SQUARE_SIZE, SQUARE_SIZE * 8 + half, SQUARE_SIZE * 6 + half, SQUARE_SIZE * 2, GREEN);
  char *which_player = "";
  if (turn == BLACK_PIECE) {
    which_player = "Black's Turn";
  } 
  else {
    which_player = "White's Turn";
  }
  if (GAME_OVER) {
    which_player = "Game Over";
  }
  Tft.drawString(which_player, SQUARE_SIZE, SQUARE_SIZE * 8 + half, 2, BLACK);

  byte white_score = 0;
  byte black_score = 0;
  get_score(&black_score, &white_score, board);
  String game_score = String(white_score) + ":" + String(black_score);
  char output[10];
  game_score.toCharArray(output, game_score.length() + 1);
  Tft.drawString(output, SQUARE_SIZE * 2, SQUARE_SIZE * 9 + half, 2, BLACK);
}

void update_board(byte board[SIZE][SIZE]) {
  byte half = SQUARE_SIZE / 2;
  for (byte x = 0; x < SIZE; x++) {
    for (byte y= 0; y < SIZE; y++) {
      word pos_x = x * SQUARE_SIZE + half;
      word pos_y = y * SQUARE_SIZE + half;
      if (board[x][y] == BLACK_PIECE) {
        Tft.fillCircle(pos_x, pos_y, half - 2, BLACK);
      } 
      else if (board[x][y] == WHITE_PIECE) {
        Tft.fillCircle(pos_x, pos_y, half - 2, WHITE);
        Tft.drawCircle(pos_x, pos_y, half - 2, BLACK);
      } 
      else if (board[x][y] == ERROR_PIECE) {
        Tft.fillCircle(pos_x, pos_y, half - 2, RED);
        board[x][y] = BLANK;
      } 
      else {
        //Tft.fillRectangle(pos_x - half + 1, pos_y - half + 1, SQUARE_SIZE - 1, SQUARE_SIZE- 1, GREEN);
        Tft.fillCircle(pos_x, pos_y, half - 2, GREEN);
      }
    }
  }
}

void flash_error(byte board[SIZE][SIZE], byte x, byte y) {
  byte prev = board[x][y];
  board[x][y]=ERROR_PIECE;
  update_board(board);
  delay(100);
  board[x][y]=prev;
  update_board(board);
}

bool pressed(byte turn, byte board[SIZE][SIZE], word pos_x, word pos_y) {
  byte x = pos_x / SQUARE_SIZE;
  byte y = pos_y / SQUARE_SIZE;
  if (flip(board, turn, x, y, true) == 0) {
    flash_error(board,x,y);
    return false;
  } 
  else {
    board[x][y] = turn;
    return true;
  }
}

void switch_turn(byte* turn) {
  if (*turn == BLACK_PIECE) {
    *turn = WHITE_PIECE;
  } 
  else {
    *turn = BLACK_PIECE;
  }
}

bool is_in_bounds(short x, short y) {
  if (x < 0) {
    return false;
  } 
  else if (x >= SIZE) {
    return false;
  } 
  else if (y < 0) {
    return false;
  } 
  else if (y >= SIZE) {
    return false;
  }
  return true;
}

unsigned char flip(byte board[SIZE][SIZE], byte turn, byte x, byte y, bool do_it) {
  if (!is_in_bounds(x,y)) {
    return 0;
  }
  if (board[x][y] != BLANK) {
    return 0;
  }
  unsigned char result = 0;
  for (byte i = 0; i < 8; i++) {
    result += flip_dir(board,turn,x,y,DIRS[i][0],DIRS[i][1], do_it);
  }
  return result;
}

unsigned char flip_dir(byte board[SIZE][SIZE], byte turn, short x, short y, short dir_x, short dir_y, bool do_it) {
  return flip_opp(board, turn, x + dir_x, y + dir_y, dir_x, dir_y, 0, do_it);
}

unsigned char flip_opp(byte board[SIZE][SIZE], byte turn, short x, short y, short dir_x, short dir_y, unsigned char to_flip, bool do_it) {
  if (!is_in_bounds(x,y)) {
    return 0;
  }
  byte value = board[x][y];
  if (value == turn) {
    return to_flip;
  } 
  else if (value == BLANK) {
    return 0;
  } 
  else {
    unsigned char thus_far = flip_opp(board, turn, x + dir_x, y + dir_y, dir_x, dir_y, to_flip + 1, do_it);
    if (do_it && thus_far > 0) {
      board[x][y] = turn;
    }
    return thus_far;
  }
}

bool has_any_moves(byte board[SIZE][SIZE], byte turn) {
  for (byte x=0; x < SIZE; x++) {
    for (byte y=0; y < SIZE; y++) {
      if (flip(board,turn,x,y,false) > 0) {
        return true;
      }
    }
  }
  return false;
}

void start_game() {
  CURRENT_PLAYER = WHITE_PIECE;
  GAME_OVER = false;
  init_board(REVERSI);
  draw_board(REVERSI);
  update_board(REVERSI);
  update_status(CURRENT_PLAYER, REVERSI);
}

void setup() {
  Serial.begin(9600);
  Tft.init();
  start_game();
}

void loop() {
  Point p = TS.getPoint();
  p.x = map(p.x, TS_MINX, TS_MAXX, MAX_X, 0);
  p.y = map(p.y, TS_MINY, TS_MAXY, MAX_Y, 0);
  if (p.z > TS.pressureThreshhold) {
    if (GAME_OVER) {
      start_game();
      return;
    }
    if (pressed(CURRENT_PLAYER, REVERSI, p.x,p.y)) {
      switch_turn(&CURRENT_PLAYER);
      if (!has_any_moves(REVERSI,CURRENT_PLAYER)) {
        switch_turn(&CURRENT_PLAYER);
        if (!has_any_moves(REVERSI,CURRENT_PLAYER)) {
          GAME_OVER = true;
        }
      }
    }
    update_board(REVERSI);
    update_status(CURRENT_PLAYER, REVERSI);
  }

}

