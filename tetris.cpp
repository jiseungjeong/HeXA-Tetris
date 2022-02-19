#include <cstdlib> //rand%7
#include <iostream>
#include <string>

class Map {
  int row_size;
  int col_size;
  int **map;

public:
  Map(const int row=18, const int col=10) : row_size(row), col_size(col) {
    map = new int *[row];
    for (int i = 0; i < row; i++) {
      map[i] = new int[col];
    }
  }
  void SetMapZero();
  void MapPrint();
};

void Map::SetMapZero() {
  for (int i = 0; i < row_size; i++) {
    for (int j = 0; j < col_size; j++) {
      map[i][j] = 0;
    }
  }
}

void Map::MapPrint() {
  for (int i = 0; i < row_size; i++) {
    std::cout << "┃";
    for (int j = 0; j < col_size; j++) {
      if (map[i][j] == 0)
        std::cout << "　";
      else
        std::cout << "▩";
    }
    std::cout << "┃" << std::endl;
  }
  for (int i = -2; i < col_size; i++) {
    std::cout << "￣";
  }
  std::cout << std::endl;
}

class Block {
  int block_row_size;
  int block_col_size;
  int rotate_state; // 0 to 3;
  int which_block_shape; // 0 to 6
  int **block;

public:
  Block(int row = 4, int col = 4)
      : block_row_size(row), block_col_size(col), rotate_state(0) {
    block = new int *[block_row_size];
    for (int i = 0; i < block_row_size; i++) {
      block[i] = new int[block_col_size];
    }
  }
  void block_initailize_to_zero();
  void block_rotate(const int n);
  void print_block();
};

void Block::block_initailize_to_zero() {
    for(int i=0; i<block_row_size;i++) {
        for(int j=0; j<block_col_size;j++) {
            block[i][j]=0;
        }
    }
}

void Block::block_rotate(const int n) {
  int temp[n][n];
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
        temp[i][j]=block[n-1-j][i];
    }
  }
  for (int i=0; i<n; i++) {
      for(int j=0;j<n; j++) {
          block[i][j]=temp[i][j];
      }
  }
}

class Block0 : public Block {
  int block[4][4]= {{1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}};
};

class Block1 : public Block {
  int block[4][4] = {{1, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}};
};

class Block2 : public Block {
  int block[4][4] = {{0, 1, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}};
};

class Block3 : public Block {
  int block[4][4] = {{1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
};

class Block4 : public Block {
  int block[4][4] = {{0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}};
};

class Block5 : public Block {
  int block[4][4] = {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
};

class Block6 : public Block {
  int block[4][4] = {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
};

int main() {
  Map main_map(18, 10);
  main_map.SetMapZero();
  Block main_block();
  Block0 block0;
  main_map.MapPrint();
}