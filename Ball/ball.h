#include<iostream>
using namespace std;

class Ball {
	private:
		const char* name;
		int xPosition;
		int yPosition;
		int speed;
		int bounceNumber;
		int xDelta;
		int yDelta;

	public:
        Ball();
        Ball(const char* name, int xPos, int yPos, int speed);
        ~Ball();

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