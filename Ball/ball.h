#include<iostream>
#include "../Square/square.h"
using namespace std;

#ifndef _BALL_
#define _BALL_

class Ball {
	private:
		const char* name;
		int xPosition;
		int yPosition;
		int speed;
		int bounceNumber;
		int xDelta;
		int yDelta;

		bool is_sleeping;
		condition_variable cv;

	public:
        Ball(const Ball&);
        Ball(const char* name, int speed, int ballDirection);
        ~Ball();

		bool isInSquare(Square square);
		void moveBall();
		void notify();
		void sleep();
		void wakeUp();

		// Getters and setters
		int getXPosition();
		void setXPosition(int position);
		int getYPosition();
		void setYPosition(int position);
        int getSpeed();
        const char* getName();
		int getBounceNumber();
		void incrementBounceNumber();
		void bounceX();
		void bounceY();
		int getXDelta();
		int getYDelta();
};

#endif