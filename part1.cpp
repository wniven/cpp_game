/***************************************************
Filename: part1.cpp
Author: CDT William Niven (259225)
Reads in user-given "board" files and prints them on ncurses (Project 3)
***************************************************/

#include <iostream>
#include <fstream>
#include <unistd.h>
#include "easycurses.h"
#include "board.h"
#include "Pos.h"
using namespace std;



int main(){

    //ask user for filename of desired board
    string filename;
    cout << "board file: ";
    cin >> filename;
    ifstream fin(filename);

    //give error and exit if fnf
    if(!fin){
        cout << "File not found!" << endl;
        return 1;
    }

    //read board from file and open ncurses
    board myBoard;
    readBoard(fin, myBoard);
    startCurses();

    //read in size of terminal window
    int rowmax, colmax;
    getWindowDimensions(rowmax, colmax);
    //if window is not big enough for terminal window
    if(myBoard.colnum > colmax || myBoard.rownum > rowmax){
        //take down ncurses, print error message, and exit program
        endCurses();
        cout << "Given board is too large for this window!" << endl;
        return 1;
    }

    //otherwise, print the board onto ncurses
    curseBoard(myBoard);
    refreshWindow();

    //diplay until user presses 'y'
    char c;
    do{
        usleep(150000);
        c = inputChar();
    } while(c != 'y');
    endCurses();

    //print location info
    spotPrint(myBoard);

    deleteBoard(myBoard);

    return 0;

}


