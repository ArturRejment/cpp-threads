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

		werase(win);
		box(win, 0, 0);

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
		this_thread::sleep_for(10ms);
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

condition_variable cv;
mutex m;
mutex m2;
bool is_in_square = false;
bool is_in_square_2 = false;

void firstFunction() {
	for(int i = 0; i < 5; i++) {
		is_in_square = true;
		is_in_square_2 = true;
		cout << "Kulka w srodku" << endl;
		cv.notify_all();
		this_thread::sleep_for(2s);
		is_in_square = false;
		is_in_square_2 = false;
		cout << "Kulka wyleciala" << endl << endl;
		cv.notify_all();
		this_thread::sleep_for(2s);
	}

}

void secondFunction() {
	unique_lock<mutex> lock(m);
	for(int i = 0; i < 20; i++) {
		cv.wait(lock, [](){return is_in_square ? false : true; });
		cout<<"LECE 1"<<endl;
		this_thread::sleep_for(400ms);
	}

}

void thirdFunction() {
	unique_lock<mutex> lock(m2);
	for(int i = 0; i < 20; i++) {
		cv.wait(lock, [](){return is_in_square_2 ? false : true; });
		cout<<"LECE 2"<<endl;
		this_thread::sleep_for(400ms);
	}

}

void test() {
	thread t1(firstFunction);
	thread t2(secondFunction);
	thread t3(thirdFunction);
	t1.join();
	t2.join();
	t3.join();
}


int main(int argc, char** argv) {
	test();
	return 0;

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> sleepTime(100, 1000);
	uniform_int_distribution<> ballDirection(1, 3);
	uniform_int_distribution<> squareSpeed(100, 600);
	uniform_int_distribution<> newThreadPause(1000, 5000);
	uniform_int_distribution<> nameIndex(0, 12);

	Square square = Square(10, 10);
	list<thread> threadList;

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
	thread printBoardThread(printBoard, win, &square);
	thread moveSquareThread(moveSquare, &square);
	
	// Start balls threads
	while (finish_flag != true) {
		ballList.push_back(Ball(namesArray[nameIndex(gen)], sleepTime(gen), ballDirection(gen)));
		threadList.push_back(thread(moveBall, &(ballList.back())));
		this_thread::sleep_for(chrono::milliseconds(newThreadPause(gen)));
	}

	// Finish all threads
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

// kulka wchodzi do prostokata ale jak jest to moze wykonac jedno przesuniecie po czym ulega uspieniu do czasu az prostokat z niej zjedzie 
