#include<iostream>
using namespace std;

class Ball {
	private:
		const char* name;
		int xPosition;
		int yPosition;
		int speed;

	public:
        Ball();
        Ball(const char* name);
        ~Ball();

		int getXPosition();
		void setXPosition(int position);
		int getYPosition();
		void setYPosition(int position);
        int getSpeed();
        const char* getName();
};