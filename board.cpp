/***************************************************
Filename: board.cpp
Author: CDT William Niven (259225)
Definition file for functions relating to playing board (Project 3)
***************************************************/

//include header
#include "board.h"
#include <cstdlib>


void readBoard(istream& in, board& fileboard){
    
    //declare necessary variables
    char junk;
    int spawncount = 0;

    //read in size of board, number of enemy spawns
    in >> fileboard.rownum >> junk >> fileboard.colnum >> fileboard.znum;

    //create array of location structs for number of enemy spawns
    fileboard.baddies = new spawn[fileboard.znum];
    //set number of rows in 2D array
    fileboard.layout = new char*[fileboard.rownum];


    for(int i = 0; i < fileboard.rownum; i++){
        //grab newline character for each line
        junk = in.get();
        //fill in second layer of 2D array (columns) for each row
        fileboard.layout[i] = new char[fileboard.colnum];
        //now loop through individual locations
        for(int j = 0; j < fileboard.colnum; j++){
            //take in character at that location
            fileboard.layout[i][j] = in.get();
            //save location info if identified as player spawn
            if(fileboard.layout[i][j] == 'Y'){
                fileboard.pcol = j;
                fileboard.prow = i;
            }
            //save location info if identified as end point
            if(fileboard.layout[i][j] == 'X'){
                fileboard.xcol = j;
                fileboard.xrow = i;
            }
            //save location info if identified as enemy spawn
            else if(fileboard.layout[i][j] == 'Z'){
                fileboard.baddies[spawncount].row = i;
                fileboard.baddies[spawncount].col = j;
                spawncount++;
            }
        }
    }

    return;

}

void curseBoard(board curboard){

    //loop through each character in 2D array of the board
    for(int i = 0; i < curboard.rownum; i++){
        for(int j = 0; j < curboard.colnum; j++){
            //just print a space instead of character is player or enemy spawn
            if(curboard.layout[i][j] == 'Z' || curboard.layout[i][j] == 'Y'){
                drawChar(' ', i, j);
            }
            //otherwise, print the actual character
            else{
                drawChar(curboard.layout[i][j], i, j);
            }
        }
    }

    return;

}

void spotPrint(board curboard){

    //use player data in board struct to give player location
    cout << "Player start: (" << curboard.prow << "," << curboard.pcol << ")" << endl;
    cout << "Spawn spots:";
    //loop through each enemy spawn and pull,print location data
    for(int i = 0; i < curboard.znum; i++){
        cout << " (" << curboard.baddies[i].row << "," << curboard.baddies[i].col << ")"; 
    }
    cout << endl;

    return;

}

void deleteBoard(board& curboard){

    //delete 2D array of characters
    for(int i = 0; i < curboard.rownum; i++){
        delete [] curboard.layout[i];
    }
    delete [] curboard.layout;

    //delete array holding enemy spawns
    delete [] curboard.baddies;
    
    return;
    
}

upPos* makeBaddies(board curboard, int shipnum){

    //create array of five spaceships per location
    upPos* ships = new upPos[curboard.znum * shipnum];
    
    //loop through spawns
    for(int i = 0; i < curboard.znum; i++){
        //loop through 5 ships at each spawn
        for(int j = 0; j < shipnum; j++){
            //location of ship is pulled from curboard struct attributes
            ships[(i*shipnum) + j].coordinates.col = curboard.baddies[i].col;
            ships[(i*shipnum) + j].coordinates.row = curboard.baddies[i].row;
            //direction for ship is random
            ships[(i*shipnum) + j].direction = rand()%4;
        }
    }

    //return the array
    return ships;

}

