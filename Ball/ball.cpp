#include "ball.h"

Ball::Ball(const char* name, int speed) {
    this->xPosition = 1;
    this->yPosition = 1;
    this->name = name;
    this->speed = speed;
}

Ball::~Ball() {}

int Ball::getXPosition() {return this->xPosition;}

int Ball::getYPosition() {return this->yPosition;}

int Ball::getSpeed() {return this->speed;}

int Ball::getBounceNumber() {return this->bounceNumber;}

const char* Ball::getName() {return this->name;}

void Ball::setXPosition(int position) {this->xPosition = position;}

void Ball::setYPosition(int position) {this->yPosition = position;}

void Ball::incrementBounceNumber() {this->bounceNumber++;}
