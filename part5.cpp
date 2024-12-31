/***************************************************
Filename: part5.cpp
Author: CDT William Niven (259225)
Builds on part4 with multiple boards and progression (Project 3)
***************************************************/

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstdlib>
#include "easycurses.h"
#include "board.h"
#include "Pos.h"
#include "ll.h"
using namespace std;



int main(){

    //read in game script
    ifstream fbig("gameScript.txt");
    //initialize list with each board
    Node* gamelist = NULL;

    //declare necessary variables for gamescript
    string gs_fh, junk;
    int gs_sn, gs_hn, gs_p, finalscore = 0;
    bool gamewinner = true;

    //read through game script, add each line to linked list
    while(fbig >> gs_fh >> gs_sn >> gs_hn >> junk >> junk >> gs_p){
        gamelist = add2back(gs_fh, gs_sn, gs_hn, gs_p, gamelist);
    }

    //stackoverflow.com/questions/16569239/how-to-use-function-srand-with-time-h
    srand(time(NULL));

    //loop through game script linked list
    for(Node* temp = gamelist; temp != NULL; temp = temp->next){
        
        //read-in this gameboard
        cout << "board file: " << temp->filename << " (" << temp->shipnum << "," << temp->huntnum << "," << temp->points << ")" << endl;

        //declare lives and win for this level
        int lives = 3;
        bool winner = false;

        //keep playing this board until win round or run out of lives
        while(lives > 0 && winner == false){
            
            //read board from file and open ncurses
            ifstream fin(temp->filename);
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
            upPos* ships = makeBaddies(myBoard, temp->shipnum);
            int totalHunters = myBoard.znum * temp->huntnum;
            //same for hunters
            upPos* hunters = makeHunters(myBoard, temp->huntnum);
            int totalBaddies = myBoard.znum * temp->shipnum;

            //declare necessary variables for game loop
            char c;
            int userdir = 5;
            int score = 500 + temp->points;
            bool going = false, invincible = false;
            winner = true;

            do{
                //initiate round setting
                usleep(150000);
                moveBaddies(myBoard, ships, totalBaddies);
                moveHunters(myBoard, hunters, user, totalHunters);
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
                    for(int i = 0; i < totalHunters; i++){
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

            //print location info for first round
            if(lives == 3)
                spotPrint(myBoard);
            //print round outcome
            if(winner){
                cout << "You won, round score = " << score << endl;
                finalscore += score;
            }
            else{
                lives--;
                cout << "You lost! " << lives << " lives left!" << endl;
            }
            sleep(2);


            //delete dynamic memory
            deleteBoard(myBoard);
            delete [] ships;
            delete [] hunters;
        }

        if(!winner){
            gamewinner = false;
            break;
        }

    }

    if(gamewinner){
        cout << "Wow, you won the entire game, you must be Neo! Final score was: " << finalscore << endl;
    }
    else{
        cout << "Out of lives for the round, final score was " << finalscore << endl;
    }

    deletelist(gamelist);

    return 0;

}


