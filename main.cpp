#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <thread>
#include <list>
#include <vector>
#include <random>
#include <chrono>
#include "Ball/ball.cpp"
#include "Square/square.cpp"

using namespace std;

const int BOARD_LENGTH = 71;
const int BOARD_WIDTH = 30;

bool finish_flag = false;

list<Ball> ballList;

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

		if (ball->getBounceNumber() >= 5 || finish_flag == true) {
			break;
		}

		ball->setXPosition(x + ball->getXDelta());
		ball->setYPosition(y + ball->getYDelta());

		// Sleep to delay ball movement
		int sl = ball->getSpeed();
		this_thread::sleep_for(chrono::milliseconds(sl));
	}
}

void moveSquare(Square *square) {
	/*
	Function responsible for moving the square up and down - depending on a position
	*/
	int upPosition;

	while(1) {
		// Check stop condition
		if (finish_flag == true) {
			break;
		}

		upPosition = square->getUpPosition();

		// Move square up
		if (square->isUpDirection()){
			square->setUpPosition(upPosition-=1);
			if (square->getUpPosition() == 1) {
				square->changeDirection();
				square->drawSpeed();
			}
		}
		// Move square down
		else {
			square->setUpPosition(upPosition+=1);
			if (square->getDownPosition() == BOARD_WIDTH-1) {
				square->changeDirection();
				square->drawSpeed();
			}
		}

		// Sleep to delay square movement
		int sl = square->getSpeed();
		this_thread::sleep_for(chrono::milliseconds(sl));
	}

}

void printBoard(WINDOW *win, Square *square) {
	/*
	Function responsible for printing the board on the screen using ncurses
	*/

	while (1) {
		// Check stop condition
		if (finish_flag == true) {
			break;
		}

		// Print all the balls on the screen
		for (Ball ball : ballList) {
			if (ball.getBounceNumber() >= 5) {
				continue;
			}
			mvwprintw(win, ball.getXPosition(), ball.getYPosition(), ball.getName());
		}

		// Print square on the screen
		wattron(win, A_STANDOUT);
		for(int i = 0; i < square->getLength(); i++){
			for (int j = 0; j < square->getHeight(); j++){
				mvwprintw(win, square->getUpPosition() + i, 10 + j, " ");
			}
		}
		wattroff(win, A_STANDOUT);
		
		// Refresh the window
		wrefresh(win);

		// Clear screen after 100ms
		this_thread::sleep_for(100ms);
		werase(win);
		box(win, 0, 0);
	}
}

void finishProgram() {
	char choice;
	while(choice != 'q'){
		choice = getch();
	}
	finish_flag = true;
}


int main(int argc, char** argv) {

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> sleepTime(300, 600);
	uniform_int_distribution<> ballDirection(1, 3);
	uniform_int_distribution<> squareSpeed(100, 600);
	uniform_int_distribution<> newThreadPause(1000, 3000);
	uniform_int_distribution<> nameIndex(0, 12);

	initscr();
	noecho();
	curs_set(0);
	WINDOW *win = newwin(BOARD_WIDTH, BOARD_LENGTH, 15, 15);
	start_color();
	use_default_colors();
	box(win, 0, 0);

	Square square = Square(10, 10);
	thread finishProgramThread(finishProgram);
	thread printBoardThread(printBoard, win, &square);
	thread moveSquareThread(moveSquare, &square);

	char *namesArray[13] = {
		(char*)"O", 
		(char*)"P", 
		(char*)"B",
		(char*)"C", 
		(char*)"G", 
		(char*)"D", 
		(char*)"U", 
		(char*)"V",
		(char*)"R",
		(char*)"T",
		(char*)"K",
		(char*)"Z",
		(char*)"F",
	};


	list<thread> threadList;

	while (finish_flag != true) {
		ballList.push_back(Ball(namesArray[nameIndex(gen)], sleepTime(gen), ballDirection(gen)));
		threadList.push_back(thread(moveBall, &(ballList.back())));
		this_thread::sleep_for(chrono::milliseconds(newThreadPause(gen)));
	}

	finishProgramThread.join();
	printBoardThread.join();
	moveSquareThread.join();
	while(!threadList.empty()){
		threadList.front().join();
		threadList.pop_front();
	}

	endwin();

	return 0;
}
