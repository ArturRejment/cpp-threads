#include "square.h"

#ifndef _SQUARE_IMPL_
#define _SQUARE_IMPL_

Square::Square(int length, int height){
    this->length = length;
    this->height = height;
    this->drawSpeed();
    this->upLeftCornerPosition = 5;
    this->upDirection = true;
}

void Square::changeDirection() {
    this->upDirection = !upDirection;
}


bool Square::isUpDirection() {
    return upDirection;
}

int Square::getDownPosition() {
    return this->upLeftCornerPosition + this->height;
}

int Square::getUpPosition() {
    return this->upLeftCornerPosition;
}

void Square::setUpPosition(int position) {
    this->upLeftCornerPosition = position;
}

void Square::drawSpeed() {
    random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> squareSpeed(100, 600);
    this->speed = squareSpeed(gen);
}

int Square::getSpeed() {
    return this->speed;
}

int Square::getLength() {
    return this->length;
}

int Square::getHeight() {
    return this->height;
}

#endif