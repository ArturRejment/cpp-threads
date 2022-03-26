#include "ball.h"

Ball::Ball(const char* name, int xPos, int yPos, int speed) {
    this->xPosition = xPos;
    this->yPosition = yPos;
    this->name = name;
    this->speed = speed;
    this->xDelta = 1;
    this->yDelta = 1;
    this->bounceNumber = 0;
}

Ball::~Ball() {}

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
