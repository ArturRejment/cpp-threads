#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include "Ball/ball.cpp"

using namespace std;

void moveBall(Ball *ball) {
	while(1){
		ball->setXPosition(ball->getXPosition()+1);
		ball->setYPosition(ball->getYPosition()+1);
		sleep(ball->getSpeed());
	}
}

int main(int argc, char** argv) {
	Ball ball = Ball("O");
	initscr();
	noecho();
	curs_set(0);

	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);

	WINDOW *win = newwin(yMax/2, xMax/2, yMax/4, xMax/4);
	box(win, 0, 0);
	init_pair(1, COLOR_RED, COLOR_YELLOW);
	start_color();
	attron(COLOR_PAIR(1));

	for (int i=2; i<20; i++){
		mvwprintw(win, ball.getXPosition(), ball.getYPosition(), ball.getName());
		ball.setXPosition(i);
		ball.setYPosition(i);
		wrefresh(win);
		fflush(stdout);
		napms(500);
		werase(win);
		box(win, 0, 0);
	}

	wgetch(win);

	attroff(COLOR_PAIR(1));
	endwin();
	return 0;
}
