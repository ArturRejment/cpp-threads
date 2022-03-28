#include "square.h"

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
    this->speed = 400000;
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