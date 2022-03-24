#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include "Ball/ball.cpp"

using namespace std;


int main(int argc, char** argv) {
	Ball ball = Ball("O");
	initscr();
	noecho();
	curs_set(0);

	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);

	WINDOW *win = newwin(yMax/2, xMax/2, yMax/4, xMax/4);
	box(win, 0, 0);

	for (int i=2; i<12; i++){
		mvwprintw(win, ball.getXPosition(), ball.getYPosition(), ball.getName());
		ball.setXPosition(i);
		ball.setYPosition(i);
		wrefresh(win);
		fflush(stdout);
		napms(200);
	}

	wgetch(win);

	endwin();
	return 0;
}
