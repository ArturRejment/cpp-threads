#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <thread>
#include <list>
#include <vector>
#include "Ball/ball.cpp"

using namespace std;

const int BOARD_LENGTH = 71;
const int BOARD_WIDTH = 30;

vector<Ball> ballList;

void moveBall(Ball *ball) {
	// Ball *ball = (Ball*)arg;
	int x, y;
	while(1){
		x = ball->getXPosition();
		y = ball->getYPosition();
		
		// Bottom 
		if (x == BOARD_WIDTH-2) {
			ball->bounceX();
			ball->incrementBounceNumber();
		}
		// Upper
		else if (x == 1) {
			ball->bounceX();
			ball->incrementBounceNumber();
		}

		// Right
		if (y == BOARD_LENGTH-2) {
			ball->bounceY();
			ball->incrementBounceNumber();
		}
		// Left
		else if (y == 1){
			ball->bounceY();
			ball->incrementBounceNumber();
		}

		if (ball->getBounceNumber() >= 5) {
			break;
		}

		ball->setXPosition(x + ball->getXDelta());
		ball->setYPosition(y + ball->getYDelta());
		int sl = ball->getSpeed();
		usleep(sl);
	}
}

bool allBallsDoneCheck() {
	for (Ball ball : ballList) {
		if (ball.getBounceNumber() < 5) {
			return false;
		}
	}
	return true;
}

void printBoard(WINDOW *win) {
	for (;;) {
		if (allBallsDoneCheck()) {
			break;
		}
		for (Ball ball : ballList) {
			if (ball.getBounceNumber() >= 5) {
				continue;
			}
			mvwprintw(win, ball.getXPosition(), ball.getYPosition(), ball.getName());
		}
		wrefresh(win);
		fflush(stdout);
		napms(100);
		werase(win);
		box(win, 0, 0);
	}
}

void removeBalls(){
	while(!ballList.empty()){
		
		this_thread::sleep_for(1s);
		for (vector<Ball>::iterator it = ballList.begin(); it != ballList.end(); ++it) {
			if ((*it).getBounceNumber() >= 5) {
				ballList.erase(it);
			}
		}
	}
}

int main(int argc, char** argv) {

	initscr();
	noecho();
	curs_set(0);
	WINDOW *win = newwin(BOARD_WIDTH, BOARD_LENGTH, 15, 15);
	box(win, 0, 0);

	thread printBoardThread(printBoard, win);

	Ball ball = Ball("O", 15, 15, 100000);
	Ball ball2 = Ball("P", 15, 15, 200000);
	ballList.push_back(ball);
	ballList.push_back(ball2);

	list<thread> threadList;

	for (int i = 0; i < 2; i++) {
		threadList.push_back(thread(moveBall, &(ballList[i])));
	}

	printBoardThread.join();
	while(!threadList.empty()){
		threadList.front().join();
		threadList.pop_front();
	}

	endwin();

	return 0;
}
