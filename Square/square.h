#ifndef _SQUARE_
#define _SQUARE_

class Square {
    private:
        int upLeftCornerPosition;
        bool upDirection;
        int length;
        int height;
        int speed;

    public:
        Square(int length, int height);
        void drawSpeed();
        int getUpPosition();
        void setUpPosition(int position);
        int getDownPosition();
        bool isUpDirection();
        void changeDirection();
        int getSpeed();
        int getLength();
        int getHeight();
};

#endif