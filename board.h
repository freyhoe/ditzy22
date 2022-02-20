#pragma once
#include <unordered_set>
#include <tuple>
#include "common.h"

struct Move{
  int8_t x = PIECE_SPAWN[0];
  int8_t y = PIECE_SPAWN[1];
  int8_t r = NORTH;
  int8_t type;
  Move(int8_t t){type = t;}
  Move(int8_t t, int8_t ex, int8_t arr){
    type = t;
    x = ex;
    r = arr;
  }
  bool operator==(const Move& otherMove) const{
    return x==otherMove.x && y == otherMove.y && r == otherMove.r;
  };
};

class MoveHash{
public:
  size_t operator()(const Move& move)const{
    size_t hsh = static_cast<int8_t>(move.type);
    hsh = (hsh * 10) + move.y;
    hsh = (hsh * 10) + move.x;
    hsh = (hsh * 4) + move.r;
    return hsh;
  }
};

class Board{
public:
  uint64_t matrix[HEIGHT];
  int8_t heights[WIDTH];
  void load_matrix(std::string *bb);
  bool cell_occupied(int8_t x, int8_t y);
  bool piece_intersects(Move piece);
  void print_pretty();
  void set_cell(int8_t x, int8_t y);
  void place_piece(Move piece);
  int8_t get_piece_droffset(Move piece);
  std::unordered_set<Move, MoveHash> get_moves(int8_t type);
  Move simulate_action(Move move, int8_t action);
  Move simulate_rotation(Move move, int8_t rotation);
  bool piece_skylit(Move move);
};
