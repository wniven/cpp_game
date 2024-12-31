#include "Pos.h"
#include <cstdlib>

// CWB 11/2016

bool equal(Pos p1, Pos p2) { 
  return p1.row == p2.row && p1.col == p2.col; 
}

Pos step(Pos p, int dir) {
  Pos next = p;
  if (dir == 0) next.row--; // North
  if (dir == 2) next.row++; // South
  if (dir == 1) next.col++; // East
  if (dir == 3) next.col--; // West
  return next;
}

int turn(int dir, int ang) {
  return (dir + (4 + ang/90)) % 4;
}

int dist(Pos p, Pos q) {
  return abs(p.row - q.row) + abs(p.col - q.col);
}
