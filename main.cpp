#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <thread>
#include <list>
#include <vector>
#include <random>
#include "Ball/ball.cpp"
#include "Square/square.cpp"

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

void moveSquare(Square *square) {
	int upPosition;

	while(1) {
		upPosition = square->getUpPosition();
		if (square->isUpDirection()){
			square->setUpPosition(upPosition-=1);
			if (square->getUpPosition() == 1) {
				square->changeDirection();
				square->drawSpeed();
			}
		}
		else {
			square->setUpPosition(upPosition+=1);
			if (square->getDownPosition() == BOARD_WIDTH-2) {
				square->changeDirection();
				square->drawSpeed();
			}
		}
		int sl = square->getSpeed();
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

void printBoard(WINDOW *win, Square *square) {
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
		wattron(win, A_STANDOUT);
		for(int i = 0; i < square->getLength(); i++){
			for (int j = 0; j < square->getHeight(); j++){
				mvwprintw(win, square->getUpPosition() + i, 6 + j, " ");
			}
		}
		wattroff(win, A_STANDOUT);
		wrefresh(win);
		fflush(stdout);
		napms(100);
		werase(win);
		box(win, 0, 0);
	}
}


int main(int argc, char** argv) {

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> sleepTime(300'000, 600'000);
	uniform_int_distribution<> ballDirection(1, 3);

	initscr();
	noecho();
	curs_set(0);
	WINDOW *win = newwin(BOARD_WIDTH, BOARD_LENGTH, 15, 15);
	start_color();
	use_default_colors();
	box(win, 0, 0);

	Square square = Square(10, 10);
	thread printBoardThread(printBoard, win, &square);
	thread moveSquareThread(moveSquare, &square);

	char *namesArray[8] = {"O", "P", "B", "C", "G", "D", "U", "R"};

	for (int i = 0; i < 8; i++) {
		ballList.push_back(Ball(namesArray[i], sleepTime(gen), ballDirection(gen)));
	}

	list<thread> threadList;

	for (int i = 0; i < 8; i++) {
		threadList.push_back(thread(moveBall, &(ballList[i])));
		this_thread::sleep_for(1s);
	}

	printBoardThread.join();
	moveSquareThread.join();
	while(!threadList.empty()){
		threadList.front().join();
		threadList.pop_front();
	}

	endwin();

	return 0;
}
