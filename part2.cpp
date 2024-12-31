/***************************************************
Filename: part2.cpp
Author: CDT William Niven (259225)
Builds on part1 with inclusion of movable player (Project 3)
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

    //create user and goal positions, draw player and refresh
    Pos user{row:myBoard.prow,col:myBoard.pcol};
    Pos goal{row:myBoard.xrow,col:myBoard.xcol};
    drawChar('P',user.row,user.col);
    refreshWindow();

    //declare necessary variables for game loop
    char c;
    int userdir = 5;
    int score = 500;
    bool going = false;

    do{
        //initiate round setting
        usleep(150000);
        drawChar(' ', user.row, user.col);

        //gather user input and move player accordingly
        c = inputChar();
        if(c == 'w'){
            userdir = 0;
            going = true;
        }
        if(c == 'a'){
            userdir = 3;
            going = true;
        }
        if(c == 's'){
            userdir = 2;
            going = true;
        }
        if(c == 'd'){
            userdir = 1;
            going = true;
        }
        if(c == 'r')
            going = false;

        if(going)
            user = step(user, userdir);
        
        //check for bouncing
        if(myBoard.layout[user.row][user.col] == '#'){
            if(userdir > 1)
                userdir -= 2;
            else
                userdir += 2;
            user = step(user, userdir);
            user = step(user, userdir);

            if(myBoard.layout[user.row][user.col] == '#'){
                if(userdir > 1)
                    userdir -= 2;
                else
                    userdir += 2;
                user = step(user, userdir);
            }
        }

        //draw new player position, adjust score, and refresh
        drawChar('P', user.row, user.col);
        score--;
        refreshWindow();

    } while(dist(user,goal) > 1);
    endCurses(); //end game when user is at 'X'

    //print location info
    spotPrint(myBoard);
    cout << "You won, high score = " << score << endl;
    deleteBoard(myBoard);

    return 0;

}


