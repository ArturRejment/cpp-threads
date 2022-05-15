#include "ball.h"

Ball::Ball(const Ball& b)
    : xPosition(b.xPosition), yPosition(b.yPosition), name(b.name), speed(b.speed), xDelta(b.xDelta), yDelta(b.yDelta), bounceNumber(b.bounceNumber), show(b.show) {
    }

Ball::Ball(const char* name, int speed, int ballDirection, condition_variable &sh): show(sh) {
    this->xPosition = 28;
    this->yPosition = 35;
    this->name = name;
    this->speed = speed;
    this->xDelta = 1;
    this->bounceNumber = 0;
    this->is_sleeping = false;
    if (ballDirection == 1) {
        this->yDelta = 1;
    }
    else if (ballDirection == 2) {
        this->yDelta = -1;
    }
    else {
        this->yDelta = 0;
    }
}

Ball::~Ball() {}

bool Ball::isInSquare(int leftUpPosition, int length, int height) {
    if(yPosition > 10 && yPosition < 10+length && xPosition > leftUpPosition && xPosition < leftUpPosition+height) {
        return true;
    }
    return false;
}

void Ball::moveBall() {
	// static mutex m;

	while(1){
        // unique_lock<mutex> lk(m);
		if (this->is_sleeping) {
			continue;
		}
		
		
		// Bottom 
		if (xPosition == BOARD_WIDTH-2) {
			this->bounceX();
			this->incrementBounceNumber();
		}
		// Upper
		else if (xPosition == 1) {
			this->bounceX();
			this->incrementBounceNumber();
		}

		// Right
		if (yPosition == BOARD_LENGTH-2) {
			this->bounceY();
			this->incrementBounceNumber();
		}
		// Left
		else if (yPosition == 1){
			this->bounceY();
			this->incrementBounceNumber();
		}

		if (this->bounceNumber >= 5 || finish_flag == true) {
			break;
		}

        this->xPosition += this->xDelta;
        this->yPosition += this->yDelta;
        xPosition = static_cast<int>(xPosition);
        yPosition = static_cast<int>(yPosition);
        // cout << "NOT MAIN" << this->name << " " << this << xPosition << " " <<yPosition <<endl;
    
        show.notify_one();
        

		// Sleep to delay ball movement
		int sl = this->speed; 
		this_thread::sleep_for(chrono::milliseconds(sl));
	}
}

int Ball::getXPosition() {return this->xPosition;}

int Ball::getYPosition() {return this->yPosition;}

int Ball::getSpeed() {return this->speed;}

int Ball::getBounceNumber() {return this->bounceNumber;}

int Ball::getXDelta() {return this->xDelta;}

int Ball::getYDelta() {return this->yDelta;}

const char* Ball::getName() {return this->name;}

void Ball::setXPosition(int position) {this->xPosition = position;}

void Ball::setYPosition(int position) {this->yPosition = position;}

void Ball::incrementBounceNumber() {this->bounceNumber++;}

void Ball::bounceX() {this->xDelta *= -1;}

void Ball::bounceY() {this->yDelta *= -1;}
