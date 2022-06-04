#include "ball.h"

#ifndef _BALL_IMPL_
#define _BALL_IMPL_

Ball::Ball(const Ball& b)
    : xPosition(b.xPosition), yPosition(b.yPosition), name(b.name), speed(b.speed), xDelta(b.xDelta), yDelta(b.yDelta), bounceNumber(b.bounceNumber), nowSleeping(b.nowSleeping), ball_counter_lock(b.ball_counter_lock) {
    }

Ball::Ball(const char* name, int speed, int ballDirection, condition_variable &cond) : ball_counter_lock(cond) {
    this->xPosition = 28;
    this->yPosition = 35;
    this->name = name;
    this->speed = speed;
    this->xDelta = 1;
    this->bounceNumber = 0;
    this->is_sleeping = false;
	this->nowSleeping = false;
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

bool Ball::isInSquare(Square square) {
	int leftUpPosition = square.getUpPosition();
	int length = square.getLength();
	int height = square.getHeight();

    if(yPosition >= 10 && yPosition < 10+length && xPosition >= leftUpPosition && xPosition <= leftUpPosition+height) {
        return true;
    }
    return false;
}

void Ball::moveBall() {
	static mutex m;

	while(1){

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
    
		// Sleep thread
		{
    		unique_lock<mutex> lk(m);

			while (this->is_sleeping){
				cv.wait(lk);
			}
		}

		// Sleep to delay ball movement
		int sl = this->speed; 
		this_thread::sleep_for(chrono::milliseconds(sl));
	}
}

void Ball::wakeUp() {
	is_sleeping = false;
	counter_ready = true;
	ball_counter_lock.notify_one();
	this->cv.notify_one(); 
}

void Ball::sleep() {
	this->is_sleeping = true; 
	counter_ready = true;
	ball_counter_lock.notify_one();
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

bool Ball::getIsSleeping() {this->is_sleeping;}

#endif