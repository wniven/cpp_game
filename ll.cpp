#include <iostream>
#include "ll.h"
using namespace std;

//modified add2back function from Class 34 notes
Node* add2back(string filename, int shipnum, int huntnum, int points, Node* L){
  if (L == NULL)
  {
    return new Node{filename, shipnum, huntnum, points, NULL};
  }
  else
  {
    // non-empty L case
    Node* last = L;
    while(last->next != NULL)
      last = last->next;

    // update last->next
    last->next = new Node{filename, shipnum, huntnum, points, NULL};

    // The head of the list didn't change
    return L;
  }
}


//pull deletelist function from Class 34 notes
void deletelist(Node* L){
  // Base case: if the list is empty, there is nothing to delete
  if (L == NULL)
    return;

  // Use recursion to delete all the second-to-last nodes
  deletelist(L->next);
  delete L;               // Then delete the first node
}