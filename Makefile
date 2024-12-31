part1: part1.cpp easycurses.cpp board.cpp Pos.cpp board.h easycurses.h Pos.h
	g++ part1.cpp easycurses.cpp board.cpp Pos.cpp -l ncurses -o part1
part2: part2.cpp easycurses.cpp board.cpp Pos.cpp board.h easycurses.h Pos.h
	g++ part2.cpp easycurses.cpp board.cpp Pos.cpp -l ncurses -o part2
part3: part3.cpp easycurses.cpp board.cpp Pos.cpp board.h easycurses.h Pos.h
	g++ part3.cpp easycurses.cpp board.cpp Pos.cpp -l ncurses -o part3
part4: part4.cpp easycurses.cpp board.cpp Pos.cpp board.h easycurses.h Pos.h
	g++ part4.cpp easycurses.cpp board.cpp Pos.cpp -l ncurses -o part4
part5: part5.cpp easycurses.cpp board.cpp Pos.cpp ll.cpp board.h easycurses.h Pos.h ll.h
	g++ part5.cpp easycurses.cpp board.cpp Pos.cpp ll.cpp -l ncurses -o part5
################## DON'T TOUCH BELOW ##################
run1: part1
	@./part1
run_%: %
	@test -f $* && echo "$*: Code compiled successfully" 

