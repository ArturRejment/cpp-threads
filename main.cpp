#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>
#include <vector>
#include <random>
#include <chrono>

const int BOARD_LENGTH = 71;
const int BOARD_WIDTH = 30;
bool finish_flag = false;


#include "Ball/ball.cpp"
#include "Square/square.cpp"

using namespace std;
mutex readd;
int counter = 0;

bool flag = true;


void moveSquare(Square *square, list<Ball> &ballList) {
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

void printBoard(WINDOW *win, Square *square, list<Ball> &ballList) {
	/*
	Function responsible for printing the board on the screen using ncurses
	*/

	init_pair(1, COLOR_BLACK, COLOR_YELLOW);
	while (1) {
		
		// Check stop condition
		if (finish_flag == true) {
			break;
		}

		werase(win);
		box(win, 0, 0);
		
		// Print square on the screen
		wattron(win, A_STANDOUT);
		for(int i = 0; i < square->getLength(); i++){
			for (int j = 0; j < square->getHeight(); j++){
				mvwprintw(win, square->getUpPosition() + i, 10 + j, " ");
			}
		}
		wattroff(win, A_STANDOUT);

		// Print all the balls on the screen
		for (Ball &ball : ballList) {
			if (ball.getBounceNumber() >= 5) {
				continue;
			}

			if (ball.isInSquare(*square)) {
				ball.sleep();
			}
			else {
				ball.wakeUp();
			}

			wattron(win, COLOR_PAIR(1));
			mvwprintw(win, ball.getXPosition(), ball.getYPosition(), ball.getName());
			wattroff(win, COLOR_PAIR(1));
		}

		mvwprintw(win, 15, 35, "%d", counter);
		
		// Refresh the window
		wrefresh(win);

		// Clear screen after 100ms
		this_thread::sleep_for(1ms);
	}
}

void finishProgram() {
	// Function that sets finish_flag after press 'q'
	char choice;
	while(choice != 'q'){
		choice = getch();
	}
	finish_flag = true;
}


int main(int argc, char** argv) {

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> sleepTime(400, 1000);
	uniform_int_distribution<> ballDirection(1, 3);
	uniform_int_distribution<> squareSpeed(100, 600);
	uniform_int_distribution<> newThreadPause(1000, 5000);
	uniform_int_distribution<> nameIndex(0, 12);

	Square square = Square(10, 10);
	list<thread> threadList;
	list<Ball> ballList;

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
	

	// Init screen and window
	initscr();
	noecho();
	curs_set(0);
	start_color();
	use_default_colors();

	WINDOW *win = newwin(BOARD_WIDTH, BOARD_LENGTH, 15, 15);

	// Start basic threads
	thread finishProgramThread(finishProgram);
	thread printBoardThread(printBoard, win, &square, ref(ballList));
    thread moveSquareThread(moveSquare, &square, ref(ballList));
	
	// Start balls threads
	while (finish_flag != true) {
		ballList.push_front(Ball(namesArray[nameIndex(gen)], sleepTime(gen), ballDirection(gen)));
		threadList.push_back(thread(&Ball::moveBall, ballList.begin()));
		this_thread::sleep_for(chrono::milliseconds(newThreadPause(gen)));
	}

	// Finish all threads
	finishProgramThread.join();
	printBoardThread.join();
	moveSquareThread.join();
	for (Ball &ball : ballList) {
		ball.wakeUp();
	}
	while(!threadList.empty()){
		threadList.front().join();
		threadList.pop_front();
	}

	endwin();

	return 0;
}

// Wyswietlic na srodku ekranu liczbe uspionych kulek