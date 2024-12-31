/***************************************************
Filename: part4.cpp
Author: CDT William Niven (259225)
Builds on part3 with inclusion of hunters (Project 3)
***************************************************/

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstdlib>
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

    //stackoverflow.com/questions/16569239/how-to-use-function-srand-with-time-h
    srand(time(NULL));

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

    //create array of baddies with location and direction in upPos
    upPos* ships = makeBaddies(myBoard, 5);
    //same for hunters
    upPos* hunters = makeHunters(myBoard, 1);
    int totalBaddies = myBoard.znum * 5;

    //declare necessary variables for game loop
    char c;
    int userdir = 5;
    int score = 500;
    bool going = false, invincible = false, winner = true;

    do{
        //initiate round setting
        usleep(150000);
        moveBaddies(myBoard, ships, totalBaddies);
        moveHunters(myBoard, hunters, user, myBoard.znum);
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
        if(c == 'i')
            invincible = true;

        if(going)
            user = step(user, userdir);
        
        //check for bouncing
        if(myBoard.layout[user.row][user.col] == '#'){
            userdir = (userdir+2)%4;
            user = step(user, userdir);
            user = step(user, userdir);

            if(myBoard.layout[user.row][user.col] == '#'){
                userdir = (userdir+2)%4;
                user = step(user, userdir);
            }
        }

        //check for collision if not invincible
        if(!invincible){
            //loop through all ships
            for(int i = 0; i < totalBaddies; i++){
                //check collision with this ship
                if(equal(ships[i].coordinates, user) || (equal(step(user, (userdir+2)%4), ships[i].coordinates) && equal(user, step(ships[i].coordinates, (ships[i].direction+2)%4))))
                    winner = false;
            }

            //loop through all hunters
            for(int i = 0; i < myBoard.znum; i++){
                //check collision with this ship
                if(equal(hunters[i].coordinates, user) || (equal(step(user, (userdir+2)%4), hunters[i].coordinates) && equal(user, step(hunters[i].coordinates, (hunters[i].direction+2)%4))))
                    winner = false;
            }
        }
        

        //draw new player position, adjust score, and refresh
        drawChar('P', user.row, user.col);
        score--;
        refreshWindow();

        //exit loop if collision
        if(winner == false){
            usleep(2000000);
            break;
        }

    } while(dist(user,goal) > 1);
    endCurses(); //end game when user is at 'X'

    //print location info
    spotPrint(myBoard);
    //print game outcome
    if(winner)
        cout << "You won, high score = " << score << endl;
    else
        cout << "You lost, they got you!" << endl;


    //delete dynamic memory
    deleteBoard(myBoard);
    delete [] ships;
    delete [] hunters;

    return 0;

}


