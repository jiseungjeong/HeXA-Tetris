#include <cstdlib> // rand()
#include <fcntl.h> // for kbhit()
#include <iostream>
#include <stdio.h>
#include <termios.h> // for kbhit()
#include <unistd.h>  // for kbhit() and for usleep()

#define MAP_ROW_SIZE 18
#define MAP_COL_SIZE 10
#define UP 119    // w
#define DOWN 115  // s
#define LEFT 97   // a
#define RIGHT 100 // d
#define I_BLOCK 0
#define L_BLOCK 1
#define T_BLOCK 2
#define SQUARE_BLOCK 3
#define J_BLOCK 4
#define S_BLOCK 5
#define Z_BLOCK 6

int kbhit(void) {
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if (ch != EOF) {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}
// 출처: https://cboard.cprogramming.com/c-programming/63166-kbhit-linux.html
// kbhit(), 키보드 입력 확인 함수 : 입력되었으면 1을 리턴, 입력이 안 되었으면
// 0을 리턴.

void gotoxy(int x, int y) {
  printf("\033[%d;%df", y, x);
  fflush(stdout);
}

class Block {
protected:
  int rotate_state; // 0 to 3
  int block[4][4];
  int block_x;
  int block_y;

public:
  Block() : rotate_state(0), block_x((MAP_COL_SIZE + 2) / 2 - 2), block_y(1) {}
  void setBlockZero();
  void blockRotate();
  int getBlockX() { return block_x; }
  int getBlockY() { return block_y; }
  void down() { block_y++; }
  void left() { block_x--; }
  void right() { block_x++; }
  void setBlockX(int x) { this->block_x = x; }
  void setBlockY(int y) { this->block_y = y; }
  void setRotate(int n) { this->rotate_state = n; }
  int getBlockRotateState() { return rotate_state; }
  int getBlockShape(int y, int x) { return block[y][x]; }
};

class IBlock : public Block {
  const int i_block[4][4] = {
      {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}};

public:
  IBlock() : Block() {

    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        block[i][j] = i_block[i][j];
      }
    }
  }
};
/*
▩　　　
▩　　　
▩　　　
▩　　　
*/

class LBlock : public Block {
  const int l_block[4][4] = {
      {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}};

public:
  LBlock() : Block() {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        block[i][j] = l_block[i][j];
      }
    }
  }
};
/*
▩　　　
▩　　　
▩▩　　
　　　　
*/

class TBlock : public Block {
  const int t_block[4][4] = {
      {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}};

public:
  TBlock() : Block() {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        block[i][j] = t_block[i][j];
      }
    }
  }
};
/*
　▩　　
▩▩　　
　▩　　
　　　　
*/

class SquareBlock : public Block {
  const int square_block[4][4] = {
      {1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

public:
  SquareBlock() : Block() {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        block[i][j] = square_block[i][j];
      }
    }
  }
};
/*
▩▩　　
▩▩　　
　　　　
　　　　
*/

class JBlock : public Block {
  const int j_block[4][4] = {
      {0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}};

public:
  JBlock() : Block() {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        block[i][j] = j_block[i][j];
      }
    }
  }
};
/*
　▩　　
　▩　　
▩▩　　
　　　　
*/

class SBlock : public Block {
  const int s_block[4][4] = {
      {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

public:
  SBlock() : Block() {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        block[i][j] = s_block[i][j];
      }
    }
  }
};
/*
　▩▩　
▩▩　　
　　　　
　　　　
*/

class ZBlock : public Block {
  const int z_block[4][4] = {
      {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

public:
  ZBlock() : Block() {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        block[i][j] = z_block[i][j];
      }
    }
  }
};
/*
▩▩　　
　▩▩　
　　　　
　　　　
*/

class Map {
  int col_size;
  int row_size;
  int point;
  Block *cur_block;
  int **map;
  int rand_block_num;

public:
  Map(const int row, const int col)
      : row_size(row), col_size(col), cur_block(nullptr), point(0) {
    map = new int *[row + 1];
    for (int i = 0; i < row + 1; i++) {
      map[i] = new int[col + 2];
    }
  }
  void setMapZero();
  void setNewBlockInMap();
  void setBlockInMap();
  bool detectCrash(int x, int y);
  bool detectRotateCrash();
  void moveBlock();
  void mapPrint();
  void setRandBlockNum() {
    srand((unsigned int)time(NULL));
    rand_block_num = rand() % 7;
  }
  void fallingBlockInMap();
  void eraseBlockInMap();
  void solidfyBlockInMap();
  void checkLineAndDelete();
  void printPoint() {
    gotoxy(0, 2);
    std::cout << "Point :: " << point << std::endl;
  }
  void lowerMap(int n);
  bool checkGameOver();
};

bool Map::checkGameOver() {
    for(int i=1;i<MAP_COL_SIZE+1;i++) {
        if(map[2][i]!=0) {
            return true;
        }
    }
    return false;
}


//여기 오류 수정 해야함 4줄이 한꺼번에 쌓이면 2줄만 사라지고 2줄 남음
void Map::lowerMap(int n) { // n번째 줄 위의 블럭들 내리기
  for (int i = n; i >= 1; i--) {
    for (int j = 1; j < MAP_COL_SIZE + 1; j++) {
      map[i][j] = map[i - 1][j];
      map[i - 1][j] = 0;
    }
  }
  for (int i = 1; i < MAP_COL_SIZE + 1; i++) {
    map[0][i] = 0;
    map[MAP_ROW_SIZE][i] = 2;
  }
}

