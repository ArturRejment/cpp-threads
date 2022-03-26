#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include "Ball/ball.cpp"

using namespace std;

void* moveBall(void* arg) {
	Ball *ball = (Ball*)arg;
	int i = 0;
	while(1){
		i += 1;
		ball->setXPosition(i);
		ball->setYPosition(i);
		int sl = ball->getSpeed();
		sleep(sl);
	}
}

int main(int argc, char** argv) {
	Ball ball = Ball("O", 1);
	Ball ball2 = Ball("P", 2);

	pthread_t t1;
	pthread_t t2;
	pthread_create(&t1, NULL, &moveBall, &ball);
	pthread_create(&t2, NULL, &moveBall, &ball2);

	initscr();
	noecho();
	curs_set(0);

	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);

	WINDOW *win = newwin(yMax/2, xMax/2, yMax/4, xMax/4);
	box(win, 0, 0);

	for (;;) {
		mvwprintw(win, ball.getXPosition(), ball.getYPosition(), ball.getName());
		mvwprintw(win, ball2.getXPosition(), ball2.getYPosition(), ball2.getName());
		wrefresh(win);
		fflush(stdout);
		napms(100);
		werase(win);
		box(win, 0, 0);
	}

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);


	wgetch(win);
	endwin();

	return 0;
}