void moveBaddies(board curboard, upPos* enemies, int totalBaddies){

    //loop through all the baddies
    for(int i = 0; i < totalBaddies; i++){

        //ensure you don't erase the goal!
        if(curboard.layout[enemies[i].coordinates.row][enemies[i].coordinates.col] != 'X')
            drawChar(' ', enemies[i].coordinates.row, enemies[i].coordinates.col);

        //1 in 10 chance of direction change
        if(rand()%10 == 0){
            //determine if left or right turn randomly
            if(rand()%2 == 1)
                enemies[i].direction += 1;
            else
                enemies[i].direction -= 1;

            //loop around if out of bounds
            if(enemies[i].direction == 4)
                enemies[i].direction = 0;
            if(enemies[i].direction == -1)
                enemies[i].direction = 3;
        }

        //move the ship!
        enemies[i].coordinates = step(enemies[i].coordinates, enemies[i].direction);

        //check for bouncing, same procedure as user bouncing
        if(curboard.layout[enemies[i].coordinates.row][enemies[i].coordinates.col] == '#'){
            enemies[i].direction =  (enemies[i].direction + 2)%4;
            enemies[i].coordinates = step(enemies[i].coordinates, enemies[i].direction);
            enemies[i].coordinates = step(enemies[i].coordinates, enemies[i].direction);

            if(curboard.layout[enemies[i].coordinates.row][enemies[i].coordinates.col] == '#'){
                enemies[i].direction =  (enemies[i].direction + 2)%4;
                enemies[i].coordinates = step(enemies[i].coordinates, enemies[i].direction);
            }
        }

        //again, don't erase the goal!
        if(curboard.layout[enemies[i].coordinates.row][enemies[i].coordinates.col] != 'X')
            drawChar('*', enemies[i].coordinates.row, enemies[i].coordinates.col);

    }


}

upPos* makeHunters(board curboard, int huntnum){

    //create array of five spaceships per location
    upPos* ships = new upPos[curboard.znum * huntnum];
    
    //loop through spawns
    for(int i = 0; i < curboard.znum; i++){
        //loop through hunters per spawn
        for(int j = 0; j < huntnum; j++){
            //location of ship is pulled from curboard struct attributes
            ships[(i*huntnum) + j].coordinates.col = curboard.baddies[i].col;
            ships[(i*huntnum) + j].coordinates.row = curboard.baddies[i].row;
            //direction for ship is random
            ships[(i*huntnum) + j].direction = rand()%4;
        }
    }

    //return the array
    return ships;

}

void moveHunters(board curboard, upPos* enemies, Pos user, int totalHunters){

    //loop through all the hunters
    for(int i = 0; i < totalHunters; i++){

        //ensure you don't erase the goal!
        if(curboard.layout[enemies[i].coordinates.row][enemies[i].coordinates.col] != 'X')
            drawChar(' ', enemies[i].coordinates.row, enemies[i].coordinates.col);

        
        //50% chance of reassesing direction
        if(rand()%2 == 1){
            //determine column and row distance from user for hunter
            int dc = user.col - enemies[i].coordinates.col;
            int dr = user.row - enemies[i].coordinates.row;
            int cdir, rdir;

            //determine row and column directions
            if(dc < 0)
                cdir = 3;
            else
                cdir = 1;
            if(dr < 0)
                rdir = 0;
            else
                rdir = 2;

            //coin flip to decide whether to assign rdir or cdir direction to hunter
            if(rand()%2 == 1)
                enemies[i].direction = cdir;
            else
                enemies[i].direction = rdir;

        }


        //move the hunter!
        enemies[i].coordinates = step(enemies[i].coordinates, enemies[i].direction);

        //check for bouncing, same procedure as user bouncing
        if(curboard.layout[enemies[i].coordinates.row][enemies[i].coordinates.col] == '#'){
            enemies[i].direction =  (enemies[i].direction + 2)%4;
            enemies[i].coordinates = step(enemies[i].coordinates, enemies[i].direction);
            enemies[i].coordinates = step(enemies[i].coordinates, enemies[i].direction);

            if(curboard.layout[enemies[i].coordinates.row][enemies[i].coordinates.col] == '#'){
                enemies[i].direction =  (enemies[i].direction + 2)%4;
                enemies[i].coordinates = step(enemies[i].coordinates, enemies[i].direction);
            }
        }

        //again, don't erase the goal!
        if(curboard.layout[enemies[i].coordinates.row][enemies[i].coordinates.col] != 'X')
            drawChar('K', enemies[i].coordinates.row, enemies[i].coordinates.col);

    }


}