void Map::checkLineAndDelete() {
  for (int i = MAP_ROW_SIZE - 1; i >= 0; i--) {
    int cnt = 0;
    for (int j = 1; j < MAP_COL_SIZE + 1; j++) {
      if (map[i][j] == 2)
        cnt++;
      else
        break;
    }
    if (cnt == MAP_COL_SIZE) { // 한 줄이 꽉 차있을 때
      point += 100;
      printPoint();
      for (int k = 1; k < MAP_COL_SIZE + 1; k++) {
        map[i][k] = 0;
      }
      lowerMap(i);
    }
  }
  mapPrint();
}

void Map::solidfyBlockInMap() {
  for (int i = 0; i < MAP_ROW_SIZE; i++) {
    for (int j = 0; j < MAP_COL_SIZE + 1; j++) {
      if (map[i][j] == 1) {
        map[i][j] = 2;
      }
    }
  }
}

void Map::eraseBlockInMap() {
  for (int i = 0; i < MAP_ROW_SIZE; i++) {
    for (int j = 0; j < MAP_COL_SIZE + 1; j++) {
      if (map[i][j] == 1)
        map[i][j] = 0;
    }
  }
}

void Map::fallingBlockInMap() {
  cur_block->setBlockY(cur_block->getBlockY() + 1);
  usleep(200000); // microseconds
}

bool Map::detectCrash(int x, int y) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (cur_block->getBlockShape(i, j) == 1) {
        int future_block =
            map[i + cur_block->getBlockY() + y][j + cur_block->getBlockX() + x];
        if (future_block == 2 || future_block == 3) {
          return true;
        }
      }
    }
  }
  return false;
}

bool Map::detectRotateCrash() {
  int temp[4][4];
  int detect_arr[4][4];
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      temp[i][j] = cur_block->getBlockShape(i, j);
    }
  }
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      detect_arr[i][j] = temp[3 - j][i];
    }
  }
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (detect_arr[i][j] == 1) {
        int future_block =
            map[i + cur_block->getBlockY()][j + cur_block->getBlockX()];
        if (future_block == 2 || future_block == 3) {
          return true;
        }
      }
    }
  }
  return false;
}

void Map::setMapZero() {
  for (int i = 0; i < row_size + 1; i++) {
    for (int j = 0; j < col_size + 2; j++) {
      if (j == 0 || j == col_size + 1)
        map[i][j] = 3; // 벽면일 때는 3
      else if (i == row_size)
        map[i][j] = 2; // 바닥일 때는 2
      else
        map[i][j] = 0; // 벽면도 아니고 바닥도 아닐 때는 0, 대상 블록은 1
    }
  }
}

void Map::setBlockInMap() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      int y = i + cur_block->getBlockY();
      int x = j + cur_block->getBlockX();
      if (cur_block->getBlockShape(i, j) == 1 && map[y][x] == 0)
        map[y][x] = cur_block->getBlockShape(i, j);
    }
  }
}

void Map::setNewBlockInMap() {
  switch (rand_block_num) { 
  case I_BLOCK:
    cur_block = new IBlock();
    break;
  case L_BLOCK:
    cur_block = new LBlock();
    break;
  case T_BLOCK:
    cur_block = new TBlock();
    break;
  case SQUARE_BLOCK:
    cur_block = new SquareBlock();
    break;
  case J_BLOCK:
    cur_block = new JBlock();
    break;
  case S_BLOCK:
    cur_block = new SBlock();
    break;
  case Z_BLOCK:
    cur_block = new ZBlock();
    break;
  }
  setBlockInMap();
}

void Map::moveBlock() {
  int input_ascii_num;
  if (kbhit()) {
    input_ascii_num = getchar();
    switch (input_ascii_num) {
    case UP:
      if (detectRotateCrash())
        return;
      cur_block->setRotate(cur_block->getBlockRotateState() + 1);
      if (cur_block->getBlockRotateState() > 3)
        cur_block->setRotate(cur_block->getBlockRotateState() - 4);
      cur_block->blockRotate();
      break;
    case DOWN:
      if (!detectCrash(0, 2))
        cur_block->down();
      break;
    case LEFT:
      if (!detectCrash(-1, 0))
        cur_block->left();
      break;
    case RIGHT:
      if (!detectCrash(1, 0))
        cur_block->right();
      break;
    default:
      break;
    }
  }
  mapPrint();
}

void Map::mapPrint() {
  gotoxy(0, 6);
  for (int i = 0; i < row_size + 1; i++) {
    for (int j = 0; j < col_size + 2; j++) {
      if (map[i][j] == 0)
        std::cout << "　";
      else if (map[i][j] == 1)
        std::cout << "▥";
      else
        std::cout << "▩";
    }
    std::cout << "　　" << std::endl;
  }
  std::cout << std::endl;
}

void Block::setBlockZero() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      block[i][j] = 0;
    }
  }
}

void Block::blockRotate() {
  int temp[4][4];
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      temp[i][j] = block[3 - j][i];
    }
  }
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      block[i][j] = temp[i][j];
    }
  }
}

int main() {
  Map main_map(MAP_ROW_SIZE, MAP_COL_SIZE);
  main_map.setMapZero();
  main_map.setRandBlockNum();
  main_map.setNewBlockInMap();
  system("clear");
  std::cout << "Tetris by Jiseung Jeong" << std::endl;
  main_map.printPoint();
  main_map.mapPrint();
  while (true) {
    if (main_map.detectCrash(0, 1)) {
      main_map.solidfyBlockInMap();
      main_map.checkLineAndDelete();
      if(main_map.checkGameOver()) break;
      main_map.setRandBlockNum();
      main_map.setNewBlockInMap();
      main_map.mapPrint();
    }
    main_map.eraseBlockInMap();
    main_map.fallingBlockInMap();
    main_map.setBlockInMap();
    main_map.mapPrint();
    main_map.moveBlock();
  }
  gotoxy(0,3);
  std::cout<<"Game Over!!"<<std::endl;
  return 0;
}