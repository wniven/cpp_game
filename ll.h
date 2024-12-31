/***************************************************
Filename: ll.h
Author: CDT William Niven (259225)
Library for linked list for use in project (Project 3)
***************************************************/

#pragma once
#include <iostream>
using namespace std;


//create Node struct with all gamescript attributes
struct Node{
    string filename;
    int shipnum, huntnum, points;
    Node* next;
};

//adds node with all attributes to the back of a linked list (L)
Node* add2back(string filename, int shipnum, int huntnum, int points, Node* L);

//deletes a linked list (list) from memory
void deletelist(Node* list);
