/***************************************************
Filename: board.h
Author: CDT William Niven (259225)
Header file for functions relating to playing board (Project 3)
***************************************************/

#pragma once
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "easycurses.h"
#include "Pos.h"
using namespace std;

//simple struct that saves row and col location of an object
struct spawn{
    int row, col;
};

//struct used to store location and direction of an enemy
struct upPos{
    Pos coordinates;
    int direction;
};

//board struct contains size of board (rownum, colnum)
// all characters of the board in 2D array (layout)
// location of player spawn (prow, pcol)
// and the number (znum), location of enemy spawns (baddies)
struct board{
    int rownum, colnum, znum, prow, pcol, xrow, xcol;
    char** layout;
    spawn* baddies;
};

//function that reads board from istream
//"in" is the istream which the board is read from
//  passed by reference
void readBoard(istream& in, board& fileboard);

//function prints board struct onto ncurses
// curboard is the board to be printed
void curseBoard(board curboard);

//function prints locations of player spawn and enemy spawns
// curboard is the board to be printed
void spotPrint(board curboard);

//function to delete all attributes of the board
// curboard is board struct to be deleted
void deleteBoard(board& curboard);

//returns an array of spaceships using location/direction struct
// curboard is board struct where enemy spawn locations are stored
// shipnum is the number of ships per spawn
upPos* makeBaddies(board curboard, int shipnum);

//function that moves all baddies and prints to ncurses
// curboard is board struct with mapping for movement
// enemies is an array of spaceships with direction/location info
// total baddies just stores the number of spaceships
void moveBaddies(board curboard, upPos* enemies, int totalBaddies);

//returns an array of hunters using location/direction struct
// curboard is board struct where enemy spawn locations are stored
// very similar to makeBaddies expect looping is a bit different
// huntnum is the number of hunters per spawn
upPos* makeHunters(board curboard, int huntnum);

//function that moves all hunters and prints to ncurses
// curboard is board struct with mapping for movement
// enemies is an array of spaceships with direction/location info
// totalHunters just stores the number of hunters
void moveHunters(board curboard, upPos* enemies, Pos user, int totalHunters);