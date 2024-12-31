/*************************************************************************
Pos.h

This file contains functions and structs that are useful
when dealing with grids.  Two important notes about representing data:
DIRECTION  : N = 0, E = 1, S = 2, W = 3
COORDINATES: we use row,column coords, not x,y                 CWB 11/2016
**************************************************************************/

#pragma once
//-- Input : dir, a direction (i.e. 0,1,2,3 as described above)
//--         ang, must be one of -270, -180, -90, 0, 90, 180, 270 
//-- Output: new direction that results from turning ang degrees clockwise
//--         Example: turn(1,180) returns 3  (i.e. E goes to W)
int turn(int dir, int ang);

//-- For representing a position on a grid.  Note: row,col format!
struct Pos {
  int row, col;
};

//-- Input: Pos's p1 and p2, Output: true if p1 equals p2, false otherwise
bool equal(Pos p1, Pos p2);

//-- Input : Pos p and direction dir (as described above)
//-- Output: Pos q resulting from stepping one unit from p in direction dir
//--         Example step(Pos{row:6,col:2},2) gives Pos{row:7,col:2} (step south)
Pos step(Pos p, int dir);

//-- Input : Pos p and Pos q
//-- Output: int d giving the "Manhattan distance" from p to q, that is
//--         the minimum number of grid steps to get from p to q.
int dist(Pos p, Pos q);

