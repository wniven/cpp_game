/**
 * This code is a wrapper for the curses library. It hides the details of
 * curses to instead provide "easy to use" functions. Using these functions
 * instead of curses directly will lose some flexibility and functionality,
 * but most basic applications don't need it.
 *
 * @author chambers
 * adapted by choi
 */
#include <iostream>
#include <curses.h>

// Global variable holding the pointer to the ncurses terminal.
WINDOW* W = 0;


// Initializes curses and returns the pointer to the window handle that
// must be passed to the drawing functions.
void startCurses() {
  W = initscr();  
  cbreak();
  noecho(); 
  nodelay(W, true);
  keypad(W, true);  // allows arrow keys
  curs_set(0);
}

// Write one character to the terminal at location (row,col) and refresh the screen
void drawCharAndRefresh(char ch, int row, int col) {
  if( W == 0 )
    std::cerr << "ERROR: drawChar() called with an uninitialized window. Call startCurses() first.\n";
  else {
    wmove(W, row, col);
    waddch(W, ch);
    wrefresh(W);
  }
}

// Write one character to the terminal at location (row,col).
void drawChar(char ch, int row, int col) {
  if( W == 0 )
    std::cerr << "ERROR: drawChar() called with an uninitialized window. Call startCurses() first.\n";
  else {
    wmove(W, row, col);
    waddch(W, ch);
  }
}

// Grab the last key pressed by the user.
int inputChar() {
  return wgetch(W);
}

// Redraw everything added to the curses window.
void refreshWindow() {
  if( W == 0 )
    std::cerr << "ERROR: refreshWindow() called with an uninitialized window. Call startCurses() first.\n";
  wrefresh(W);
}

// Close curses and return the terminal to normal operation.
void endCurses() {
  endwin();
}

// Sets the rows/cols parameters to the dimensions of the given window.
void getWindowDimensions(int& rows, int& cols) {
  if( W == 0 )
    std::cerr << "ERROR: getWindowDimensions() called with an uninitialized window. Call startCurses() first.\n";
  // Call it anyway to set ints to -1
  getmaxyx(W, rows, cols);
}
