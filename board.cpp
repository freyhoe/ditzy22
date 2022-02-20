#include "board.h"

typedef uint64_t bitboard;

bool Board::cell_occupied(int8_t x, int8_t y){
  if(x<0 || x>= WIDTH || y<0){
    return true;
  }
  else if(y>=HEIGHT){
    return false;
  }
  return ((matrix[y] >> x) & 1) !=0;
}

void Board::set_cell(int8_t x, int8_t y){
  matrix[y] |= (1ULL<<x);
  if(y+1>heights[x]){
    heights[x]=y+1;
  }
}

void Board::print_pretty(){
  std::cout << '\n';
  for(int j = 0; j < WIDTH*2-1; j++){
    std::cout<<"_";
  }
  for(int i = HEIGHT-1; i >=0; i--){
    std::cout << '\n';
    for(int j = 0; j < WIDTH; j++){
      if(cell_occupied(j,i)){
        std::cout<<"X ";
      }
      else{
        std::cout<<"_ ";
      }
    }
  }
  std::cout << '\n';
  for(int j = 0; j < WIDTH*2-1; j++){
    std::cout<<"^";
  }
}
void Board::place_piece(Move piece){
  for(int i = 0; i < 4; i++){
    set_cell(PIECE_SHAPES[piece.type][piece.r][i][0]+piece.x,PIECE_SHAPES[piece.type][piece.r][i][1]+piece.y);
  }
}
int8_t Board::get_piece_droffset(Move piece){
  int8_t droffset = HEIGHT;
  for(int i = 0; i < 4; i++){
    int8_t x = PIECE_SHAPES[piece.type][piece.r][i][0] + piece.x;
    int8_t y = PIECE_SHAPES[piece.type][piece.r][i][1] + piece.y;
    if(y-heights[x]<droffset){
      droffset = y-heights[x];
    }
  }
  return droffset;
}
bool Board::piece_intersects(Move piece){
  for(int i = 0; i < 4; i++){
    if(cell_occupied(PIECE_SHAPES[piece.type][piece.r][i][0]+piece.x, PIECE_SHAPES[piece.type][piece.r][i][1]+piece.y)){
      return true;
    }
  }
  return false;
}

void Board::load_matrix(std::string *bb){
  for(int i = 0; i < HEIGHT; i++){
    matrix[i]=0;
  }
  for(int i = 0; i < WIDTH; i++){
    heights[i]= 0;
  }
  for(int i = HEIGHT-1; i >=0; i--){
    for(int j = 0; j < WIDTH; j++){
      if(bb[i][j]=='X'){
        set_cell(j,HEIGHT-i-1);
      }
    }
  }
}

