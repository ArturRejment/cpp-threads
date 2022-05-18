#include<iostream>
using namespace std;

class Ball {
	public:
		const char* name;
		int xPosition;
		int yPosition;
		int speed;
		int bounceNumber;
		int xDelta;
		int yDelta;

		bool is_sleeping;
		condition_variable cv;
		condition_variable &show;

        Ball(const Ball&);
        Ball(const char* name, int speed, int ballDirection, condition_variable &show);
        ~Ball();

		bool isInSquare(int LeftUpPosition, int length, int height);
		void moveBall();

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