Move Board::simulate_rotation(Move move, int8_t rotation){
  int8_t nRot = move.r + rotation+1;
  if(nRot >= 4){
    nRot-=4;
  }
  int8_t iK = (rotation == FLIP) ? 2 : 5;
  int8_t kickI = (move.type==PIECE_I) ? 0 : 1;
  for(int i = 0; i < iK; i++){
    Move nMove = move;
    int8_t dx = KICK_TABLE[kickI][move.r][rotation][i][0];
    int8_t dy = KICK_TABLE[kickI][move.r][rotation][i][1];
    nMove.x+=dx;
    nMove.y+=dy;
    nMove.r = nRot;
    if(!piece_intersects(nMove)){
      return nMove;
    }
  }
  return Move(PIECE_NULL);
}
bool Board::piece_skylit(Move piece){
  for(int i = 0; i < 4; i++){
    if(heights[PIECE_SHAPES[piece.type][piece.r][i][0]+piece.x] > PIECE_SHAPES[piece.type][piece.r][i][1]+piece.y){
      return false;
    }
  }
  return true;
}
Move Board::simulate_action(Move move, int8_t action){
  Move nMove = move;
  switch (action) {
    case LEFT:{
      nMove.x-=1;
      if(piece_intersects(nMove)){nMove.type = PIECE_NULL;}
      break;
    }
    case RIGHT:{
      nMove.x+=1;
      if(piece_intersects(nMove)){nMove.type = PIECE_NULL;}
      break;
    }
    case SD:{
      int8_t droffset = get_piece_droffset(move);
      if(droffset == 0){nMove.type = PIECE_NULL;}
      else{nMove.y-=droffset;}
      break;
    }
    case CW:
    case CCW:
    case FLIP:{
      nMove = simulate_rotation(move,action);
      break;
    }
  }
  return nMove;
}
std::unordered_set<Move, MoveHash> Board::get_moves(int8_t type){
  std::vector<int8_t> pushActions = {LEFT, RIGHT, CW, CCW, FLIP};
  if(type==PIECE_O){
    pushActions = {LEFT, RIGHT};
  }
  std::vector<Move> hds;
  switch (type) {
    case PIECE_O:{
      hds = {Move(type,0,NORTH),Move(type,1,NORTH),Move(type,2,NORTH),Move(type,3,NORTH),Move(type,4,NORTH),Move(type,5,NORTH),Move(type,6,NORTH),Move(type,7,NORTH),Move(type,8,NORTH)};
      break;
    }
    case PIECE_I:{
      hds = {
        Move(type,1,NORTH),Move(type,2,NORTH),Move(type,3,NORTH),Move(type,4,NORTH),Move(type,5,NORTH),Move(type,6,NORTH),Move(type,7,NORTH),
        Move(type,0,EAST),Move(type,1,EAST),Move(type,2,EAST),Move(type,3,EAST),Move(type,4,EAST),Move(type,5,EAST),Move(type,6,EAST),Move(type,7,EAST),Move(type,8,EAST),Move(type,9,EAST),
        Move(type,2,SOUTH),Move(type,3,SOUTH),Move(type,4,SOUTH),Move(type,5,SOUTH),Move(type,6,SOUTH),Move(type,7,SOUTH),Move(type,8,SOUTH),
        Move(type,0,WEST),Move(type,1,WEST),Move(type,2,WEST),Move(type,3,WEST),Move(type,4,WEST),Move(type,5,WEST),Move(type,6,WEST),Move(type,7,WEST),Move(type,8,WEST),Move(type,9,WEST),
      };
      break;
    }
    default:{
      hds = {
        Move(type,1,NORTH),Move(type,2,NORTH),Move(type,3,NORTH),Move(type,4,NORTH),Move(type,5,NORTH),Move(type,6,NORTH),Move(type,7,NORTH),Move(type,8,NORTH),
        Move(type,0,EAST),Move(type,1,EAST),Move(type,2,EAST),Move(type,3,EAST),Move(type,4,EAST),Move(type,5,EAST),Move(type,6,EAST),Move(type,7,EAST),Move(type,8,EAST),
        Move(type,1,SOUTH),Move(type,2,SOUTH),Move(type,3,SOUTH),Move(type,4,SOUTH),Move(type,5,SOUTH),Move(type,6,SOUTH),Move(type,7,SOUTH),Move(type,8,SOUTH),
        Move(type,1,WEST),Move(type,2,WEST),Move(type,3,WEST),Move(type,4,WEST),Move(type,5,WEST),Move(type,6,WEST),Move(type,7,WEST),Move(type,8,WEST),Move(type,9,WEST),
      };
      break;
    }
  }
  std::vector<Move> stack;
  std::unordered_set<Move, MoveHash>visited;
  for(const auto& i : hds){
    Move newM = simulate_action(i,SD);
    stack.push_back(newM);
    visited.insert(newM);
  }

/*  while(!stack.empty()){
    Move move = stack.back();
    stack.pop_back();
    for(int8_t action : pushActions){
      Move newM = simulate_action(move, action);
      if(newM.type!=PIECE_NULL && !piece_skylit(newM) && visited.insert(newM).second){
        stack.push_back(newM);
      }
    }
  }*/
  return visited;
}

/**
*
*/
